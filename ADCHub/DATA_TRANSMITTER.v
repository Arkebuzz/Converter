//----------------------DATA SEND TO MAIN----------------

module DATA_TRANSMITTER #(
   parameter DATA_WIDTH = 13  // 128 бит максимум! Правь размер bit_counter здесь и в receiver
) (
   input CLOCK_10,
   input [DATA_WIDTH-1:0] DATA,  // Требуется подать данные в течение <100 нс после подачи флага READY_TO_SEND 0 -> 1
   output READY_TO_SEND,         // Флаг о готовности передачи нового пакета данных
   output FO_OUT
);

localparam PULSE_1_LEN = 5;    // Длина импульса для 1
localparam PULSE_0_LEN = 1;    // Длина импульса для 0
localparam BIT_PERIOD = 5;     // Полный период на бит
localparam RESET_PULSES = 10;  // Количество тактов для сброса 

localparam ST_RESET = 1'b0;
localparam ST_SEND  = 1'b1;
reg state = ST_RESET;

reg [6:0] bit_counter = 0;  // 128 бит
reg [3:0] pulse_counter = 0;

reg ready2send = 0;
assign READY_TO_SEND = ready2send;

reg fo_out = 0;
assign FO_OUT = fo_out;

reg [DATA_WIDTH-1:0] data;


// Такт = 100 нс
always @(posedge CLOCK_10) begin
   fo_out <= 0;
   pulse_counter <= pulse_counter + 4'b1;

   case (state)
      ST_RESET: begin  // 1000 нс FO_OUT=0
         if (pulse_counter == RESET_PULSES - 1) begin
            ready2send <= 1;
         end
         else if (pulse_counter == RESET_PULSES) begin
            data <= DATA;
            ready2send <= 0;
            bit_counter <= 0;
            pulse_counter <= 1;
            state <= ST_SEND;
         end
      end

      ST_SEND: begin
         if (data[bit_counter] == 1 && pulse_counter < PULSE_1_LEN) begin
            fo_out <= 1;
         end
         else if (pulse_counter == PULSE_0_LEN) begin  // data[bit_counter] == 0
            fo_out <= 1;
         end

         if (pulse_counter == BIT_PERIOD) begin
            bit_counter <= bit_counter + 7'b1;
            pulse_counter <= 1;

            if (bit_counter == DATA_WIDTH - 1) begin
               state <= ST_RESET;
            end
         end
      end 
   endcase
end

endmodule