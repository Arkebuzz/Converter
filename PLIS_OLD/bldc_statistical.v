`define INVERSE
module bldc_statistical (
    input wire clk,
    input wire rst_n,
    
    input wire hall_a_in,
    input wire hall_b_in,
    input wire hall_c_in,
    
    output reg [31:0] theta_elec            /* synthesis noprune */,
    output reg signed [31:0] velocity       /* synthesis noprune */,
    output reg signed [31:0] accel          /* synthesis noprune */,
    
    output reg [7:0] error_code             /* synthesis noprune */,
    output wire critical_error              /* synthesis noprune */,
    output wire timeout_warning             /* synthesis noprune */
);

    // =========================================================================
    // ПАРАМЕТРЫ
    // =========================================================================
    localparam [31:0] T_MIN_TICKS          = 32'd16_000;
    localparam [31:0] TIMER_MAX            = 32'd50_000_000;
    localparam [9:0]  DEBOUNCE_THRESH      = 10'd500;
    localparam [31:0] INVALID_CODE_TIMEOUT = 32'd5_000;
    localparam [31:0] STUCK_TIMEOUT_MIN    = 32'd250_000;
    localparam [31:0] STUCK_TIMEOUT_MAX    = 32'd100_000_000;
    localparam [31:0] INC_MAX              = 32'd43_000;
    localparam signed [31:0] ACC_MAX       =  32'sd300_000;
    localparam signed [31:0] ACC_MIN       = -32'sd300_000;
    localparam [31:0] SECTOR_WIDTH         = 32'h2AAAAAAA;
    localparam [2:0]  DIR_CONFIRM_TH       = 3'd3;
    localparam [31:0] MOVING_PERIOD_TH     = 32'd5_000_000;
    localparam [31:0] DECAY_INTERVAL       = 32'd500_000;
    localparam [4:0]  DECAY_SHIFT          = 5'd8;
    localparam [7:0]  CONSEC_ERR_LIMIT     = 8'd10;
    localparam [7:0]  ERR_WINDOW_TH        = 8'd20;
    localparam [7:0]  GOOD_WINDOW_TH       = 8'd30;
    localparam [15:0] STARTUP_TICKS        = 16'd2000;

    // =========================================================================
    // 1. СИНХРОНИЗАЦИЯ + ДЕБАУНС
    // =========================================================================
    reg hall_a_s1, hall_a_s2;
    reg hall_b_s1, hall_b_s2;
    reg hall_c_s1, hall_c_s2;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            {hall_a_s1, hall_a_s2} <= 2'b0;
            {hall_b_s1, hall_b_s2} <= 2'b0;
            {hall_c_s1, hall_c_s2} <= 2'b0;
        end else begin
            hall_a_s1 <= hall_a_in; hall_a_s2 <= hall_a_s1;
            hall_b_s1 <= hall_b_in; hall_b_s2 <= hall_b_s1;
            hall_c_s1 <= hall_c_in; hall_c_s2 <= hall_c_s1;
        end
    end

    wire [2:0] hall_sync = {hall_c_s2, hall_b_s2, hall_a_s2};

    reg [2:0]  hall_filt;
    reg [2:0]  hall_candidate;
    reg [9:0]  debounce_cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            hall_filt      <= hall_sync;
            hall_candidate <= hall_sync;
            debounce_cnt   <= 0;
        end else begin
            if (hall_sync != hall_candidate) begin
                hall_candidate <= hall_sync;
                debounce_cnt   <= 0;
            end else if (debounce_cnt < DEBOUNCE_THRESH) begin
                debounce_cnt <= debounce_cnt + 1'b1;
            end else begin
                hall_filt <= hall_candidate;
            end
        end
    end

    // =========================================================================
    // 2. ФАЗА ИНИЦИАЛИЗАЦИИ
    // =========================================================================
    reg [15:0] startup_cnt;
    reg        startup_done;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            startup_cnt  <= 0;
            startup_done <= 1'b0;
        end else if (!startup_done) begin
            if (startup_cnt < STARTUP_TICKS)
                startup_cnt <= startup_cnt + 1'b1;
            else
                startup_done <= 1'b1;
        end
    end

    // =========================================================================
    // 3. РЕГИСТРАЦИЯ ИЗМЕНЕНИЙ И ВАЛИДАЦИЯ (КОНВЕЙЕР)
    // =========================================================================
    
    // Stage 1: Регистрируем изменение
    reg [2:0] hall_prev;
    reg       hall_change_stage1;
	 reg [2:0] hall_filt_saved;
	 reg [2:0] hall_prev_saved;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            hall_prev          <= hall_filt;
            hall_change_stage1 <= 1'b0;
				hall_filt_saved <= hall_filt;
				hall_prev_saved <= hall_filt;
        end else if (!startup_done) begin
            hall_prev          <= hall_filt;
            hall_change_stage1 <= 1'b0;
				hall_filt_saved <= hall_filt;
				hall_prev_saved <= hall_filt;
        end else begin
            hall_change_stage1 <= (hall_filt != hall_prev);
            if (hall_filt != hall_prev) begin
                hall_prev <= hall_filt;
					 hall_filt_saved <= hall_filt;
					 hall_prev_saved <= hall_prev;
					 end
				
        end
    end

    // Stage 2: Регистрируем результаты проверок
    reg valid_code_stage2;
    reg valid_1bit_stage2;
    reg valid_step_stage2;
    reg step_is_fwd_stage2;
    reg step_is_rev_stage2;
    reg [2:0] hall_filt_stage2;
    reg [2:0] hall_prev_stage2;
    reg [31:0] period_timer_stage2;
    reg startup_done_stage2;
	 reg hall_change_stage2;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            valid_code_stage2   <= 1'b0;
            valid_1bit_stage2   <= 1'b0;
            valid_step_stage2   <= 1'b0;
            step_is_fwd_stage2  <= 1'b0;
            step_is_rev_stage2  <= 1'b0;
            hall_filt_stage2    <= 3'b0;
            hall_prev_stage2    <= 3'b0;
            period_timer_stage2 <= 0;
            startup_done_stage2 <= 1'b0;
				hall_change_stage2 <= 1'b0;
        end else begin
            startup_done_stage2 <= startup_done;
            hall_filt_stage2    <= hall_filt_saved;
            hall_prev_stage2    <= hall_prev_saved;
            period_timer_stage2 <= period_timer;
            
            if (startup_done) begin
                // Проверка valid_code
                case (hall_filt_saved)
                    3'b001, 3'b011, 3'b010, 3'b110, 3'b100, 3'b101: 
                        valid_code_stage2 <= 1'b1;
                    default: 
                        valid_code_stage2 <= 1'b0;
                endcase
                
					 if (hall_change_stage1) begin
					 hall_change_stage2<=1;
                // Проверка valid_1bit
                begin
                    reg [2:0] hall_diff_tmp;
                    hall_diff_tmp = hall_filt_saved ^ hall_prev_saved;
                    valid_1bit_stage2 <= (hall_diff_tmp == 3'b001) ||
                                         (hall_diff_tmp == 3'b010) ||
                                         (hall_diff_tmp == 3'b100);
                end
                
                // Проверка forward step
					`ifdef INVERSE
					// When INVERSE defined: physical reverse sequence becomes "forward"
					case (hall_prev_saved)
						 3'b001: step_is_fwd_stage2 <= (hall_filt_saved == 3'b101);
						 3'b101: step_is_fwd_stage2 <= (hall_filt_saved == 3'b100);
						 3'b100: step_is_fwd_stage2 <= (hall_filt_saved == 3'b110);
						 3'b110: step_is_fwd_stage2 <= (hall_filt_saved == 3'b010);
						 3'b010: step_is_fwd_stage2 <= (hall_filt_saved == 3'b011);
						 3'b011: step_is_fwd_stage2 <= (hall_filt_saved == 3'b001);
						 default: step_is_fwd_stage2 <= 1'b0;
					endcase

					// Проверка reverse step
					case (hall_prev_saved)
						 3'b001: step_is_rev_stage2 <= (hall_filt_saved == 3'b011);
						 3'b011: step_is_rev_stage2 <= (hall_filt_saved == 3'b010);
						 3'b010: step_is_rev_stage2 <= (hall_filt_saved == 3'b110);
						 3'b110: step_is_rev_stage2 <= (hall_filt_saved == 3'b100);
						 3'b100: step_is_rev_stage2 <= (hall_filt_saved == 3'b101);
						 3'b101: step_is_rev_stage2 <= (hall_filt_saved == 3'b001);
						 default: step_is_rev_stage2 <= 1'b0;
					endcase
					`else
					// Normal (non-inverted) direction
					case (hall_prev_saved)
						 3'b001: step_is_fwd_stage2 <= (hall_filt_saved == 3'b011);
						 3'b011: step_is_fwd_stage2 <= (hall_filt_saved == 3'b010);
						 3'b010: step_is_fwd_stage2 <= (hall_filt_saved == 3'b110);
						 3'b110: step_is_fwd_stage2 <= (hall_filt_saved == 3'b100);
						 3'b100: step_is_fwd_stage2 <= (hall_filt_saved == 3'b101);
						 3'b101: step_is_fwd_stage2 <= (hall_filt_saved == 3'b001);
						 default: step_is_fwd_stage2 <= 1'b0;
					endcase

					case (hall_prev_saved)
						 3'b001: step_is_rev_stage2 <= (hall_filt_saved == 3'b101);
						 3'b101: step_is_rev_stage2 <= (hall_filt_saved == 3'b100);
						 3'b100: step_is_rev_stage2 <= (hall_filt_saved == 3'b110);
						 3'b110: step_is_rev_stage2 <= (hall_filt_saved == 3'b010);
						 3'b010: step_is_rev_stage2 <= (hall_filt_saved == 3'b011);
						 3'b011: step_is_rev_stage2 <= (hall_filt_saved == 3'b001);
						 default: step_is_rev_stage2 <= 1'b0;
					endcase
					`endif
                
                valid_step_stage2 <= step_is_fwd_stage2 || step_is_rev_stage2;
					 end
					 
					 if (hall_change_stage2) begin hall_change_stage2<=0; end;
					 
            end else begin
                valid_code_stage2  <= 1'b0;
                valid_1bit_stage2  <= 1'b0;
                valid_step_stage2  <= 1'b0;
                step_is_fwd_stage2 <= 1'b0;
                step_is_rev_stage2 <= 1'b0;
					 hall_change_stage2<=0;
            end
        end
    end

    // Stage 3: Финальные события и флаг первого перехода
    reg hall_valid_step;
    reg hall_event_good;
    reg first_valid_seen /* synthesis noprune */;
    reg [2:0] valid_transition_count /* synthesis noprune */;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            hall_valid_step         <= 1'b0;
            hall_event_good         <= 1'b0;
            first_valid_seen        <= 1'b0;
            valid_transition_count  <= 0;
        end else begin
            // Базовая валидация (без проверки T_MIN_TICKS для первых переходов)
            if (valid_transition_count < 3'd2) begin
                // Первые 2 перехода: только базовые проверки
                hall_valid_step <= startup_done_stage2 &&
                                  hall_change_stage2 &&
                                  valid_code_stage2 &&
                                  valid_1bit_stage2 &&
                                  valid_step_stage2;
            end else begin
                // Последующие переходы: полные проверки
                hall_valid_step <= startup_done_stage2 &&
                                  hall_change_stage2 &&
                                  valid_code_stage2 &&
                                  valid_1bit_stage2 &&
                                  valid_step_stage2 &&
                                  (period_timer_stage2 >= T_MIN_TICKS);
            end
            
            // Счётчик валидных переходов
            if (hall_valid_step && valid_transition_count < 3'd7)
                valid_transition_count <= valid_transition_count + 1'b1;
            
            // Устанавливаем флаг СРАЗУ при первом валидном переходе
            if (hall_valid_step && !first_valid_seen)
                first_valid_seen <= 1'b1;
            
            // Good event: разрешаем обновление сразу после первого валидного
            // Используем комбинацию текущего hall_valid_step И установленного флага
            hall_event_good <= hall_valid_step &&
                              (first_valid_seen || hall_valid_step) &&  // Разрешаем в том же такте
                              !error_code[5] &&
                              !error_code[6];
        end
    end

    // =========================================================================
    // 4. НАПРАВЛЕНИЕ
    // =========================================================================
    reg direction /* synthesis noprune */;
    reg direction_candidate;
    reg [2:0] dir_confirm_cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            direction           <= 1'b1;
            direction_candidate <= 1'b1;
            dir_confirm_cnt     <= 0;
        end else if (hall_event_good) begin
            if (step_is_fwd_stage2) begin
                if (direction_candidate == 1'b1) begin
                    if (dir_confirm_cnt >= DIR_CONFIRM_TH)
                        direction <= 1'b1;
                    else
                        dir_confirm_cnt <= dir_confirm_cnt + 1'b1;
                end else begin
                    direction_candidate <= 1'b1;
                    dir_confirm_cnt     <= 0;
                end
            end else if (step_is_rev_stage2) begin
                if (direction_candidate == 1'b0) begin
                    if (dir_confirm_cnt >= DIR_CONFIRM_TH)
                        direction <= 1'b0;
                    else
                        dir_confirm_cnt <= dir_confirm_cnt + 1'b1;
                end else begin
                    direction_candidate <= 1'b0;
                    dir_confirm_cnt     <= 0;
                end
            end
        end
    end

    // =========================================================================
    // 5. ДЕКОДИРОВАНИЕ СЕКТОРА
    // =========================================================================
    reg [2:0]  sector;
    reg [31:0] sector_angle_base;

    always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        sector <= 0;
        sector_angle_base <= 32'h00000000;
    end else if (!startup_done) begin
        `ifdef INVERSE
			  case (hall_filt)
					3'b001: begin sector <= 0; sector_angle_base <= 32'h00000000; end
					3'b101: begin sector <= 1; sector_angle_base <= 32'h2AAAAAAA; end
					3'b100: begin sector <= 2; sector_angle_base <= 32'h55555555; end
					3'b110: begin sector <= 3; sector_angle_base <= 32'h80000000; end
					3'b010: begin sector <= 4; sector_angle_base <= 32'hAAAAAAAA; end
					3'b011: begin sector <= 5; sector_angle_base <= 32'hD5555555; end
					default: begin sector <= 0; sector_angle_base <= 32'h00000000; end
			  endcase
			  `else
			  case (hall_filt)
					3'b001: begin sector <= 0; sector_angle_base <= 32'h00000000; end
					3'b011: begin sector <= 1; sector_angle_base <= 32'h2AAAAAAA; end
					3'b010: begin sector <= 2; sector_angle_base <= 32'h55555555; end
					3'b110: begin sector <= 3; sector_angle_base <= 32'h80000000; end
					3'b100: begin sector <= 4; sector_angle_base <= 32'hAAAAAAAA; end
					3'b101: begin sector <= 5; sector_angle_base <= 32'hD5555555; end
					default: begin sector <= 0; sector_angle_base <= 32'h00000000; end
			  endcase
			  `endif
		 end else begin
			  `ifdef INVERSE
			  case (hall_filt_stage2)
					3'b001: begin sector <= 0; sector_angle_base <= 32'h00000000; end
					3'b101: begin sector <= 1; sector_angle_base <= 32'h2AAAAAAA; end
					3'b100: begin sector <= 2; sector_angle_base <= 32'h55555555; end
					3'b110: begin sector <= 3; sector_angle_base <= 32'h80000000; end
					3'b010: begin sector <= 4; sector_angle_base <= 32'hAAAAAAAA; end
					3'b011: begin sector <= 5; sector_angle_base <= 32'hD5555555; end
					default: begin sector <= sector; sector_angle_base <= sector_angle_base; end
			  endcase
			  `else
			  case (hall_filt_stage2)
					3'b001: begin sector <= 0; sector_angle_base <= 32'h00000000; end
					3'b011: begin sector <= 1; sector_angle_base <= 32'h2AAAAAAA; end
					3'b010: begin sector <= 2; sector_angle_base <= 32'h55555555; end
					3'b110: begin sector <= 3; sector_angle_base <= 32'h80000000; end
					3'b100: begin sector <= 4; sector_angle_base <= 32'hAAAAAAAA; end
					3'b101: begin sector <= 5; sector_angle_base <= 32'hD5555555; end
					default: begin sector <= sector; sector_angle_base <= sector_angle_base; end
			  endcase
			  `endif
		 end
	end

    // =========================================================================
    // 6. СТАТИСТИКА
    // =========================================================================
    reg [31:0] period_timer;
    reg [31:0] period_history [0:5];
    reg [2:0]  hist_ptr;
    reg [31:0] sum_periods;
    reg [31:0] avg_period;
    reg [31:0] angle_increment_per_tick /* synthesis noprune */;
    reg        period_reset;

    //wire [31:0] new_sum       = sum_periods - period_history[hist_ptr] + period_timer;
    //wire [31:0] new_avg       = new_sum / 6;
    //wire [31:0] raw_increment = (new_avg > 0) ? (SECTOR_WIDTH / new_avg) : 0;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            period_timer             <= 0;
            period_reset             <= 0;
            hist_ptr                 <= 0;
            sum_periods              <= TIMER_MAX * 6;
            avg_period               <= TIMER_MAX;
            angle_increment_per_tick <= 0;
            period_history[0] <= TIMER_MAX; period_history[1] <= TIMER_MAX;
            period_history[2] <= TIMER_MAX; period_history[3] <= TIMER_MAX;
            period_history[4] <= TIMER_MAX; period_history[5] <= TIMER_MAX;
        end else if (!startup_done) begin
            period_timer <= 0;
            period_reset <= 0;
        end else begin
            if (period_reset) begin
                period_reset <= 0;
                period_timer <= 0;
            end else if (period_timer < TIMER_MAX) begin
                period_timer <= period_timer + 1'b1;
            end

            if (hall_valid_step) begin
                sum_periods              <= sum_periods - period_history[hist_ptr] + period_timer;
                period_history[hist_ptr] <= period_timer;
                hist_ptr <= (hist_ptr == 3'd5) ? 3'd0 : hist_ptr + 1'b1;
                avg_period <= (sum_periods - period_history[hist_ptr] + period_timer) / 6;
					 period_reset             <= 1'b1;
            end else if (period_timer == TIMER_MAX) begin
                hist_ptr    <= 0;
                avg_period  <= TIMER_MAX;
                sum_periods <= TIMER_MAX * 6;
                period_history[0] <= TIMER_MAX; period_history[1] <= TIMER_MAX;
                period_history[2] <= TIMER_MAX; period_history[3] <= TIMER_MAX;
                period_history[4] <= TIMER_MAX; period_history[5] <= TIMER_MAX;
            end
				
				if (period_timer == TIMER_MAX) begin angle_increment_per_tick <= 0;  end
				else if (avg_period > 0) begin angle_increment_per_tick <= SECTOR_WIDTH / avg_period; end
            else begin angle_increment_per_tick <= 0;  end        
				
        end
    end

    // =========================================================================
    // 7. УГОЛ, СКОРОСТЬ, УСКОРЕНИЕ + ДЕГРАДАЦИЯ
    // =========================================================================
    reg signed [31:0] vel_prev_sector;

    always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        theta_elec      <= 32'h00000000;
        velocity        <= 0;
        accel           <= 0;
        vel_prev_sector <= 0;
    end else if (!startup_done) begin
        theta_elec <= sector_angle_base;
    end else begin
        if (critical_error) begin
            velocity        <= 0;
            accel           <= 0;
            vel_prev_sector <= 0;
        end else if (hall_event_good) begin
            // При событии - точная синхронизация
            theta_elec <= sector_angle_base;

            // Обновление скорости
            if (direction)
                velocity <= $signed({1'b0, angle_increment_per_tick});
            else
                velocity <= -$signed({1'b0, angle_increment_per_tick});

            // Расчёт ускорения
            begin : accel_calc
                reg signed [31:0] cur_vel;
                reg signed [31:0] raw_a;

                if (direction)
                    cur_vel = $signed({1'b0, angle_increment_per_tick});
                else
                    cur_vel = -$signed({1'b0, angle_increment_per_tick});

                if (avg_period > 0)
                    raw_a = ((cur_vel - vel_prev_sector) <<< 10) / $signed({1'b0, avg_period});
                else
                    raw_a = 0;

                if (raw_a > ACC_MAX)      accel <= ACC_MAX;
                else if (raw_a < ACC_MIN) accel <= ACC_MIN;
                else                      accel <= raw_a;

                vel_prev_sector <= cur_vel;
            end
            
        end else begin
            // Угол движется с постоянной скоростью между событиями
            if (direction)
                theta_elec <= theta_elec + angle_increment_per_tick;
            else
                theta_elec <= theta_elec - angle_increment_per_tick;

            // Velocity остаётся константой между событиями
            // (обновляется только при hall_event_good)
        end

        // Полная остановка при таймауте
        if (period_timer == TIMER_MAX && !critical_error) begin
            velocity        <= 0;
            accel           <= 0;
            vel_prev_sector <= 0;
        end
    end
end

    // =========================================================================
    // 8. STUCK-ДЕТЕКТОР
    // =========================================================================
    wire motor_is_moving = (avg_period < MOVING_PERIOD_TH);

    wire [33:0] stuck_raw = {2'b0, avg_period} << 2;
    wire [31:0] dynamic_stuck_timeout =
        (stuck_raw[33:32] != 2'b0)             ? STUCK_TIMEOUT_MAX :
        (stuck_raw[31:0]  > STUCK_TIMEOUT_MAX) ? STUCK_TIMEOUT_MAX :
        (stuck_raw[31:0]  < STUCK_TIMEOUT_MIN) ? STUCK_TIMEOUT_MIN :
                                                  stuck_raw[31:0];

    reg [31:0] stuck_cnt_a, stuck_cnt_b, stuck_cnt_c;
    reg        hall_a_prev_s, hall_b_prev_s, hall_c_prev_s;
    reg        stuck_a, stuck_b, stuck_c;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            stuck_cnt_a   <= 0; stuck_cnt_b   <= 0; stuck_cnt_c   <= 0;
            hall_a_prev_s <= 0; hall_b_prev_s <= 0; hall_c_prev_s <= 0;
            stuck_a       <= 0; stuck_b       <= 0; stuck_c       <= 0;
        end else begin
            hall_a_prev_s <= hall_filt[0];
            hall_b_prev_s <= hall_filt[1];
            hall_c_prev_s <= hall_filt[2];

            if (!startup_done || !motor_is_moving || error_code[5] || error_code[6]) begin
                stuck_cnt_a <= 0; stuck_cnt_b <= 0; stuck_cnt_c <= 0;
                stuck_a     <= 0; stuck_b     <= 0; stuck_c     <= 0;
            end else begin
                if (hall_filt[0] != hall_a_prev_s) begin
                    stuck_cnt_a <= 0; stuck_a <= 0;
                end else if (stuck_cnt_a < dynamic_stuck_timeout)
                    stuck_cnt_a <= stuck_cnt_a + 1'b1;
                else
                    stuck_a <= 1'b1;

                if (hall_filt[1] != hall_b_prev_s) begin
                    stuck_cnt_b <= 0; stuck_b <= 0;
                end else if (stuck_cnt_b < dynamic_stuck_timeout)
                    stuck_cnt_b <= stuck_cnt_b + 1'b1;
                else
                    stuck_b <= 1'b1;

                if (hall_filt[2] != hall_c_prev_s) begin
                    stuck_cnt_c <= 0; stuck_c <= 0;
                end else if (stuck_cnt_c < dynamic_stuck_timeout)
                    stuck_cnt_c <= stuck_cnt_c + 1'b1;
                else
                    stuck_c <= 1'b1;
            end
        end
    end

    // =========================================================================
    // 9. ЕДИНЫЙ БЛОК ОШИБОК
    // =========================================================================
    reg [31:0] invalid_code_cnt;
    reg [7:0]  consec_err_cnt;
    reg [7:0]  err_window_cnt;
    reg [7:0]  good_window_cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            error_code        <= 8'b0;
            invalid_code_cnt  <= 0;
            consec_err_cnt    <= 0;
            err_window_cnt    <= 0;
            good_window_cnt   <= 0;
        end else if (!startup_done) begin
            error_code       <= 8'b0;
            invalid_code_cnt <= 0;
            consec_err_cnt   <= 0;
            err_window_cnt   <= 0;
            good_window_cnt  <= 0;
        end else begin

            // ---- [0] ILLEGAL CODE ----
            if (hall_change_stage2 && !valid_code_stage2) begin
                error_code[0] <= 1'b1;
            end

            // ---- [1] BAD TRANSITION ----
            if (hall_change_stage2 && valid_code_stage2 && valid_1bit_stage2 
                && !valid_step_stage2 && first_valid_seen) begin
                error_code[1] <= 1'b1;
            end

            // ---- [2] GLITCH ----
            if (hall_change_stage2 && valid_code_stage2 && valid_step_stage2
                && (period_timer_stage2 < T_MIN_TICKS) 
                && first_valid_seen 
                && valid_transition_count >= 3'd2) begin  // Проверяем glitch только после 2-х переходов
                error_code[2] <= 1'b1;
            end

            // ---- [3] TIMEOUT ----
            error_code[3] <= (period_timer >= TIMER_MAX) && motor_is_moving;

            // ---- [4] STUCK ----
            if (error_code[5] || error_code[6] || !motor_is_moving)
                error_code[4] <= 1'b0;
            else
                error_code[4] <= stuck_a | stuck_b | stuck_c;

            // ---- [6] INVALID HOLD ----
            if (valid_code_stage2) begin
                invalid_code_cnt <= 0;
                error_code[6]    <= 1'b0;
            end else begin
                if (invalid_code_cnt < INVALID_CODE_TIMEOUT)
                    invalid_code_cnt <= invalid_code_cnt + 1'b1;
                else
                    error_code[6] <= 1'b1;
            end

            // ---- [5] PERSISTENT FAULT ----
            if (hall_change_stage2 && !hall_valid_step && first_valid_seen) begin
                if (consec_err_cnt < 8'hFF) consec_err_cnt <= consec_err_cnt + 1'b1;
                if (err_window_cnt < 8'hFF) err_window_cnt <= err_window_cnt + 1'b1;
                good_window_cnt <= 0;
            end

            if (hall_valid_step) begin
                consec_err_cnt <= 0;
                if (err_window_cnt > 0) err_window_cnt <= err_window_cnt - 1'b1;
                if (good_window_cnt < 8'hFF) good_window_cnt <= good_window_cnt + 1'b1;
            end

            if ((consec_err_cnt >= CONSEC_ERR_LIMIT)
                || (err_window_cnt >= ERR_WINDOW_TH)) begin
                error_code[5]   <= 1'b1;
                good_window_cnt <= 0;
            end

            // ---- Автосброс мгновенных по hall_valid_step ----
            if (hall_valid_step) begin
                error_code[0] <= 1'b0;
                error_code[1] <= 1'b0;
                error_code[2] <= 1'b0;
            end

            // ---- Восстановление persistent ----
            if (error_code[5] && (good_window_cnt >= GOOD_WINDOW_TH)) begin
                error_code[5]  <= 1'b0;
                consec_err_cnt <= 0;
                err_window_cnt <= 0;
            end

            // ---- Timeout сброс ----
            if (period_reset)
                error_code[3] <= 1'b0;
        end
    end

    // =========================================================================
    // 10. ВЫХОДЫ
    // =========================================================================
    assign critical_error  = error_code[4] | error_code[5] | error_code[6];
    assign timeout_warning = error_code[3];

endmodule