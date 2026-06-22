module PCON_DATA_IO (clockin, FO_INPUT, FO_OUTP, DATA_TO_SEND, DATA_RECIEVED, DATA_CLOCK, IO_ERR);

input FO_INPUT;
input clockin;
input [155:0] DATA_TO_SEND;

output FO_OUTP;
output [155:0] DATA_RECIEVED;
output DATA_CLOCK;
output [2:0] IO_ERR;

reg [7:0] FO_INPUT_CRITICAL_DATA;
reg [95:0] FO_INPUT_REGULAR_DATA;

reg [15:0]  TRANSMITT_DATA_VALUE;
reg [7:0] TX_SM_COUNTER;
reg [4:0] TX_SM_STATE;
reg [4:0] TX_BIT_COUNTER;
reg [4:0] TX_VAL_NUM_COUNTER;
reg [4:0] TX_SLOW_VAL_NUM_COUNTER;


reg [155:0] DATA_OUTPUT_VALUE;

reg FO_OUTPUT;
reg DATA_READY;
reg ERROR_FLAG;
reg CRITICAL_ERROR_FLAG;
reg NOTRANSMITTION_FAULT_FLAG;
reg [2:0] ERROR_OUTPUT;
reg [15:0] NOCHANGE_COUNTER;
reg ERROR_FLAG_COUNTER;
reg ERROR_FLAG_COUNTER_PREV;
reg CRITICAL_ERROR_FLAG_COUNTER;

reg FO_INPUT_REG;
reg FO_INPUT_PREV;
reg [63:0] RX_DATA_READ_VALUE;
reg [11:0] RX_DATA_VALUE;
reg [11:0] RX_DATA_CHECK_VALUE;
reg [7:0] RX_CRITICAL_DATA_VALUE;
reg [7:0] RX_CRITICAL_DATA_CHECK_VALUE;
reg [3:0] RX_DATA_NUM;
reg [3:0] RX_DATA_NUM_CHECK;
reg [3:0] RX_SLOW_VAL_NUM_COUNTER;

reg [15:0] RX_SM_STATE;
reg [15:0] RX_SM_COUNTER;
reg [5:0] RX_BIT_COUNTER;
reg [5:0] RX_BIT_COUNTER_OLD;
reg [15:0] RX_POSVAL_COUNTER;
reg [15:0] RX_NEGVAL_COUNTER;
reg [15:0] RX_CHECKVAL_COUNTER;

assign FO_OUTP = FO_OUTPUT;
assign DATA_RECIEVED = DATA_OUTPUT_VALUE;
assign DATA_CLOCK = DATA_READY;
assign IO_ERR = ERROR_OUTPUT;

initial
begin
TX_SM_COUNTER = 0;
TX_SM_STATE = 0;
TX_BIT_COUNTER=0;
TX_VAL_NUM_COUNTER=0;
end

always@(posedge clockin)
begin

//TRANSMITT --------------------------------------------------

if (TX_SM_STATE==0) //Generate start signal
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER>75)
		begin
		TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[7:0];
		FO_OUTPUT <= 0;
		TX_SM_STATE <= 1;
		TX_SM_COUNTER <= 0;
		TX_BIT_COUNTER <= 0;
		end
	else
		begin
		FO_OUTPUT<=0;
		end
	
	end
else if (TX_SM_STATE==1) //Transmitt critical data	
	begin
	
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER==28)
		begin
		TX_SM_COUNTER<=0;
		TX_BIT_COUNTER <= TX_BIT_COUNTER+1;
		if (TX_BIT_COUNTER==7)
			begin
			FO_OUTPUT <= 0;
			TX_SM_STATE <= 2;
			TX_SM_COUNTER <= 0;
			TX_BIT_COUNTER <= 0;
			end
		end
	if (TX_SM_COUNTER==0)
		begin
		FO_OUTPUT <= 1;
		end
	if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
		begin
		if (TX_SM_COUNTER==8)
			begin
			FO_OUTPUT<=0;
			end
		end
	else
		begin
		if (TX_SM_COUNTER==20)
			begin
			FO_OUTPUT<=0;
			end
		end
	
	end
