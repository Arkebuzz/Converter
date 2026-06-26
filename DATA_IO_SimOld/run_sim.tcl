if {![file exists work]} {
    vlib work
    vmap work work
}

project compileall

vsim -gui work.tb_system_top

add wave -noupdate  -divider "FIBER OPTIC INTERFACE"
add wave -noupdate  /tb_system_top/fiber_pcon_to_inv
add wave -noupdate  /tb_system_top/fiber_inv_to_pcon

add wave -noupdate -divider "INVERTER REGISTERS"
add wave -noupdate -unsigned /tb_system_top/u_inverter/INP_PWMA
add wave -noupdate -unsigned /tb_system_top/u_inverter/INP_PWMB
add wave -noupdate -unsigned /tb_system_top/u_inverter/INP_PWMC
add wave -noupdate -bin      /tb_system_top/u_inverter/INP_COMMAND
add wave -noupdate           /tb_system_top/u_inverter/ERROR_LATCH
add wave -noupdate -unsigned /tb_system_top/u_inverter/PWM_COUNTER
add wave -noupdate -bin      /tb_system_top/u_inverter/PWMSYNC

add wave -noupdate -divider "IGBT GATE CONTROL"
add wave -noupdate -bin /tb_system_top/ctrl_top
add wave -bin /tb_system_top/ctrl_bot


# 5. Запуск выполнения сценария
run 150 us
wave zoom full