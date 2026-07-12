// ------------------------------------------------------------ //
//               Получение значений с AMC1304M25                //
//                                                              //
//                                                              //
// ------------------------------------------------------------ //

module AMC1304M25_READER (
   input  CLOCK_20,          // Часы 20 мГц
   input  [4:1]  AMC_DATA,   // 4 AMC
   output [4:1]  AMC_CLK,    // Часы 20 мГц для AMC
   output [11:0] VOLTAGE     // Напряжение   
);

localparam size_per_cycle = 4095;  // Новое значение примерно за ? нс, обновление раз в ? передачи

// Тактирование 5 мГц для всех AMC
assign AMC_CLK[1] = 0;
assign AMC_CLK[2] = 0;
assign AMC_CLK[3] = 0;
assign AMC_CLK[4] = CLOCK_20;

// Счётчики единиц для каждого канала
// reg [11:0] count_1 = 0;
// reg [11:0] count_2 = 0;
// reg [11:0] count_3 = 0;
reg [11:0] count_4 = 0;

// Счётчик циклов измерения
reg [11:0] cycle_counter = 0;

// Регистры для хранения результата
// reg [11:0] temp_1 = 0;
// reg [11:0] temp_2 = 0;
// reg [11:0] temp_3 = 0;
reg [11:0] voltage = 0;

// assign TEMPERATURE_1 = temp_1;
// assign TEMPERATURE_2 = temp_2;
// assign TEMPERATURE_3 = temp_3;
assign VOLTAGE = voltage;

always @(posedge CLOCK_20) begin
   cycle_counter <= cycle_counter + 12'b1;

   // if (AMC_DATA[1]) count_1 <= count_1 + 12'b1;
   // if (AMC_DATA[2]) count_2 <= count_2 + 12'b1;
   // if (AMC_DATA[3]) count_3 <= count_3 + 12'b1;
   if (AMC_DATA[4]) count_4 <= count_4 + 12'b1;
   
   if (cycle_counter == size_per_cycle) begin
      // Сохранение результата и сброс счетчика
      // temp_1 <= count_1;
      // temp_2 <= count_2;
      // temp_3 <= count_3;
      voltage <= count_4;
      
      // count_1 <= 0;
      // count_2 <= 0;
      // count_3 <= 0;
      count_4 <= 0;
      
      cycle_counter <= 0;
   end
end

endmodule
