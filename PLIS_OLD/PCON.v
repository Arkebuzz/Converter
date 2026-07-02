//`define NO_CALC_DEBUG
//`define NOSYNC
`define BOARD_D2_DPR_REROUTE
module PCON(clock,clock12,
	reset_button, D_INP, D_OUTP,
	InternalPowerGood, 
	CPU_DIN,
	ADRESS_DATA, ADRESS_TOP, nBLE, nBHE, ALE, nCS0, nCS1, WEn, OEn);

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
reg [20:1] DOUT_DATA;


//------------------------------------------
//LOGIC SIGNALS
reg CalculationsError;
reg PWM_Recalc;
reg [31:0] Error_Flags /*synthesis noprune */;
reg [31:0] Error_Latch /*synthesis noprune */;
reg [15:0] CalculationsError_Mask;
reg [15:0] Error_Reset_Counter;
reg GlobalError;
reg SystemErrorReset;
//-------------------------------------


reg [31:0] PWM_Counter;
reg [31:0] PWM_AMP; 
reg [31:0] PWM_AMP_MAX;
reg signed [31:0] PWM_AMP_CALC; 
reg signed [31:0] PWM_AMP_INT;
reg [31:0] PWM_OutpFreqCounts; 
reg [31:0] PWM_PhaseShiftCounts; 
reg [31:0] PWM_Freq_Counts;
reg [31:0] PCON_PWM_Freq_Counts;
reg [31:0] PWM_CurrentPhase;
reg [31:0] PWM_CurrentPhase_CALC;  
reg [31:0] PWM_CurrentPhaseSET;  
reg [1:0] PH_GROUP_SM;

reg [31:0] SPEED_COUNTS;
reg [15:0] SPEED_DIV;
//-------------------PWM RECALC-----------------------

reg [15:0] PWM_RecalcCounter;

reg [7:0] SM_FP_ConvIntToDouble;
reg [31:0] FP_ConvIntToDoubleIN; 
reg [63:0] FP_ConvIntToDoubleOUT; 
wire [63:0] FP_ConvIntToDoubleOUTwire; 

reg [7:0] SM_FP_DIV; 
reg [63:0] FP_DIV_INA; 
reg [63:0] FP_DIV_INB;
reg [63:0] FP_DIV_OUT;  
wire [63:0] FP_DIV_OUTwire; 
wire FP_DIV_divbyzero; 
wire FP_DIV_isnan; 
wire FP_DIV_overflow;
wire FP_DIV_underflow;
wire FP_DIV_zero;

reg [7:0] SM_FP_MULT; 
reg [63:0] FP_MULT_INA; 
reg [63:0] FP_MULT_INB;
reg [63:0] FP_MULT_OUT;  
wire [63:0] FP_MULT_OUTwire; 
wire FP_MULT_isnan; 
wire FP_MULT_overflow;
wire FP_MULT_underflow;
wire FP_MULT_zero;


reg [7:0] SM_FP_CONVBACK; 
reg [63:0] FP_CONVBACK_IN; 
reg [31:0] FP_CONVBACK_OUT;  
wire [31:0] FP_CONVBACK_OUTwire; 
wire FP_CONVBACK_isnan; 
wire FP_CONVBACK_overflow;
wire FP_CONVBACK_underflow;

reg [7:0] SM_FP_CONVDTF; 
reg [63:0] FP_CONVDTF_IN; 
reg [31:0] FP_CONVDTF_OUT;  
wire [31:0] FP_CONVDTF_OUTwire; 
wire FP_CONVDTF_isnan; 
wire FP_CONVDTF_overflow;
wire FP_CONVDTF_underflow;

reg [7:0] SM_FP_CONVFTD; 
reg [31:0] FP_CONVFTD_IN; 
reg [63:0] FP_CONVFTD_OUT;  
wire [63:0] FP_CONVFTD_OUTwire; 
wire FP_CONVFTD_isnan; 
wire FP_CONVFTD_overflow;
wire FP_CONVFTD_underflow;

reg [7:0] SM_FP_ADDSUB;
reg FP_ADDSUB_CONTROL;
reg [63:0] FP_ADDSUB_INA; 
reg [63:0] FP_ADDSUB_INB; 
reg [63:0] FP_ADDSUB_OUT;  
wire [63:0] FP_ADDSUB_OUTwire; 
wire FP_ADDSUB_isnan; 
wire FP_ADDSUB_overflow;
wire FP_ADDSUB_underflow;
wire FP_ADDSUB_zero;

reg [7:0] SM_FP_SIN; 
reg [31:0] FP_SIN_IN; 
reg [31:0] FP_SIN_OUT;  
wire [31:0] FP_SIN_OUTwire; 

wire [31:0] PWM_AMP_W; 
wire [31:0] PWM_AMP_MAX_W; 
wire [31:0] PWM_OutpFreqCounts_W; 
wire [31:0] PWM_PhaseShiftCounts_W; 
wire [31:0] PWM_Freq_Counts_W;
wire [31:0] PWM_CurrentPhase_W;  
wire [31:0] PWM_CurrentPhaseSET_W;  

reg [63:0] FP_PWM_PhaseCounts; // FP_PWM_PhaseCounts = CONVERT(PWM_CurrentPhase)
reg [63:0] FP_PWM_PhaseShiftCounts; // FP_PWM_PhaseShiftCounts = CONVERT(PWM_PhaseShiftCounts)
reg [63:0] FP_PWM_OutputFrequencyCounts; // FP_PWM_OutputFrequencyCounts = CONVERT(PWM_OutpFreqCounts)

reg [63:0] FP_PWM_AmpMax; // FP_PWM_AmpMax = CONVERT(PWM_AMP_MAX)
reg [63:0] FP_PWM_AmpSet; // FP_PWM_AmpSet = CONVERT(PWM_AMP)
reg [63:0] FP_PWM_MaxCounts; // FP_PWM_MaxCounts = CONVERT(PWM_Freq_Counts)

wire [63:0] FP_PWM_Num_3_W; // FP_PWM_Num_3 = 64'h4008000000000000;
wire [63:0] FP_PWM_Num_2PI_W; // FP_PWM_Num_2PI = 64'h401921FB54442D18;
wire [63:0] FP_PWM_Num_1_W; // FP_PWM_Num_1 = 64'h3FF0000000000000;
wire [63:0] FP_PWM_Num_2_W; // FP_PWM_Num_2 = 64'h4000000000000000;
wire [63:0] FP_PWM_Num_1div3_W; //FP_PWM_Num_1div3 = 64'h467A4B9CF15C92C9;

reg [63:0] FP_PWM_Num_3; // FP_PWM_Num_3 = 64'h4008000000000000;
reg [63:0] FP_PWM_Num_2PI; // FP_PWM_Num_2PI = 64'h401921FB54442D18;
reg [63:0] FP_PWM_Num_1; // FP_PWM_Num_1 = 64'h3FF0000000000000;
reg [63:0] FP_PWM_Num_2; // FP_PWM_Num_2 = 64'h4000000000000000;
reg [63:0] FP_PWM_Num_1div3; //FP_PWM_Num_1div3 = 64'h467A4B9CF15C92C9;

reg [63:0] FP_CALC_PHASEA_COUNTS; //FP_CALC_PHASEA_COUNTS = FP_PWM_PhaseCounts + FP_PWM_PhaseShiftCounts  
reg [63:0] FP_CALC_PHASEA_SECTOR; //FP_CALC_PHASEA_SECTOR = FP_CALC_PHASEA_COUNTS/FP_PWM_OutputFrequencyCounts
reg [63:0] FP_CALC_PHASEB_SECTOR; // FP_CALC_PHASEB_SECTOR = FP_CALC_PHASEA_SECTOR + FP_PWM_Num_1div3
reg [63:0] FP_CALC_PHASEC_SECTOR; // FP_CALC_PHASEC_SECTOR = FP_CALC_PHASEA_SECTOR - FP_PWM_Num_1div3 

reg [63:0] FP_CALC_PHASEA_PHASE; // FP_CALC_PHASEA_PHASE = FP_CALC_PHASEA_SECTOR*FP_PWM_Num_2PI
reg [63:0] FP_CALC_PHASEB_PHASE; // FP_CALC_PHASEB_PHASE = FP_CALC_PHASEB_SECTOR*FP_PWM_Num_2PI
reg [63:0] FP_CALC_PHASEC_PHASE; // FP_CALC_PHASEC_PHASE = FP_CALC_PHASEC_SECTOR*FP_PWM_Num_2PI

reg [31:0] FP_CALC_PHASEA_PHASE_F; // FP_CALC_PHASEA_PHASE_F = CONVERTDTF(FP_CALC_PHASEA_PHASE)
reg [31:0] FP_CALC_PHASEB_PHASE_F; // FP_CALC_PHASEB_PHASE_F = CONVERTDTF(FP_CALC_PHASEB_PHASE)
reg [31:0] FP_CALC_PHASEC_PHASE_F; // FP_CALC_PHASEC_PHASE_F = CONVERTDTF(FP_CALC_PHASEC_PHASE)

reg [31:0] FP_CALC_PHASEA_SIN_F; // FP_CALC_PHASEA_SIN_F = sin(FP_CALC_PHASEA_PHASE_F)
reg [31:0] FP_CALC_PHASEB_SIN_F; // FP_CALC_PHASEB_SIN_F = sin(FP_CALC_PHASEB_PHASE_F)
reg [31:0] FP_CALC_PHASEC_SIN_F; // FP_CALC_PHASEC_SIN_F = sin(FP_CALC_PHASEC_PHASE_F)

reg [63:0] FP_CALC_PHASEA_SIN; // FP_CALC_PHASEA_SIN = CONVERTFTD(FP_CALC_PHASEA_SIN_F)
reg [63:0] FP_CALC_PHASEB_SIN; // FP_CALC_PHASEB_SIN = CONVERTFTD(FP_CALC_PHASEB_SIN_F)
reg [63:0] FP_CALC_PHASEC_SIN; // FP_CALC_PHASEC_SIN = CONVERTFTD(FP_CALC_PHASEC_SIN_F)

reg [63:0] FP_CALC_HALFMAXCOUNTS; // FP_CALC_HALFMAXCOUNTS = FP_PWM_MaxCounts / FP_PWM_Num_2
reg [63:0] FP_CALC_RELATIVEAMP; // FP_CALC_RELATIVEAMP = FP_PWM_AmpSet / FP_PWM_AmpMax
reg [63:0] FP_CALC_ABSOLUTEAMP; // FP_CALC_ABSOLUTEAMP = FP_CALC_RELATIVEAMP * FP_CALC_HALFMAXCOUNTS

reg [63:0] FP_PWM_PHAPWM_SCALED; // FP_PWM_PHAPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEA_SIN
reg [63:0] FP_PWM_PHBPWM_SCALED; // FP_PWM_PHBPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEB_SIN
reg [63:0] FP_PWM_PHCPWM_SCALED; // FP_PWM_PHCPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEC_SIN

reg [63:0] FP_CALC_PHASEAPWM_SHIFTED; // FP_CALC_PHASEAPWM_SHIFTED = FP_PWM_PHAPWM_SCALED + FP_CALC_HALFMAXCOUNTS
reg [63:0] FP_CALC_PHASEBPWM_SHIFTED; // FP_CALC_PHASEBPWM_SHIFTED = FP_PWM_PHBPWM_SCALED + FP_CALC_HALFMAXCOUNTS
reg [63:0] FP_CALC_PHASECPWM_SHIFTED; // FP_CALC_PHASECPWM_SHIFTED = FP_PWM_PHCPWM_SCALED + FP_CALC_HALFMAXCOUNTS

reg [31:0] PhA_PWM_CALC; // PhA_PWM_CALC = CONVERTBACK(FP_CALC_PHASEAPWM_SHIFTED)
reg [31:0] PhB_PWM_CALC; // PhB_PWM_CALC = CONVERTBACK(FP_CALC_PHASEBPWM_SHIFTED)
reg [31:0] PhC_PWM_CALC; // PhC_PWM_CALC = CONVERTBACK(FP_CALC_PHASECPWM_SHIFTED)

reg [31:0] PhU1_PWM_CALC; // PhA_PWM_CALC = CONVERTBACK(FP_CALC_PHASEAPWM_SHIFTED)
reg [31:0] PhV1_PWM_CALC; // PhB_PWM_CALC = CONVERTBACK(FP_CALC_PHASEBPWM_SHIFTED)
reg [31:0] PhW1_PWM_CALC; // PhC_PWM_CALC = CONVERTBACK(FP_CALC_PHASECPWM_SHIFTED)

reg [31:0] PhU2_PWM_CALC; // PhA_PWM_CALC = CONVERTBACK(FP_CALC_PHASEAPWM_SHIFTED)
reg [31:0] PhV2_PWM_CALC; // PhB_PWM_CALC = CONVERTBACK(FP_CALC_PHASEBPWM_SHIFTED)
reg [31:0] PhW2_PWM_CALC; // PhC_PWM_CALC = CONVERTBACK(FP_CALC_PHASECPWM_SHIFTED)

reg [31:0] PhU3_PWM_CALC; // PhA_PWM_CALC = CONVERTBACK(FP_CALC_PHASEAPWM_SHIFTED)
reg [31:0] PhV3_PWM_CALC; // PhB_PWM_CALC = CONVERTBACK(FP_CALC_PHASEBPWM_SHIFTED)
reg [31:0] PhW3_PWM_CALC; // PhC_PWM_CALC = CONVERTBACK(FP_CALC_PHASECPWM_SHIFTED)

reg [31:0] InitCalcCounter;

reg [11:0] PhU1_PWM; 
reg [11:0] PhV1_PWM; 
reg [11:0] PhW1_PWM; 
reg [11:0] PhU2_PWM; 
reg [11:0] PhV2_PWM; 
reg [11:0] PhW2_PWM; 
reg [11:0] PhU3_PWM; 
reg [11:0] PhV3_PWM; 
reg [11:0] PhW3_PWM; 

reg [8:0] RAM_SM_COUNTER;
reg [9:0] RAM_ADRESS;
reg [63:0] RAM_DATAIN;
wire [63:0] RAM_DATAOUTwire;
reg [63:0] RAM_DATAOUT;
reg RAM_WREN;

reg clock2_reg;

reg [2:1] SKiiP_ERR_DATA;
reg PCON_IN_DATA;
reg [6:1] SENS_IN_DATA;
reg [4:1] DINPUT_DATA;
reg INP_1V2OK_DATA;
reg INP_2V5OK_DATA;
reg INP_3V3OK_DATA;

reg [4:1] SKiiP_ENABLE;
reg [4:1] SKiiP_CONTROL;
reg [2:1] DIGITAL_OUTPUT;

reg BOARD_FO_EN_BUF;
reg OUTP_EN_BUF;
reg [2:1] DOUTPUT_BUF;


reg [25:0] clock_div;
reg [5:0] clock_div2;

reg [35:0] DATA_TO_EXC /* synthesis noprune */;
wire [143:0] DATA_FROM_EXC_wire;
reg [143:0] DATA_FROM_EXC /* synthesis noprune */;
wire EXC_DATA_READY;
wire [2:0] EXC_IO_ERR;
wire EXC_FOUT;

reg [47:0] DATA_TO_INV1 /* synthesis noprune */;
wire [95:0] DATA_FROM_INV1_wire;
reg [95:0] DATA_FROM_INV1 /* synthesis noprune */;
wire INV1_DATA_READY/* synthesis noprune */;
wire [2:0] INV1_IO_ERR;
wire INV1_FOUT;

reg [47:0] DATA_TO_INV2 /* synthesis noprune */;
wire [95:0] DATA_FROM_INV2_wire;
reg [95:0] DATA_FROM_INV2 /* synthesis noprune */;
wire INV2_DATA_READY/* synthesis noprune */;
wire [2:0] INV2_IO_ERR;
wire INV2_FOUT;

reg [47:0] DATA_TO_INV3 /* synthesis noprune */;
wire [95:0] DATA_FROM_INV3_wire;
reg [95:0] DATA_FROM_INV3 /* synthesis noprune */;
wire INV3_DATA_READY/* synthesis noprune */;
wire [2:0] INV3_IO_ERR;
wire INV3_FOUT;

wire [2:0] DPR_DATA_wire;
reg [2:0] DPR_DATA /* synthesis noprune */;
wire [71:0] DPR_TEMP_DATA_wire;
reg [71:0] DPR_TEMP_DATA /* synthesis noprune */;
reg DPR_DATA_READY;
wire DPR_DATA_READY_wire;
reg [2:0] DPR_IO_ERR;
wire [2:0] DPR_IO_ERR_wire; 

`ifdef BOARD_D2_DPR_REROUTE
	wire [2:0] DPR_DATA_wire2;
	reg [2:0] DPR_DATA2 /* synthesis noprune */;
	wire [71:0] DPR_TEMP_DATA_wire2;
	reg [71:0] DPR_TEMP_DATA2 /* synthesis noprune */;
	reg DPR_DATA_READY2;
	wire DPR_DATA_READY_wire2;
	reg [2:0] DPR_IO_ERR2;
	wire [2:0] DPR_IO_ERR_wire2; 
