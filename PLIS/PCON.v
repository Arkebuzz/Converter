module PCON(
   CLOCK_50,
   CLOCK_12,
   reset_button, 
   D_INP, 
   D_OUTP,
   InternalPowerGood, 
   CPU_DIN,
   ADRESS_DATA, 
   ADRESS_TOP, 
   nBLE, 
   nBHE, 
   ALE, 
   nCS0, 
   nCS1, 
   WEn, 
   OEn
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
localparam DATA_WIDTH = 48;

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
reg [11:0] const_1;
reg [11:0] const_2;

always @(posedge CLOCK_50) begin
   out <= {const_1, const_2, current_1, current_2};  // Чисто, чтобы квартус не оптимизировал проект в 0
   
   if (rc_data_ready) begin
      {const_2, const_1, current_2, current_1} <= rc_data;
   end
end

endmodule