//----------------------DATA SEND TO MAIN----------------

module DATA_TRANSMITTER #(
   parameter DATA_WIDTH = 13
) (
   input CLOCK_10,
   input RESET,                  // Сброс состояния, 10 тактов (1000 нс) FO_OUT = 0
   input [DATA_WIDTH-1:0] DATA,  // Требуется подать данные в течение <100 нс после подачи флага READY_TO_SEND 0 -> 1
   output READY_TO_SEND,         // Флаг о готовности передачи нового пакета данных
   output FO_OUT
);

localparam ST_WAIT = 0;
localparam ST_SEND = 1;
localparam ST_RESET = 2;
reg [1:0] state = ST_RESET;

reg [3:0] bit_counter = 0;
reg [3:0] pulse_counter = 0;

reg ready2send = 1;
assign READY_TO_SEND = ready2send;

reg fo_out = 0;
assign FO_OUT = fo_out;

reg [DATA_WIDTH-1:0] data;


// Такт = 100 нс
always @(posedge CLOCK_10) begin
   fo_out <= 0;
   pulse_counter <= pulse_counter + 1;

   if (RESET) begin
      ready2send <= 0;
      pulse_counter <= 1;
      state <= ST_RESET;
   end
   else begin
      case (state)
         ST_WAIT: begin
            data <= DATA;
            ready2send <= 0;
            bit_counter <= 0;
            pulse_counter <= 1;
            state <= ST_SEND;
         end

         ST_SEND: begin
            if (data[bit_counter] == 1 && pulse_counter < 5) begin
               fo_out <= 1;
            end
            else if (pulse_counter == 1) begin  // data[bit_counter] == 0
               fo_out <= 1;
            end

            if (pulse_counter == 5) begin
               bit_counter <= bit_counter + 1;
               pulse_counter <= 1;

               if (bit_counter == DATA_WIDTH - 1) begin
                  ready2send <= 1;
                  state <= ST_WAIT;
               end
            end
         end 

         ST_RESET: begin
            if (pulse_counter == 10) begin
               data <= 0;
               ready2send <= 1;
               state <= ST_WAIT;
            end
         end
      endcase
   end
end

endmodule