`include "DATA_ADRESSES.vh"

//`define DBG 67

module PCON(
   input CLOCK_50,
   input CLOCK_12,
   
   // Связь с ADCHub  
   input  [20:1] D_INP,   // 3 (ADCHub1), 4 (ADCHub2) - используются
   output [20:1] D_OUTP,  // 4 (ADCHub1), 5 (ADCHub2) - используются
   
   // Связь с контроллером:
   inout [15:0] ADRESS_DATA, 
   input ALE,   
   input nCS0, 
   input nCS1, 
   input WEn, 
   input OEn,
   
   // Хрен знает, что, но кажись от контроллера:
   input [6:1] CPU_DIN,  // [6] = ActivateInvertor
   
   // Не используется:
   input [19:16] ADRESS_TOP,         
   input nBLE,               
   input nBHE,               
   input InternalPowerGood,
   input reset_button
);

assign D_OUTP[3:1]  = 0;
assign D_OUTP[20:6] = 0;

// Описание ошибок:
// 0: Потерян сигнал с f28m35
// 1: Потерян сигнал с ADCHub1
// 2: Потерян сигнал с ADCHub2
//    ADCHub1:
// 3: Ошибки транизисторов 1
// 4: Ошибки транизисторов 2
// 5: Превышение порога тока
// 6: Потеря соединения со стороны ADCHub1
//    ADCHub2:
// 7: Ошибки транизисторов 1
// 8: Ошибки транизисторов 2
// 9: Превышение порога тока
// 10: Потеря соединения со стороны ADCHub2
reg [10:0] errors = 0;
reg [10:0] errors_latch = 0;
reg reset_errors_inp = 0;
reg reset_errors = 0;
reg [11:0] reset_errors_delay = 0;
localparam RESET_ERRORS_DELAY = 12'd4095;  // На 50 мГц должно успеть прилететь на ADCHub и обратно дважды

// Обмен с ADCHub
`ifdef DBG
localparam DATA_TO_ADC_WIDTH = 16;
localparam DATA_FROM_ADC_WIDTH = 16;
`else
localparam DATA_TO_ADC_WIDTH = 16;
localparam DATA_FROM_ADC_WIDTH = 32;
`endif

// Получение с ADCHub1 - входное напряжение
wire [DATA_FROM_ADC_WIDTH-1:0] rc_data_1;
wire rc_data_ready_1;
wire rc_connect_fail_1;
wire rc_invalid_data_1;

wire fo_in_1;
`ifdef DBG
assign fo_in_1 = ~D_INP[3];
`else
assign fo_in_1 = D_INP[3];
`endif

DATA_RECEIVER Receiver1 (
   .CLOCK(CLOCK_50),
   .FO_IN(fo_in_1), 
   .DATA(rc_data_1),
   .DATA_READY(rc_data_ready_1),
   .ERR_CONNECT_FAIL(rc_connect_fail_1),
   .ERR_INVALID_DATA(rc_invalid_data_1)
);
defparam Receiver1.DATA_WIDTH = DATA_FROM_ADC_WIDTH;
defparam Receiver1.TICK_LEN_RECEIV = 20;  // 50 мГц
defparam Receiver1.PULSE_1_LEN     = 400;  
defparam Receiver1.RESET_LEN       = 1000; 
defparam Receiver1.MAX_ERROR       = 100; 

reg [11:0] current_1;
reg [11:0] voltage_inp;


// Отправка на ADCHub1
reg mode_up;
reg converter_on;      // Текущий режим, учитывающий состояние ошибок
reg converter_on_inp;  // Сигнал с C28

reg [DATA_TO_ADC_WIDTH-1:0] data_to_send_1;
wire ready_to_send_1;
wire fo_out_1;

