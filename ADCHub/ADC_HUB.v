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
// reg CLOCK_10;
reg CLOCK_5;

// АЦП ADS7886
wire [11:0] current_1;
wire [11:0] current_2;

ADS7886_READER ADSReader(CLOCK_5, ADC_DATA, ADC_CLK, ADC_NCS, current_1, current_2);

// Такт = 50 нс
always @(posedge CLOCK_20) begin
   div_counter <= div_counter + 2'd1;
   // CLOCK_10 <= div_counter[0];  // 10 мГц
   CLOCK_5 <= div_counter[1];   // 5  мГц
end

endmodule
