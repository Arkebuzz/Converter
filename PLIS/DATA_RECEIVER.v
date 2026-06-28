//----------------------DATA RECEIVE FROM ADCHub----------------

module DATA_RECEIVER #(
   parameter DATA_WIDTH = 13      // 128 бит максимум!
) (
   input CLOCK_50, 
   input FO_IN, 
   output [DATA_WIDTH-1:0] DATA,  // Полученные данные
   output DATA_READY,             // Сигнал, что данные получены, сигнал "1" 20 нс
   output ERR_CONNECT_FAIL,       // Потеря сигнала 
   output ERR_INVALID_DATA        // Сигнал о завершении пакета пришел в неожиданный момент, "1" в течение всего времени, пока DATA некорректна
);

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

reg prev_inp;
reg [7:0] signal_counter = 0;


// Такт = 20 нс
always @(posedge CLOCK_50) begin
   prev_inp <= FO_IN;
   data_ready <= 0;

   if (signal_counter < 255) begin
      signal_counter <= signal_counter + 1;
   end else begin
      connect_fail <= 1;
   end
 
   // 0 -> 1
   if (prev_inp == 0 && FO_IN == 1) begin
      if (signal_counter >= 45) begin
         data_out <= data_temp;
         data_temp <= 0;
         bit_counter <= 0;
         invalid_data <= (bit_counter != DATA_WIDTH);
         data_ready <= 1;
      end

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

      if (bit_counter == DATA_WIDTH) begin  // Получено на бит больше, чем должны были получить
         data_out <= data_temp;
         invalid_data <= 1;
         data_ready <= 1;
      end 
      else if (bit_counter <= DATA_WIDTH) begin
         bit_counter <= bit_counter + 1;
      end

      signal_counter <= 0;
      connect_fail <= 0;
   end
end

endmodule