`endif

reg [31:0] PWM_AMP_INP; 
reg [31:0] PWM_PhaseShiftCounts_INP; 
reg [31:0] Chopper_PWM_PW;
reg [31:0] Chopper_PWM_PW_INP;
reg [31:0] Excitation_PWM_PW;  
reg [31:0] Excitation_PWM_PW_INP;
reg [31:0] MainFan_PWM_PW;
reg [31:0] MainFan_PWM_PW_INP;


reg [31:0] ERROR_FLAGS /*synthesis preserve */;
reg [31:0] ERROR_LATCH /*synthesis preserve */;

reg[11:0] EXC_ChoppCurrent /* synthesis noprune */;
reg[11:0] EXC_WindingCurrent1 /* synthesis noprune */;
reg[11:0] EXC_WindingCurrent2 /* synthesis noprune */;
reg[11:0] EXC_VoltageIn /* synthesis noprune */;
reg[11:0] EXC_VoltageOut /* synthesis noprune */;
reg[11:0] EXC_Temp1 /* synthesis noprune */;
reg[11:0] EXC_Temp2 /* synthesis noprune */;
reg[11:0] EXC_Temp3 /* synthesis noprune */;
reg[11:0] EXC_Errors /* synthesis noprune */;

reg[11:0] INV1_IGBTCurrent1 /* synthesis noprune */;
reg[11:0] INV1_IGBTCurrent2 /* synthesis noprune */;
reg[11:0] INV1_IGBTCurrent3 /* synthesis noprune */;
reg[11:0] INV1_Temp1 /* synthesis noprune */;
reg[11:0] INV1_Temp2 /* synthesis noprune */;
reg[11:0] INV1_Temp3 /* synthesis noprune */;
reg[11:0] INV1_Voltage /* synthesis noprune */;
reg[11:0] INV1_ERRORS /* synthesis noprune */;

reg[11:0] INV2_IGBTCurrent1 /* synthesis noprune */;
reg[11:0] INV2_IGBTCurrent2 /* synthesis noprune */;
reg[11:0] INV2_IGBTCurrent3 /* synthesis noprune */;
reg[11:0] INV2_Temp1 /* synthesis noprune */;
reg[11:0] INV2_Temp2 /* synthesis noprune */;
reg[11:0] INV2_Temp3 /* synthesis noprune */;
reg[11:0] INV2_Voltage /* synthesis noprune */;
reg[11:0] INV2_ERRORS /* synthesis noprune */;

reg[11:0] INV3_IGBTCurrent1 /* synthesis noprune */;
reg[11:0] INV3_IGBTCurrent2 /* synthesis noprune */;
reg[11:0] INV3_IGBTCurrent3 /* synthesis noprune */;
reg[11:0] INV3_Temp1 /* synthesis noprune */;
reg[11:0] INV3_Temp2 /* synthesis noprune */;
reg[11:0] INV3_Temp3 /* synthesis noprune */;
reg[11:0] INV3_Voltage /* synthesis noprune */;
reg[11:0] INV3_ERRORS /* synthesis noprune */;

reg[11:0] GEN_TEMP1/* synthesis noprune */;
reg[11:0] GEN_TEMP2/* synthesis noprune */;
reg[11:0] GEN_TEMP3/* synthesis noprune */;
reg[11:0] GEN_TEMP4/* synthesis noprune */;
reg[11:0] GEN_TEMP5/* synthesis noprune */;
reg[11:0] GEN_TEMP6/* synthesis noprune */;

reg DPR_RESET;
reg [2:0] hall_filtered/* synthesis noprune */;
reg [31:0] theta_elec/* synthesis noprune */;
reg signed [31:0] velocity /* synthesis noprune */;
reg signed [31:0] accel /* synthesis noprune */;
wire [2:0] hall_filtered_wire;
wire [31:0] theta_elec_wire/* synthesis noprune */;
wire [31:0] velocity_wire/* synthesis noprune */;
wire [31:0] accel_wire;
wire [7:0] DPR_error_code/* synthesis noprune */;
wire DPR_critical_error;
wire DPR_timeout_warning;
reg [7:0] DPR_ERRORS/* synthesis noprune */;

wire vel_sign      = velocity[31];
wire [31:0] vel_abs = vel_sign ? -velocity : velocity;
wire [63:0] phaseshift_abs = {32'b0, vel_abs} * {32'b0, PCON_PWM_Freq_Counts};
wire [31:0] delta_angle = vel_sign 
    ? -phaseshift_abs[31:0]   
    :  phaseshift_abs[31:0];
 




reg [7:0] RESET_COUNTER;



reg [13:0] EMIF_ADRESS;
reg [15:0] EMIF_DATA_TO_RAM;
reg [15:0] EMIF_DATA_FROM_RAM;
wire [15:0] EMIF_DATA_FROM_RAM_wire;
reg EMIF_WREN;

reg [15:0] EMIF_FILL_COUNTER; 

reg [15:0] Error_Reset_Command;
reg [15:0] Error_Reset_Command_Inp;

reg [15:0] EMIF_Watchdog_PrevValue;
reg [15:0] EMIF_Watchdog_CurrentValue;
reg [15:0] EMIF_Watchdog_CurrentValue_Inp;
wire [15:0] EMIF_Watchdog_CurrentValue_W;
reg EMIF_INIT;
reg [15:0] DATA_COMMIT_COMMAND; 
parameter DATA_COMMIT_VALUE = 43690;
parameter Error_Reset_Value = 2014;
parameter Error_Reset_Length = 30000;

reg Activate_Inverter;
reg Activate_Inverter_Input;
reg Activate_Excitation;
reg Activate_Excitation_Input;

reg PWM_Sync /* synthesis noprune */;

reg [31:0] ActivatedCounter; //TEMP!!!!
reg ActivatedError; //TEMP!!!!

assign D_OUTP[2] = BOARD_FO_EN_BUF;
assign D_OUTP[3] = ~EXC_FOUT;
assign D_OUTP[4] = ~INV1_FOUT;
assign D_OUTP[5] = ~INV2_FOUT;
assign D_OUTP[6] = ~INV3_FOUT;
assign D_OUTP[19] = 0; //TEMP!!!!
assign D_OUTP[20] = D_INP[19]; //TEMP!!!!


EXC_DATA_IO ExcitationModule(clock, D_INP[1], EXC_FOUT, DATA_TO_EXC, DATA_FROM_EXC_wire, EXC_DATA_READY, EXC_IO_ERR);
INV_DATA_IO InvModule1(clock, D_INP[3], INV1_FOUT, DATA_TO_INV1, DATA_FROM_INV1_wire, INV1_DATA_READY, INV1_IO_ERR, PWM_Sync);
INV_DATA_IO InvModule2(clock, D_INP[4], INV2_FOUT, DATA_TO_INV2, DATA_FROM_INV2_wire, INV2_DATA_READY, INV2_IO_ERR, PWM_Sync);
INV_DATA_IO InvModule3(clock, D_INP[5], INV3_FOUT, DATA_TO_INV3, DATA_FROM_INV3_wire, INV3_DATA_READY, INV3_IO_ERR, PWM_Sync);
DPR_DATA_IN DPRModule(clock, D_INP[19], DPR_DATA_wire, DPR_TEMP_DATA_wire, DPR_DATA_READY_wire, DPR_IO_ERR_wire);

`ifdef BOARD_D2_DPR_REROUTE
DPR_DATA_IN DPRModule2(clock, D_INP[20], DPR_DATA_wire2, DPR_TEMP_DATA_wire2, DPR_DATA_READY_wire2, DPR_IO_ERR_wire2);
`endif

bldc_statistical(.clk(clock), .rst_n(DPR_RESET), .hall_a_in(~DPR_DATA[0]), .hall_b_in(~DPR_DATA[1]), .hall_c_in(~DPR_DATA[2]), 
	.theta_elec(theta_elec_wire), .velocity(velocity_wire), .accel(accel_wire), 
	.error_code(DPR_error_code), .critical_error(DPR_critical_error), .timeout_warning(DPR_timeout_warning));

assign FP_PWM_Num_2PI_W = 64'h401921FB54442D18;
assign FP_PWM_Num_3_W = 64'h4008000000000000;
assign FP_PWM_Num_2_W = 64'h4000000000000000;
assign FP_PWM_Num_1_W = 64'h3FF0000000000000;
assign FP_PWM_Num_1div3_W = 64'h3FD5555555555555;



//pllka(.inclk0(clock),.c0(clock2));
//RAMACCESS(.address(RAM_ADRESS),.clock(clock),.data(RAM_DATAIN),.wren(RAM_WREN),.q(RAM_DATAOUTwire));
EMIF(.ad(ADRESS_DATA), .cs0(nCS0), .cs1(nCS1), .wr(WEn), .oe(OEn), .ale(ALE), .clk(clock), .clk2(clock), .ftmdata(EMIF_DATA_TO_RAM), .ftmadress(EMIF_ADRESS) , .ftmwen(EMIF_WREN) , .mtfdata(EMIF_DATA_FROM_RAM_wire));
   
FP_CONVERT(.clock(clock), .dataa(FP_ConvIntToDoubleIN), .result(FP_ConvIntToDoubleOUTwire));
FP_DIV(.clock(clock),.dataa(FP_DIV_INA),.datab(FP_DIV_INB),.division_by_zero(FP_DIV_divbyzero),.nan(FP_DIV_isnan),.overflow(FP_DIV_overflow),.result(FP_DIV_OUTwire),.underflow(FP_DIV_underflow),.zero(FP_DIV_zero));
FP_MULT(.clock(clock),.dataa(FP_MULT_INA),.datab(FP_MULT_INB),.nan(FP_MULT_isnan),.overflow(FP_MULT_overflow),.result(FP_MULT_OUTwire),.underflow(FP_MULT_underflow),.zero(FP_MULT_zero));
CONVBACK(.clock(clock),.dataa(FP_CONVBACK_IN),.nan(FP_CONVBACK_isnan),.overflow(FP_CONVBACK_overflow),.result(FP_CONVBACK_OUTwire),.underflow(FP_CONVBACK_underflow));
CONVDTF(.clock(clock), .dataa(FP_CONVDTF_IN),.nan(FP_CONVDTF_isnan),.overflow(FP_CONVDTF_overflow),.result(FP_CONVDTF_OUTwire),.underflow(FP_CONVDTF_underflow));
CONVERTFTD(.clock(clock), .dataa(FP_CONVFTD_IN),.nan(FP_CONVFTD_isnan),.overflow(FP_CONVFTD_overflow),.result(FP_CONVFTD_OUTwire),.underflow(FP_CONVFTD_underflow));
FP_ADDSUB(.add_sub(FP_ADDSUB_CONTROL),.clock(clock),.dataa(FP_ADDSUB_INA),.datab(FP_ADDSUB_INB),.nan(FP_ADDSUB_isnan),.overflow(FP_ADDSUB_overflow),.result(FP_ADDSUB_OUTwire),.underflow(FP_ADDSUB_underflow),.zero(FP_ADDSUB_zero));
FP_SIN(.clock(clock),.data(FP_SIN_IN),.result(FP_SIN_OUTwire));


//Инициализация
initial
begin
PWM_PhaseShiftCounts = 0;
PWM_AMP = 5000;
PWM_AMP_MAX = 10000;
PWM_Freq_Counts = 10000; // 200uS Duty Cycle
PWM_OutpFreqCounts = 1000000; //50Hz
FP_PWM_Num_2PI = 64'h401921FB54442D18;
FP_PWM_Num_3 = 64'h4008000000000000;
FP_PWM_Num_2 = 64'h4000000000000000;
FP_PWM_Num_1 = 64'h3FF0000000000000;
FP_PWM_Num_1div3 = 64'h3FD5555555555555;
PWM_RecalcCounter=0;
BOARD_FO_EN_BUF=1;
DOUTPUT_BUF = 0;
EMIF_INIT = 0;
DPR_RESET<=0;
RESET_COUNTER<=0;
PWM_Sync<=0;
ActivatedCounter<=0;
end


//----------------------------

//Защитная логика. Выполняется всегда.
always@*
begin

Activate_Excitation_Input = CPU_DIN[5];


EMIF_DATA_FROM_RAM = EMIF_DATA_FROM_RAM_wire;

CalculationsError = 	FP_ADDSUB_isnan | FP_ADDSUB_overflow |  FP_DIV_divbyzero |
							FP_DIV_isnan | FP_DIV_overflow | FP_MULT_isnan | FP_MULT_overflow | 
							FP_CONVDTF_isnan | FP_CONVDTF_overflow |  FP_CONVFTD_isnan |
							FP_CONVFTD_overflow |  FP_CONVBACK_isnan | FP_CONVBACK_overflow;
							
`ifndef NO_CALC_DEBUG
Error_Flags[0] = CalculationsError; 
`else
Error_Flags[0] = 0; 
`endif

