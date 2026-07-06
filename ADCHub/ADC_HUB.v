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
reg CLOCK_5;

// Такт = 50 нс
always @(posedge CLOCK_20) begin
   div_counter <= div_counter + 2'b1;
   CLOCK_5 <= div_counter[1];   // 5  мГц
end


// Управление транзисторами
reg converter_on = 0;
reg mode_up;
reg [12:0] pwm_counter;

PWM_GENERATOR GenPWM (
   .CLOCK_20(CLOCK_20), 
   .CONVERTER_ON(converter_on),
   .MODE_UP(mode_up),
   .PWM(pwm_counter),
   .CTRL_TOP(CTRL_TOP[1]),  // Хз че с индексами
   .CTRL_BOT(CTRL_BOT[2])   // Хз че с индексами
);


// АЦП ADS7886
wire [11:0] current;

ADS7886_READER ADSReader(
   .CLOCK_5(CLOCK_5),   
   .ADC_DATA(ADC_DATA[1]),  
   .ADC_CLK(ADC_CLK[1]),
   .ADC_NCS(ADC_NCS[1]),
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
localparam DATA_OUT_WIDTH = 32;  // 1 ток 12 бит + 1 напряжение по 12 бит + 8 бит ошибок

reg [DATA_OUT_WIDTH-1:0] data_to_send;
wire ready_to_send;

DATA_TRANSMITTER Transmitter (
   .CLOCK(CLOCK_20),
   .DATA(data_to_send), 
   .READY_TO_SEND(ready_to_send), 
   .FO_OUT(FO_OUTPUT)
);
defparam Transmitter.DATA_WIDTH = DATA_OUT_WIDTH;
defparam Transmitter.TICK_LEN = 50;
defparam Transmitter.PULSE_0_LEN = 100;
defparam Transmitter.PULSE_1_LEN = 400;
defparam Transmitter.BIT_LEN = 600;    
defparam Transmitter.RESET_LEN = 1000;


// Получение данных с центрального ПЛИС
localparam DATA_INP_WIDTH = 16;

wire [DATA_INP_WIDTH-1:0] receiv_data;
wire rc_data_ready;
wire rc_connect_fail;
wire rc_invalid_data;

DATA_RECEIVER Receiver (
   .CLOCK(CLOCK_20), 
   .FO_IN(FO_INPUT), 
   .DATA(receiv_data),
   .DATA_READY(rc_data_ready),
   .ERR_CONNECT_FAIL(rc_connect_fail),
   .ERR_INVALID_DATA(rc_invalid_data)
);
defparam Receiver.DATA_WIDTH = DATA_INP_WIDTH;
defparam Receiver.TICK_LEN_RECEIV = 50;  // 20 мГц
defparam Receiver.PULSE_1_LEN     = 400;  
defparam Receiver.RESET_LEN       = 1000; 
defparam Receiver.MAX_ERROR       = 100; 

reg reset_errors;
reg [3:0] errors;
reg [3:0] errors_latch;

always @(posedge CLOCK_20) begin
   errors[0] <= (~IGBT_ERR[1]) || (~IGBT_ERR[2]);
   errors[1] <= (~IGBT_ERR[3]) || (~IGBT_ERR[4]);
   errors[2] <= current > 3208 || current < 204;  // хз что тут за пределы, надо спросить
   errors[3] <= rc_connect_fail | rc_invalid_data;
   
   errors_latch <= errors_latch | errors;  
   // Сброс защелки только по сигналу свыше
   if (reset_errors) begin
      errors_latch <= 0;
      reset_errors <= 0;
   end
   
   if (ready_to_send) begin
      data_to_send <= {voltage, current, errors_latch, errors};
   end   
   
   if (rc_data_ready && rc_invalid_data == 0) begin
      {pwm_counter, mode_up, converter_on, reset_errors} <= receiv_data;
   end
end

endmodule
