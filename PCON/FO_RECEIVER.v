// DATA корректна только в момент DATA_READY == 1 && ERR_CONNECT_FAIL == 0 && ERR_INVALID_DATA == 0!

module FO_RECEIVER #(
   parameter DATA_WIDTH      = 64,   // Размер пакета данных
   parameter TICK_LEN_RECEIV = 20,   // Длительность такта приёмника
   // Все следующие параметры должны быть кратны длине такта, иначе работоспособность не гарантируется!
   // Quartus не умеет в проверки. Единицы измерения такие же, что и у длины такта.
   parameter PULSE_1_LEN     = 400,  // Длительность высокого импульса для 1
   parameter RESET_LEN       = 1000, // Длительность низкого импульса после пакета данных
   parameter MAX_ERROR       = 100,  // Длительность разброса реальной длительности импульсов
   parameter CRC_POLY        = 16'hED2F  // Полином CRC
) (
   input CLOCK, 
   input FO_INP, 
   output [DATA_WIDTH-1:0] DATA,  // Полученные данные
   output DATA_READY,             // Сигнал готовности данных - высокий импульс 1 такт
   output ERR_CONNECT_FAIL,       // Потеря сигнала 
   output ERR_INVALID_DATA        // Выскойи импульс в течение всего времени, пока DATA некорректна
);

wire curr_inp;
reg  prev_inp;

STABILIZATOR stab(
   .CLOCK(CLOCK),
   .FO_INP(FO_INP),
   .FO_STAB(curr_inp)
);

localparam PULSE_1_TICKS = (PULSE_1_LEN - MAX_ERROR) / TICK_LEN_RECEIV;
localparam RESET_TICKS   = (RESET_LEN   - MAX_ERROR) / TICK_LEN_RECEIV;
localparam CONFAIL_LEN   = 4 * RESET_LEN / TICK_LEN_RECEIV;
localparam CONFAIL_TICKS = (CONFAIL_LEN < 255) ? CONFAIL_LEN : 255;

localparam DATA_TR_WIDTH = DATA_WIDTH + 16;

localparam COUNTER_WIDTH = $clog2(DATA_TR_WIDTH + 1);
localparam TIMER_WIDTH   = $clog2(CONFAIL_TICKS + 1);

reg [TIMER_WIDTH:0] timer = 0;

wire curr_bit;
assign curr_bit = (timer >= PULSE_1_TICKS);

reg [15:0] crc_calc;

reg [15:0] crc_recv;
reg [DATA_WIDTH-1:0] data;
assign DATA = data;

reg [COUNTER_WIDTH:0] bit_counter = 0;

reg data_ready;
assign DATA_READY = data_ready;

reg connect_fail = 0;
assign ERR_CONNECT_FAIL = connect_fail;

reg invalid_data = 0;
assign ERR_INVALID_DATA = invalid_data;


always @(posedge CLOCK) begin
   prev_inp <= curr_inp;
   data_ready <= 0;

   if (timer < CONFAIL_TICKS) begin
      timer <= timer + 1'b1;
   end else begin
      connect_fail <= 1;
   end
 
   // 0 -> 1
   if (prev_inp == 0 && curr_inp == 1) begin
      if (timer >= RESET_TICKS) begin
         bit_counter <= 0;
         crc_calc <= 0;
         invalid_data <= (bit_counter != DATA_TR_WIDTH) || crc_calc != crc_recv;
         data_ready <= 1;
      end

      timer <= 0;
      connect_fail <= 0;
   end
   // 1 -> 0
   else if (prev_inp == 1 && curr_inp == 0) begin
      if (bit_counter < DATA_WIDTH) begin
         bit_counter <= bit_counter + 1'b1;
         data <= {curr_bit, data[DATA_WIDTH-1:1]};

         if (crc_calc[15] ^ curr_bit) begin
            crc_calc <= {crc_calc[14:0], 1'b0} ^ CRC_POLY;
         end else begin
            crc_calc <= {crc_calc[14:0], 1'b0};
         end
      end 
      else if (bit_counter < DATA_TR_WIDTH) begin
         bit_counter <= bit_counter + 1'b1;
         crc_recv <= {curr_bit, crc_recv[15:1]};
      end 
      else begin
         invalid_data <= 1;
         data_ready <= 1;
      end

      timer <= 0;
      connect_fail <= 0;
   end
end

endmodule


module STABILIZATOR #(
   parameter [2:0] LATCH = 3
) (
   input CLOCK,
   input FO_INP,
   output reg FO_STAB = 0
);

reg [2:0] counter = 0;
reg curr = 0;

always @(posedge CLOCK) begin
   curr <= FO_INP;

   if (FO_STAB != curr) begin
      counter <= counter + 1'b1;

      if (counter > LATCH) begin
         FO_STAB <= curr;
         counter <= 0;
      end
   end else begin
      counter <= 0;
   end
end

endmodule