if {![file exists work]} {
    vlib work
    vmap work work
}

project compileall

vsim -gui work.tb_system_top

add wave -noupdate -divider "=== TOP LEVEL ==="
add wave -noupdate /tb_system_top/CLOCK_10

add wave -noupdate -divider "=== TRANSMITTER (u_tx) ==="
add wave -noupdate /tb_system_top/u_tx/DATA
add wave -noupdate /tb_system_top/u_tx/DATA_READY
add wave -noupdate /tb_system_top/u_tx/FO_OUT
add wave -noupdate -unsigned /tb_system_top/u_tx/state
add wave -noupdate -unsigned /tb_system_top/u_tx/bit_counter
add wave -noupdate -unsigned /tb_system_top/u_tx/pulse_counter
add wave -noupdate /tb_system_top/u_tx/ready2send

add wave -noupdate -divider "=== RECEIVER (u_rx) ==="
add wave -noupdate /tb_system_top/u_rx/FO_IN
add wave -noupdate /tb_system_top/u_rx/prev_inp
add wave -noupdate -unsigned /tb_system_top/u_rx/pos_counter
add wave -noupdate -unsigned /tb_system_top/u_rx/neg_counter
add wave -noupdate /tb_system_top/u_rx/data_temp
add wave -noupdate /tb_system_top/u_rx/data_out
add wave -noupdate /tb_system_top/u_rx/data_ready
add wave -noupdate /tb_system_top/u_rx/error

add wave -noupdate -divider "=== OUTPUTS ==="
add wave -noupdate /tb_system_top/DATA_RX
add wave -noupdate /tb_system_top/DATA_READY_RX
add wave /tb_system_top/ERROR_RX
