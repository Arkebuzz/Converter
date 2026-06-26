`timescale 1ns/1ns

module tb_system_top;
    reg CLOCK_10 = 0;

    always #50 CLOCK_10 = ~CLOCK_10;  // 10 МГц
    
    wire FO;
    wire [14:0] DATA_RX;
    wire DATA_READY_RX;
    wire ERROR_RX;
    
    DATA_TRANSMITTER u_tx (
        .CLOCK_10(CLOCK_10),
        .DATA_READY(1'b1),
        .DATA(15'b101010001110011),
        .READY_TO_SEND(),
        .FO_OUT(FO)
    );
    
    ADC_HUB_RECEIVER u_rx (
        .CLOCK_10(CLOCK_10),
        .FO_IN(FO),
        .DATA(DATA_RX),
        .DATA_READY(DATA_READY_RX),
        .ERROR(ERROR_RX)
    );

endmodule