module DATA_RECEIVER #(
   parameter DATA_WIDTH      = 13,   // 128 бит максимум! Правь размер bit_counter здесь и в transmitter
   parameter TICK_LEN_RECEIV = 20,   // Длительность такта приёмника
   // Все следующие параметры должны быть кратны длине такта, иначе работоспособность не гарантируется!
   // Quartus не умеет в проверки. Единицы измерения такие же, что и у длины такта.
   parameter PULSE_1_LEN     = 400,  // Длина импульса "1" для 1
   parameter RESET_LEN       = 1000, // Длительность сигнала "0" после пакета данных
   parameter MAX_ERROR       = 100   // Длительность разброса реальной длины сигнала
) (
   input CLOCK, 
   input FO_IN, 
   output [DATA_WIDTH-1:0] DATA,  // Полученные данные
   output DATA_READY,             // Сигнал, что данные получены, сигнал "1" 20 нс
   output ERR_CONNECT_FAIL,       // Потеря сигнала 
   output ERR_INVALID_DATA        // Сигнал о завершении пакета пришел в неожиданный момент, "1" в течение всего времени, пока DATA некорректна
);

localparam PULSE_1_TICKS = (PULSE_1_LEN - MAX_ERROR) / TICK_LEN_RECEIV;
localparam RESET_TICKS   = (RESET_LEN   - MAX_ERROR) / TICK_LEN_RECEIV;
localparam CONFAIL_LEN   = 4 * RESET_LEN / TICK_LEN_RECEIV;
localparam CONFAIL_TICKS = (CONFAIL_LEN < 255) ? CONFAIL_LEN : 255;

reg [6:0] bit_counter = 0;  // 128 бит
reg [DATA_WIDTH-1:0] data_temp;
reg [DATA_WIDTH-1:0] data_out;
assign DATA = data_out;

reg data_ready;
assign DATA_READY = data_ready;

reg connect_fail = 0;
assign ERR_CONNECT_FAIL = connect_fail;

reg invalid_data = 0;
assign ERR_INVALID_DATA = invalid_data;

reg curr_inp;
reg prev_inp;
reg [7:0] timer = 0;  // 256 значений, в 4 раза больше, чем на передатчике (см. CONFAIL_LEN)

always @(posedge CLOCK) begin
	curr_inp <= FO_IN;
   prev_inp <= curr_inp;
   data_ready <= 0;

   if (timer < CONFAIL_TICKS) begin
      timer <= timer + 8'b1;
   end else begin
      connect_fail <= 1;
   end
 
   // 0 -> 1
   if (prev_inp == 0 && curr_inp == 1) begin
      if (timer >= RESET_TICKS) begin
         data_out <= data_temp;
         data_temp <= 0;
         bit_counter <= 0;
         invalid_data <= (bit_counter != DATA_WIDTH);
         data_ready <= 1;
      end

      timer <= 0;
      connect_fail <= 0;
   end
   // 1 -> 0
   else if (prev_inp == 1 && curr_inp == 0) begin
      if (timer >= PULSE_1_TICKS) begin
         data_temp[bit_counter] <= 1;
      end
      else begin
         data_temp[bit_counter] <= 0;
      end

      if (bit_counter == DATA_WIDTH) begin  // Получено на бит больше, чем должны были получить
         data_out <= data_temp;
         invalid_data <= 1;
         data_ready <= 1;
      end 
      else if (bit_counter <= DATA_WIDTH) begin
         bit_counter <= bit_counter + 7'b1;
      end

      timer <= 0;
      connect_fail <= 0;
   end
end

endmodule
