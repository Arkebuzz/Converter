module PWM_GENERATOR (
   input CLOCK_20, 
   input CONVERTER_ON,
   input MODE_UP,
   input [12:0] PWM,
   output CTRL_TOP, 
   output CTRL_BOT
);

reg [12:0] pwm_counter = 0;
reg [12:0] pwm_local_value;

reg ctrl_top;
reg ctrl_bot;
assign CTRL_TOP = ctrl_top;
assign CTRL_BOT = ctrl_bot;


always @(posedge CLOCK_20) begin
   if (CONVERTER_ON) begin
      pwm_counter <= pwm_counter + 1;
      
      if (pwm_counter == 0) begin
         pwm_local_value <= PWM;
      end
      
      if (MODE_UP) begin
         ctrl_top <= 1;
         
         if (pwm_counter < pwm_local_value) begin
            ctrl_bot <= 1;
         end else begin
            ctrl_bot <= 0;
         end
      end
      else begin  // MODE_DOWN
         if (pwm_counter < pwm_local_value) begin
            ctrl_top <= 1;
         end else begin
            ctrl_top <= 0;
         end
      end
   end 
   else begin
      ctrl_top <= 0;
      ctrl_bot <= 0;
      pwm_counter <= 0;
   end
end

endmodule