else if (TX_SM_STATE==2) //Repeat critical data	transmittion	
	begin
	
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER==28)
		begin
		TX_SM_COUNTER<=0;
		TX_BIT_COUNTER <= TX_BIT_COUNTER+1;
		if (TX_BIT_COUNTER==7)
			begin
			if (TX_VAL_NUM_COUNTER==0)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[19:8]; end
			else if (TX_VAL_NUM_COUNTER==1)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[31:20]; end
			else if (TX_VAL_NUM_COUNTER==2)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[43:32]; end
			else if (TX_VAL_NUM_COUNTER==3)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[55:44]; end
			else if (TX_VAL_NUM_COUNTER==4)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[67:56]; end
			else if (TX_VAL_NUM_COUNTER==5)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[79:68]; end
			else if (TX_VAL_NUM_COUNTER==6)
			begin TRANSMITT_DATA_VALUE[11:0] <= DATA_TO_SEND[91:80]; end
			else if (TX_VAL_NUM_COUNTER==7)
			begin 
				TRANSMITT_DATA_VALUE[11:8] = TX_SLOW_VAL_NUM_COUNTER;
				if (TX_SLOW_VAL_NUM_COUNTER==0) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[99:92]; end
				if (TX_SLOW_VAL_NUM_COUNTER==1) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[107:100]; end
				if (TX_SLOW_VAL_NUM_COUNTER==2) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[115:108]; end
				if (TX_SLOW_VAL_NUM_COUNTER==3) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[123:116]; end
				if (TX_SLOW_VAL_NUM_COUNTER==4) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[131:124]; end
				if (TX_SLOW_VAL_NUM_COUNTER==5) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[139:132]; end
				if (TX_SLOW_VAL_NUM_COUNTER==6) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[147:140]; end
				if (TX_SLOW_VAL_NUM_COUNTER==7) begin TRANSMITT_DATA_VALUE[7:0] <= DATA_TO_SEND[155:148]; end
			end
			
			TRANSMITT_DATA_VALUE[15:12] <= TX_VAL_NUM_COUNTER;
			FO_OUTPUT <= 1;
			TX_SM_STATE <= 3;
			TX_SM_COUNTER <= 0;
			TX_BIT_COUNTER <= 0;
			end
		end
	if (TX_SM_COUNTER==0)
		begin
		FO_OUTPUT <= 1;
		end
	if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
		begin
		if (TX_SM_COUNTER==8)
			begin
			FO_OUTPUT<=0;
			end
		end
	else
		begin
		if (TX_SM_COUNTER==20)
			begin
			FO_OUTPUT<=0;
			end
		end
	end	
else if (TX_SM_STATE==3) //Transmitt regular data	
	begin
	
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER==28)
		begin
		TX_SM_COUNTER<=0;
		TX_BIT_COUNTER <= TX_BIT_COUNTER+1;
		if (TX_BIT_COUNTER==15)
			begin
			FO_OUTPUT <= 1;
			TX_SM_STATE <= 4;
			TX_SM_COUNTER <= 0;
			TX_BIT_COUNTER <= 0;
			end
		end
	if (TX_SM_COUNTER==0)
		begin
		FO_OUTPUT <= 1;
		end
	if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
		begin
		if (TX_SM_COUNTER==8)
			begin
			FO_OUTPUT<=0;
			end
		end
	else
		begin
		if (TX_SM_COUNTER==20)
			begin
			FO_OUTPUT<=0;
			end
		end
	end	
else if (TX_SM_STATE==4) //Repeat regular data transmittion
	begin
	
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER==28)
		begin
		TX_SM_COUNTER<=0;
		TX_BIT_COUNTER <= TX_BIT_COUNTER+1;
		if (TX_BIT_COUNTER==15)
			begin
			FO_OUTPUT <= 1;
			TX_SM_STATE <= 5;
			TX_SM_COUNTER <= 0;
			TX_BIT_COUNTER <= 0;
			end
		end
	if (TX_SM_COUNTER==0)
		begin
		FO_OUTPUT <= 1;
		end
	if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
		begin
		if (TX_SM_COUNTER==8)
			begin
			FO_OUTPUT<=0;
			end
		end
	else
		begin
		if (TX_SM_COUNTER==20)
			begin
			FO_OUTPUT<=0;
			end
		end
	end	
