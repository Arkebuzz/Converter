// ------------------------------------------------------------ //
//                Получение значений с ADS7886                  //
//                                                              //
//  Значения обновляются каждые 18 тактов на 5 мГц = 3600 нс    //
// ------------------------------------------------------------ //

module ADS7886_READER (
   input CLOCK_5,         // Часы 5 мГц
   input ADC_DATA,        // Данные 3-х АЦП, по биту на каждый
   output ADC_CLK,        // Часы АЦП, 5 мГц
   output ADC_NCS,        // Сигнал выбор чипа, 1->0 для запуска передачи данных
   output [11:0] CURRENT  // Измеренный ток
);

localparam ST_OFF  = 0;
localparam ST_ON   = 1;
localparam ST_SKIP = 2;
localparam ST_READ = 3;
reg [1:0] state = ST_OFF;

reg [3:0] counter;

reg [11:0] data = 0;

reg [11:0] current;
assign CURRENT = current;

assign ADC_CLK = CLOCK_5;

reg nCS = 0;   // chip select
assign ADC_NCS = nCS;

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
         data[counter] <= ADC_DATA;
         
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
         
         current <= data;
      end
   endcase
end

endmodule