Error_Flags[1] = EXC_Errors[0]|EXC_Errors[1]|EXC_Errors[2]; //EXC IGBT
Error_Flags[2] = EXC_Errors[3]|EXC_Errors[4]|EXC_Errors[5]|EXC_Errors[6]; //EXC PROT
Error_Flags[3] = INV1_ERRORS[0]|INV1_ERRORS[1]|INV1_ERRORS[2]; // IGBT
Error_Flags[4] = INV1_ERRORS[4]|INV1_ERRORS[5]|INV1_ERRORS[6]; //VERR,IERR
Error_Flags[5] = INV2_ERRORS[0]|INV2_ERRORS[1]|INV2_ERRORS[2]; // IGBT
Error_Flags[6] = INV2_ERRORS[4]|INV2_ERRORS[5]|INV2_ERRORS[6]; //VERR,IERR
Error_Flags[7] = INV3_ERRORS[0]|INV3_ERRORS[1]|INV3_ERRORS[2]; // IGBT
Error_Flags[8] = INV3_ERRORS[4]|INV3_ERRORS[5]|INV3_ERRORS[6]; //VERR,IERR
Error_Flags[9] = INV3_ERRORS[0];
Error_Flags[10] = INV3_ERRORS[1];
Error_Flags[11] = INV3_ERRORS[2];
Error_Flags[12] = INV1_ERRORS[3]; //SYNC TEMP
Error_Flags[13] = INV2_ERRORS[3];
Error_Flags[14] = INV3_ERRORS[3];
Error_Flags[15] = 0;
    
Error_Flags[16] = 0; //TEMP!!!!
Error_Flags[17] = 0;
Error_Flags[18] = 0;
Error_Flags[19] = 0;
Error_Flags[20] = 0;
Error_Flags[21] = 0;
Error_Flags[22] = 0;
Error_Flags[23] = DPR_critical_error;
Error_Flags[24] = ~D_INP[14]; //ERR BUTTON
Error_Flags[25] = EXC_IO_ERR[0]||INV1_IO_ERR[0]||INV2_IO_ERR[0]||INV3_IO_ERR[0]||DPR_IO_ERR_wire[0]; //NO DATA
Error_Flags[26] = ~CPU_DIN[1]; //C28_ERR
Error_Flags[27] = INV1_ERRORS[7]; //INV1 TRANSMITT
Error_Flags[28] = INV2_ERRORS[7]; //INV2 TRANSMITT
Error_Flags[29] = INV3_ERRORS[7]; //INV3 TRANSMITT
Error_Flags[30] = EXC_Errors[7]; //EXC TRANSMITT
Error_Flags[31] = GlobalError;

if (Error_Reset_Counter == 0) 
	begin
	Error_Latch = Error_Latch | Error_Flags;
	CalculationsError_Mask[0] = CalculationsError_Mask[0]|FP_ADDSUB_isnan;
	CalculationsError_Mask[1] = CalculationsError_Mask[1]|FP_ADDSUB_overflow;
	CalculationsError_Mask[2] = CalculationsError_Mask[2]|FP_DIV_divbyzero;
	CalculationsError_Mask[3] = CalculationsError_Mask[3]|FP_DIV_isnan;
	CalculationsError_Mask[4] = CalculationsError_Mask[4]|FP_DIV_overflow;
	CalculationsError_Mask[5] = CalculationsError_Mask[5]|FP_MULT_isnan;
	CalculationsError_Mask[6] = CalculationsError_Mask[6]|FP_MULT_overflow;
	CalculationsError_Mask[7] = CalculationsError_Mask[7]|FP_CONVDTF_isnan;
	CalculationsError_Mask[8] = CalculationsError_Mask[8]|FP_CONVDTF_overflow;
	CalculationsError_Mask[9] = CalculationsError_Mask[9]|FP_CONVFTD_isnan;
	CalculationsError_Mask[10] = CalculationsError_Mask[10]|FP_CONVFTD_overflow;
	CalculationsError_Mask[11] = CalculationsError_Mask[11]|FP_CONVBACK_isnan;
	CalculationsError_Mask[12] = CalculationsError_Mask[12]|FP_CONVBACK_overflow;
	end
