`timescale 1ns / 1ps

module tb_INV3PH;

reg CLOCK_20;
reg CLOCK_50;
reg [4:1] AMC_DATA;
reg [3:1] ADC_DATA;
reg VDR_ERR;
reg VSENS_ERR;
reg nPGOOD;
reg FO_INPUT;
reg [6:1] IGBT_ERR;

wire [4:1] AMC_CLK;
wire [3:1] ADC_CLK;
wire [3:1] ADC_NCS;
wire [3:1] CTRL_TOP;
wire [3:1] CTRL_BOT;
wire FO_OUTPUT;
wire FO_nEN;


INV3PH uut (
    .CLOCK_20   (CLOCK_20),
    .CLOCK_50   (CLOCK_50),
    .AMC_DATA   (AMC_DATA),
    .AMC_CLK    (AMC_CLK),
    .ADC_DATA   (ADC_DATA),
    .ADC_CLK    (ADC_CLK),
    .ADC_NCS    (ADC_NCS),
    .VDR_ERR    (VDR_ERR),
    .VSENS_ERR  (VSENS_ERR),
    .nPGOOD     (nPGOOD),
    .CTRL_TOP   (CTRL_TOP),
    .CTRL_BOT   (CTRL_BOT),
    .FO_INPUT   (FO_INPUT),
    .FO_OUTPUT  (FO_OUTPUT),
    .FO_nEN     (FO_nEN),
    .IGBT_ERR   (IGBT_ERR)
);


always #25 CLOCK_20 = ~CLOCK_20;   // 20 MHz


initial begin
    CLOCK_20 = 0;
    CLOCK_50 = 0;
    AMC_DATA = 0;
    ADC_DATA = 1;
    VDR_ERR = 0;
    VSENS_ERR = 0;
    nPGOOD = 1;
    FO_INPUT = 0;
    IGBT_ERR = 6'b000000;
end

endmodule