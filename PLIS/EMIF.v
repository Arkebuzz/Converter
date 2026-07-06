module EMIF(
   input CLOCK, 

   // Приходят с микроконтроллера:
   inout [15:0] AD,  // Шина адреса/даты
   input nCS0,       // Чипселект 1
   input nCS1,       // Чипселект 2
   input WE,         // Флаг записи (0 - контроллер пишет)
   input OE,         // Флаг чтения (0 - контроллер читает)
   input ALE,        // Флаг адреса (1 - адресс на шине)
   
   // Работа внутри ПЛИС:   
   input [15:0] DATA_FROM_FPGA,   
   input [6:0] ADRS_FROM_FPGA,  
   input WRE_FROM_FPGA,           // 0->1  =>  запись по ADRS_FROM_FPGA значения DATA_FROM_FPGA
   output [15:0] DATA_FROM_MICRO  // Если WRE_FROM_FPGA = 0, то в DATA_FROM_MICRO записывается значение по адресу ADRS_FROM_FPGA
);


reg [15:0] data_from_micro;
wire [15:0] data_from_micro_wire;
assign DATA_FROM_MICRO = data_from_micro;

reg [15:0] data;
wire [15:0] data_wire;
assign data_wire = data;

reg [6:0] adress;
wire [6:0] adress_wire;
assign adress_wire = adress;

wire [15:0] q_ram;

reg [15:0] buffer;
assign AD = (~OE) ? buffer : 16'bz;

reg wren;
wire wren_wire;
assign wren_wire = wren;


always @* begin
   // Явные защелки

   data_from_micro = data_from_micro_wire;
   
   if (nCS0 != 1 || nCS1 != 0) begin
      wren = 0;
      buffer = 16'bz;
   end
   else begin
      wren = ~WE;
      
      if (ALE == 1) begin
         adress = AD[6:0];
      end
      
      if (WE == 0) begin
         data = AD;
      end
      
      if (OE == 0) begin
         buffer = q_ram;
      end else begin
         buffer = 16'bz;
      end
   end
end

MPU_RAM mpu (
   .clock_a(CLOCK),
   .clock_b(CLOCK),
   .address_a(adress_wire),
   .address_b(ADRS_FROM_FPGA), 
   .data_a(data_wire), 
   .data_b(DATA_FROM_FPGA), 
   .wren_a(wren_wire), 
   .wren_b(WRE_FROM_FPGA), 
   .q_a(q_ram), 
   .q_b(data_from_micro_wire)
);

endmodule