else 
	begin
	CalculationsError_Mask = 0;
	Error_Latch = Error_Flags & (32'h07FFFFFF);
	end
	
if ((Error_Latch == 0))
		begin
		GlobalError=0;
		BOARD_FO_EN_BUF=0;
		OUTP_EN_BUF=0;
		Activate_Inverter = Activate_Inverter_Input;
		Activate_Excitation = Activate_Excitation_Input;
		end
else
		begin
		if (Error_Reset_Counter == 0) 
		begin
		BOARD_FO_EN_BUF=1;
		OUTP_EN_BUF=1;
		end
		else
		begin
		BOARD_FO_EN_BUF=0;
		OUTP_EN_BUF=0;
		end
		Activate_Inverter = 0;
		Activate_Excitation = 0;
		GlobalError=1;
		end
end




//----------------------------
always@(posedge EXC_DATA_READY) begin DATA_FROM_EXC <= DATA_FROM_EXC_wire; end
always@(posedge INV1_DATA_READY) begin DATA_FROM_INV1 <= DATA_FROM_INV1_wire; end
always@(posedge INV2_DATA_READY) begin DATA_FROM_INV2 <= DATA_FROM_INV2_wire;  end
always@(posedge INV3_DATA_READY) begin DATA_FROM_INV3 <= DATA_FROM_INV3_wire; end
`ifdef BOARD_D2_DPR_REROUTE
always@(posedge DPR_DATA_READY_wire2) begin DPR_TEMP_DATA <= DPR_TEMP_DATA_wire2; end
`else
always@(posedge DPR_DATA_READY_wire) begin DPR_TEMP_DATA <= DPR_TEMP_DATA_wire; end
`endif

//Main clocked cycle
always@(posedge clock)
begin

if (Error_Reset_Counter == 0)
	begin	
		DPR_RESET<=1;
		SystemErrorReset = 0;
		DATA_TO_EXC[24]<=0; //nERR RESET
		DATA_TO_INV1[36] <= 0; //nReset Error
		DATA_TO_INV2[36] <= 0; //nReset Error
		DATA_TO_INV3[36] <= 0; //nReset Error
	end
else
	begin
	DPR_RESET<=0; 
	Error_Reset_Counter = Error_Reset_Counter - 1;
	SystemErrorReset = 1;
	DATA_TO_EXC[24] <= 1; //ERR RESET
	DATA_TO_INV1[36] <= 1; //Reset Error
	DATA_TO_INV2[36] <= 1; //Reset Error
	DATA_TO_INV3[36] <= 1; //Reset Error	
	end

DPR_ERRORS<=DPR_error_code;
theta_elec<=theta_elec_wire;
velocity<=velocity_wire;
accel<=accel_wire;
hall_filtered<=hall_filtered_wire;

if (Activate_Excitation==1)
begin
	DATA_TO_EXC[11:0]  <= Chopper_PWM_PW[11:0]; //BUCK_PWM 2500-MAX  
	DATA_TO_EXC[23:12] <= Excitation_PWM_PW[11:0]; //EXC_PWM 2500-MAX
	DATA_TO_EXC[25]<=1; //EXC ACTIVATE
end
else
begin
	DATA_TO_EXC[11:0] <= 0; //BUCK_PWM 2500-MAX
	DATA_TO_EXC[23:12] <= 0; //EXC_PWM 2500-MAX
	DATA_TO_EXC[25] <= 0; //EXC ACTIVATE
end



PCON_PWM_Freq_Counts = 16500; //x10 PWM_Freq_Counts, 10000 - 5kHz, 16500 - 3kHz,
PWM_Freq_Counts = 1650; // 200uS Duty Cycle at 20MHz , 1000 - 5kHz, 1650 - 3kHz

if (PWM_Counter<PCON_PWM_Freq_Counts) 
		begin 
		PWM_Counter = PWM_Counter + 1; 
		if (PWM_Counter>(PCON_PWM_Freq_Counts-50)) begin PWM_Sync <= 1; end
		else begin PWM_Sync <= 0; end;
		if (CPU_DIN[6]==0) begin Activate_Inverter_Input = 0; end
		if (PWM_Recalc == 0) 
				begin 
				PhU1_PWM = PhU1_PWM_CALC; 
				PhV1_PWM = PhV1_PWM_CALC;
				PhW1_PWM = PhW1_PWM_CALC;
				
				PhU2_PWM = PhU2_PWM_CALC; 
				PhV2_PWM = PhV2_PWM_CALC;
				PhW2_PWM = PhW2_PWM_CALC;
				
				PhU3_PWM = PhU3_PWM_CALC; 
				PhV3_PWM = PhV3_PWM_CALC;
				PhW3_PWM = PhW3_PWM_CALC;
				PWM_Recalc = 1; 
				PWM_RecalcCounter=0; 
				end
		end 
	else
		begin
		Activate_Inverter_Input = CPU_DIN[6];
		PWM_Counter = 0;
		PWM_AMP_MAX = 10000;
		PWM_CurrentPhase = theta_elec + PWM_PhaseShiftCounts + delta_angle;
		PWM_OutpFreqCounts = 32'h7FFFFFFF;
		PWM_Sync <= 0;
		end

if (Activate_Inverter==1)
begin
	//if (ActivatedCounter<500000000) begin ActivatedCounter<=ActivatedCounter+1; end //TEMP!!!!
	//if (ActivatedCounter>100000000) begin ActivatedError <= 1; end //TEMP!!!!
	DATA_TO_INV1[37] <= 1; //Activate inverter	
	DATA_TO_INV2[37] <= 1; //Activate inverter
	DATA_TO_INV3[37] <= 1; //Activate inverter
	
	DATA_TO_INV1[39:38] <= 2; //SET CURRENT LIMIT
	DATA_TO_INV2[39:38] <= 2; //SET CURRENT LIMIT 
	DATA_TO_INV3[39:38] <= 2; //SET CURRENT LIMIT 
	
	DATA_TO_INV1[11:0] <= PhU1_PWM_CALC;
	DATA_TO_INV1[23:12] <= PhV1_PWM_CALC;
	DATA_TO_INV1[35:24] <= PhW1_PWM_CALC;
	DATA_TO_INV2[11:0] <= PhU2_PWM_CALC;
	DATA_TO_INV2[23:12] <= PhV2_PWM_CALC;
	DATA_TO_INV2[35:24] <= PhW2_PWM_CALC;
	DATA_TO_INV3[11:0] <= PhU3_PWM_CALC;
	DATA_TO_INV3[23:12] <= PhV3_PWM_CALC;
	DATA_TO_INV3[35:24] <= PhW3_PWM_CALC;
end
else 
begin
	//ActivatedCounter<=0; //TEMP!!!!
   //ActivatedError<=0; //TEMP!!!!
	
	DATA_TO_INV1[37] <= 0; //Activate inverter 
	DATA_TO_INV2[37] <= 0; //Activate inverter 
	DATA_TO_INV3[37] <= 0; //Activate inverter 
	
	DATA_TO_INV1[39:38] <= 0; //SET CURRENT LIMIT 
	DATA_TO_INV2[39:38] <= 0; //SET CURRENT LIMIT 
	DATA_TO_INV3[39:38] <= 0; //SET CURRENT LIMIT 

	DATA_TO_INV1[11:0] <= PhU1_PWM_CALC;
	DATA_TO_INV1[23:12] <= PhV1_PWM_CALC;
	DATA_TO_INV1[35:24] <= PhW1_PWM_CALC;

	DATA_TO_INV2[11:0] <= PhU2_PWM_CALC;
	DATA_TO_INV2[23:12] <= PhV2_PWM_CALC;
	DATA_TO_INV2[35:24] <= PhW2_PWM_CALC;

	DATA_TO_INV3[11:0] <= PhU3_PWM_CALC;
	DATA_TO_INV3[23:12] <= PhV3_PWM_CALC;
	DATA_TO_INV3[35:24] <= PhW3_PWM_CALC;
end


EXC_ChoppCurrent = DATA_FROM_EXC[107:96];
EXC_WindingCurrent1 = DATA_FROM_EXC[95:84];
EXC_WindingCurrent2 = DATA_FROM_EXC[83:72];
EXC_VoltageIn = DATA_FROM_EXC[119:108]; 
EXC_VoltageOut = DATA_FROM_EXC[131:120]; 
EXC_Temp1 = DATA_FROM_EXC[11:0];
EXC_Temp2 = DATA_FROM_EXC[23:12];
EXC_Temp3 = DATA_FROM_EXC[35:24];
EXC_Errors = DATA_FROM_EXC[47:36];


INV1_IGBTCurrent1 = DATA_FROM_INV1[11:0];
INV1_IGBTCurrent2	= DATA_FROM_INV1[23:12];
INV1_IGBTCurrent3 = DATA_FROM_INV1[35:24];		
INV1_Voltage = DATA_FROM_INV1[47:36];
if (DATA_FROM_INV1[59:58]==0) begin INV1_Temp1[9:0] = DATA_FROM_INV1[57:48]; end
else if (DATA_FROM_INV1[59:58]==1) begin INV1_Temp2[9:0] = DATA_FROM_INV1[57:48]; end
else if (DATA_FROM_INV1[59:58]==2) begin INV1_Temp3[9:0] = DATA_FROM_INV1[57:48]; end				
INV1_ERRORS = DATA_FROM_INV1[71:60];


INV2_IGBTCurrent1 = DATA_FROM_INV2[11:0];
INV2_IGBTCurrent2	= DATA_FROM_INV2[23:12];
INV2_IGBTCurrent3 = DATA_FROM_INV2[35:24];
INV2_Voltage = DATA_FROM_INV2[47:36];
if (DATA_FROM_INV2[59:58]==0) begin INV2_Temp1[9:0] = DATA_FROM_INV2[57:48]; end
else if (DATA_FROM_INV2[59:58]==1) begin INV2_Temp2[9:0] = DATA_FROM_INV2[57:48]; end
else if (DATA_FROM_INV2[59:58]==2) begin INV2_Temp3[9:0] = DATA_FROM_INV2[57:48]; end
INV2_ERRORS = DATA_FROM_INV2[71:60];


INV3_IGBTCurrent1 = DATA_FROM_INV3[11:0];
INV3_IGBTCurrent2	= DATA_FROM_INV3[23:12];
INV3_IGBTCurrent3 = DATA_FROM_INV3[35:24];
INV3_Voltage = DATA_FROM_INV3[47:36];


if (DATA_FROM_INV3[59:58]==0) begin INV3_Temp1[9:0] = DATA_FROM_INV3[57:48]; end
else if (DATA_FROM_INV3[59:58]==1) begin INV3_Temp2[9:0] = DATA_FROM_INV3[57:48]; end
else if (DATA_FROM_INV3[59:58]==2) begin INV3_Temp3[9:0] = DATA_FROM_INV3[57:48]; end
INV3_ERRORS = DATA_FROM_INV3[71:60];


DPR_DATA <= DPR_DATA_wire;
GEN_TEMP1 = DPR_TEMP_DATA[11:0]; //TPH3
GEN_TEMP2 = DPR_TEMP_DATA[23:12]; //TPH2
GEN_TEMP3 = DPR_TEMP_DATA[35:24]; //TPH1
GEN_TEMP4 = DPR_TEMP_DATA[47:36]; //EXC1
GEN_TEMP5 = DPR_TEMP_DATA[59:48]; //BACKB
GEN_TEMP6 = DPR_TEMP_DATA[71:60]; //EXC2


//EMIF DATA EXCHANGE
begin
	if (EMIF_INIT==0)
	begin
	  if (EMIF_FILL_COUNTER==100) begin	EMIF_ADRESS = 50;	EMIF_DATA_TO_RAM = Error_Reset_Command;	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==102) begin	EMIF_ADRESS = 51;	EMIF_DATA_TO_RAM = EMIF_Watchdog_CurrentValue;	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==104) begin	EMIF_ADRESS = 52;	EMIF_DATA_TO_RAM = PWM_AMP[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==106) begin	EMIF_ADRESS = 53;	EMIF_DATA_TO_RAM = PWM_AMP[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==108) begin	EMIF_ADRESS = 54;	EMIF_DATA_TO_RAM = PWM_OutpFreqCounts[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==110) begin	EMIF_ADRESS = 55;	EMIF_DATA_TO_RAM = PWM_OutpFreqCounts[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==112) begin	EMIF_ADRESS = 56;	EMIF_DATA_TO_RAM = PWM_PhaseShiftCounts[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==114) begin	EMIF_ADRESS = 57;	EMIF_DATA_TO_RAM = PWM_PhaseShiftCounts[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==116) begin	EMIF_ADRESS = 58;	EMIF_DATA_TO_RAM = PWM_CurrentPhaseSET[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==118) begin	EMIF_ADRESS = 59;	EMIF_DATA_TO_RAM = PWM_CurrentPhaseSET[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==120) begin	EMIF_ADRESS = 60;	EMIF_DATA_TO_RAM = Chopper_PWM_PW[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==122) begin	EMIF_ADRESS = 61;	EMIF_DATA_TO_RAM = Chopper_PWM_PW[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==124) begin	EMIF_ADRESS = 62;	EMIF_DATA_TO_RAM = Excitation_PWM_PW[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==126) begin	EMIF_ADRESS = 63;	EMIF_DATA_TO_RAM = Excitation_PWM_PW[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==128) begin	EMIF_ADRESS = 64;	EMIF_DATA_TO_RAM = MainFan_PWM_PW[31:16];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==130) begin	EMIF_ADRESS = 65;	EMIF_DATA_TO_RAM = MainFan_PWM_PW[15:0];	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==132) begin	EMIF_ADRESS = 66;	EMIF_DATA_TO_RAM = 0;	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==134) begin	EMIF_ADRESS = 67;	EMIF_DATA_TO_RAM = 0;	EMIF_WREN = 1;	end
	  else if (EMIF_FILL_COUNTER==200) begin	EMIF_ADRESS = 100;	EMIF_DATA_TO_RAM = 0;	EMIF_WREN = 1;	end
	  else   begin	EMIF_WREN = 0;	end
	  EMIF_FILL_COUNTER = EMIF_FILL_COUNTER+1;
	  if (EMIF_FILL_COUNTER>203)	  begin	  EMIF_FILL_COUNTER=0;	  EMIF_WREN = 0;	  EMIF_INIT = 1;	  end
	end
	else
	 begin     
	 if (EMIF_FILL_COUNTER==0)			begin	EMIF_ADRESS = 0;	EMIF_DATA_TO_RAM = Error_Latch[31:16];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==2)	begin	EMIF_ADRESS = 1;	EMIF_DATA_TO_RAM = Error_Latch[15:0];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==4)	begin	EMIF_ADRESS = 2;	EMIF_DATA_TO_RAM = Error_Flags[31:16];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==6)	begin	EMIF_ADRESS = 3;	EMIF_DATA_TO_RAM = Error_Flags[15:0];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==8)	begin EMIF_ADRESS = 4;	EMIF_DATA_TO_RAM = EMIF_Watchdog_PrevValue;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==10)	begin	EMIF_ADRESS = 5;	EMIF_DATA_TO_RAM = PhU1_PWM;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==12)	begin	EMIF_ADRESS = 6;	EMIF_DATA_TO_RAM = GEN_TEMP5;	EMIF_WREN = 1;	end	
	 else if (EMIF_FILL_COUNTER==14)	begin	EMIF_ADRESS = 7;	EMIF_DATA_TO_RAM = EXC_VoltageIn;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==16)	begin	EMIF_ADRESS = 8;	EMIF_DATA_TO_RAM = EXC_VoltageOut;	EMIF_WREN = 1;	end		
	 else if (EMIF_FILL_COUNTER==18)	begin	EMIF_ADRESS = 9;	EMIF_DATA_TO_RAM = EXC_Temp1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==20)	begin	EMIF_ADRESS = 10;	EMIF_DATA_TO_RAM = EXC_Temp2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==22)	begin	EMIF_ADRESS = 11;	EMIF_DATA_TO_RAM = EXC_Temp3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==23)	begin	EMIF_WREN = 0;	end

	 //DATA READ ---------------------------
	 else if (EMIF_FILL_COUNTER==51)	begin EMIF_ADRESS = 100; end
	 else if (EMIF_FILL_COUNTER==55)	begin DATA_COMMIT_COMMAND = EMIF_DATA_FROM_RAM; end
	 
	 else if (EMIF_FILL_COUNTER==56)	begin EMIF_ADRESS = 50; end
	 else if (EMIF_FILL_COUNTER==60)	begin Error_Reset_Command_Inp = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==61)	begin EMIF_ADRESS = 51; end
	 else if (EMIF_FILL_COUNTER==65)	begin EMIF_Watchdog_CurrentValue_Inp = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==66)	begin EMIF_ADRESS = 52; end
	 else if (EMIF_FILL_COUNTER==70)	begin PWM_AMP_INP[31:16] = EMIF_DATA_FROM_RAM; end
	 else if (EMIF_FILL_COUNTER==71)	begin EMIF_ADRESS = 53; end
	 else if (EMIF_FILL_COUNTER==75)	begin PWM_AMP_INP[15:0] = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==86)	begin EMIF_ADRESS = 56; end
	 else if (EMIF_FILL_COUNTER==90)	begin PWM_PhaseShiftCounts_INP[31:16] = EMIF_DATA_FROM_RAM; end
	 else if (EMIF_FILL_COUNTER==91)	begin EMIF_ADRESS = 57; end
	 else if (EMIF_FILL_COUNTER==95)	begin PWM_PhaseShiftCounts_INP[15:0] = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==131)	begin EMIF_ADRESS = 60; end
	 else if (EMIF_FILL_COUNTER==135)	begin Chopper_PWM_PW_INP[31:16] = EMIF_DATA_FROM_RAM; end
	 else if (EMIF_FILL_COUNTER==136)	begin EMIF_ADRESS = 61; end
	 else if (EMIF_FILL_COUNTER==140)	begin Chopper_PWM_PW_INP[15:0] = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==141)	begin EMIF_ADRESS = 62; end
	 else if (EMIF_FILL_COUNTER==145)	begin Excitation_PWM_PW_INP[31:16] = EMIF_DATA_FROM_RAM; end
	 else if (EMIF_FILL_COUNTER==146)	begin EMIF_ADRESS = 63; end
	 else if (EMIF_FILL_COUNTER==150)	begin Excitation_PWM_PW_INP[15:0] = EMIF_DATA_FROM_RAM; end

	 else if (EMIF_FILL_COUNTER==151)	begin EMIF_ADRESS = 64; end
	 else if (EMIF_FILL_COUNTER==155)	begin MainFan_PWM_PW_INP[31:16] = EMIF_DATA_FROM_RAM; end
	 else if (EMIF_FILL_COUNTER==156)	begin EMIF_ADRESS = 65; end
	 else if (EMIF_FILL_COUNTER==160)	begin MainFan_PWM_PW_INP[15:0] = EMIF_DATA_FROM_RAM; end
	 
	 //Chopper input values ------------------------
	 else if (EMIF_FILL_COUNTER==227)	begin	EMIF_ADRESS = 12;	EMIF_DATA_TO_RAM = PWM_CurrentPhase[31:16];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==229)	begin	EMIF_ADRESS = 13;	EMIF_DATA_TO_RAM = PWM_CurrentPhase[15:0];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==231)	begin	EMIF_ADRESS = 14;	EMIF_DATA_TO_RAM = velocity[31:16];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==233)	begin	EMIF_ADRESS = 15;	EMIF_DATA_TO_RAM = velocity[15:0];	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==235)	begin	EMIF_ADRESS = 16;	EMIF_DATA_TO_RAM = GEN_TEMP1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==237)	begin	EMIF_ADRESS = 17;	EMIF_DATA_TO_RAM = GEN_TEMP2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==239)	begin	EMIF_ADRESS = 18;	EMIF_DATA_TO_RAM = EXC_ChoppCurrent;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==241)	begin	EMIF_ADRESS = 19;	EMIF_DATA_TO_RAM = EXC_WindingCurrent1; 	EMIF_WREN = 1;	end 
	 else if (EMIF_FILL_COUNTER==243)	begin	EMIF_ADRESS = 20;	EMIF_DATA_TO_RAM = EXC_WindingCurrent2; 	EMIF_WREN = 1;	end  
	 else if (EMIF_FILL_COUNTER==245)	begin	EMIF_ADRESS = 21;	EMIF_DATA_TO_RAM = EXC_Temp1; 	EMIF_WREN = 1;	end 
	 else if (EMIF_FILL_COUNTER==247)	begin	EMIF_ADRESS = 22;	EMIF_DATA_TO_RAM = EXC_Temp2; 	EMIF_WREN = 1;	end  
	 else if (EMIF_FILL_COUNTER==249)	begin	EMIF_ADRESS = 23;	EMIF_DATA_TO_RAM = EXC_Temp3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==251)	begin	EMIF_ADRESS = 24;	EMIF_DATA_TO_RAM = INV1_IGBTCurrent1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==253)	begin	EMIF_ADRESS = 25;	EMIF_DATA_TO_RAM = INV1_IGBTCurrent2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==255)	begin	EMIF_ADRESS = 26;	EMIF_DATA_TO_RAM = INV1_IGBTCurrent3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==257)	begin	EMIF_ADRESS = 27;	EMIF_DATA_TO_RAM = INV1_Voltage;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==259)	begin	EMIF_ADRESS = 28;	EMIF_DATA_TO_RAM = INV1_Temp1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==261)	begin	EMIF_ADRESS = 29;	EMIF_DATA_TO_RAM = INV1_Temp2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==263)	begin	EMIF_ADRESS = 30;	EMIF_DATA_TO_RAM = INV1_Temp3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==265)	begin	EMIF_ADRESS = 31;	EMIF_DATA_TO_RAM = INV1_ERRORS;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==267)	begin	EMIF_ADRESS = 32;	EMIF_DATA_TO_RAM = INV2_IGBTCurrent1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==269)	begin	EMIF_ADRESS = 33;	EMIF_DATA_TO_RAM = INV2_IGBTCurrent2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==271)	begin	EMIF_ADRESS = 34;	EMIF_DATA_TO_RAM = INV2_IGBTCurrent3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==273)	begin	EMIF_ADRESS = 35;	EMIF_DATA_TO_RAM = INV2_Voltage;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==275)	begin	EMIF_ADRESS = 36;	EMIF_DATA_TO_RAM = INV2_Temp1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==277)	begin	EMIF_ADRESS = 37;	EMIF_DATA_TO_RAM = INV2_Temp2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==279)	begin	EMIF_ADRESS = 38;	EMIF_DATA_TO_RAM = INV2_Temp3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==281)	begin	EMIF_ADRESS = 39;	EMIF_DATA_TO_RAM = INV2_ERRORS;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==283)	begin	EMIF_ADRESS = 40;	EMIF_DATA_TO_RAM = INV3_IGBTCurrent1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==285)	begin	EMIF_ADRESS = 41;	EMIF_DATA_TO_RAM = INV3_IGBTCurrent2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==287)	begin	EMIF_ADRESS = 42;	EMIF_DATA_TO_RAM = INV3_IGBTCurrent3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==289)	begin	EMIF_ADRESS = 43;	EMIF_DATA_TO_RAM = INV3_Voltage;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==291)	begin	EMIF_ADRESS = 44;	EMIF_DATA_TO_RAM = INV3_Temp1;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==293)	begin	EMIF_ADRESS = 45;	EMIF_DATA_TO_RAM = INV3_Temp2;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==295)	begin	EMIF_ADRESS = 46;	EMIF_DATA_TO_RAM = INV3_Temp3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==297)	begin	EMIF_ADRESS = 47;	EMIF_DATA_TO_RAM = GEN_TEMP3;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==299)	begin	EMIF_ADRESS = 48;	EMIF_DATA_TO_RAM = GEN_TEMP4;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==301)	begin	EMIF_ADRESS = 49;	EMIF_DATA_TO_RAM = GEN_TEMP6;	EMIF_WREN = 1;	end
	 else if (EMIF_FILL_COUNTER==303)	begin	EMIF_WREN = 0;	end
	 
	 
	 else if (EMIF_FILL_COUNTER==332)
		begin
			if (PWM_AMP_INP > PWM_AMP_MAX) begin PWM_AMP_INP = PWM_AMP_MAX; end else begin PWM_AMP_INP = PWM_AMP_INP; end
			Chopper_PWM_PW_INP = Chopper_PWM_PW_INP;
			Excitation_PWM_PW_INP = Excitation_PWM_PW_INP;
			MainFan_PWM_PW_INP = MainFan_PWM_PW_INP;
		end
	 else if (EMIF_FILL_COUNTER==334)
		begin
		if (DATA_COMMIT_COMMAND == DATA_COMMIT_VALUE)
				begin
				Error_Reset_Command = Error_Reset_Command_Inp;
				EMIF_Watchdog_CurrentValue = EMIF_Watchdog_CurrentValue_Inp;
				PWM_AMP = PWM_AMP_INP;
				PWM_PhaseShiftCounts = PWM_PhaseShiftCounts_INP; 
				Chopper_PWM_PW = Chopper_PWM_PW_INP;
				Excitation_PWM_PW = Excitation_PWM_PW_INP;
				MainFan_PWM_PW = MainFan_PWM_PW_INP;
				EMIF_ADRESS = 100;
				EMIF_DATA_TO_RAM = 0;
				EMIF_WREN = 1;
				end
		else
				begin
				Error_Reset_Command = Error_Reset_Command;
				EMIF_Watchdog_CurrentValue = EMIF_Watchdog_CurrentValue;
				PWM_AMP = PWM_AMP;
				PWM_PhaseShiftCounts = PWM_PhaseShiftCounts;
				Chopper_PWM_PW = Chopper_PWM_PW;
				Excitation_PWM_PW = Excitation_PWM_PW; 
				MainFan_PWM_PW = MainFan_PWM_PW;
				EMIF_ADRESS = 100;
				EMIF_DATA_TO_RAM = 0;
				EMIF_WREN = 0;
				end
		end
	 else if (EMIF_FILL_COUNTER==338)
		begin
		if (Error_Reset_Command == Error_Reset_Value)
			begin
			Error_Reset_Counter = Error_Reset_Length;
			Error_Reset_Command = 0;
			EMIF_ADRESS = 50;
			EMIF_DATA_TO_RAM = 0;
			EMIF_WREN = 1;
			end
		end
	 else
		begin
		EMIF_WREN = 0;
		EMIF_ADRESS = EMIF_ADRESS;
		end
		
	  EMIF_FILL_COUNTER = EMIF_FILL_COUNTER+1;
	  if (EMIF_FILL_COUNTER>340)	  begin	  EMIF_FILL_COUNTER=0;	  EMIF_WREN = 0;	  end
	 end 
	 
end
//EMIF DATA EXCHANGE


//Sine PWM Calculation and PWM Controls
`ifndef NO_CALC_DEBUG
begin
FP_CONVBACK_OUT = FP_CONVBACK_OUTwire;
FP_ConvIntToDoubleOUT = FP_ConvIntToDoubleOUTwire;
FP_DIV_OUT = FP_DIV_OUTwire;
FP_MULT_OUT = FP_MULT_OUTwire;
FP_ADDSUB_OUT = FP_ADDSUB_OUTwire;
FP_CONVDTF_OUT = FP_CONVDTF_OUTwire;
FP_SIN_OUT = FP_SIN_OUTwire;
FP_CONVFTD_OUT = FP_CONVFTD_OUTwire;

//--------------RECALC 40 deg 477,218,588 1C71C71C 80 deg 954,437,177 38E38E39

if (PWM_Recalc == 1)
begin
if (PH_GROUP_SM==0) begin PWM_CurrentPhase_CALC <= PWM_CurrentPhase; end
else if (PH_GROUP_SM==1) begin PWM_CurrentPhase_CALC <= PWM_CurrentPhase - 32'h1C71C71C; end
else if (PH_GROUP_SM==2) begin PWM_CurrentPhase_CALC <= PWM_CurrentPhase - 32'h38E38E39; end
else begin PH_GROUP_SM<=0; end

PWM_RecalcCounter = PWM_RecalcCounter + 1;
//--------------------- CONVERSION INT TO DOUBLE STATE MACHINE -------------------------
if (PWM_RecalcCounter<10) //Init
	begin
	FP_ConvIntToDoubleIN=0;
	end
else if (PWM_RecalcCounter<20) //Write
	begin
	FP_ConvIntToDoubleIN = PWM_AMP;
	end
else if (PWM_RecalcCounter<21) //Read
	begin
	FP_PWM_AmpSet = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<30)//Write
	begin
	FP_ConvIntToDoubleIN = PWM_AMP_MAX;
	end
