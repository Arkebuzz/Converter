module INV3PH (
   clock_20,   // Часы 20 MHz
   clock_50,   // Не используется
   AMC_DATA,   // 
   AMC_CLK,    // Назначены часы 20 MHz ?
   ADC_DATA,   // Данные 3-х АЦП, по биту на каждый
   ADC_CLK,    // Назначены часы 5 MHz
   ADC_NCS,    // Сигнал выбор чипа, 1->0 для запуска передачи данных
   VDR_ERR,    // Не используется
   VSENS_ERR,  // Не используется
   nPGOOD,     // Не используется
   CTRL_TOP,   // Управление верхними IGBT (3 штуки, пользуем 1)
   CTRL_BOT,   // Управление нижними  IGBT (3 штуки, пользуем 1)
   FO_INPUT,   // Оптоволокно ОТ главной ПЛИС
   FO_OUTPUT,  // Оптоволокно К  главной ПЛИС
   FO_nEN,     // Присвоено 0 ?
   IGBT_ERR    // Ошибки IGBT
);

input clock_20;
input clock_50;
input [4:1] AMC_DATA;
input [3:1] ADC_DATA;
input VDR_ERR;
input VSENS_ERR;
input nPGOOD;
input FO_INPUT;
input [6:1] IGBT_ERR;

output [4:1] AMC_CLK;
output [3:1] ADC_CLK;
output [3:1] ADC_NCS;
output [3:1] CTRL_TOP;
output [3:1] CTRL_BOT;
output FO_OUTPUT;
output FO_nEN;


// Так было, пусть будет:
assign FO_nEN = 0;


// Часы меньшей герцовки
reg [1:0] div_counter;
reg clock_10 = 0;
reg clock_5 = 0;


// АЦП ADS7886

reg adc_clock;
reg adc_chipselect;
reg [2:0] adc_state;
reg [3:0] adc_counter;
reg [11:0] adc1_data;
reg [11:0] adc2_data;
// Выход тока:
reg [11:0] current1;
reg [11:0] current2;

assign ADC_CLK[1] = adc_clock;
assign ADC_CLK[2] = adc_clock;
assign ADC_CLK[3] = 0;
assign ADC_NCS[1] = adc_chipselect;
assign ADC_NCS[2] = adc_chipselect;
assign ADC_NCS[3] = 0;


initial begin
   div_counter = 0;
   
   // Настройка АЦП ADS7886
   adc_clock <= 1;
   adc_chipselect <= 1;
   adc_state <= 0;
end


// Такт = 50 нс
always @(posedge clock_20) begin
   div_counter <= div_counter + 1;
   clock_10 <= div_counter[0];  // 10 MHz
   clock_5 <= div_counter[1];   // 5 MHz
end


// Такт = 100 нс
always @(posedge clock_10) begin
   // ------------------------------------------------------------ //
   //                Получение значений с ADS7886                  //
   //                                                              //
   //  Полный цикл: 4000 нс = 4 мс = 20 тактов                     //
   // ------------------------------------------------------------ //
   
   if (adc_state == 0) begin  // Инициализация АЦП, nCS: 1 -> 0
      adc_clock = 1;
      adc_chipselect <= 0;
      adc_counter <= 0;
      adc_state <= 1;
   end
   else if (adc_state == 1) begin  // Пропуск первых 4 нулей
      adc_clock <= ~adc_clock;
      
      if (adc_counter == 6) begin
         adc_state <= 2;
         adc_counter <= 11;
      end else begin
         adc_counter <= adc_counter + 1;
      end
   end
   else if (adc_state == 2) begin  // Чтение значащих 12 бит
      adc_clock <= ~adc_clock;
      
      if (adc_clock == 0) begin
         adc1_data[adc_counter] <= ADC_DATA[1];
         adc2_data[adc_counter] <= ADC_DATA[2];
         adc_counter <= adc_counter - 1;  // Выполняется в конце, в adc_data старое значение
      end
      
      if (adc_counter == 15) begin // adc_counter == -1
         adc_state <= 3;
      end
   end
   else if (adc_state == 3) begin  
      // Запись результата
      current1 <= adc1_data;
      current2 <= adc2_data;
      
      // Переход в состояние ожидания
      adc_chipselect <= 1;
      adc_clock = 1;
      
      adc_counter <= 0;
      adc_state <= 4;
   end
   else if (adc_state == 4) begin  // Ожидание следующего цикла
      adc_counter <= adc_counter + 1;
      
      if (adc_counter == 6) begin
         adc_state <= 0;
      end
   end
