# Очистка консоли
cls

# 1. Создание библиотеки для компиляции
vlib work
vmap work work

# 2. Компиляция модулей (ЗАМЕНИТЕ ПУТИ НА ВАШИ РЕАЛЬНЫЕ)
vlog "C:/Users/Arkebuzz/Documents/Converter/Code/PLIS/INV_DATA_IO.v"
vlog "C:/Users/Arkebuzz/Documents/Converter/Code/ModelSim/INV3.v"

# Тестбенч лежит в самой папке ModelSim, поэтому его путь можно не менять
vlog tb_system_top.v

# 3. Запуск симуляции
vsim -novopt work.tb_system_top

# 4. Добавление сигналов на временную диаграмму (Wave)
add wave -divider "FIBER OPTIC INTERFACE"
add wave -hex /tb_system_top/fiber_pcon_to_inv
add wave -hex /tb_system_top/fiber_inv_to_pcon

add wave -divider "INVERTER REGISTERS"
add wave -unsigned /tb_system_top/u_inverter/INP_PWMA
add wave -unsigned /tb_system_top/u_inverter/INP_PWMB
add wave -unsigned /tb_system_top/u_inverter/INP_PWMC
add wave -bin      /tb_system_top/u_inverter/INP_COMMAND
add wave -hex      /tb_system_top/u_inverter/ERROR_LATCH
add wave -unsigned /tb_system_top/u_inverter/PWM_COUNTER
add wave -bin      /tb_system_top/u_inverter/PWMSYNC

add wave -divider "IGBT GATE CONTROL"
add wave -bin /tb_system_top/ctrl_top
add wave -bin /tb_system_top/ctrl_bot


# 5. Запуск выполнения сценария
run 150 us
wave zoom full