else if (PWM_RecalcCounter<31)//Read
	begin
	FP_PWM_AmpMax = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<40)//Write
	begin
	FP_ConvIntToDoubleIN = PWM_Freq_Counts;
	end
else if (PWM_RecalcCounter<41)//Read
	begin
	FP_PWM_MaxCounts = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<50)//Write
	begin
	FP_ConvIntToDoubleIN = PWM_OutpFreqCounts; 
	end
else if (PWM_RecalcCounter<51)//Read
	begin
	FP_PWM_OutputFrequencyCounts = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<60)//Write
	begin
	FP_ConvIntToDoubleIN = PWM_PhaseShiftCounts; 
	end
else if (PWM_RecalcCounter<61)//Read
	begin
	FP_PWM_PhaseShiftCounts = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<70)//Write
	begin
	FP_ConvIntToDoubleIN = PWM_CurrentPhase_CALC; 
	end
else if (PWM_RecalcCounter<71)//Read
	begin
	FP_PWM_PhaseCounts = FP_ConvIntToDoubleOUT;
	end
else if (PWM_RecalcCounter<72)
	begin
	FP_ConvIntToDoubleIN = 0;
	end
//--------------------- ADDSUB STATE MACHINE -------------------------
else if (PWM_RecalcCounter < 75) //Init
	begin
	FP_ADDSUB_CONTROL =1;
	FP_ADDSUB_INA = 0;
	FP_ADDSUB_INB = 0;
	end
