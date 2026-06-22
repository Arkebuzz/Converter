module DPR_DATA_IN (clockin, FO_INPUT, DPR_DATA_OUT, TEMP_DATA_OUT, DATA_CLOCK, IO_ERR);

input FO_INPUT;
input clockin;

output [71:0] TEMP_DATA_OUT;
output [2:0] DPR_DATA_OUT;
output DATA_CLOCK;
output [2:0] IO_ERR;

reg [71:0] TEMP_DATA/*synthesis noprune */;
reg [2:0] DPR_DATA/*synthesis noprune */;
reg [2:0] DPR_DATA_TEMP /*synthesis noprune */;
reg [71:0] RX_DATA_READ_VALUE/*synthesis noprune */;

reg FO_INPUT_REG/*synthesis noprune */;
reg FO_INPUT_PREV/*synthesis noprune */;

reg [7:0] RX_DATA_BIT_COUNTER/*synthesis noprune */;
reg [2:0] RX_BIT_COUNTER/*synthesis noprune */;
reg [8:0] RX_POSVAL_COUNTER/*synthesis noprune */;
reg [8:0] RX_NEGVAL_COUNTER/*synthesis noprune */;
reg DATA_READY /*synthesis noprune */;
reg DATA_READY_OUT /*synthesis noprune */;
reg NOTRANSMITTION_FAULT_FLAG /*synthesis noprune */;
reg [2:0] ERROR_OUTPUT /*synthesis noprune */;

assign TEMP_DATA_OUT = TEMP_DATA;
assign DPR_DATA_OUT = DPR_DATA;
assign DATA_CLOCK = DATA_READY_OUT;
assign IO_ERR = ERROR_OUTPUT;

reg FO_NEGEDGE;

always@(posedge clockin)
begin

//-----------------------------------DATA RECIEVE-------------------------------------
// 1. Synchronize Input used for edge detection
FO_INPUT_REG <= FO_INPUT;         // Current State
FO_INPUT_PREV <= FO_INPUT_REG;    // Previous State
ERROR_OUTPUT[0]<=NOTRANSMITTION_FAULT_FLAG;
DATA_READY_OUT<=DATA_READY;

if (FO_INPUT_REG==0) 
	begin 
	if (RX_NEGVAL_COUNTER<400)	begin RX_NEGVAL_COUNTER <= RX_NEGVAL_COUNTER +1; end
	if (FO_INPUT_PREV==0) begin RX_POSVAL_COUNTER<=0; end
	if (RX_NEGVAL_COUNTER==48) begin TEMP_DATA <= RX_DATA_READ_VALUE;end 
	if (RX_NEGVAL_COUNTER==50) begin DATA_READY = 1; end
	end
else 
	begin 
	DATA_READY = 0;
	if (FO_INPUT_PREV==1) begin RX_NEGVAL_COUNTER<=0; end
	if (RX_POSVAL_COUNTER<400) begin RX_POSVAL_COUNTER <= RX_POSVAL_COUNTER +1; end
	DATA_READY =0;
	end

if (RX_NEGVAL_COUNTER>300 || RX_POSVAL_COUNTER>300) begin NOTRANSMITTION_FAULT_FLAG<=1; end
else begin NOTRANSMITTION_FAULT_FLAG<=0; end


// Detect RISING Edge (0 -> 1)
if (FO_INPUT_REG == 1 && FO_INPUT_PREV == 0) begin
    // This logic was in: always@(posedge FO_INPUT)
	 if (RX_NEGVAL_COUNTER>50) 
		begin 
		RX_DATA_BIT_COUNTER <= 0; 
		RX_BIT_COUNTER <= 0; 
		end
	else 
		begin 
		if (RX_BIT_COUNTER<3) begin RX_BIT_COUNTER<= RX_BIT_COUNTER+1; end
		else begin RX_DATA_BIT_COUNTER <= RX_DATA_BIT_COUNTER+1; RX_BIT_COUNTER<=0; DPR_DATA<=DPR_DATA_TEMP; end
		end
end
// Detect FALLING Edge (1 -> 0)
else if (FO_INPUT_REG == 0 && FO_INPUT_PREV == 1) begin
    // This logic was in: always@(negedge FO_INPUT)
    if (RX_POSVAL_COUNTER > 16) 
		begin 
		if (RX_BIT_COUNTER<3) begin DPR_DATA_TEMP[RX_BIT_COUNTER]<=1; end
		else begin RX_DATA_READ_VALUE[RX_DATA_BIT_COUNTER]<=1; end
		end
	else 
		begin 
		if (RX_BIT_COUNTER<3) begin DPR_DATA_TEMP[RX_BIT_COUNTER]<=0; end
		else begin RX_DATA_READ_VALUE[RX_DATA_BIT_COUNTER]<=0; end 
		end
end
else begin end
//RECIEVE END-------------------------------------------------
	
end





endmodule