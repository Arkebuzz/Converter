if {![file exists ADCHub]} {
    vlib ADCHub
    vmap ADCHub ADCHub
}

project compileall

vsim -gui ADCHub.tb_INV3PH

delete wave *

add wave sim:/tb_INV3PH/CLOCK_20
add wave sim:/tb_INV3PH/uut/CLOCK_10
add wave sim:/tb_INV3PH/uut/CLOCK_5
add wave sim:/tb_INV3PH/uut/ADC_CLOCK
add wave sim:/tb_INV3PH/uut/ADC_nCHIPSELECT
add wave -radix unsigned sim:/tb_INV3PH/uut/ADC_STATE
add wave -radix unsigned sim:/tb_INV3PH/uut/ADC_COUNTER
add wave sim:/tb_INV3PH/uut/ADC1_DATA
add wave sim:/tb_INV3PH/uut/CURRENT_1

run 10000ns

wave zoom full