//FP_CALC_PHASEA_COUNTS = FP_PWM_PhaseCounts + FP_PWM_PhaseShiftCounts 	
else if (PWM_RecalcCounter < 95) //Write
	begin
	FP_ADDSUB_CONTROL =1;
	FP_ADDSUB_INA = FP_PWM_OutputFrequencyCounts;
	FP_ADDSUB_INB = FP_PWM_OutputFrequencyCounts; 
	end
else if (PWM_RecalcCounter < 100) //Read 
	begin
	if (FP_ADDSUB_isnan==1)	FP_PWM_OutputFrequencyCounts = 0;
	else if (FP_ADDSUB_overflow==1)	FP_PWM_OutputFrequencyCounts = 0;
	else if (FP_ADDSUB_underflow==1) FP_PWM_OutputFrequencyCounts = 0;
	else if (FP_ADDSUB_zero==1) FP_PWM_OutputFrequencyCounts = 0;
	else	FP_PWM_OutputFrequencyCounts = FP_ADDSUB_OUT;
	FP_CALC_PHASEA_COUNTS = FP_PWM_PhaseCounts;
	end
//--------------------- DIV STATE MACHINE -------------------------
else if (PWM_RecalcCounter < 120) //Init
	begin
	FP_DIV_INA = FP_PWM_Num_2;
	FP_DIV_INB = FP_PWM_Num_2;
	end	
//FP_CALC_PHASEA_SECTOR = FP_CALC_PHASEA_COUNTS/FP_PWM_OutputFrequencyCounts
else if (PWM_RecalcCounter < 160) //Write
	begin
	FP_DIV_INA = FP_CALC_PHASEA_COUNTS;
	FP_DIV_INB = FP_PWM_OutputFrequencyCounts; 
	end
else if (PWM_RecalcCounter < 165) //Read
	begin
	if (FP_DIV_divbyzero==1)	FP_CALC_PHASEA_SECTOR = 0;
	else if (FP_DIV_isnan==1)	FP_CALC_PHASEA_SECTOR = 0;
	else if (FP_DIV_overflow==1) FP_CALC_PHASEA_SECTOR = 0;
	else if (FP_DIV_underflow==1) FP_CALC_PHASEA_SECTOR = 0;
	else if (FP_DIV_zero==1) FP_CALC_PHASEA_SECTOR = 0;
	else	FP_CALC_PHASEA_SECTOR = FP_DIV_OUT;
	end 
else if (PWM_RecalcCounter < 166) //Read
	begin
	FP_DIV_INA = FP_PWM_Num_2;
	FP_DIV_INB = FP_PWM_Num_2;
	end 
//FP_CALC_PHASEB_SECTOR = FP_CALC_PHASEA_SECTOR - FP_PWM_Num_1div3
else if (PWM_RecalcCounter < 190) //Write
	begin
	FP_ADDSUB_CONTROL =0;
	FP_ADDSUB_INA = FP_CALC_PHASEA_SECTOR;
	FP_ADDSUB_INB = FP_PWM_Num_1div3;
	end
else if (PWM_RecalcCounter < 195) //Read
	begin
	if (FP_ADDSUB_isnan==1)	FP_CALC_PHASEB_SECTOR = 0;
	else if (FP_ADDSUB_overflow==1)	FP_CALC_PHASEB_SECTOR = 0;
	else if (FP_ADDSUB_underflow==1) FP_CALC_PHASEB_SECTOR = 0;
	else if (FP_ADDSUB_zero==1) FP_CALC_PHASEB_SECTOR = 0;
	else	FP_CALC_PHASEB_SECTOR = FP_ADDSUB_OUT;
	end 
//FP_CALC_PHASEC_SECTOR = FP_CALC_PHASEA_SECTOR + FP_PWM_Num_1div3
else if (PWM_RecalcCounter < 220) //Write
	begin
	FP_ADDSUB_CONTROL =1;
	FP_ADDSUB_INA = FP_CALC_PHASEA_SECTOR;
	FP_ADDSUB_INB = FP_PWM_Num_1div3;
	end
