module PCON(
   CLOCK_50,
   CLOCK_12,
   
   // Связь с ADCHub
   D_INP,   // 3, 4, 5  - используются
   D_OUTP,  // 4, 5, 6  - используются
   
   // Связь с контроллером:
   ADRESS_DATA, 
   ALE,   
   nCS0, 
   nCS1, 
   WEn, 
   OEn,
   
   // Хрен знает, что, но кажись от контроллера:
   CPU_DIN,
   
   // Не используется:
   ADRESS_TOP,         
   nBLE,               
   nBHE,               
   InternalPowerGood,
   reset_button
);

//Входы FPGA
input CLOCK_50;
input CLOCK_12;
input reset_button;
input InternalPowerGood;
input [20:1] D_INP;
input [6:1] CPU_DIN;

//Интерфейс с CPU (EPI)
inout [15:0] ADRESS_DATA;
input [19:16] ADRESS_TOP;
input nBLE;
input nBHE;
input ALE;
input nCS0;
input nCS1;
input WEn;
input OEn;

//Выходы FPGA
output [20:1] D_OUTP;
reg [20:1] out;
assign D_OUTP = out;

// TEST
localparam DATA_WIDTH = 56;

wire [DATA_WIDTH-1:0] rc_data;
wire rc_data_ready;
wire rc_connect_fail;
wire rc_invalid_data;

DATA_RECEIVER Receiver (
   .CLOCK_50(CLOCK_50), 
   .FO_IN(D_INP[3]), 
   .DATA(rc_data),
   .DATA_READY(rc_data_ready),
   .ERR_CONNECT_FAIL(rc_connect_fail),
   .ERR_INVALID_DATA(rc_invalid_data)
);
defparam Receiver.DATA_WIDTH = DATA_WIDTH;

reg [11:0] current_1;
reg [11:0] current_2;
reg [9:0]  voltage_1;
reg [9:0]  voltage_2;
reg [11:0] const_1;

always @(posedge CLOCK_50) begin
   out <= {const_1, voltage_1, voltage_2, current_1, current_2};  // Чисто, чтобы квартус не оптимизировал проект в 0
   
   if (rc_data_ready) begin
      {const_1, voltage_2, voltage_1, current_2, current_1} <= rc_data;
   end
end

endmodule