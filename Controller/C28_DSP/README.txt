1. Определиться будет ли код считываться с флеш или с оп:
    * ОП: быстро, но нужно каждый раз копировать код из ccs потому что после вкл/выкл ОП обнуляется
    * Флеш: флеш память ядра, остается при перезагрузке. Медленная поэтому надо будет копировать код из нее в ОП если этот код должен быть быстрым
Сейчас стоит Флеш.

Чтобы функция которая должна быть быстрой (напр вектор прерывания) была в ОП
Можно сказать линкеру чтобы ее потом туда засунуло:
/*
#pragma CODE_SECTION(my_critical_interrupt_handler, "ramfuncs")

interrupt void my_critical_interrupt_handler(void) {}
*/

2. Общая память:
    TODO: не забыть синхронизовать общую память в арм ядре
 
3. У C28 есть фича, что регистры для конфигурации по умолчанию заблокированы.
Чтобы ими пользоваться нужно сначала делать `EALLOW;` а потом `EDIS;`

4. Пины сгруппированы в группы и порты:
    GpioG1CtrlRegs содержит Port A and Port B.
    GpioG2CtrlRegs содержит Port C, Port D, Port E, и тд

    Port A: пины GPIO0 до GPIO31.
    ...
    Port E: пины GPIO128 до GPIO159.
 
    Direction = Input (0) / Output (1)
5. Арм стартует первым, запускает ДСП, ДСП встает в режим ожидания,
Арм посылает прерывание (MTOCIPCINT1 = Master To Control IPC Interrupt 1) на ДСП и запускает его,
Арм говорит ДСП откуда брать код: с Флеш или ОП.

6. Изначально все пины принадлежат к Арм ядру кроме GPIO128-GPIO135 - их может трогать только ДСП
Чтобы ДСП получил доступ к другим пинам Арм должен дать ему доступ через GPIOPinConfigureCoreSelect()

7. Пины оптоволокно <-> FPGA:
    FPGA_INPUT1                    = 83
    FPGA_INPUT2   .. FPGA_INPUT8   = 85 ..  91
    FPGA_OUTPUT1  .. FPGA_OUTPUT4  = 98  .. 101
    FPGA_OUTPUT5  .. FPGA_OUTPUT8  = 103 .. 106
    FPGA_OUTPUT9  .. FPGA_OUTPUT10 = 114 .. 115
    FPGA_INPUT9   .. FPGA_INPUT12  = 110 .. 113
    FPGA_OUTPUT11 .. FPGA_OUTPUT13 = 119 .. 121
    FPGA_OUTPUT14                  = 125
    FPGA_INPUT13  .. FPGA_INPUT16  = 126 .. 129
    FPGA_INPUT17  .. FPGA_INPUT18  = 132 .. 133
    FPGA_INPUT19  .. FPGA_INPUT20  = 135 .. 136
    FPGA_OUTPUT15                  = 137
    FPGA_OUTPUT16 .. FPGA_OUTPUT19 = 141 .. 144
    FPGA_OUTPUT20                  = 4
    
8. 