end

endmodule

//reg [11:0] ADC1_DATA;
//reg [11:0] ADC2_DATA;
//reg [11:0] ADC3_DATA;
//
//reg [2:0] ADC_SM_STATE;
//reg [3:0] ADC_SM_COUNTER;
//reg ADC_CHIPSELECT;
//reg ADC_CLOCK;
//reg [1:0] ADC_CLOCK_DIV;
//
//reg TOPCTRL_1;
//reg TOPCTRL_2;
//reg TOPCTRL_3;
//reg BOTCTRL_1;
//reg BOTCTRL_2;
//reg BOTCTRL_3;
//
//reg TOP1;
//reg TOP2;
//reg TOP3;
//reg BOT1;
//reg BOT2;
//reg BOT3;
//
//reg [5:0] TOP1_DT;
//reg [5:0] TOP2_DT;
//reg [5:0] TOP3_DT;
//reg [5:0] BOT1_DT;
//reg [5:0] BOT2_DT;
//reg [5:0] BOT3_DT;
//
//reg FO_OUT;
//reg [1:0] DATA_SEND_SM;
//reg [4:0] PULSE_SEND_COUNTER;
//reg [3:0] BIT_SEND_COUNTER;
//reg [2:0] DATA_SEND_COUNTER;
//reg BitToSend;
//
//reg [11:0] CURRENT_A;
//reg [11:0] CURRENT_B;
//reg [11:0] CURRENT_C;
//reg [11:0] VOLTAGE_IN;
//reg [11:0] TEMP_OUT;
//
//reg [1:0] TEMP_SM;
//reg [11:0] TEMP_DATA_REG1;
//reg [1:0] TEMP_DATA_COUNTER;
//
//reg [11:0] TEMP_DATA_REG4;
//reg [11:0] TEMP_COUNTER;
//
//assign ADC_CLK[1] = ADC_CLOCK;
//assign ADC_CLK[2] = ADC_CLOCK;
//assign ADC_CLK[3] = ADC_CLOCK;
//assign ADC_NCS[1] = ADC_CHIPSELECT;
//assign ADC_NCS[2] = ADC_CHIPSELECT;
//assign ADC_NCS[3] = ADC_CHIPSELECT;
//assign AMC_CLK[1] = clock_20;
//assign AMC_CLK[2] = clock_20;
//assign AMC_CLK[3] = clock_20;
//assign AMC_CLK[4] = clock_20;
//
//`ifdef LEFT
//assign CTRL_TOP[1] = TOPCTRL_1;
//assign CTRL_TOP[2] = TOPCTRL_2;
//assign CTRL_TOP[3] = TOPCTRL_3;  //NEW - TOPCTRL_3, OLD - BOTCTRL_3
//assign CTRL_BOT[1] = BOTCTRL_1;
//assign CTRL_BOT[2] = BOTCTRL_2;
//assign CTRL_BOT[3] = BOTCTRL_3;  //NEW - BOTCTRL_3, OLD - TOPCTRL_3
//`endif
//
//`ifdef RIGHT
//assign CTRL_TOP[1] = TOPCTRL_3;  //NEW - TOPCTRL_3, OLD - BOTCTRL_3
//assign CTRL_TOP[2] = TOPCTRL_2;
//assign CTRL_TOP[3] = TOPCTRL_1;
//assign CTRL_BOT[1] = BOTCTRL_3;  //NEW - BOTCTRL_3, OLD - TOPCTRL_3
//assign CTRL_BOT[2] = BOTCTRL_2;
//assign CTRL_BOT[3] = BOTCTRL_1;
//`endif
//
//
//assign FO_nEN = 0;
//assign FO_OUTPUT = FO_OUT;
//
//parameter DT_COUNT = 40;
//reg [11:0] PWM_COUNTER;
//reg [11:0] PWMA;
//reg [11:0] PWMB;
//reg [11:0] PWMC;
//reg [1:0] PWMDIV;
//
//reg [9:0] ERROR_FLAGS /* synthesis noprune */;
//reg [9:0] ERROR_LATCH /* synthesis noprune */;
//reg [2:0] Error_Reset_Counter /* synthesis noprune */;
//
////reg CURRENT_ERR;
////reg VOLTAGE_ERR;
////reg TEMP_ERR;
//reg Latch_TA;
//reg Latch_TB;
//reg Latch_TC;
//reg Latch_BA;
//reg Latch_BB;
//reg Latch_BC;
//
//reg [5:0] RX_POSVAL_COUNTER /* synthesis noprune */;
//reg [5:0] RX_NEGVAL_COUNTER /* synthesis noprune */;
//reg [3:0] RX_BIT_COUNTER /* synthesis noprune */;
//reg NOTRANSMITTION_FAULT_FLAG /* synthesis noprune */;
//reg FO_INPUT_REG /* synthesis noprune */;
//reg FO_INPUT_PREV /* synthesis noprune */;
//
//reg [13:0] RX_DATA_READ_VALUE /* synthesis noprune */;
//reg DATA_READY /* synthesis noprune */;
//reg ERR_RESET /* synthesis noprune */;
//reg ActivateInverter /* synthesis noprune */;
//reg ActivateInverterCommand /* synthesis noprune */;
//reg [11:0] INP_PWMA /* synthesis noprune */;
//reg [11:0] INP_PWMB /* synthesis noprune */;
//reg [11:0] INP_PWMC /* synthesis noprune */;
//reg [11:0] INP_COMMAND /* synthesis noprune */;
//
//reg DATA_RESET /* synthesis noprune */;
//reg PWMSYNC /* synthesis noprune */;
//reg SYN_RESET /* synthesis noprune */;
//
//reg [11:0] TOP_CLIM;
//reg [11:0] BOT_CLIM;
//
//
//wire [11:0] ERR_OUT = {
//   ActivateInverter,     // [11]
//   ERR_RESET,            // [10]
//   ERROR_LATCH[9:0]     // [9:0]
//};
//
//
//initial begin
//   DATA_SEND_SM <= 0;
//   ADC_SM_STATE <= 0;
//   ActivateInverter <= 0;
//   ERR_RESET <= 0;
//end
//
//always@(posedge clock_20) begin
//   //----------------------ERRORS HANDLING----------------------------
//
//   ERROR_FLAGS[0] <= (~IGBT_ERR[1]) | (~IGBT_ERR[2]); //ERR_T1
//   ERROR_FLAGS[1] <= (~IGBT_ERR[3]) | (~IGBT_ERR[4]); //ERR_B1
//   ERROR_FLAGS[2] <= (~IGBT_ERR[5]) | (~IGBT_ERR[6]); //ERR_T2
//   //PWMSYNC ERR ERROR_FLAGS[3]<= 0;
//   ERROR_FLAGS[4] <= 0;
//   ERROR_FLAGS[5] <= 0;
//   //ERROR_FLAGS[6]<= CURRENT_ERR; //IERR
//   ERROR_FLAGS[7] <= NOTRANSMITTION_FAULT_FLAG; //VERR
//   ERROR_FLAGS[8] <= 0; //NOTRANSMITTION_FAULT_FLAG
//   ERROR_FLAGS[9] <= 0; //SUPPLY_ERR
//
//   if (Error_Reset_Counter == 0) begin
//      ERROR_LATCH <= ERROR_LATCH | ERROR_FLAGS;
//   end
//   else begin
//      ERROR_LATCH <= ERROR_FLAGS;
//   end
//
//   if (Error_Reset_Counter > 0) begin
//      Error_Reset_Counter <= Error_Reset_Counter - 1;
//   end
//   else if (ERR_RESET == 1 && ActivateInverter == 0) begin
//      Error_Reset_Counter <= 7;
//   end
//
//   if (CURRENT_A > 3208 || CURRENT_A < 204 ||
//       CURRENT_B > 3208 || CURRENT_B < 204 ||
//       CURRENT_C > 3208 || CURRENT_C < 204 )
//   begin
//      ERROR_FLAGS[6] <= 1;
//   end
//   else begin
//      ERROR_FLAGS[6] <= 0;
//   end
//
//   //if (VOLTAGE_IN>2777 || VOLTAGE_IN<1362) begin VOLTAGE_ERR<=1; end else begin VOLTAGE_ERR<=0; end
//
//   //if (OUTPUT_DATA[48:37]>2991 || OUTPUT_DATA[48:37]<1900 ||
//   //  OUTPUT_DATA[60:49]>2991 || OUTPUT_DATA[60:49]<1900 ||
//   //  OUTPUT_DATA[72:61]>2991 || OUTPUT_DATA[72:61]<1900   ) begin TEMP_ERR<=1; end else begin TEMP_ERR<=0; end
//
//
//   //----------------------PWM GENERATION----------------------------
//
//   if (PWMSYNC == 0) begin
//      if (PWMDIV < 3) begin
//         PWMDIV <= PWMDIV + 1;
//      end
//      else begin
//         PWMDIV <= 0;
//
//         if (PWM_COUNTER < 4000) begin
//            PWM_COUNTER <= PWM_COUNTER + 1;
//         end
//
//         SYN_RESET <= 1;
//      end
//
//      //TOP_CLIM
//      if (CURRENT_A > TOP_CLIM) begin
//         Latch_TA <= 1;
//      end
//      if (CURRENT_B > TOP_CLIM) begin
//         Latch_TB <= 1;
//      end
//      if (CURRENT_C > TOP_CLIM) begin
//         Latch_TC <= 1;
//      end
//
//      //BOT_CLIM
//      if (CURRENT_A < BOT_CLIM) begin
//         Latch_BA <= 1;
//      end
//      if (CURRENT_B < BOT_CLIM) begin
//         Latch_BB <= 1;
//      end
//      if (CURRENT_C < BOT_CLIM) begin
//         Latch_BC <= 1;
//      end
//
//      if (ActivateInverter == 0) begin
//         ActivateInverterCommand <= 0;
//      end
//
//      PWMA <= PWMA;
//      PWMB <= PWMB;
//      PWMC <= PWMC;
//   end
//
//   else begin
//      SYN_RESET <= 0;
//      PWMSYNC <= 0;
//
//      if (PWM_COUNTER < 500) begin
//         ERROR_FLAGS[3] <= 1;
//      end //HIGH FREQ PROTECTION
//      else begin
//         ERROR_FLAGS[3] <= 0;
//         PWM_COUNTER <= 0;
//      end
//
//      ActivateInverterCommand <= ActivateInverter;
//      PWMA <= INP_PWMA;
//      PWMB <= INP_PWMB;
//      PWMC <= INP_PWMC;
//      Latch_TA <= 0;
//      Latch_TB <= 0;
//      Latch_TC <= 0;
//      Latch_BA <= 0;
//      Latch_BB <= 0;
//      Latch_BC <= 0;
//   end
//
//   if (ERROR_LATCH == 0 && ActivateInverterCommand == 1) begin
//      if (PWM_COUNTER < PWMA) begin
//         TOP1 <= 1 & (~Latch_TA);
//         BOT1 <= 0;
//      end
//      else begin
//         TOP1 <= 0;
//         BOT1 <= 1 & (~Latch_BA);
//      end
//
//      if (PWM_COUNTER < PWMB) begin
//         TOP2 <= 1 & (~Latch_TB);
//         BOT2 <= 0;
//      end
//      else begin
//         TOP2 <= 0;
//         BOT2 <= 1 & (~Latch_BB);
//      end
//
//      if (PWM_COUNTER < PWMC) begin
//         TOP3 <= 1 & (~Latch_TC);
//         BOT3 <= 0;
//      end
//      else begin
//         TOP3 <= 0;
//         BOT3 <= 1 & (~Latch_BC);
//      end
//   end
//   else begin
//      TOP1 <= 0;
//      TOP2 <= 0;
//      TOP3 <= 0;
//      BOT1 <= 0;
//      BOT2 <= 0;
//      BOT3 <= 0;
//   end
//
//
//   //----------------DEAD TIME CONTROL-----------------
//
//   if (TOP1 == 1) begin
//      BOT1_DT <= DT_COUNT;
//
//      if (TOP1_DT == 0) begin
//         TOPCTRL_1 <= 1;
//      end
//   end
//   else begin
//      TOPCTRL_1 <= 0;
//
//      if (BOT1_DT != 0) begin
//         BOT1_DT <= BOT1_DT - 1;
//      end
//   end
//
//   if (BOT1 == 1) begin
//      TOP1_DT <= DT_COUNT;
//
//      if (BOT1_DT == 0) begin
//         BOTCTRL_1 <= 1;
//      end
//   end
//   else begin
//      BOTCTRL_1 <= 0;
//
//      if (TOP1_DT != 0) begin
//         TOP1_DT <= TOP1_DT - 1;
//      end
//   end
//
//   if (TOP2 == 1) begin
//      BOT2_DT <= DT_COUNT;
//
//      if (TOP2_DT == 0) begin
//         TOPCTRL_2 <= 1;
//      end
//   end
//   else begin
//      TOPCTRL_2 <= 0;
//
//      if (BOT2_DT != 0) begin
//         BOT2_DT <= BOT2_DT - 1;
//      end
//   end
//
//   if (BOT2 == 1) begin
//      TOP2_DT <= DT_COUNT;
//
//      if (BOT2_DT == 0) begin
//         BOTCTRL_2 <= 1;
//      end
//   end
//   else begin
//      BOTCTRL_2 <= 0;
//
//      if (TOP2_DT != 0) begin
//         TOP2_DT <= TOP2_DT - 1;
//      end
//   end
//
//   if (TOP3 == 1) begin
//      BOT3_DT <= DT_COUNT;
//
//      if (TOP3_DT == 0) begin
//         TOPCTRL_3 <= 1;
//      end
//   end
//   else begin
//      TOPCTRL_3 <= 0;
//
//      if (BOT3_DT != 0) begin
//         BOT3_DT <= BOT3_DT - 1;
//      end
//   end
//
//   if (BOT3 == 1) begin
//      TOP3_DT <= DT_COUNT;
//
//      if (BOT3_DT == 0) begin
//         BOTCTRL_3 <= 1;
//      end
//   end
//   else begin
//      BOTCTRL_3 <= 0;
//
//      if (TOP3_DT != 0) begin
//         TOP3_DT <= TOP3_DT - 1;
//      end
//   end
//
//
//   //---------------------- READ ADC CURRENTS--------------------------
//
//   if (ADC_SM_STATE == 0) //Initiate ADC, nCS -> low, wait cycle
//   begin
//      ADC_SM_COUNTER <= 15;
//      ADC_CHIPSELECT <= 0;
//      ADC_SM_STATE <= 1;
//      ADC_CLOCK <= 1;
//      ADC_CLOCK_DIV <= 0;
//   end
//   else if (ADC_SM_STATE == 1) begin
//      ADC_CHIPSELECT <= 0;
//      ADC_CLOCK <= 0;
//
//      if (ADC_CLOCK_DIV < 3) begin
//         ADC_CLOCK_DIV <= ADC_CLOCK_DIV + 1;
//      end
//      else begin
//         ADC_CLOCK_DIV <= 0;
//      end
//
//      if (ADC_CLOCK_DIV == 2) begin
//         ADC1_DATA[ADC_SM_COUNTER] <= ADC_DATA[1];
//         ADC2_DATA[ADC_SM_COUNTER] <= ADC_DATA[2];
//         ADC3_DATA[ADC_SM_COUNTER] <= ADC_DATA[3];
//         ADC_SM_COUNTER <= ADC_SM_COUNTER - 1;
//         ADC_SM_STATE <= 2;
//         ADC_CLOCK_DIV <= 0;
//      end
//   end
//   else if (ADC_SM_STATE == 2) //READ 16 BITS
//   begin
//      ADC_CHIPSELECT <= 0;
//
//      if (ADC_CLOCK_DIV < 3) begin
//         ADC_CLOCK_DIV <= ADC_CLOCK_DIV + 1;
//      end
//      else begin
//         ADC_CLOCK_DIV <= 0;
//      end
//
//      if (ADC_CLOCK_DIV < 2) begin
//         ADC_CLOCK <= 1;
//      end
//      else begin
//         ADC_CLOCK <= 0;
//      end
//
//      if (ADC_CLOCK_DIV == 0) // Copy results to reg
//      begin
//         ADC1_DATA[ADC_SM_COUNTER] <= ADC_DATA[1];
//         ADC2_DATA[ADC_SM_COUNTER] <= ADC_DATA[2];
//         ADC3_DATA[ADC_SM_COUNTER] <= ADC_DATA[3];
//      end
//
//      if (ADC_CLOCK_DIV == 3) begin
//         if (ADC_SM_COUNTER == 0) begin
//            ADC_SM_STATE <= 3;
//         end
//         else begin
//            ADC_SM_COUNTER <= ADC_SM_COUNTER - 1;
//         end
//      end
//   end
//   else if (ADC_SM_STATE == 3) //Turn OFF and go to wait cycles
//   begin
//`ifdef LEFT
//      CURRENT_A <= ADC1_DATA[11: 0];
//      CURRENT_B <= ADC2_DATA[11: 0];
//      CURRENT_C <= ADC3_DATA[11: 0];
//`endif
// `ifdef RIGHT
//
//      CURRENT_A <= ADC3_DATA[11: 0];
//      CURRENT_B <= ADC2_DATA[11: 0];
//      CURRENT_C <= ADC1_DATA[11: 0];
//`endif
//
//      ADC_CLOCK <= 1;
//      ADC_CHIPSELECT <= 1;
//      ADC_SM_COUNTER <= 0;
//      ADC_SM_STATE <= 4;
//   end
//   else if (ADC_SM_STATE == 4) //Wait cycles
//   begin
//      ADC_SM_COUNTER <= ADC_SM_COUNTER + 1;
//      ADC_CLOCK <= 1;
//      ADC_CHIPSELECT <= 1;
//
//      if (ADC_SM_COUNTER > 5) begin
//         ADC_SM_COUNTER <= 0;
//         ADC_SM_STATE <= 0;
//      end
//   end
//
//
//   //---------------------- READ AMC VOLTAGES --------------------------
//
//   if (TEMP_SM == 0) //ResetData, Counter
//   begin
//      TEMP_DATA_REG1 <= 0;
//      TEMP_DATA_REG4 <= 0;
//      TEMP_COUNTER <= 0;
//      TEMP_SM <= 1;
//
//      if (TEMP_DATA_COUNTER < 2) begin
//         TEMP_DATA_COUNTER <= TEMP_DATA_COUNTER + 1;
//      end
//      else begin
//         TEMP_DATA_COUNTER <= 0;
//      end
//   end
//
//   if (TEMP_SM == 1) //Measure
//   begin
//      TEMP_COUNTER <= TEMP_COUNTER + 1;
//
//      if (TEMP_COUNTER == 4095) begin
//         TEMP_SM <= 2;
//      end
//
//      if (TEMP_DATA_COUNTER == 0) begin
//         if (AMC_DATA[1] == 1) begin
//            TEMP_DATA_REG1 <= TEMP_DATA_REG1 + 1;
//         end
//      end
//      else if (TEMP_DATA_COUNTER == 1) begin
//         if (AMC_DATA[2] == 1) begin
//            TEMP_DATA_REG1 <= TEMP_DATA_REG1 + 1;
//         end
//      end
//      else if (TEMP_DATA_COUNTER == 2) begin
//         if (AMC_DATA[3] == 1) begin
//            TEMP_DATA_REG1 <= TEMP_DATA_REG1 + 1;
//         end
//      end
//
//      if (AMC_DATA[4] == 1) begin
//         TEMP_DATA_REG4 <= TEMP_DATA_REG4 + 1;
//      end
//   end
//
//   if (TEMP_SM == 2) //Copy data, Switch MUX
//   begin
//`ifdef LEFT
//      if (TEMP_DATA_COUNTER == 0) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 0;
//      end
//      else if (TEMP_DATA_COUNTER == 1) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 1;
//      end
//      else if (TEMP_DATA_COUNTER == 2) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 2;
//      end
//`endif
//`ifdef RIGHT
//      if (TEMP_DATA_COUNTER == 0) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 2;
//      end
//      else if (TEMP_DATA_COUNTER == 1) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 1;
//      end
//      else if (TEMP_DATA_COUNTER == 2) begin
//         TEMP_OUT[9: 0] <= TEMP_DATA_REG1[11: 3];
//         TEMP_OUT[11: 10] = 0;
//      end
//`endif
//      VOLTAGE_IN <= TEMP_DATA_REG4[11: 0]; //TEMP
//      TEMP_SM <= 0;
//   end
//
//
//   //----------------------DATA SEND TO PCON----------------
//
//   if (DATA_SEND_SM == 0) //RESET TRANSMISSION
//   begin
//      DATA_SEND_SM <= 1;
//      PULSE_SEND_COUNTER <= 0;
//      BIT_SEND_COUNTER <= 0;
//      DATA_SEND_COUNTER <= 0;
//   end
//   else if (DATA_SEND_SM == 1) //START GENERATION
//   begin
//      if (PULSE_SEND_COUNTER < 25) begin
//         PULSE_SEND_COUNTER <= PULSE_SEND_COUNTER + 1;
//         FO_OUT <= 0;
//      end
//      else begin
//         PULSE_SEND_COUNTER <= 0;
//         FO_OUT <= 0;
//         DATA_SEND_SM <= 2;
//      end
//   end
//   else if (DATA_SEND_SM == 2) //BIT STREAM DATA1
//   begin
//      if (PULSE_SEND_COUNTER == 0) begin
//         if (DATA_SEND_COUNTER == 0)
//            BitToSend <= CURRENT_A[BIT_SEND_COUNTER];  //CURRENT_A[BIT_SEND_COUNTER]
//         else if (DATA_SEND_COUNTER == 1)
//            BitToSend <= CURRENT_B[BIT_SEND_COUNTER];  //CURRENT_B[BIT_SEND_COUNTER]
//         else if (DATA_SEND_COUNTER == 2)
//            BitToSend <= CURRENT_C[BIT_SEND_COUNTER];
//         else if (DATA_SEND_COUNTER == 3)
//            BitToSend <= VOLTAGE_IN[BIT_SEND_COUNTER];
//         else if (DATA_SEND_COUNTER == 4)
//            BitToSend <= TEMP_OUT[BIT_SEND_COUNTER];
//         else if (DATA_SEND_COUNTER == 5)
//            BitToSend <= ERR_OUT[BIT_SEND_COUNTER];
//         //else  BitToSend <= 0;
//      end
//
//      if (BitToSend == 1 && PULSE_SEND_COUNTER < 8) begin
//         PULSE_SEND_COUNTER <= PULSE_SEND_COUNTER + 1;
//         FO_OUT <= 1;
//      end
//      else if (BitToSend == 0 && PULSE_SEND_COUNTER < 2) begin
//         PULSE_SEND_COUNTER <= PULSE_SEND_COUNTER + 1;
//         FO_OUT <= 1;
//      end
//      else if (PULSE_SEND_COUNTER < 10) begin
//         PULSE_SEND_COUNTER <= PULSE_SEND_COUNTER + 1;
//         FO_OUT <= 0;
//      end
//      else begin
//         if (BIT_SEND_COUNTER < 11) begin
//            BIT_SEND_COUNTER <= BIT_SEND_COUNTER + 1;
//         end
//         else begin
//            if (DATA_SEND_COUNTER < 5) begin
//               DATA_SEND_COUNTER <= DATA_SEND_COUNTER + 1;
//            end
//            else begin
//               DATA_SEND_SM <= 0;
//            end
//
//            BIT_SEND_COUNTER <= 0;
//         end
//
//         FO_OUT <= 0;
//         PULSE_SEND_COUNTER <= 0;
//      end
//   end
//
//
//   //-----------------------------------DATA RECIEVE-------------------------------------
//
//   // 1. Synchronize Input used for edge detection
//
//   FO_INPUT_REG <= FO_INPUT;         // Current State
//   FO_INPUT_PREV <= FO_INPUT_REG;    // Previous State
//
//   if (DATA_READY == 1) begin
//      ERR_RESET <= INP_COMMAND[0];
//      ActivateInverter <= INP_COMMAND[1];
//
//      case (INP_COMMAND[3: 2])
//         2'b00: begin
//            TOP_CLIM <= 2252;
//            BOT_CLIM <= 1160;
//         end // Case 0
//         2'b01: begin
//            TOP_CLIM <= 2525;
//            BOT_CLIM <= 887;
//         end  // Case 1
//         2'b10: begin
//            TOP_CLIM <= 2798;
//            BOT_CLIM <= 614;
//         end // Case 2
//         2'b11: begin
//            TOP_CLIM <= 3072;
//            BOT_CLIM <= 341;
//         end // Case 3
//         default: begin
//            TOP_CLIM <= 2252;
//            BOT_CLIM <= 1160;
//         end // Optional but recommended
//      endcase
//   end
//
//   if (FO_INPUT_REG == 0) begin
//      if (RX_NEGVAL_COUNTER < 62) begin
//         RX_NEGVAL_COUNTER <= RX_NEGVAL_COUNTER + 1;
//      end
//      if (FO_INPUT_PREV == 0) begin
//         RX_POSVAL_COUNTER <= 0;
//      end
//   end
//   else begin
//      if (RX_POSVAL_COUNTER < 62) begin
//         RX_POSVAL_COUNTER <= RX_POSVAL_COUNTER + 1;
//      end
//      if (FO_INPUT_PREV == 1) begin
//         RX_NEGVAL_COUNTER <= 0;
//      end
//      if (RX_POSVAL_COUNTER == 20 && SYN_RESET == 1) begin
//         PWMSYNC <= 1;
//         DATA_RESET <= 1;
//      end
//   end
//
//   if (RX_NEGVAL_COUNTER > 60 || RX_POSVAL_COUNTER > 60) begin
//      NOTRANSMITTION_FAULT_FLAG <= 1;
//   end
//   else begin
//      NOTRANSMITTION_FAULT_FLAG <= 0;
//   end
//
//   // Detect RISING Edge (0 -> 1)
//   if (FO_INPUT_REG == 1 && FO_INPUT_PREV == 0) begin
//      // This logic was in: always@(posedge FO_INPUT)
//      if (RX_NEGVAL_COUNTER > 20) begin
//         if (DATA_RESET == 0) begin
//            if (RX_DATA_READ_VALUE[13: 12] == 0) begin
//               INP_PWMA <= RX_DATA_READ_VALUE[11: 0];
//            end
//            else if (RX_DATA_READ_VALUE[13: 12] == 1) begin
//               INP_PWMB <= RX_DATA_READ_VALUE[11: 0];
//            end
//            else if (RX_DATA_READ_VALUE[13: 12] == 2) begin
//               INP_PWMC <= RX_DATA_READ_VALUE[11: 0];
//            end
//            else if (RX_DATA_READ_VALUE[13: 12] == 3) begin
//               INP_COMMAND <= RX_DATA_READ_VALUE[11: 0];
//            end
//
//            DATA_READY <= 1;
//            RX_BIT_COUNTER <= 0;
//         end
//         else begin
//            RX_BIT_COUNTER <= 0;
//            DATA_READY <= 0;
//            DATA_RESET <= 0;
//         end
//      end
//      else begin
//         DATA_READY <= 0;
//         RX_BIT_COUNTER <= RX_BIT_COUNTER + 1;
//      end
//   end
//
//   // Detect FALLING Edge (1 -> 0)
//   else if (FO_INPUT_REG == 0 && FO_INPUT_PREV == 1) begin
//      // This logic was in: always@(negedge FO_INPUT)
//      if (RX_POSVAL_COUNTER > 5) begin
//         RX_DATA_READ_VALUE[RX_BIT_COUNTER] <= 1;
//      end
//      else begin
//         RX_DATA_READ_VALUE[RX_BIT_COUNTER] <= 0;
//      end
//   end
//
//end  // always@(posedge clock_20)
//
//endmodule
