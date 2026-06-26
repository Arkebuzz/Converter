//----------------------DATA RECEIVE FROM ADCHub----------------

module DATA_RECEIVER (
   input CLOCK_10, 
   input FO_IN, 
   output [14:0] DATA,
   output DATA_READY,
   output ERROR
);

reg [3:0] bit_counter;
reg [14:0] data_temp;
reg [14:0] data_out;
assign DATA = data_out;

reg data_ready;
assign DATA_READY = data_ready;

reg error = 0;
assign ERROR = error;

reg prev_inp;

reg [5:0] pos_counter = 0;
reg [5:0] neg_counter = 0;


always@ (posedge CLOCK_10) begin
   prev_inp <= FO_IN;

   if (FO_IN == 1) begin
      if (pos_counter < 31) begin
         pos_counter <= pos_counter + 2'd1;
      end else begin
         error <= 1;
      end
   end 
   else begin
      if (neg_counter < 31) begin
         neg_counter <= neg_counter + 2'd1;
      end else begin
         error <= 1;
      end
   end 
   
   // 0 -> 1
   if (prev_inp == 0 && FO_IN == 1) begin
      if (neg_counter >= 7) begin
         bit_counter <= 0;
         data_out <= data_temp;
         data_temp <= 0;
         data_ready <= 1;
      end else begin
         bit_counter <= bit_counter + 2'd1;
      end

      neg_counter <= 0;
      error <= 0;
   end
   // 1 -> 0
   else if (prev_inp == 1 && FO_IN == 0) begin
      if (pos_counter >= 3) begin
         data_temp[bit_counter] <= 1;
      end
      else begin
         data_temp[bit_counter] <= 0;
      end

      pos_counter <= 0;
      error <= 0;
   end
end

endmodule