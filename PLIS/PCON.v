module PCON(
   input CLOCK_50,
   input CLOCK_12,
   
   // Связь с ADCHub
   input  [20:1] D_INP,   // 3, 4, 5  - используются
   output [20:1] D_OUTP,  // 4, 5, 6  - используются
   
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


reg [20:1] out;
assign D_OUTP = out;


// Обмен с ADCHub
localparam DATA_WIDTH = 36;

// ADCHub1 - входное напряжение
wire [DATA_WIDTH-1:0] rc_data_1;
wire rc_data_ready_1;
wire rc_connect_fail_1;
wire rc_invalid_data_1;

DATA_RECEIVER Receiver1 (
   .CLOCK_50(CLOCK_50), 
   .FO_IN(D_INP[3]), 
   .DATA(rc_data_1),
   .DATA_READY(rc_data_ready_1),
   .ERR_CONNECT_FAIL(rc_connect_fail_1),
   .ERR_INVALID_DATA(rc_invalid_data_1)
);
defparam Receiver1.DATA_WIDTH = DATA_WIDTH;

reg [11:0] current_1;
reg [11:0] voltage_inp;
reg [11:0] const_1;

// ADCHub2 - выходное напряжение
wire [DATA_WIDTH-1:0] rc_data_2;
wire rc_data_ready_2;
wire rc_connect_fail_2;
wire rc_invalid_data_2;

DATA_RECEIVER Receiver2 (
   .CLOCK_50(CLOCK_50), 
   .FO_IN(D_INP[4]), 
   .DATA(rc_data_2),
   .DATA_READY(rc_data_ready_2),
   .ERR_CONNECT_FAIL(rc_connect_fail_2),
   .ERR_INVALID_DATA(rc_invalid_data_2)
);
defparam Receiver2.DATA_WIDTH = DATA_WIDTH;

reg [11:0] current_2;
reg [11:0] voltage_out;
reg [11:0] const_2;


// Обмен с f28m35
   wire [15:0] EMIF_DATA_TO_RAM;   
   wire [13:0] EMIF_ADRESS;  
   wire EMIF_WREN;           // 0->1  =>  запись по ADRS_FROM_FPGA значения DATA_FROM_FPGA
   wire [15:0] EMIF_DATA_FROM_RAM_wire; 

EMIF Emif (
   .AD(ADRESS_DATA), 
   .nCS0(nCS0), 
   .nCS1(nCS1), 
   .WE(WEn), 
   .OE(OEn), 
   .ALE(ALE), 
   .CLOCK(CLOCK_50), 
   .DATA_FROM_FPGA(EMIF_DATA_TO_RAM), 
   .ADRS_FROM_FPGA(EMIF_ADRESS), 
   .WRE_FROM_FPGA(EMIF_WREN), 
   .DATA_FROM_MICRO(EMIF_DATA_FROM_RAM_wire)
);


always @(posedge CLOCK_50) begin
   // Чисто, чтобы квартус не оптимизировал проект в 0:
   out <= {const_2, voltage_out, current_2, const_1, voltage_inp, current_1};  
   
   // ADCHub1 приём
   if (rc_data_ready_1) begin
      {const_1, voltage_inp, current_1} <= rc_data_1;
   end
   
   // ADCHub2 приём
   if (rc_data_ready_2) begin
      {const_2, voltage_out, current_2} <= rc_data_2;
   end
   
   // f28m35
   
end

endmodule