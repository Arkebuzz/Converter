module MEMORY_IO(clk, 
MTF_RESET_VALUE, MTF_EMIF_WATCHDOG_RESET_VALUE, MTF_PWM_AMP_VALUE, MTF_PWM_MAX_AMP_VALUE, MTF_PWM_OUTP_WAVE_LENGTH, MTF_PWM_OUTP_PHASE_VALUE, MTF_MAIN_PWM_FREQ_COUNTS, 
MTF_MAIN_PWM_OUTP_PHASE_SET, MTF_CHOPPER_PWM_FREQ_COUNTS, MTF_CHOPPER_PWM_AMP_VALUE, MTF_EXC_PWM_FREQ_COUNTS, MTF_EXC_PWM_AMP_VALUE, MTF_FAN_PWM_FREQ_COUNTS, MTF_FAN_PWM_AMP_VALUE, 
MTF_DOUT_VALUE,

FTM_ERROR_LATCH, FTM_EMIF_WATCHDOG_VALUE, FTM_ERROR_STATE, FTM_DIN_VALUE,

ram_adress, ram_data_toram, ram_data_fromram, ram_wren

);

input clk;

output MTF_RESET_VALUE, MTF_EMIF_WATCHDOG_RESET_VALUE, MTF_PWM_AMP_VALUE, MTF_PWM_MAX_AMP_VALUE, MTF_PWM_OUTP_WAVE_LENGTH, MTF_PWM_OUTP_PHASE_VALUE, MTF_MAIN_PWM_FREQ_COUNTS, 
MTF_MAIN_PWM_OUTP_PHASE_SET, MTF_CHOPPER_PWM_FREQ_COUNTS, MTF_CHOPPER_PWM_AMP_VALUE, MTF_EXC_PWM_FREQ_COUNTS, MTF_EXC_PWM_AMP_VALUE, MTF_FAN_PWM_FREQ_COUNTS, MTF_FAN_PWM_AMP_VALUE, 
MTF_DOUT_VALUE;

input FTM_ERROR_LATCH, FTM_EMIF_WATCHDOG_VALUE, FTM_ERROR_STATE, FTM_DIN_VALUE;

output ram_adress, ram_data_toram, ram_wren;
input ram_data_fromram;


//=======================================================
// INTERNAL DECLARATIONS
//=======================================================

reg [13:0] adress_OUT;
reg [15:0] data_IN;
reg [15:0] data_OUT;
reg wren_OUT;

reg [0:7] StartingCounter;

reg [15:0] RESET_VALUE;
reg [15:0] EMIF_WATCHDOG_RESET_VALUE;
reg [31:0] PWM_AMP_VALUE;
reg [31:0] PWM_MAX_AMP_VALUE;
reg [31:0] PWM_OUTP_WAVE_LENGTH;
reg [31:0] PWM_OUTP_PHASE_VALUE;
reg [31:0] MAIN_PWM_FREQ_COUNTS;
reg [31:0] MAIN_PWM_OUTP_PHASE_SET;
reg [31:0] CHOPPER_PWM_FREQ_COUNTS;
reg [31:0] CHOPPER_PWM_AMP_VALUE;
reg [31:0] EXC_PWM_FREQ_COUNTS;
reg [31:0] EXC_PWM_AMP_VALUE;
reg [31:0] FAN_PWM_FREQ_COUNTS;
reg [31:0] FAN_PWM_AMP_VALUE;
reg [15:0] DOUT_VALUE;

assign ram_data_toram = data_OUT;
assign ram_adress = adress_OUT;
assign ram_wren = wren_OUT;