else if (PWM_RecalcCounter < 225) //Read
	begin
	if (FP_ADDSUB_isnan==1)	FP_CALC_PHASEC_SECTOR = 0;
	else if (FP_ADDSUB_overflow==1)	FP_CALC_PHASEC_SECTOR = 0;
	else if (FP_ADDSUB_underflow==1) FP_CALC_PHASEC_SECTOR = 0;
	else if (FP_ADDSUB_zero==1) FP_CALC_PHASEC_SECTOR = 0;
	else	FP_CALC_PHASEC_SECTOR = FP_ADDSUB_OUT;
	end 	
//--------------------- MULT STATE MACHINE -------------------------
else if (PWM_RecalcCounter<245) //Init
	begin
	FP_MULT_INA = 0; 
	FP_MULT_INB = 0;
	end
// FP_CALC_PHASEA_PHASE = FP_CALC_PHASEA_SECTOR*FP_PWM_Num_2PI
else if (PWM_RecalcCounter<265) //Write
	begin
	FP_MULT_INA = FP_CALC_PHASEA_SECTOR;
	FP_MULT_INB = FP_PWM_Num_2PI;
	end
else if (PWM_RecalcCounter<270) //Read
	begin
	if (FP_MULT_isnan==1)	FP_CALC_PHASEA_PHASE = 0;
	else if (FP_MULT_overflow==1)	FP_CALC_PHASEA_PHASE = 0;
	else if (FP_MULT_underflow==1) FP_CALC_PHASEA_PHASE = 0;
	else if (FP_MULT_zero==1) FP_CALC_PHASEA_PHASE = 0;
	else	FP_CALC_PHASEA_PHASE = FP_MULT_OUT;
	end
// FP_CALC_PHASEB_PHASE = FP_CALC_PHASEB_SECTOR*FP_PWM_Num_2PI
else if (PWM_RecalcCounter<290) //Write
	begin
	FP_MULT_INA = FP_CALC_PHASEB_SECTOR;
	FP_MULT_INB = FP_PWM_Num_2PI;
	end
else if (PWM_RecalcCounter<295) //Read
	begin
	if (FP_MULT_isnan==1)	FP_CALC_PHASEB_PHASE = 0;
	else if (FP_MULT_overflow==1)	FP_CALC_PHASEB_PHASE = 0;
	else if (FP_MULT_underflow==1) FP_CALC_PHASEB_PHASE = 0;
	else if (FP_MULT_zero==1) FP_CALC_PHASEB_PHASE = 0;
	else	FP_CALC_PHASEB_PHASE = FP_MULT_OUT;
	end
// FP_CALC_PHASEC_PHASE = FP_CALC_PHASEC_SECTOR*FP_PWM_Num_2PI
else if (PWM_RecalcCounter<315) //Write
	begin
	FP_MULT_INA = FP_CALC_PHASEC_SECTOR;
	FP_MULT_INB = FP_PWM_Num_2PI;
	end
else if (PWM_RecalcCounter<320) //Read
	begin
	if (FP_MULT_isnan==1)	FP_CALC_PHASEC_PHASE = 0;
	else if (FP_MULT_overflow==1)	FP_CALC_PHASEC_PHASE = 0;
	else if (FP_MULT_underflow==1) FP_CALC_PHASEC_PHASE = 0;
	else if (FP_MULT_zero==1) FP_CALC_PHASEC_PHASE = 0;
	else	FP_CALC_PHASEC_PHASE = FP_MULT_OUT;
	end	
//--------------------- DoubleToFloat CONVERSION STATE MACHINE -------------------------
else if (PWM_RecalcCounter<330) //Init
	begin 
	FP_CONVDTF_IN = 0;
	end
// FP_CALC_PHASEA_PHASE_F = CONVERTDTF(FP_CALC_PHASEA_PHASE)
else if (PWM_RecalcCounter<340)//Read
	begin
	FP_CONVDTF_IN = FP_CALC_PHASEA_PHASE;
	end
else if (PWM_RecalcCounter<345)//Write
	begin
	if (FP_CONVDTF_isnan==1)	FP_CALC_PHASEA_PHASE_F = 0;
	else if (FP_CONVDTF_overflow==1)	FP_CALC_PHASEA_PHASE_F = 0;
	else if (FP_CONVDTF_underflow==1) FP_CALC_PHASEA_PHASE_F = 0;
	else	FP_CALC_PHASEA_PHASE_F = FP_CONVDTF_OUT;
	end
// FP_CALC_PHASEB_PHASE_F = CONVERTDTF(FP_CALC_PHASEB_PHASE)
else if (PWM_RecalcCounter<355)//Read
	begin
	FP_CONVDTF_IN = FP_CALC_PHASEB_PHASE;
	end
else if (PWM_RecalcCounter<360)//Write
	begin
	if (FP_CONVDTF_isnan==1)	FP_CALC_PHASEB_PHASE_F = 0;
	else if (FP_CONVDTF_overflow==1)	FP_CALC_PHASEB_PHASE_F = 0;
	else if (FP_CONVDTF_underflow==1) FP_CALC_PHASEB_PHASE_F = 0;
	else	FP_CALC_PHASEB_PHASE_F = FP_CONVDTF_OUT;
	end
// FP_CALC_PHASEC_PHASE_F = CONVERTDTF(FP_CALC_PHASEC_PHASE)	
else if (PWM_RecalcCounter<370)//Read
	begin
	FP_CONVDTF_IN = FP_CALC_PHASEC_PHASE;
	end
else if (PWM_RecalcCounter<375)//Write
	begin
	if (FP_CONVDTF_isnan==1)	FP_CALC_PHASEC_PHASE_F = 0;
	else if (FP_CONVDTF_overflow==1)	FP_CALC_PHASEC_PHASE_F = 0;
	else if (FP_CONVDTF_underflow==1) FP_CALC_PHASEC_PHASE_F = 0;
	else	FP_CALC_PHASEC_PHASE_F = FP_CONVDTF_OUT;
	end
//--------------------- SIN STATE MACHINE -------------------------
else if (PWM_RecalcCounter<425) //Init
	begin
	FP_SIN_IN = 0;
	end
// FP_CALC_PHASEA_SIN_F = sin(FP_CALC_PHASEA_PHASE_F)
else if (PWM_RecalcCounter<475) //Write
	begin
	FP_SIN_IN = FP_CALC_PHASEA_PHASE_F;
	end
else if (PWM_RecalcCounter<480) //read
	begin
	FP_CALC_PHASEA_SIN_F = FP_SIN_OUT;
	end
// FP_CALC_PHASEB_SIN_F = sin(FP_CALC_PHASEB_PHASE_F)
else if (PWM_RecalcCounter<530) //Write
	begin
	FP_SIN_IN = FP_CALC_PHASEB_PHASE_F;
	end
else if (PWM_RecalcCounter<535) //read
	begin
	FP_CALC_PHASEB_SIN_F = FP_SIN_OUT;
	end
// FP_CALC_PHASEC_SIN_F = sin(FP_CALC_PHASEC_PHASE_F)	
else if (PWM_RecalcCounter<585) //Write
	begin
	FP_SIN_IN = FP_CALC_PHASEC_PHASE_F;
	end
else if (PWM_RecalcCounter<590) //read
	begin
	FP_CALC_PHASEC_SIN_F = FP_SIN_OUT;
	end
//--------------------- FloatToDouble CONVERSION STATE MACHINE -------------------------
else if (PWM_RecalcCounter<600) //Init
	begin
	FP_CONVFTD_IN = 0;
	end
// FP_CALC_PHASEA_SIN = CONVERTFTD(FP_CALC_PHASEA_SIN_F)
else if (PWM_RecalcCounter<610) //Write
	begin
	FP_CONVFTD_IN = FP_CALC_PHASEA_SIN_F; 
	end
else if (PWM_RecalcCounter<615) //Read
	begin
	if (FP_CONVFTD_isnan==1)	FP_CALC_PHASEA_SIN = 0;
	else if (FP_CONVFTD_overflow==1)	FP_CALC_PHASEA_SIN = 0;
	else if (FP_CONVFTD_underflow==1) FP_CALC_PHASEA_SIN = 0;
	else	FP_CALC_PHASEA_SIN = FP_CONVFTD_OUT;
	end
// FP_CALC_PHASEB_SIN = CONVERTFTD(FP_CALC_PHASEB_SIN_F)
else if (PWM_RecalcCounter<625) //Write
	begin
	FP_CONVFTD_IN = FP_CALC_PHASEB_SIN_F;
	end
else if (PWM_RecalcCounter<630) //Read
	begin
	if (FP_CONVFTD_isnan==1)	FP_CALC_PHASEB_SIN = 0;
	else if (FP_CONVFTD_overflow==1)	FP_CALC_PHASEB_SIN = 0;
	else if (FP_CONVFTD_underflow==1) FP_CALC_PHASEB_SIN = 0;
	else	FP_CALC_PHASEB_SIN = FP_CONVFTD_OUT;
	end
// FP_CALC_PHASEC_SIN = CONVERTFTD(FP_CALC_PHASEC_SIN_F)
else if (PWM_RecalcCounter<640) //Write
	begin
	FP_CONVFTD_IN = FP_CALC_PHASEC_SIN_F;
	end
else if (PWM_RecalcCounter<645) //Read
	begin
	if (FP_CONVFTD_isnan==1)	FP_CALC_PHASEC_SIN = 0;
	else if (FP_CONVFTD_overflow==1)	FP_CALC_PHASEC_SIN = 0;
	else if (FP_CONVFTD_underflow==1) FP_CALC_PHASEC_SIN = 0;
	else	FP_CALC_PHASEC_SIN = FP_CONVFTD_OUT;
	end
//--------------------- DIV STATE MACHINE -------------------------
// FP_CALC_HALFMAXCOUNTS = FP_PWM_MaxCounts / FP_PWM_Num_2	
else if (PWM_RecalcCounter < 685) //Write
	begin
	FP_DIV_INA = FP_PWM_MaxCounts;
	FP_DIV_INB = FP_PWM_Num_2;
	end
else if (PWM_RecalcCounter < 690) //Read
	begin
	if (FP_DIV_divbyzero==1)	FP_CALC_HALFMAXCOUNTS = 0;
	else if (FP_DIV_isnan==1)	FP_CALC_HALFMAXCOUNTS = 0;
	else if (FP_DIV_overflow==1) FP_CALC_HALFMAXCOUNTS = 0;
	else if (FP_DIV_underflow==1) FP_CALC_HALFMAXCOUNTS = 0;
	else if (FP_DIV_zero==1) FP_CALC_HALFMAXCOUNTS = 0;
	else	FP_CALC_HALFMAXCOUNTS = FP_DIV_OUT;
	end 
// FP_CALC_RELATIVEAMP = FP_PWM_AmpSet / FP_PWM_AmpMax
else if (PWM_RecalcCounter < 730) //Write
	begin
	FP_DIV_INA = FP_PWM_AmpSet;
	FP_DIV_INB = FP_PWM_AmpMax;
	end
else if (PWM_RecalcCounter < 735) //Read
	begin
	if (FP_DIV_divbyzero==1)	FP_CALC_RELATIVEAMP = 0;
	else if (FP_DIV_isnan==1)	FP_CALC_RELATIVEAMP = 0;
	else if (FP_DIV_overflow==1) FP_CALC_RELATIVEAMP = 0;
	else if (FP_DIV_underflow==1) FP_CALC_RELATIVEAMP = 0;
	else if (FP_DIV_zero==1) FP_CALC_RELATIVEAMP = 0;
	else	FP_CALC_RELATIVEAMP = FP_DIV_OUT;
	end
