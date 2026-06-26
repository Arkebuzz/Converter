if {![file exists ADCHub]} {
    vlib ADCHub
    vmap ADCHub ADCHub
}

project compileall

vsim -gui ADCHub.tb_INV3PH

add wave -noupdate sim:/tb_INV3PH/CLOCK_20
add wave -noupdate sim:/tb_INV3PH/uut/CLOCK_10
add wave -noupdate sim:/tb_INV3PH/uut/CLOCK_5
add wave -noupdate -label "ADC_CLOCK" sim:/tb_INV3PH/uut/adc_clock
add wave -noupdate -label "R/nCS" sim:/tb_INV3PH/uut/ADSReader/nChipSelect
add wave -noupdate -radix unsigned -label "R/state" sim:/tb_INV3PH/uut/ADSReader/state
add wave -noupdate -radix unsigned -label "R/count" sim:/tb_INV3PH/uut/ADSReader/counter
add wave -noupdate -label "R/data" sim:/tb_INV3PH/uut/ADSReader/data_1
add wave -noupdate -label "R/CURR1" sim:/tb_INV3PH/uut/ADSReader/current_1
add wave -label "R/CURR_MAIN1" sim:/tb_INV3PH/uut/current_1

run 10000ns

wave zoom full