assign MTF_RESET_VALUE = RESET_VALUE;
assign MTF_EMIF_WATCHDOG_RESET_VALUE = EMIF_WATCHDOG_RESET_VALUE;
assign MTF_PWM_AMP_VALUE = PWM_AMP_VALUE;
assign MTF_PWM_MAX_AMP_VALUE = PWM_MAX_AMP_VALUE;
assign MTF_PWM_OUTP_WAVE_LENGTH = PWM_OUTP_WAVE_LENGTH;
assign MTF_PWM_OUTP_PHASE_VALUE = PWM_OUTP_PHASE_VALUE;
assign MTF_MAIN_PWM_FREQ_COUNTS = MAIN_PWM_FREQ_COUNTS;
assign MTF_MAIN_PWM_OUTP_PHASE_SET = MAIN_PWM_OUTP_PHASE_SET;
assign MTF_CHOPPER_PWM_FREQ_COUNTS = CHOPPER_PWM_FREQ_COUNTS;
assign MTF_CHOPPER_PWM_AMP_VALUE = CHOPPER_PWM_AMP_VALUE;
assign MTF_EXC_PWM_FREQ_COUNTS = EXC_PWM_FREQ_COUNTS;
assign MTF_EXC_PWM_AMP_VALUE = EXC_PWM_AMP_VALUE;
assign MTF_FAN_PWM_FREQ_COUNTS = FAN_PWM_FREQ_COUNTS;
assign MTF_FAN_PWM_AMP_VALUE = FAN_PWM_AMP_VALUE;
assign MTF_DOUT_VALUE = DOUT_VALUE;

initial
begin
data_OUT = 0;
wren_OUT = 0;
StartingCounter = 0;
RESET_VALUE = 2014;
EMIF_WATCHDOG_RESET_VALUE =  1;
PWM_AMP_VALUE = 0;
PWM_MAX_AMP_VALUE = 10000;
PWM_OUTP_WAVE_LENGTH = 1000000;
PWM_OUTP_PHASE_VALUE = 0;
MAIN_PWM_FREQ_COUNTS = 10000;
MAIN_PWM_OUTP_PHASE_SET = 0;
CHOPPER_PWM_FREQ_COUNTS = 10000;
CHOPPER_PWM_AMP_VALUE = 0;
EXC_PWM_FREQ_COUNTS = 10000;
EXC_PWM_AMP_VALUE = 0;
FAN_PWM_FREQ_COUNTS = 10000;
FAN_PWM_AMP_VALUE = 10000;
DOUT_VALUE = 0;
end

always@(posedge clk)
begin

data_IN = ram_data_fromram;

if (StartingCounter<200) //Upload default values to RAM
	begin
	StartingCounter = StartingCounter+1;
	
	if (StartingCounter == 1)
		begin 
		adress_OUT = 0;
		data_OUT = 1111;
		wren_OUT = 1;
		end
	else if (StartingCounter==5)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==10)
		begin
		adress_OUT = 1;
		data_OUT = 2222;
		wren_OUT = 1;
		end
	else if (StartingCounter==15)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==20)
		begin
		adress_OUT = 2;
		data_OUT = 3333;
		wren_OUT = 1;
		end
	else if (StartingCounter==25)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==30)
		begin
		adress_OUT = 3;
		data_OUT = 4444;
		wren_OUT = 1;
		end
	else if (StartingCounter==35)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==40)
		begin
		adress_OUT = 4;
		data_OUT = 5555;
		wren_OUT = 1;
		end
	else if (StartingCounter==45)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==50)
		begin
		adress_OUT = 5;
		data_OUT = 6666;
		wren_OUT = 1;
		end
	else if (StartingCounter==55)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
		
	else if (StartingCounter==60)
		begin
		adress_OUT = 6;
		data_OUT = 7777;
		wren_OUT = 1;
		end
	else if (StartingCounter==65)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==70)
		begin
		adress_OUT = 7;
		data_OUT = 8888;
		wren_OUT = 1;
		end
	else if (StartingCounter==75)
		begin
		adress_OUT = 0;
		data_OUT = 0;
		wren_OUT = 0;
		end
		
	else if (StartingCounter==180)
		begin
		StartingCounter=0;
		end
	
	end //Upload end
	
	
	
else		//Provide data IO with MCU
	begin
	
	end


end




//=======================================================
//  STRUCTURE
//=======================================================

endmodule