else if (TX_SM_STATE==5) //End pulse generation
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	FO_OUTPUT<=1;
	if (TX_SM_COUNTER==28)
		begin
		if (TX_VAL_NUM_COUNTER==7)
			begin
			if (TX_SLOW_VAL_NUM_COUNTER<7) begin TX_SLOW_VAL_NUM_COUNTER = TX_SLOW_VAL_NUM_COUNTER + 1; end
			else begin TX_SLOW_VAL_NUM_COUNTER=0; end
			TX_VAL_NUM_COUNTER = 0;
			end
		else
			begin
			TX_VAL_NUM_COUNTER = TX_VAL_NUM_COUNTER + 1;
			end
		FO_OUTPUT <= 0;
		TX_SM_STATE <= 0;
		TX_SM_COUNTER <= 0;
		TX_BIT_COUNTER <= 0;
		end
	end
//TRANSMITT END-----------------------------------------------


//RECIEVE ----------------------------------------------------
FO_INPUT_REG = FO_INPUT;
RX_SM_COUNTER = RX_SM_COUNTER + 1;

ERROR_OUTPUT[0] = NOTRANSMITTION_FAULT_FLAG;
ERROR_OUTPUT[1] = CRITICAL_ERROR_FLAG;
ERROR_OUTPUT[2] = ERROR_FLAG;

if (FO_INPUT_PREV!=FO_INPUT_REG)
	begin
		NOCHANGE_COUNTER = 0;
		NOTRANSMITTION_FAULT_FLAG = 0;
		if (FO_INPUT_PREV==0)
		begin
				if (RX_SM_COUNTER<3)
				begin
				if (RX_CHECKVAL_COUNTER<14)
						begin
						RX_DATA_READ_VALUE[RX_BIT_COUNTER] = 1;
						end
					else
						begin
						RX_DATA_READ_VALUE[RX_BIT_COUNTER] = 0;
						end
					
				end	
				if (RX_SM_COUNTER>3)
					begin
					RX_POSVAL_COUNTER = 0;
					RX_NEGVAL_COUNTER = 0;
					RX_BIT_COUNTER_OLD =  RX_BIT_COUNTER;
					end
					
				if (RX_SM_COUNTER>4)
					begin
					FO_INPUT_PREV = FO_INPUT_REG;
					end
				end
		else
			begin
				RX_CRITICAL_DATA_VALUE = RX_DATA_READ_VALUE[8:1];
				RX_CRITICAL_DATA_CHECK_VALUE = RX_DATA_READ_VALUE[16:9];
				RX_DATA_VALUE = RX_DATA_READ_VALUE[28:17];
				RX_DATA_NUM =  RX_DATA_READ_VALUE[32:29];
				RX_DATA_CHECK_VALUE = RX_DATA_READ_VALUE[44:33];
				RX_DATA_NUM_CHECK = RX_DATA_READ_VALUE[48:45];
				RX_BIT_COUNTER = RX_BIT_COUNTER_OLD + 1;
				RX_SLOW_VAL_NUM_COUNTER = RX_DATA_READ_VALUE[28:25];
				
				if (RX_SM_COUNTER>4)
					begin
					FO_INPUT_PREV = FO_INPUT_REG;
					end
			end
	end
else
	begin
	RX_SM_COUNTER = 0;
	if (NOCHANGE_COUNTER>250)
		begin
		NOTRANSMITTION_FAULT_FLAG = 1;
		end
	else
		begin
		NOCHANGE_COUNTER = NOCHANGE_COUNTER + 1;
		end
	end

if (FO_INPUT_REG==1)
	begin
	RX_POSVAL_COUNTER = RX_POSVAL_COUNTER + 1;
	RX_NEGVAL_COUNTER = RX_NEGVAL_COUNTER;
	RX_CHECKVAL_COUNTER = RX_CHECKVAL_COUNTER;
	end
