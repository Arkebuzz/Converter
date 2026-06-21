module EMIF(ad, cs0, cs1, wr, oe, ale, clk, clk2, ftmdata, ftmadress , ftmwen , mtfdata);

inout [15:0] ad;

input cs0, cs1;

input wr, oe, ale, clk, clk2;

input [15:0] ftmdata;
input [13:0] ftmadress;
input ftmwen;

output [15:0] mtfdata;


//=======================================================
// INTERNAL DECLARATIONS
//=======================================================
reg [3:0] SRAM_IO_COUNTER;

reg [13:0] adress;
reg [15:0] data;
reg [15:0] buffer;
reg wren;

reg [13:0] adress2;
reg [15:0] data2;
reg wren2;

wire wren_ram, wren_ram2;
wire [15:0] q_ram, q_ram2;
wire [15:0] data_ram, data_ram2;
wire [13:0] adress_ram, adress_ram2;


//=======================================================
//  STRUCTURE
//=======================================================

assign ad = (~oe) ? buffer : 'bz;
assign data_ram = data;
assign adress_ram = adress;
assign wren_ram = wren;

assign data_ram2 = ftmdata;
assign adress_ram2 = ftmadress;
assign wren_ram2 = ftmwen;
assign mtfdata = data2;

always@*
begin
data2 = q_ram2;
	if (cs0!=1 || cs1!=0)
	begin
		buffer='bz;
		adress = adress;//13'b0000000000000;
		wren = 0;
		data = data;//16'b0000000000000000;
	end
	else
		begin
			wren = ~wr;
			if (ale==1)
				begin
				adress=ad[13:0];
				end
			else
				begin
				adress = adress;
				end
			if (oe==0)
				begin
				buffer=q_ram;
				end
			else
				begin
				buffer='bz;
				end
			if (wr==0)
			begin
				data=ad[15:0];
			end
		end
end

MPU_RAM (.clock_a(clk),.clock_b(clk2),.address_a(adress_ram),.address_b(adress_ram2), .data_a(data_ram), .data_b(data_ram2), .wren_a(wren_ram), .wren_b(wren_ram2), .q_a(q_ram), .q_b(q_ram2));


endmodule
