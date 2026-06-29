// ------------------------------------------------------------ //
//               Получение значений с AMC1304M25                //
// ------------------------------------------------------------ //

module AMC1304M25_READER (
   input  CLOCK_20,          // Часы 20 мГц
   input  [4:1] AMC_DATA,    // 4 AMC
   output [4:1] AMC_CLK,     // Часы 20 мГц для AMC
   output [9:0] VOLTAGE_1,   // Напряжение с канала 1 и т.д.
   output [9:0] VOLTAGE_2      
   // output [9:0] VOLTAGE_3,      
   // output [9:0] VOLTAGE_4       
);

// Тактирование 5 мГц для всех AMC
assign AMC_CLK[1] = CLOCK_20;
assign AMC_CLK[2] = CLOCK_20;
assign AMC_CLK[3] = 0;
assign AMC_CLK[4] = 0;

// Счётчики единиц для каждого канала
reg [9:0] count_1 = 0;
reg [9:0] count_2 = 0;
// reg [9:0] count_3 = 0;
// reg [9:0] count_4 = 0;

// Счётчик циклов измерения
reg [9:0] cycle_counter = 0;

// Регистры для хранения результата
reg [9:0] voltage_1 = 0;
reg [9:0] voltage_2 = 0;
// reg [9:0] voltage_3 = 0;
// reg [9:0] voltage_4 = 0;

assign VOLTAGE_1 = voltage_1;
assign VOLTAGE_2 = voltage_2;
// assign VOLTAGE_3 = voltage_3;
// assign VOLTAGE_4 = voltage_4;

localparam size_per_cycle = 1023;  // Новое значение примерно за 51200 нс, обновление раз в 2 передачи

always @(posedge CLOCK_20) begin
   cycle_counter <= cycle_counter + 10'b1;

   if (AMC_DATA[1]) count_1 <= count_1 + 10'b1;
   if (AMC_DATA[2]) count_2 <= count_2 + 10'b1;
   // if (AMC_DATA[3]) count_3 <= count_3 + 10'b1;
   // if (AMC_DATA[4]) count_4 <= count_4 + 10'b1;
   
   if (cycle_counter == size_per_cycle) begin
      // Сохранение результата и сброс счетчика
      voltage_1 <= count_1;
      voltage_2 <= count_2;
      // voltage_3 <= count_3;
      // voltage_4 <= count_4;
      
      count_1 <= 0;
      count_2 <= 0;
      // count_3 <= 0;
      // count_4 <= 0;
      
      cycle_counter <= 0;
   end
end

endmodule