`timescale 1ns/1ns

module tb_system_top;
    reg CLOCK_20 = 0;
    reg CLOCK_50 = 0;
    reg CLOCK_2_5 = 0;
    reg CLOCK_1_25 = 0;
    reg CLOCK = 0;

    always #10 CLOCK_50 = ~CLOCK_50;
    always #25 CLOCK_20 = ~CLOCK_20;
    always #200 CLOCK_2_5 = ~CLOCK_2_5;
    always #400 CLOCK_1_25 = ~CLOCK_1_25;

    wire FO;

    // DATA_TRANSMITTER u_tx (
    //     .CLOCK_10(CLOCK_10),
    //     .DATA_READY(1'b1),
    //     .DATA(15'b101010001110011),
    //     .READY_TO_SEND(),
    //     .FO_OUT(FO)
    // );
    
    // ADC_HUB_RECEIVER u_rx (
    //     .CLOCK_10(CLOCK_10),
    //     .FO_IN(FO),
    //     .DATA(DATA_RX),
    //     .DATA_READY(DATA_READY_RX),
    //     .ERROR(ERROR_RX)
    // );


    ADC_HUB u_adc_hub (
        .CLOCK_20(CLOCK_20),
        .CLOCK_50(CLOCK),
        .VDR_ERR(0),
        .VSENS_ERR(0),
        .nPGOOD(0),
        .AMC_DATA({CLOCK_20, CLOCK_20, CLOCK_2_5, 1'b1}),
        .AMC_CLK(),
        .ADC_DATA({1'b0, CLOCK_2_5, CLOCK_1_25}),
        .ADC_CLK(),
        .ADC_NCS(),
        .IGBT_ERR(0),
        .CTRL_TOP(),
        .CTRL_BOT(),
        .FO_INPUT(0),
        .FO_OUTPUT(FO),
        .FO_nEN()
    );
    
    PCON u_pcon (
        .CLOCK_50(CLOCK_50),
        .CLOCK_12(CLOCK), 
        .reset_button(1'b0),
        .D_INP({18'b0, FO, 2'b0}),
        .D_OUTP(),
        .InternalPowerGood(1'b1),
        .CPU_DIN(16'b0),
        .ADRESS_DATA(),
        .ADRESS_TOP(),
        .nBLE(),
        .nBHE(),
        .ALE(),
        .nCS0(),
        .nCS1(),
        .WEn(),
        .OEn()
    );


endmodule