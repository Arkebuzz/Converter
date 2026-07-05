module ADC_HUB (
   input CLOCK_20,   // Часы 20 мГц
   input CLOCK_50,   // Часы 20 мГц №2, не используется

   // Не используется, ошибки питания
   input VDR_ERR,
   input VSENS_ERR, 
   input nPGOOD,

   // АЦП AMC1304M25 сигма-дельта 1-3 на температуру и 4 на напряжение
   input  [4:1] AMC_DATA,  // Данные 4-х АЦП, по биту на каждый
   output [4:1] AMC_CLK,   // Назначены часы 20 мГц

   // АЦП ADS7886 - поток бит на ток, используем 1-й
   input  [3:1] ADC_DATA,  // Данные 3-х АЦП, по биту на каждый
   output [3:1] ADC_CLK,   // Часы 5 мГц
   output [3:1] ADC_NCS,   // Сигнал выбор чипа, 1->0 для запуска передачи данных

   // Управление IGBT (6 штуки, пользуем 1 верхний и 1 нижний)
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
   div_counter <= div_counter + 2'b1;
   CLOCK_10 <= div_counter[0];  // 10 мГц
   CLOCK_5 <= div_counter[1];   // 5  мГц
end


// АЦП ADS7886
wire [11:0] current;

ADS7886_READER ADSReader(
   .CLOCK_5(CLOCK_5),   
   .ADC_DATA(ADC_DATA),  
   .ADC_CLK(ADC_CLK),
   .ADC_NCS(ADC_NCS),
   .CURRENT(current)
);


// АЦП AMC1304M25
wire [11:0] voltage;

AMC1304M25_READER AMCReader (
   .CLOCK_20(CLOCK_20),
   .AMC_DATA(AMC_DATA),
   .AMC_CLK(AMC_CLK),
   .VOLTAGE(voltage)
);


// Передача данных на центральный ПЛИС
localparam DATA_WIDTH = 24;  // 1 ток 12 бит + 1 напряжение по 12 бит

reg [DATA_WIDTH-1:0] data_to_send;
wire ready_to_send;

DATA_TRANSMITTER Transmitter (
   .CLOCK_10(CLOCK_10),
   .DATA(data_to_send), 
   .READY_TO_SEND(ready_to_send), 
   .FO_OUT(FO_OUTPUT)
);
defparam Transmitter.DATA_WIDTH = DATA_WIDTH;

always @(posedge ready_to_send) begin
   data_to_send <= {voltage, current};
end

endmodule
