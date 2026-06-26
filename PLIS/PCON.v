module PCON(clock,clock12,
   reset_button, D_INP, D_OUTP,
   InternalPowerGood, 
   CPU_DIN,
   ADRESS_DATA, ADRESS_TOP, nBLE, nBHE, ALE, nCS0, nCS1, WEn, OEn
);

//Входы FPGA
input clock;
input clock12;
input reset_button;

input InternalPowerGood;
input [20:1] D_INP;
reg [20:1] DINP_DATA;

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


// TEST
reg CLOCK_5 = 0;
reg [3:0] div_counter;

always@(posedge clock) begin
   div_counter <= div_counter + 1;
   
   case (div_counter)
      0: begin
         CLOCK_5 <= ~CLOCK_5;
      end
      5: begin
         CLOCK_5 <= ~CLOCK_5;
         div_counter <= 0;
      end
   endcase
end

wire [15:0] data;
wire data_ready;
wire error;

DATA_RECEIVER Receiver(CLOCK_5, D_INP[3], data, data_ready, error);

endmodule