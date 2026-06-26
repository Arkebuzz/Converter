module INV_DATA_IO (clockin, FO_INPUT, FO_OUTP, DATA_TO_SEND, DATA_RECIEVED, DATA_CLOCK, IO_ERR, SYNC_IN);

localparam OutputNumBits = 48;
localparam InputNumBits = 96;

input FO_INPUT;
input clockin;
input [OutputNumBits-1:0] DATA_TO_SEND;
input SYNC_IN;

reg [13:0]  TRANSMITT_DATA_VALUE /*synthesis noprune */;
reg [1:0] TX_DATA_COUNTER /*synthesis noprune */;

output FO_OUTP;
output [95:0] DATA_RECIEVED;
output DATA_CLOCK;
output [2:0] IO_ERR;


reg [7:0] TX_SM_COUNTER /*synthesis noprune */;
reg [2:0] TX_SM_STATE /*synthesis noprune */;
reg [9:0] TX_BIT_COUNTER /*synthesis noprune */;
reg [4:0] TX_VAL_NUM_COUNTER/*synthesis noprune */;
reg [4:0] TX_SLOW_VAL_NUM_COUNTER/*synthesis noprune */;


reg [95:0] DATA_OUTPUT_VALUE /*synthesis noprune */;
reg [95:0] RX_DATA_READ_VALUE /*synthesis noprune */;

reg FO_OUTPUT;
reg DATA_READY/*synthesis noprune */;
reg DATA_READY_OUT/*synthesis noprune */;
reg ERROR_FLAG/*synthesis noprune */;
reg CRITICAL_ERROR_FLAG/*synthesis noprune */;
reg NOTRANSMITTION_FAULT_FLAG/*synthesis noprune */;
reg [2:0] ERROR_OUTPUT/*synthesis noprune */;
reg [15:0] NOCHANGE_COUNTER/*synthesis noprune */;
reg ERROR_FLAG_COUNTER/*synthesis noprune */;
reg ERROR_FLAG_COUNTER_PREV/*synthesis noprune */;
reg CRITICAL_ERROR_FLAG_COUNTER/*synthesis noprune */;

reg FO_INPUT_REG/*synthesis noprune */;
reg FO_INPUT_PREV/*synthesis noprune */;
reg FO_INPUT_VAL/*synthesis noprune */;

reg RX_RESET;
reg [5:0] RX_SM_STATE/* synthesis noprune */;
reg [15:0] RX_SM_COUNTER/*synthesis noprune */;
reg [9:0] RX_BIT_COUNTER/* synthesis noprune */;
reg [9:0] RX_BIT_COUNTER_OLD/*synthesis noprune */;
reg [15:0] RX_POSVAL_COUNTER/* synthesis noprune */;
reg [15:0] RX_NEGVAL_COUNTER/* synthesis noprune */;
reg [15:0] RX_CHECKVAL_COUNTER/*synthesis noprune */;
reg POSVAL_RESET /*synthesis noprune */;
reg [4:0] DATA_READY_COUNTER;
reg SYNC_REG;
reg SYNC_PREV;

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
DATA_READY_COUNTER=0;
end


always@(posedge clockin)
begin

//TRANSMITT --------------------------------------------------
SYNC_REG<=SYNC_IN;
SYNC_PREV<=SYNC_REG;

