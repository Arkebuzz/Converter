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
   div_counter <= div_counter + 2'd1;
   CLOCK_10 <= div_counter[0];  // 10 мГц
   CLOCK_5 <= div_counter[1];   // 5  мГц
end


// АЦП ADS7886
wire [11:0] current_1;
wire [11:0] current_2;

ADS7886_READER ADSReader(CLOCK_5, ADC_DATA, ADC_CLK, ADC_NCS, current_1, current_2);


// Передача данных на центральный ПЛИС
localparam OBJ_CUR1 = 0;
localparam OBJ_CUR2 = 1;
reg [2:0] object_to_send = OBJ_CUR1;

reg new_data_to_send = 0;
reg [15:0] data_to_send;
wire ready_to_send;

DATA_TRANSMITTER Transmitter(CLOCK_5, new_data_to_send, data_to_send, ready_to_send, FO_OUTPUT);

// Такт = 200 нс
always @(posedge CLOCK_5) begin
   if (ready_to_send) begin
      case (object_to_send)
         OBJ_CUR1: begin
            data_to_send <= { 4'b001, current_1 };
            new_data_to_send <= 1;
         end

         OBJ_CUR2: begin
            data_to_send <= { 4'b002, current_1 };
            new_data_to_send <= 1;
         end
      endcase
   end else begin
      new_data_to_send <= 0;
   end
end

endmodule
