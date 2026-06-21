module EXC_DATA_IO (clockin, FO_INPUT, FO_OUTP, DATA_TO_SEND, DATA_RECIEVED, DATA_CLOCK, IO_ERR);

localparam OutputNumBits = 36;
localparam InputNumBits = 144;

input FO_INPUT;
input clockin;
input [OutputNumBits-1:0] DATA_TO_SEND;
reg [OutputNumBits-1:0]  TRANSMITT_DATA_VALUE;


output FO_OUTP;
output [143:0] DATA_RECIEVED/*synthesis noprune */;
output DATA_CLOCK;
output [2:0] IO_ERR;


reg [7:0] TX_SM_COUNTER;
reg [4:0] TX_SM_STATE;
reg [9:0] TX_BIT_COUNTER;
reg [4:0] TX_VAL_NUM_COUNTER;
reg [4:0] TX_SLOW_VAL_NUM_COUNTER;


reg [143:0] DATA_OUTPUT_VALUE/*synthesis noprune */;
reg [143:0] RX_DATA_READ_VALUE;

reg FO_OUTPUT;
reg DATA_READY;
reg DATA_READY_OUT;
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

reg RX_RESET;
reg [5:0] RX_SM_STATE/* synthesis noprune */;
reg [15:0] RX_SM_COUNTER;
reg [9:0] RX_BIT_COUNTER/* synthesis noprune */;
reg [9:0] RX_BIT_COUNTER_OLD;
reg [15:0] RX_POSVAL_COUNTER/* synthesis noprune */;
reg [15:0] RX_NEGVAL_COUNTER/* synthesis noprune */;
reg [15:0] RX_CHECKVAL_COUNTER;
reg POSVAL_RESET /*synthesis noprune */;

assign FO_OUTP = ~FO_OUTPUT;
assign DATA_RECIEVED = DATA_OUTPUT_VALUE;
assign DATA_CLOCK = DATA_READY_OUT;
assign IO_ERR = ERROR_OUTPUT;

initial
begin
TX_SM_COUNTER = 0;
TX_SM_STATE = 0;
TX_BIT_COUNTER=0;
TX_VAL_NUM_COUNTER=0;
DATA_READY_OUT=0;
end

always@(posedge clockin)
begin

//TRANSMITT --------------------------------------------------

if (TX_SM_STATE==0) //Generate start signal
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER>75)
		begin
		TRANSMITT_DATA_VALUE <= DATA_TO_SEND;
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
else if (TX_SM_STATE==1) //Transmitt data	
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
		if (TX_SM_COUNTER==25)
			begin
			TX_SM_COUNTER<=0;
			if (TX_BIT_COUNTER==(OutputNumBits-1))
				begin
				FO_OUTPUT <= 0;
				TX_SM_STATE <= 0;
				TX_SM_COUNTER <= 0;
				TX_BIT_COUNTER <= 0;
				end
			else
				begin
				TX_BIT_COUNTER <= TX_BIT_COUNTER+1;
				end
			end
		else if (TX_SM_COUNTER==0)
			begin
			FO_OUTPUT <= 1;
			end
		else if (TX_SM_COUNTER==6)
			begin
			if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
				begin
				FO_OUTPUT<=0;
				end
			end
		else if (TX_SM_COUNTER==17) 
			begin
				FO_OUTPUT<=0;
			end
	end

//TRANSMITT END-----------------------------------------------


//RECIEVE ----------------------------------------------------
// 1. Synchronize Input used for edge detection
FO_INPUT_REG <= FO_INPUT;         // Current State
FO_INPUT_PREV <= FO_INPUT_REG;    // Previous State
DATA_READY_OUT <= DATA_READY;

ERROR_OUTPUT[0] <= NOTRANSMITTION_FAULT_FLAG;
ERROR_OUTPUT[1] <= CRITICAL_ERROR_FLAG;
ERROR_OUTPUT[2] <= ERROR_FLAG;


if (FO_INPUT_REG==0) 
	begin 
	if (RX_NEGVAL_COUNTER<400)	begin RX_NEGVAL_COUNTER <= RX_NEGVAL_COUNTER +1; end
	if (FO_INPUT_PREV==0) begin RX_POSVAL_COUNTER<=0; end
	if (RX_NEGVAL_COUNTER==48) begin DATA_OUTPUT_VALUE = RX_DATA_READ_VALUE; end
	if (RX_NEGVAL_COUNTER==50) begin DATA_READY=1; end
	end
else 
	begin 
	DATA_READY=0;
	if (FO_INPUT_PREV==1) begin RX_NEGVAL_COUNTER<=0; end
	if (RX_POSVAL_COUNTER<400) 	begin RX_POSVAL_COUNTER <= RX_POSVAL_COUNTER +1; end
	end


if (RX_NEGVAL_COUNTER>300 || RX_POSVAL_COUNTER>300) begin NOTRANSMITTION_FAULT_FLAG<=1; end
else begin NOTRANSMITTION_FAULT_FLAG<=0; end

// Detect RISING Edge (0 -> 1)
if (FO_INPUT_REG == 1 && FO_INPUT_PREV == 0) begin
    // This logic was in: always@(posedge FO_INPUT)
    if (RX_NEGVAL_COUNTER>50) begin RX_BIT_COUNTER <= 0; end
	else begin RX_BIT_COUNTER <= RX_BIT_COUNTER+1; end
end
// Detect FALLING Edge (1 -> 0)
else if (FO_INPUT_REG == 0 && FO_INPUT_PREV == 1) begin
    // This logic was in: always@(negedge FO_INPUT)
    if (RX_POSVAL_COUNTER > 16) begin RX_DATA_READ_VALUE[RX_BIT_COUNTER] <=  1; end
	else begin RX_DATA_READ_VALUE[RX_BIT_COUNTER] <=  0; end
end
else begin end

//RECIEVE END-------------------------------------------------

end
















endmodule