else if (PWM_RecalcCounter < 736) //Read
	begin
	FP_DIV_INA = FP_PWM_Num_2;
	FP_DIV_INB = FP_PWM_Num_2;
	end 		
//--------------------- MULT STATE MACHINE -------------------------
// FP_CALC_ABSOLUTEAMP = FP_CALC_RELATIVEAMP * FP_CALC_HALFMAXCOUNTS
else if (PWM_RecalcCounter<755) //Write
	begin
	FP_MULT_INA = FP_CALC_RELATIVEAMP;
	FP_MULT_INB = FP_CALC_HALFMAXCOUNTS;
	end
else if (PWM_RecalcCounter<760) //Read
	begin
	if (FP_MULT_isnan==1)	FP_CALC_ABSOLUTEAMP = 0;
	else if (FP_MULT_overflow==1)	FP_CALC_ABSOLUTEAMP = 0;
	else if (FP_MULT_underflow==1) FP_CALC_ABSOLUTEAMP = 0;
	else if (FP_MULT_zero==1) FP_CALC_ABSOLUTEAMP = 0;
	else	FP_CALC_ABSOLUTEAMP = FP_MULT_OUT;
	end
// FP_PWM_PHAPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEA_SIN 
else if (PWM_RecalcCounter<780) //Write
	begin
	FP_MULT_INA = FP_CALC_ABSOLUTEAMP;
	FP_MULT_INB = FP_CALC_PHASEA_SIN;
	end
else if (PWM_RecalcCounter<785) //Read
	begin
	if (FP_MULT_isnan==1)	FP_PWM_PHAPWM_SCALED = 0;
	else if (FP_MULT_overflow==1)	FP_PWM_PHAPWM_SCALED = 0;
	else if (FP_MULT_underflow==1) FP_PWM_PHAPWM_SCALED = 0;
	else if (FP_MULT_zero==1) FP_PWM_PHAPWM_SCALED = 0;
	else	FP_PWM_PHAPWM_SCALED = FP_MULT_OUT;
	end
// FP_PWM_PHBPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEB_SIN
else if (PWM_RecalcCounter<805) //Write
	begin
	FP_MULT_INA = FP_CALC_ABSOLUTEAMP;
	FP_MULT_INB = FP_CALC_PHASEB_SIN;
	end
else if (PWM_RecalcCounter<810) //Read
	begin
	if (FP_MULT_isnan==1)	FP_PWM_PHBPWM_SCALED = 0;
	else if (FP_MULT_overflow==1)	FP_PWM_PHBPWM_SCALED = 0;
	else if (FP_MULT_underflow==1) FP_PWM_PHBPWM_SCALED = 0;
	else if (FP_MULT_zero==1) FP_PWM_PHBPWM_SCALED = 0;
	else	FP_PWM_PHBPWM_SCALED = FP_MULT_OUT;
	end
// FP_PWM_PHCPWM_SCALED = FP_CALC_ABSOLUTEAMP * FP_CALC_PHASEC_SIN
else if (PWM_RecalcCounter<830) //Write
	begin
	FP_MULT_INA = FP_CALC_ABSOLUTEAMP;
	FP_MULT_INB = FP_CALC_PHASEC_SIN;
	end
else if (PWM_RecalcCounter<835) //Read
	begin
	if (FP_MULT_isnan==1)	FP_PWM_PHCPWM_SCALED = 0;
	else if (FP_MULT_overflow==1)	FP_PWM_PHCPWM_SCALED = 0;
	else if (FP_MULT_underflow==1) FP_PWM_PHCPWM_SCALED = 0;
	else if (FP_MULT_zero==1) FP_PWM_PHCPWM_SCALED = 0;
	else	FP_PWM_PHCPWM_SCALED = FP_MULT_OUT;
	end	
//--------------------- ADDSUB STATE MACHINE -------------------------
// FP_CALC_PHASEAPWM_SHIFTED = FP_PWM_PHAPWM_SCALED + FP_CALC_HALFMAXCOUNTS
else if (PWM_RecalcCounter < 855) //Write
	begin
	FP_ADDSUB_CONTROL =0;
	FP_ADDSUB_INA = FP_CALC_HALFMAXCOUNTS; 
	FP_ADDSUB_INB = FP_PWM_PHAPWM_SCALED;
	end
else if (PWM_RecalcCounter < 860) //Read
	begin
	if (FP_ADDSUB_isnan==1)	FP_CALC_PHASEAPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_overflow==1)	FP_CALC_PHASEAPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_underflow==1) FP_CALC_PHASEAPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_zero==1) FP_CALC_PHASEAPWM_SHIFTED = 0;
	else	FP_CALC_PHASEAPWM_SHIFTED = FP_ADDSUB_OUT;
	end
// FP_CALC_PHASEBPWM_SHIFTED = FP_PWM_PHBPWM_SCALED + FP_CALC_HALFMAXCOUNTS
else if (PWM_RecalcCounter < 880) //Write
	begin
	FP_ADDSUB_CONTROL =0;
	FP_ADDSUB_INA = FP_CALC_HALFMAXCOUNTS;
	FP_ADDSUB_INB = FP_PWM_PHBPWM_SCALED;
	end
else if (PWM_RecalcCounter < 885) //Read
	begin
	if (FP_ADDSUB_isnan==1)	FP_CALC_PHASEBPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_overflow==1)	FP_CALC_PHASEBPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_underflow==1) FP_CALC_PHASEBPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_zero==1) FP_CALC_PHASEBPWM_SHIFTED = 0;
	else	FP_CALC_PHASEBPWM_SHIFTED = FP_ADDSUB_OUT;
	end
// FP_CALC_PHASECPWM_SHIFTED = FP_PWM_PHCPWM_SCALED + FP_CALC_HALFMAXCOUNTS		
else if (PWM_RecalcCounter < 905) //Write
	begin
	FP_ADDSUB_CONTROL =0;
	FP_ADDSUB_INA = FP_CALC_HALFMAXCOUNTS;
	FP_ADDSUB_INB = FP_PWM_PHCPWM_SCALED;
	end
else if (PWM_RecalcCounter < 910) //Read
	begin
	if (FP_ADDSUB_isnan==1)	FP_CALC_PHASECPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_overflow==1)	FP_CALC_PHASECPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_underflow==1) FP_CALC_PHASECPWM_SHIFTED = FP_CALC_HALFMAXCOUNTS;
	else if (FP_ADDSUB_zero==1) FP_CALC_PHASECPWM_SHIFTED = 0;
	else	FP_CALC_PHASECPWM_SHIFTED = FP_ADDSUB_OUT;
	end
//--------------------- DoubleToInt CONVERSION STATE MACHINE -------------------------
if (PWM_RecalcCounter<920) //Init
	begin
	FP_CONVBACK_IN = 0;
	end
// PhA_PWM_CALC = CONVERTBACK(FP_CALC_PHASEAPWM_SHIFTED)
else if (PWM_RecalcCounter<930) //Write
	begin
	FP_CONVBACK_IN = FP_CALC_PHASEAPWM_SHIFTED; 
	end 
else if (PWM_RecalcCounter<935) //Read
	begin
	if (FP_CONVBACK_isnan==1)	PhA_PWM_CALC = 5000;
	else if (FP_CONVBACK_overflow==1)	PhA_PWM_CALC = 5000;
	else if (FP_CONVBACK_underflow==1) PhA_PWM_CALC = 5000;
	else	PhA_PWM_CALC = FP_CONVBACK_OUT;
	end
// PhB_PWM_CALC = CONVERTBACK(FP_CALC_PHASEBPWM_SHIFTED)
else if (PWM_RecalcCounter<945) //Write
	begin
	FP_CONVBACK_IN = FP_CALC_PHASEBPWM_SHIFTED;
	end
else if (PWM_RecalcCounter<950) //Read
	begin
	if (FP_CONVBACK_isnan==1)	PhB_PWM_CALC = 5000;
	else if (FP_CONVBACK_overflow==1)	PhB_PWM_CALC = 5000;
	else if (FP_CONVBACK_underflow==1) PhB_PWM_CALC = 5000;
	else	PhB_PWM_CALC = FP_CONVBACK_OUT;
	end
// PhC_PWM_CALC = CONVERTBACK(FP_CALC_PHASECPWM_SHIFTED)	
else if (PWM_RecalcCounter<955) //Write
	begin
	FP_CONVBACK_IN = FP_CALC_PHASECPWM_SHIFTED;
	end
else if (PWM_RecalcCounter<960) //Read
	begin
	if (FP_CONVBACK_isnan==1)	PhC_PWM_CALC = 5000;
	else if (FP_CONVBACK_overflow==1)	PhC_PWM_CALC = 5000;
	else if (FP_CONVBACK_underflow==1) PhC_PWM_CALC = 5000;
	else	PhC_PWM_CALC = FP_CONVBACK_OUT;
	end
else if (PWM_RecalcCounter<980)
	begin
	if (PH_GROUP_SM==0) begin PhU1_PWM_CALC=PhA_PWM_CALC; PhV1_PWM_CALC=PhB_PWM_CALC; PhW1_PWM_CALC=PhC_PWM_CALC;  end
	else if (PH_GROUP_SM==1) begin PhU2_PWM_CALC=PhA_PWM_CALC; PhV2_PWM_CALC=PhB_PWM_CALC; PhW2_PWM_CALC=PhC_PWM_CALC;  end
	else if (PH_GROUP_SM==2) begin PhU3_PWM_CALC=PhA_PWM_CALC; PhV3_PWM_CALC=PhB_PWM_CALC; PhW3_PWM_CALC=PhC_PWM_CALC;  end
	end
else
	begin
	if (PH_GROUP_SM==0) begin PWM_RecalcCounter = 0; PWM_Recalc = 1; PH_GROUP_SM<=1;  end
	else if (PH_GROUP_SM==1) begin PWM_RecalcCounter = 0; PWM_Recalc = 1; PH_GROUP_SM<=2; end
	else if (PH_GROUP_SM==2) begin PWM_RecalcCounter = 0; PWM_Recalc = 0; PH_GROUP_SM<=0; end
	end
end
//--------------RECALC END
end

`endif
//Sine PWM Calculation and PWM Controls END


//------------DEBUG MEMORY OUTPUT ----------------------
/*begin     
RAM_DATAOUT = RAM_DATAOUTwire;
if (RAM_SM_COUNTER==0) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==2) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==4) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end 
else if (RAM_SM_COUNTER==6) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==8) 
	begin
	RAM_DATAIN = 0;    
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==10) 
	begin
	RAM_DATAIN = PWM_CurrentPhaseSET;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==12) 
	begin
	RAM_DATAIN = PWM_OutpFreqCounts;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==14) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==16) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==18) 
	begin
	RAM_DATAIN = 32'hFFFFFFFF;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==20) 
	begin
	RAM_DATAIN = CalculationsError_Mask;
	RAM_WREN = 1;
	end
else if (RAM_SM_COUNTER==22) 
	begin
	RAM_DATAIN = 0;
	RAM_WREN = 1;
	end
else
	begin
	RAM_WREN=0;
	end
RAM_SM_COUNTER = RAM_SM_COUNTER + 1;
RAM_ADRESS = RAM_SM_COUNTER;
end 
*/
//------------DEBUG MEMORY OUTPUT ----------------------

end

endmodule