# Временные ограничения для INV3PH

# Тактовые сигналы
create_clock -period 50.000 -name clock_20 [get_ports {clock_20}]

# create_generated_clock -name clock_10 -source [get_ports {clock_20}] -divide_by 2 [get_nets {clock_10}]
# create_generated_clock -name clock_5 -source [get_ports {clock_20}] -divide_by 4 [get_nets {clock_5}]

# По умолчанию: все входы приходят в течение 2 нс до такта
set_input_delay -clock clock_20 2.0 [all_inputs]

# По умолчанию: все выходы должны быть готовы через 5 нс после такта
set_output_delay -clock clock_20 5.0 [all_outputs]