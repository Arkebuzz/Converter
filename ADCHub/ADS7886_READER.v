// ------------------------------------------------------------ //
//                Получение значений с ADS7886                  //
//                                                              //
//  Полный цикл: 4000 нс = 4 мс = 20 тактов  (1 такт = 200 нс)  //
//  Для доп. информации смотри временную диаграмму в Docs       //
// ------------------------------------------------------------ //


module ADS7886_READER (
   input CLOCK_5,         // Часы 5 мГц
   input [3:1] ADC_DATA,  // Данные 3-х АЦП, по биту на каждый

   // output [3:1] ADC_CLK,  // Часы 5  мГц со сдвигом влево на 50 нс, ГЕНЕРИРУЕТСЯ ВНЕ МОДУЛЯ!
   output [3:1]  ADC_NCS,    // Сигнал выбор чипа, 1->0 для запуска передачи данных
   output [11:0] CURRENT_1,  // Измеренные токи
   output [11:0] CURRENT_2
);


localparam ST_INIT = 0;
localparam ST_SKIP = 1;
localparam ST_READ = 2;
localparam ST_SAVE = 3;
localparam ST_WAIT = 4;


reg nChipSelect;
reg [2:0] state;
reg [3:0] counter;

reg [11:0] data_1;
reg [11:0] data_2;
reg [11:0] data_3;

reg [11:0] current_1;
reg [11:0] current_2;

assign ADC_NCS[1] = nChipSelect;
assign ADC_NCS[2] = nChipSelect;
assign ADC_NCS[3] = nChipSelect;
assign CURRENT_1 = current_1;
assign CURRENT_2 = current_2;


initial begin
   state <= 0;
   nChipSelect <= 1;
end


always @(posedge CLOCK_5) begin
   if (state == ST_INIT) begin  // nCS: 1 -> 0
      nChipSelect <= 0;
      counter <= 0;
      state <= ST_SKIP;
   end
   else if (state == ST_SKIP) begin  // Пропуск первых 4 нулей
      if (counter == 2) begin
         state <= ST_READ;
         counter <= 11;
      end else begin
         counter <= counter + 1;
      end
   end
   else if (state == ST_READ) begin  // Чтение значащих 12 бит
      data_1[counter] <= ADC_DATA[1];
      data_2[counter] <= ADC_DATA[2];
      
      if (counter == 0) begin
         state <= ST_SAVE;
         counter <= 0;
      end else begin
         counter <= counter - 1;
      end
   end
   else if (state == ST_SAVE) begin  
      // Запись результата
      current_1 <= data_1;
      current_2 <= data_2;
      
      // Переход в состояние ожидания
      nChipSelect <= 1;
      state <= ST_WAIT;
   end
   else if (state == ST_WAIT) begin  // Ожидание следующего цикла
      if (counter == 2) begin
         state <= ST_INIT;
      end else begin
         counter <= counter + 1;
      end
   end
end

endmodule
