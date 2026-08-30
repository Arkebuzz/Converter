# Временные ограничения для INV3PH

# Тактовые сигналы
create_clock -name CLOCK_50 -period 20.000 [get_ports CLOCK_50]
create_clock -name CLOCK_12 -period 83.333 [get_ports CLOCK_12]

# По умолчанию: все входы приходят в течение 2 нс до такта
set_input_delay -clock CLOCK_50 2.0 [all_inputs]

# По умолчанию: все выходы должны быть готовы через 2 нс после такта
set_output_delay -clock CLOCK_50 2.0 [all_outputs]

# Чего-то ему ещё надо
derive_clock_uncertainty