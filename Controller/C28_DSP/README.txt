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
    
8. Пины FPGA <-> F28:
    F28 = Master, FPGA = Slave
    EPI = External Peripheral Interface
    DA = Data+Address (both on the same wires)
    ALE = Address Latch Enable (Switches modes: address on wires -> set ALE to 1 -> data on wires)
    MWEN/MOEN = Master Write/Output Enable (commands to fpga: write = f28 -> fpga, output = fpga -> f28)
    NBLEN/NHLEN = Non-Byte Low/High Enable (if only 10 of 20 bits needed: NBLEN = DA0..DA9, NBHIGH = DA10..DA19 ???)
    NCS0/NCS1 = NChipselects (why 2 ???)

    NBLEN_EPI0S0 = 79 <-> 32
    NBHEN_EPI0S0 = 77 <-> 33
    DA0_EPI0S0   = 76 <-> 35
    DA1_EPI0S1   .. DA2_EPI0S2   = 72 .. 71 <-> 36 .. 37
    DA3_EPI0S3   .. DA8_EPI0S8   = 69 .. 64 <-> 38 .. 43
    DA9_EPI0S9   .. DA11_EPI0S11 = 60 .. 58 <-> 45 .. 47
    DA12_EPI0S12 .. DA14_EPI0S14 = 51 .. 49 <-> 48 .. 50
    DA15_EPI0S15 = 46 <-> 51
    ALE_EPI0S30  = 44 <-> 53
    MWEN_EPI0S29 = 43 <-> 56
    MOEN_EPI0S28 = 42 <-> 57
    DA16_EPI0S16 .. DA18_EPI0S18 = 31 .. 33 <-> 63 .. 61
    DA19_EPI0S19 = 39 <-> 60
    NCS0_EPI0S26 = 28 <-> 79
    NCS1_EPI0S27 = 30 <-> 80
    
    // ?? Что они делают
    CPU_INOUT4   = 80 <-> 14
    CPU_INOUT3   = 7  <-> 13
    CPU_OUT1     = 25 <-> 5
    CPU_OUT2     = 53 <-> 6
    CPU_OUT3     = 54 <-> 7
    CPU_OUT4     = 55 <-> 8
   
9. REGISTERS INFO:
    CTOMIPCSET (Set):           C28 can write here to raise the flags
    CTOMIPCFLG (Flag):          Read-only status register that reflects what's in the flag
    CTOMIPCACK (Acknowledge):   This register belongs to the M3
    So C28 can clear it's own flag - it can only set it
    The flag can be cleared only when M3 sends ACK
  