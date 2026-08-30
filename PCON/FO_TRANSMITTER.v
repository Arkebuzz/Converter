module FO_TRANSMITTER #(
   parameter DATA_WIDTH  = 64,   // Размер пакета данных
   parameter TICK_LEN    = 20,   // Длительность такта передатчика
   // Все следующие параметры должны быть кратны длине такта, иначе работоспособность не гарантируется!
   // Quartus не умеет в проверки. Единицы измерения такие же, что и у длины такта.
   parameter PULSE_0_LEN = 160,  // Длина высокого импульса для передачи 0, меньше, чем PULSE_1_LEN
   parameter PULSE_1_LEN = 400,  // Длина высокого импульса для передачи 1, больше, чем PULSE_0_LEN
   parameter BIT_LEN     = 560,  // Полная длительность передачи одного бита
   parameter RESET_LEN   = 1000, // Длительность низкого импульса после пакета данных
   parameter CRC_POLY    = 16'hED2F  // Полином CRC
) (
   input CLOCK,
   input [DATA_WIDTH-1:0] DATA,  // Требуется подать данные в течение 2 тактов после подачи флага READY_TO_SEND 0 -> 1
   output READY_TO_SEND,         // Сигнал о скорой отправке нового пакета данных
   output FO_OUT
);

reg ready2send = 0;
assign READY_TO_SEND = ready2send;

reg fo_out = 0;
assign FO_OUT = fo_out;

localparam PULSE_0_TICKS = PULSE_0_LEN / TICK_LEN;
localparam PULSE_1_TICKS = PULSE_1_LEN / TICK_LEN;
localparam BIT_TICKS     = BIT_LEN     / TICK_LEN;
localparam RESET_TICKS   = RESET_LEN   / TICK_LEN;

localparam DATA_TR_WIDTH = DATA_WIDTH + 16;

localparam COUNTER_WIDTH = $clog2(DATA_TR_WIDTH);
localparam TIMER_WIDTH   = $clog2(RESET_TICKS + 1);

localparam ST_RESET = 1'b0;
localparam ST_SEND  = 1'b1;
reg state = ST_RESET;

reg [15:0] crc_reg;

reg [DATA_TR_WIDTH-1:0] data;
reg [COUNTER_WIDTH:0] bit_counter = 0;
reg [TIMER_WIDTH:0] timer = 0;

wire bit_to_send;
assign bit_to_send = data[bit_counter];


always @(posedge CLOCK) begin
   fo_out <= 0;
   timer <= timer + 1'b1;

   case (state)
      ST_RESET: begin  // FO_OUT = 0
         if          (timer == RESET_TICKS - 2) begin
            ready2send <= 1;
         end else if (timer == RESET_TICKS - 1) begin
            ready2send <= 0;
         end else if (timer == RESET_TICKS) begin
            data <= DATA;
            bit_counter <= 0;
            timer <= 1;
            crc_reg <= 0;
            state <= ST_SEND;
         end
      end

      ST_SEND: begin
         if (timer == 1 && bit_counter < DATA_WIDTH) begin
            if (crc_reg[15] ^ bit_to_send) begin
               crc_reg <= {crc_reg[14:0], 1'b0} ^ CRC_POLY;
            end else begin
               crc_reg <= {crc_reg[14:0], 1'b0};
            end
         end else begin
            data[DATA_TR_WIDTH-1:DATA_WIDTH] <= crc_reg;
         end

         if          (bit_to_send == 1 && timer <= PULSE_1_TICKS) begin
            fo_out <= 1;
         end else if (bit_to_send == 0 && timer <= PULSE_0_TICKS) begin
            fo_out <= 1;
         end

         if (timer == BIT_TICKS) begin
            timer <= 1;
            bit_counter <= bit_counter + 1'b1;

            if (bit_counter == DATA_TR_WIDTH - 1) begin
               state <= ST_RESET;
            end
         end
      end
   endcase
end

endmodule
