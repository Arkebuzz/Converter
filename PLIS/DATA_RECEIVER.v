//----------------------DATA RECEIVE FROM ADCHub----------------

module DATA_RECEIVER #(
   parameter DATA_WIDTH = 13
) (
   input CLOCK_50, 
   input FO_IN, 
   input RESET,                   // Сброс состояния, запускать синхронно с отправкой сигнала сброса на transmitter, 800 нс не читаем вход.
   output [DATA_WIDTH-1:0] DATA,  // Полученные данные
   output DATA_READY,             // Сигнал, что данные получены, сигнал "1" 20 нс
   output ERR_CONNECT_FAIL        // Потеря сигнала 
);

reg [3:0] bit_counter = 0;
reg [DATA_WIDTH-2:0] data_temp;  // Последний бит сразу в data_out
reg [DATA_WIDTH-1:0] data_out;
assign DATA = data_out;

reg data_ready;
assign DATA_READY = data_ready;

reg connect_fail = 0;
assign ERR_CONNECT_FAIL = connect_fail;

reg prev_inp;
reg [7:0] signal_counter = 0;

reg [5:0] reset = 0;

always @(posedge CLOCK_50) begin
   prev_inp <= FO_IN;
   data_ready <= 0;

   if (reset > 0) begin
      reset <= reset - 1;
   end

   if (RESET) begin
      signal_counter <= 0;
      bit_counter <= 0;
      data_out <= 0;
      data_temp <= 0;
      reset <= 40;
   end
   else if (reset == 0) begin
      if (signal_counter < 255) begin
         signal_counter <= signal_counter + 1;
      end else begin
         connect_fail <= 1;
      end
 
      // 0 -> 1
      if (prev_inp == 0 && FO_IN == 1) begin
         signal_counter <= 0;
         connect_fail <= 0;
      end
      // 1 -> 0
      else if (prev_inp == 1 && FO_IN == 0) begin
         if (signal_counter >= 17) begin
            data_temp[bit_counter] <= 1;
         end
         else begin
            data_temp[bit_counter] <= 0;
         end

         if (bit_counter == DATA_WIDTH - 1) begin
            bit_counter <= 0;
            data_out <= { signal_counter >= 17, data_temp };
            data_temp <= 0;
            data_ready <= 1;
         end else begin
            bit_counter <= bit_counter + 1;
         end

         signal_counter <= 0;
         connect_fail <= 0;
      end
   end
end

endmodule