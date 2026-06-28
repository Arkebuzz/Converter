//----------------------DATA RECEIVE FROM ADCHub----------------
// Не используется. Возможно применение на ADCHub.

module DATA_RECEIVER #(
   parameter DATA_WIDTH = 13
) (
   input CLOCK_10, 
   input FO_IN, 
   input RESET,
   output [DATA_WIDTH-1:0] DATA,
   output DATA_READY,
   output ERR_CONNECT_FAIL
);

reg [3:0] bit_counter;
reg [DATA_WIDTH-1:0] data_temp;
reg [DATA_WIDTH-1:0] data_out;
assign DATA = data_out;

reg data_ready;
assign DATA_READY = data_ready;

reg connect_fail = 0;
assign ERR_CONNECT_FAIL = connect_fail;

reg prev_inp;
reg [5:0] signal_counter = 0;

reg [2:0] reset = 0;

always@ (posedge CLOCK_10) begin
   prev_inp <= FO_IN;

   if (RESET) begin
      signal_counter <= 0;
      bit_counter <= 0;
      data_out <= 0;
      data_temp <= 0;
      data_ready <= 0;
      reset <= 7;
   end
   else if (reset == 0) begin
      if (signal_counter < 31) begin
         signal_counter <= signal_counter + 1;
      end else begin
         connect_fail <= 1;
      end
 
      // 0 -> 1
      if (prev_inp == 0 && FO_IN == 1) begin
         if (bit_counter == DATA_WIDTH - 1) begin
            bit_counter <= 0;
            data_out <= data_temp;
            data_temp <= 0;
            data_ready <= 1;
         end else begin
            bit_counter <= bit_counter + 1;
         end

         signal_counter <= 0;
         connect_fail <= 0;
      end
      // 1 -> 0
      else if (prev_inp == 1 && FO_IN == 0) begin
         if (signal_counter >= 3) begin
            data_temp[bit_counter] <= 1;
         end
         else begin
            data_temp[bit_counter] <= 0;
         end

         signal_counter <= 0;
         connect_fail <= 0;
      end
   end
end

endmodule