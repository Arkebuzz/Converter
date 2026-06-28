// ------------------------------------------------------------ //
//                Получение значений с ADS7886                  //
// ------------------------------------------------------------ //

module ADS7886_READER (
   input CLOCK_5,             // Часы 5 мГц
   input  [3:1]  ADC_DATA,    // Данные 3-х АЦП, по биту на каждый
   output [3:1]  ADC_CLK,
   output [3:1]  ADC_NCS,     // Сигнал выбор чипа, 1->0 для запуска передачи данных
   output [11:0] CURRENT_1,   // Измеренные токи
   output [11:0] CURRENT_2
);

localparam ST_SKIP = 0;
localparam ST_READ = 1;
localparam ST_OFF  = 2;
localparam ST_ON   = 3;
reg [1:0] state = ST_SKIP;

reg [3:0] counter = 15;

reg [11:0] data_1;
reg [11:0] data_2;

reg [11:0] current_1 = 0;
reg [11:0] current_2 = 0;
assign CURRENT_1 = current_1;
assign CURRENT_2 = current_2;

assign ADC_CLK[1] = CLOCK_5;
assign ADC_CLK[2] = CLOCK_5;
assign ADC_CLK[3] = 0;  // 3й не используется

reg nCS = 0;   // chip select
assign ADC_NCS[1] = nCS;
assign ADC_NCS[2] = nCS;
assign ADC_NCS[3] = 1;  // 3й не используется

// Такт = 200 нс
// Один замер считывается за 18 циклов
// То есть первый бит следующего замера будет на 19м подьеме часиков
always @(posedge CLOCK_5) begin
   counter <= counter - 4'b1;
   
   case (state)
      ST_SKIP: begin
         // подождали первые 3 такта мусора
         if (counter == 12) begin
            state <= ST_READ;
         end
      end
      
      ST_READ: begin
         data_1[counter] <= ADC_DATA[1];
         data_2[counter] <= ADC_DATA[2];
         
         if (counter == 0) begin
            state <= ST_OFF;
         end
      end
      
      // Нужно скипнуть 2 такта: чтобы АЦП отдохнула и чтобы АЦП засетапилась
      ST_OFF: begin
         nCS <= 1;   // выключаем АЦП
         counter <= 0;
         state <= ST_ON;
      end
      
      ST_ON: begin
         nCS <= 0;   // включаем АЦП
         state <= ST_SKIP;
         
         current_1 <= data_1;
         current_2 <= data_2;
      end
   endcase
end

endmodule