else
	begin
	RX_NEGVAL_COUNTER = RX_NEGVAL_COUNTER + 1;
	RX_POSVAL_COUNTER = RX_POSVAL_COUNTER;
	RX_CHECKVAL_COUNTER = RX_NEGVAL_COUNTER;
	end

if (RX_NEGVAL_COUNTER>50 && RX_NEGVAL_COUNTER<55)
	begin
	RX_BIT_COUNTER = 0;
	
		if (RX_CRITICAL_DATA_VALUE==RX_CRITICAL_DATA_CHECK_VALUE)
			begin
			DATA_OUTPUT_VALUE[7:0] = RX_CRITICAL_DATA_VALUE;
			CRITICAL_ERROR_FLAG = 0;
			CRITICAL_ERROR_FLAG_COUNTER=0;
			end
		else
			begin
				if (CRITICAL_ERROR_FLAG_COUNTER==1)
					begin
					CRITICAL_ERROR_FLAG = 1;
					DATA_OUTPUT_VALUE[7:0] = 0;
					end
				else
					begin
					CRITICAL_ERROR_FLAG_COUNTER = 1;
					DATA_OUTPUT_VALUE[7:0] = DATA_OUTPUT_VALUE[7:0];
					end
			end
			
			
		if ((RX_DATA_VALUE==RX_DATA_CHECK_VALUE) && (RX_DATA_NUM==RX_DATA_NUM_CHECK))
			begin
			if (RX_DATA_NUM==0)
			begin 
			DATA_OUTPUT_VALUE[19:8] = RX_DATA_VALUE;
			ERROR_FLAG_COUNTER = 0;
			end
			else if (RX_DATA_NUM==1)
			begin DATA_OUTPUT_VALUE[31:20] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==2)
			begin DATA_OUTPUT_VALUE[43:32] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==3)
			begin DATA_OUTPUT_VALUE[55:44] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==4)
			begin DATA_OUTPUT_VALUE[67:56] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==5)
			begin DATA_OUTPUT_VALUE[79:68] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==6)
			begin DATA_OUTPUT_VALUE[91:80] = RX_DATA_VALUE; end
			else if (RX_DATA_NUM==7)
				begin 
				if (RX_SLOW_VAL_NUM_COUNTER==0) begin DATA_OUTPUT_VALUE[99:92] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==1) begin DATA_OUTPUT_VALUE[107:100] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==2) begin DATA_OUTPUT_VALUE[115:108] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==3) begin DATA_OUTPUT_VALUE[123:116] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==4) begin DATA_OUTPUT_VALUE[131:124] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==5) begin DATA_OUTPUT_VALUE[139:132] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==6) begin DATA_OUTPUT_VALUE[147:140] = RX_DATA_VALUE[7:0]; end
				if (RX_SLOW_VAL_NUM_COUNTER==7) begin DATA_OUTPUT_VALUE[155:148] = RX_DATA_VALUE[7:0]; end
				if (ERROR_FLAG_COUNTER==0 && ERROR_FLAG_COUNTER_PREV==0)
					begin
					ERROR_FLAG = 0;
					end
				else 
					begin
					ERROR_FLAG_COUNTER_PREV = ERROR_FLAG_COUNTER;
					end
				
				end
			end
		else
			begin
			if (ERROR_FLAG_COUNTER_PREV==1)
				begin
				ERROR_FLAG = 1;
				end
			else
				begin
				ERROR_FLAG_COUNTER = 1;
				end
			end
			
	end
	
if (RX_NEGVAL_COUNTER>55)
	begin
	RX_CRITICAL_DATA_VALUE = 0;
	RX_CRITICAL_DATA_CHECK_VALUE = 0;
	RX_DATA_VALUE = 0;
	RX_DATA_NUM =  0;
	RX_DATA_CHECK_VALUE = 0;
	RX_DATA_NUM_CHECK = 0;
	RX_DATA_READ_VALUE = 0;
	end
//RECIEVE END-------------------------------------------------


end


endmodule
