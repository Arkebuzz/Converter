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
localparam DATA_WIDTH = 13;

wire [DATA_WIDTH-1:0] data;
wire data_ready;
wire error;

reg reset = 0;

DATA_RECEIVER Receiver (
   .CLOCK_50(CLOCK_50), 
   .FO_IN(D_INP[3]), 
   .RESET(reset),
   .DATA(data),
   .DATA_READY(data_ready),
   .ERR_CONNECT_FAIL(error)
);
defparam Receiver.DATA_WIDTH = DATA_WIDTH;

localparam OBJ_CUR1 = 0;
localparam OBJ_CUR2 = 1;
reg received_obj = OBJ_CUR1;

reg [11:0] current_1;
reg [11:0] current_2;

always @(posedge CLOCK_50) begin
   out <= {current_1, current_2};  // Чисто, чтобы квартус не оптимизировал проект в 0
   
   if (data_ready) begin
      case (received_obj)
         OBJ_CUR1: begin
            current_1 <= data;
            received_obj <= OBJ_CUR2;
         end

         OBJ_CUR2: begin
            current_2 <= data;
            received_obj <= OBJ_CUR1;
         end
      endcase
   end
end

endmodule