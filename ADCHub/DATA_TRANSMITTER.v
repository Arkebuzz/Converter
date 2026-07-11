// TODO: READY_TO_SEND можно вырезать

module DATA_TRANSMITTER #(
   parameter DATA_WIDTH  = 13,   // 128 бит максимум! Правь размер bit_counter здесь и в receiver
   parameter TICK_LEN    = 100,  // Длительность такта передатчика, нс
   // Все следующие параметры должны быть кратны длине такта, иначе работоспособность не гарантируется!
   // Quartus не умеет в проверки. Единицы измерения такие же, что и у длины такта.
   parameter PULSE_0_LEN = 100,  // Длина импульса "1" для 0, меньше, чем PULSE_1_LEN
   parameter PULSE_1_LEN = 400,  // Длина импульса "1" для 1, больше, чем PULSE_0_LEN
   parameter BIT_LEN     = 600,  // Полная длительность передачи 1 бита
   parameter RESET_LEN   = 1000  // Длительность сигнала "0" после пакета данных
) (
   input CLOCK,
   input [DATA_WIDTH-1:0] DATA,  // Требуется подать данные в течение 2 тактов после подачи флага READY_TO_SEND 0 -> 1
   output READY_TO_SEND,         // Сигнал о скорой отправке нового пакета данных
   output FO_OUT
);

localparam PULSE_0_TICKS = PULSE_0_LEN / TICK_LEN;
localparam PULSE_1_TICKS = PULSE_1_LEN / TICK_LEN;
localparam BIT_TICKS     = BIT_LEN     / TICK_LEN;
localparam RESET_TICKS   = RESET_LEN   / TICK_LEN;

localparam ST_RESET = 1'b0;
localparam ST_SEND  = 1'b1;
reg state = ST_RESET;

reg [6:0] bit_counter = 0;    // 128 бит
reg [5:0] pulse_counter = 0;  // 64 бит, 50 мГц потянет

reg ready2send = 0;
assign READY_TO_SEND = ready2send;

reg fo_out = 0;
assign FO_OUT = fo_out;

reg [DATA_WIDTH-1:0] data;

always @(posedge CLOCK) begin
   fo_out <= 0;
   pulse_counter <= pulse_counter + 6'b1;

   case (state)
      ST_RESET: begin  // FO_OUT = 0
         if      (pulse_counter == RESET_TICKS - 2) begin
            ready2send <= 1;
         end
         else if (pulse_counter == RESET_TICKS - 1) begin
            ready2send <= 0;
         end
         else if (pulse_counter == RESET_TICKS) begin
            data <= DATA;
            bit_counter <= 0;
            pulse_counter <= 1;
            state <= ST_SEND;
         end
      end

      ST_SEND: begin
         if      (data[bit_counter] == 1 && pulse_counter <= PULSE_1_TICKS) begin
            fo_out <= 1;
         end
         else if (data[bit_counter] == 0 && pulse_counter <= PULSE_0_TICKS) begin
            fo_out <= 1;
         end

         if (pulse_counter == BIT_TICKS) begin
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