`ifdef DBG
assign D_OUTP[4] = ~fo_out_1;
`else
assign D_OUTP[4] = ~fo_out_1;
`endif

DATA_TRANSMITTER Transmitter1 (
   .CLOCK(CLOCK_50),
   .DATA(data_to_send_1), 
   .READY_TO_SEND(ready_to_send_1), 
   .FO_OUT(fo_out_1)
);
defparam Transmitter1.DATA_WIDTH = DATA_TO_ADC_WIDTH;
defparam Transmitter1.TICK_LEN    = 20;  // 50 мГц
defparam Transmitter1.PULSE_0_LEN = 100;
defparam Transmitter1.PULSE_1_LEN = 400;
defparam Transmitter1.BIT_LEN     = 600;    
defparam Transmitter1.RESET_LEN   = 1000;


// ADCHub2 - выходное напряжение
wire [DATA_FROM_ADC_WIDTH-1:0] rc_data_2;
wire rc_data_ready_2;
wire rc_connect_fail_2;
wire rc_invalid_data_2;

wire fo_in_2;
`ifdef DBG
assign fo_in_2 = ~D_INP[4];
`else
assign fo_in_2 = D_INP[4];
`endif

DATA_RECEIVER Receiver2 (
   .CLOCK(CLOCK_50), 
   .FO_IN(fo_in_2), 
   .DATA(rc_data_2),
   .DATA_READY(rc_data_ready_2),
   .ERR_CONNECT_FAIL(rc_connect_fail_2),
   .ERR_INVALID_DATA(rc_invalid_data_2)
);
defparam Receiver2.DATA_WIDTH = DATA_FROM_ADC_WIDTH;
defparam Receiver2.TICK_LEN_RECEIV = 20;   // 50 мГц
defparam Receiver2.PULSE_1_LEN     = 400;  
defparam Receiver2.RESET_LEN       = 1000; 
defparam Receiver2.MAX_ERROR       = 100; 

reg [11:0] current_2;
reg [11:0] voltage_out;


// Отправка на ADCHub2
reg [DATA_TO_ADC_WIDTH-1:0] data_to_send_2;
wire ready_to_send_2;
wire fo_out_2;

