//----------------------DATA SEND TO MAIN----------------

module DATA_TRANSMITTER (
   input CLOCK_10, 
   input DATA_READY,      // Флаг о наличии новых данных на отправку
   input [15:0] DATA,     // Данные на отправку
   output READY_TO_SEND,  // Флаг о готовности отправки следующего набора данных
   output FO_OUT
);

localparam ST_WAIT = 0;
localparam ST_SEND = 1;
localparam ST_FINISH = 2;
reg [1:0] state = ST_FINISH;

reg [3:0] bit_counter = 0;
reg [3:0] pulse_counter = 0;

reg ready2send = 1;
assign READY_TO_SEND = ready2send;

reg fo_out = 0;
assign FO_OUT = fo_out;


always @(posedge CLOCK_10) begin
   pulse_counter <= pulse_counter + 1;

   case (state)
      ST_WAIT: begin
         if (DATA_READY) begin
            ready2send <= 0;
            bit_counter <= 0;
            pulse_counter <= 0;
            state <= ST_SEND;
         end
      end
   
      ST_SEND: begin
         if (DATA[bit_counter] == 1) begin
            if (pulse_counter < 4) begin
               fo_out <= 1;
            end else begin
               fo_out <= 0;
            end
         end
         else begin
            if (pulse_counter < 1) begin
               fo_out <= 1;
            end else begin
               fo_out <= 0;
            end
         end

         if (pulse_counter == 5) begin
            bit_counter <= bit_counter + 1;
            pulse_counter <= 0;

            if (bit_counter == 15) begin
               state <= ST_FINISH;
            end
         end
      end 

      ST_FINISH: begin
         ready2send = 1;

         if (pulse_counter == 9) begin
            state <= ST_WAIT;
         end
      end
   endcase
end

endmodule