if {![file exists work]} {
    vlib work
    vmap work work
}

project compileall -incremental

if {[catch {restart -f}] == 0} {
    run 100us
} else {
    vsim -gui work.tb_system_top
    
    add wave -noupdate -divider "=== TOP LEVEL ==="
    add wave -noupdate /tb_system_top/CLOCK_50
    add wave -noupdate /tb_system_top/CLOCK_20

    add wave -noupdate -divider "=== ADCHub ==="
    add wave -noupdate /tb_system_top/u_adc_hub/CLOCK_10
    add wave -noupdate /tb_system_top/u_adc_hub/ready_to_send
    add wave -noupdate -unsigned /tb_system_top/u_adc_hub/object_to_send
    add wave -noupdate /tb_system_top/u_adc_hub/current_1
    add wave -noupdate /tb_system_top/u_adc_hub/current_2

    add wave -noupdate -divider "=== TRANSMITTER (u_tx) ==="
    add wave -noupdate /tb_system_top/u_adc_hub/Transmitter/DATA
    add wave -noupdate /tb_system_top/u_adc_hub/Transmitter/data
    add wave -noupdate /tb_system_top/u_adc_hub/Transmitter/RESET
    add wave -noupdate /tb_system_top/u_adc_hub/Transmitter/FO_OUT
    add wave -noupdate -unsigned /tb_system_top/u_adc_hub/Transmitter/state
    add wave -noupdate -unsigned /tb_system_top/u_adc_hub/Transmitter/bit_counter
    add wave -noupdate -unsigned /tb_system_top/u_adc_hub/Transmitter/pulse_counter
    add wave -noupdate /tb_system_top/u_adc_hub/Transmitter/ready2send

    add wave -noupdate -divider "=== RECEIVER (u_rx) ==="
    add wave -noupdate /tb_system_top/u_pcon/Receiver/FO_IN
    add wave -noupdate /tb_system_top/u_pcon/Receiver/prev_inp
    add wave -noupdate -unsigned /tb_system_top/u_pcon/Receiver/signal_counter
    add wave -noupdate -unsigned /tb_system_top/u_pcon/Receiver/bit_counter
    add wave -noupdate /tb_system_top/u_pcon/Receiver/data_temp
    add wave -noupdate /tb_system_top/u_pcon/Receiver/data_out
    add wave -noupdate /tb_system_top/u_pcon/Receiver/data_ready
    add wave -noupdate /tb_system_top/u_pcon/Receiver/connect_fail
    add wave -noupdate /tb_system_top/u_pcon/Receiver/RESET

    add wave -noupdate -divider "=== PCON ==="
    add wave -noupdate /tb_system_top/u_pcon/current_1
    add wave /tb_system_top/u_pcon/current_2

    run 100us
}