`ifdef DBG
assign D_OUTP[5] = ~fo_out_2;
`else
assign D_OUTP[5] = ~fo_out_2;
`endif

DATA_TRANSMITTER Transmitter2 (
   .CLOCK(CLOCK_50),
   .DATA(data_to_send_2), 
   .READY_TO_SEND(ready_to_send_2), 
   .FO_OUT(fo_out_2)
);
defparam Transmitter2.DATA_WIDTH = DATA_TO_ADC_WIDTH;
defparam Transmitter2.TICK_LEN    = 20;  // 50 мГц
defparam Transmitter2.PULSE_0_LEN = 100;
defparam Transmitter2.PULSE_1_LEN = 400;
defparam Transmitter2.BIT_LEN     = 600;
defparam Transmitter2.RESET_LEN   = 1000;


// Обмен с f28m35
reg  [15:0] emif_data_to_micro;
wire [15:0] emif_data_to_micro_wire;
assign emif_data_to_micro_wire = emif_data_to_micro;

reg  [6:0]  emif_adress; 
wire [6:0]  emif_adress_wire; 
assign emif_adress_wire = emif_adress; 

// 0->1  =>  запись по ADRS_FROM_FPGA значения DATA_FROM_FPGA
reg emif_wren;  
wire emif_wren_wire;  
assign emif_wren_wire = emif_wren; 

wire [15:0] emif_data_from_micro; 

EMIF Emif (
   .AD(ADRESS_DATA), 
   .nCS0(nCS0), 
   .nCS1(nCS1), 
   .WE(WEn), 
   .OE(OEn), 
   .ALE(ALE), 
   .CLOCK(CLOCK_50), 
   .DATA_FROM_FPGA(emif_data_to_micro_wire), 
   .ADRS_FROM_FPGA(emif_adress_wire), 
   .WRE_FROM_FPGA(emif_wren_wire), 
   .DATA_FROM_MICRO(emif_data_from_micro)
);

reg [6:0]  emif_state_counter = 0;
reg [12:0] pwm_counter;
reg [7:0]  watch_dog_curr;
reg [7:0]  watch_dog_prev;
reg [7:0]  watch_dog_timer;

always @(posedge CLOCK_50) begin
   errors[1] <= rc_connect_fail_1;
   errors[2] <= rc_connect_fail_2;
   errors_latch <= errors_latch | errors;
   
   if (reset_errors_inp) begin
      reset_errors <= 1;
      reset_errors_inp <= 0;
      reset_errors_delay <= RESET_ERRORS_DELAY;
   end
   if (reset_errors_delay == 1) begin
      errors_latch <= 0;
      reset_errors <= 0;
   end
   if (reset_errors_delay > 0) begin
      reset_errors_delay <= reset_errors_delay - 12'b1;
   end
   
   converter_on <= converter_on_inp && (errors_latch == 0);

   // ADCHub1 приём
   if (rc_data_ready_1 && rc_invalid_data_1 == 0) begin
      {voltage_inp, current_1, errors_latch[6:3], errors[6:3]} <= rc_data_1;
   end
   
   // ADCHub1 отправка
   if (ready_to_send_1) begin
		`ifdef DBG
      data_to_send_1 <= 67;
		`else
      data_to_send_1 <= {pwm_counter, mode_up, converter_on, reset_errors};
		`endif
   end   
   
   // ADCHub2 приём
   if (rc_data_ready_2 && rc_invalid_data_2 == 0) begin
      {voltage_out, current_2, errors_latch[10:7], errors[10:7]} <= rc_data_2;
   end
   
   // ADCHub2 отправка
   if (ready_to_send_2) begin
		`ifdef DBG
      data_to_send_2 <= 69;
		`else
      data_to_send_2 <= {13'b0, 1'b0, 1'b0, reset_errors};
		`endif
   end 
   
   // f28m35
   emif_state_counter <= emif_state_counter + 7'b1;
   emif_wren <= 0;

   // Передача параметров с ADChub1-2
   if          (emif_state_counter == 0) begin
      emif_adress <= `ADR_ERRORS;
      emif_data_to_micro <= errors;
      emif_wren <= 1;
   end else if (emif_state_counter == 2) begin
      emif_adress <= `ADR_ERRORS_LATCH;
      emif_data_to_micro <= errors_latch;
      emif_wren <= 1;
   end else if (emif_state_counter == 4) begin
      emif_adress <= `ADR_VOLTAGE_INP;
      emif_data_to_micro <= voltage_inp;
      emif_wren <= 1;
   end else if (emif_state_counter == 6) begin
      emif_adress <= `ADR_VOLTAGE_OUT;
      emif_data_to_micro <= voltage_out;
      emif_wren <= 1;
   end else if (emif_state_counter == 8) begin
      emif_adress <= `ADR_CURRENT_1;
      emif_data_to_micro <= current_1;
      emif_wren <= 1;
   end else if (emif_state_counter == 10) begin
      emif_adress <= `ADR_CURRENT_2;
      emif_data_to_micro <= current_2;
      emif_wren <= 1;
   end
   // Чтение параметров
   else if (emif_state_counter == 12) begin
      emif_adress <= `ADR_WATCHDOG;
   end else if (emif_state_counter == 16) begin
      watch_dog_curr <= emif_data_from_micro;
   end
   else if (emif_state_counter == 17) begin
      emif_adress <= `ADR_CONV_CTRL;
   end else if (emif_state_counter == 21) begin
      {pwm_counter, mode_up, converter_on_inp, reset_errors_inp} <= emif_data_from_micro;
   end
   // Сброс 
   else if (emif_state_counter == 100) begin
      if (watch_dog_curr == watch_dog_prev) begin
         watch_dog_timer <= watch_dog_timer + 8'b1;
      end else begin
         watch_dog_prev <= watch_dog_curr;
         watch_dog_timer <= 0;
         errors[0] <= 0;
      end
      
      if (watch_dog_timer > 200) begin
         errors[0] <= 1;
      end

      emif_state_counter <= 0;
   end
end

endmodule
