module ADC_HUB (
   input CLOCK_20,   // Часы 20 мГц
   input CLOCK_50,   // Не используется

   // Не используется, ошибки питания
   input VDR_ERR,
   input VSENS_ERR, 
   input nPGOOD,

   // АЦП сигма-дельта на напряжение, 4 штуки
   input  [4:1] AMC_DATA,  // Сигма-дельта АЦП
   output [4:1] AMC_CLK,   // Назначены часы 20 мГц ?

   // АЦП ADS7886 - поток бит на ток, 3 штуки
   input  [3:1] ADC_DATA,  // Данные 3-х АЦП, по биту на каждый
   output [3:1] ADC_CLK,   // Часы 5 мГц
   output [3:1] ADC_NCS,   // Сигнал выбор чипа, 1->0 для запуска передачи данных

   // Управление верхними IGBT (3 штуки, пользуем 1)
   input  [6:1] IGBT_ERR,
   output [3:1] CTRL_TOP,
   output [3:1] CTRL_BOT,

   // Оптоволокно c главной ПЛИС
   input  FO_INPUT,
   output FO_OUTPUT,
   output FO_nEN      // Включение приемника оптоволокна
);

// Включение приемника оптоволокна
assign FO_nEN = 0;


// Часы меньшей герцовки
reg [1:0] div_counter = 0;
reg CLOCK_10;
reg CLOCK_5;

// Такт = 50 нс
always @(posedge CLOCK_20) begin
   div_counter <= div_counter + 1;
   CLOCK_10 <= div_counter[0];  // 10 мГц
   CLOCK_5 <= div_counter[1];   // 5  мГц
end


// АЦП ADS7886
wire [11:0] current_1;
wire [11:0] current_2;

ADS7886_READER ADSReader(
   .CLOCK_5(CLOCK_5),   
   .ADC_DATA(ADC_DATA),  
   .ADC_CLK(ADC_CLK),
   .ADC_NCS(ADC_NCS),
   .CURRENT_1(current_1), 
   .CURRENT_2(current_2)
);


// Передача данных на центральный ПЛИС
localparam DATA_WIDTH = 13;  // 13 бит под флаг для ошибок. Если возникла ошибка, прерываем текущую передачу и отправляем 12-битный пакет с ошибками.
                             // А может и нахрен не надо, можно безболененно выкинуть.
localparam OBJ_CUR1 = 0;
localparam OBJ_CUR2 = 1;
reg [2:0] object_to_send = OBJ_CUR1;

reg tr_reset = 0;  // Может приходить с центральной ПЛИС для синхронизации

reg [DATA_WIDTH-1:0] data_to_send;
wire ready_to_send;

DATA_TRANSMITTER Transmitter (
   .CLOCK_10(CLOCK_10), 
   .RESET(tr_reset), 
   .DATA(data_to_send), 
   .READY_TO_SEND(ready_to_send), 
   .FO_OUT(FO_OUTPUT)
);
defparam Transmitter.DATA_WIDTH = DATA_WIDTH;

always @(posedge ready_to_send) begin
   case (object_to_send)
      OBJ_CUR1: begin
         data_to_send <= { 1'b0, current_1 };
         object_to_send <= OBJ_CUR2;
      end

      OBJ_CUR2: begin
         data_to_send <= { 1'b0, current_2 };
         object_to_send <= OBJ_CUR1;
      end
   endcase
end

endmodule
