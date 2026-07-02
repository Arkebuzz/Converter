# Временные ограничения для INV3PH

# Тактовые сигналы
create_clock -period 50.000 -name CLOCK_20 [get_ports {CLOCK_20}]

create_generated_clock -name CLOCK_10 -source [get_ports {CLOCK_20}] -divide_by 2 [get_nets {CLOCK_10}]
create_generated_clock -name CLOCK_5 -source [get_ports {CLOCK_20}] -divide_by 4 [get_nets {CLOCK_5}]

# По умолчанию: все входы приходят в течение 5 нс до такта
set_input_delay -clock CLOCK_20 5.0 [all_inputs]

# По умолчанию: все выходы должны быть готовы через 5 нс после такта
set_output_delay -clock CLOCK_20 5.0 [all_outputs]