if (TX_SM_STATE==0) //Generate start signal
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if ((SYNC_REG==1) && (SYNC_PREV==0)) //GENERATE SYNC SIGNAL
		begin  
		FO_OUTPUT <= 1;
		TX_SM_STATE <= 2;
		TX_SM_COUNTER <= 0;
		TX_BIT_COUNTER <= 0;
		end  
	else if (TX_SM_COUNTER>75)
		begin
		if (TX_DATA_COUNTER==0) begin TRANSMITT_DATA_VALUE[13:12]<=0; TRANSMITT_DATA_VALUE[11:0]<= DATA_TO_SEND[11:0]; end //
		else if (TX_DATA_COUNTER==1) begin TRANSMITT_DATA_VALUE[13:12]<=1; TRANSMITT_DATA_VALUE[11:0]<= DATA_TO_SEND[23:12];  end //
		else if (TX_DATA_COUNTER==2) begin TRANSMITT_DATA_VALUE[13:12]<=2; TRANSMITT_DATA_VALUE[11:0]<= DATA_TO_SEND[35:24]; end  //
		else if (TX_DATA_COUNTER==3) begin TRANSMITT_DATA_VALUE[13:12]<=3; TRANSMITT_DATA_VALUE[11:0]<= DATA_TO_SEND[47:36];  end  //
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
		if ((SYNC_REG==1) && (SYNC_PREV==0)) //GENERATE SYNC SIGNAL
		begin  
		FO_OUTPUT <= 1;
		TX_SM_STATE <= 2;
		TX_SM_COUNTER <= 0;
		TX_BIT_COUNTER <= 0;
		end  
		else if (TX_SM_COUNTER==25)
			begin
			TX_SM_COUNTER<=0;
			if (TX_BIT_COUNTER==13)
				begin
				if (TX_DATA_COUNTER<3) begin  TX_DATA_COUNTER <= TX_DATA_COUNTER+1; end
				else begin TX_DATA_COUNTER <= 0; end
				FO_OUTPUT <= 0;
				TX_SM_STATE <= 0;
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
		else if (TRANSMITT_DATA_VALUE[TX_BIT_COUNTER]==0)
			begin
			if (TX_SM_COUNTER==5)
				begin
				FO_OUTPUT<=0;
				end
			end
		else
			begin
			if (TX_SM_COUNTER==18)
				begin
				FO_OUTPUT<=0;
				end
			end
	end
else if (TX_SM_STATE==2) //Transmitt sync pulse
	begin
	TX_SM_COUNTER <= TX_SM_COUNTER + 1;
	if (TX_SM_COUNTER>75)
		begin
		FO_OUTPUT <= 0;
		TX_SM_STATE <= 0;
		TX_BIT_COUNTER <= 0;
		TX_DATA_COUNTER <= 0;
		TX_SM_COUNTER<=0;
		end
	else
		begin
		FO_OUTPUT <= 1;
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
	if (RX_NEGVAL_COUNTER == 48) begin DATA_OUTPUT_VALUE = RX_DATA_READ_VALUE; end
	if (RX_NEGVAL_COUNTER == 50) begin DATA_READY = 1;  end
	end
else 
	begin 
	DATA_READY = 0;
	if (FO_INPUT_PREV==1) begin RX_NEGVAL_COUNTER<=0; end
	if (RX_POSVAL_COUNTER<400) 	begin RX_POSVAL_COUNTER <= RX_POSVAL_COUNTER +1; end
	end

if (RX_NEGVAL_COUNTER>300 || RX_POSVAL_COUNTER>300) begin NOTRANSMITTION_FAULT_FLAG<=1; end
else begin NOTRANSMITTION_FAULT_FLAG<=0; end

// Detect RISING Edge (0 -> 1)
if (FO_INPUT_REG == 1 && FO_INPUT_PREV == 0) begin
    // This logic was in: always@(posedge FO_INPUT)
	 if (RX_NEGVAL_COUNTER > 50) begin 
        RX_BIT_COUNTER <= 0; 
    end
    else begin 
        RX_BIT_COUNTER <= RX_BIT_COUNTER + 1; 
    end
end
// Detect FALLING Edge (1 -> 0)
else if (FO_INPUT_REG == 0 && FO_INPUT_PREV == 1) begin
    // This logic was in: always@(negedge FO_INPUT)
    if (RX_POSVAL_COUNTER > 16) begin 
        RX_DATA_READ_VALUE[RX_BIT_COUNTER] <= 1; 
    end
    else begin 
        RX_DATA_READ_VALUE[RX_BIT_COUNTER] <= 0; 
    end
end
else begin end

//RECIEVE END-------------------------------------------------

end















endmodule