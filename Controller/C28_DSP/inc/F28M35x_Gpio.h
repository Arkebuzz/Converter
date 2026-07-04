//###########################################################################
// FILE:   F28M35x_Gpio.h
// TITLE:  F28M35x Device GPIO Register Definitions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_GPIO_H
#define F28M35x_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// GPIO Individual Register Bit Definitions:

struct GPACTRL_BITS {                // bits description
        Uint16 QUALPRD0 : 8;         // 7:0    GPIO0 to 7 Qual Cycles
        Uint16 QUALPRD1 : 8;         // 15:8   GPIO8 to 15 Qual Cycles
        Uint16 QUALPRD2 : 8;         // 23:16  GPIO16 to 23 Qual Cycles
        Uint16 QUALPRD3 : 8;         // 31:24  GPIO24 to 31 Qual Cycles
};

union GPACTRL_REG {
        Uint32 all;
        struct GPACTRL_BITS bit;
};

struct GPBCTRL_BITS {                // bits description
        Uint16 QUALPRD0 : 8;         // 7:0    GPIO32 to 39 Qual Cycles
        Uint16 QUALPRD1 : 8;         // 15:8   GPIO40 to 47 Qual Cycles
        Uint16 QUALPRD2 : 8;         // 23:16  GPIO48 to 55 Qual Cycles
        Uint16 QUALPRD3 : 8;         // 31:24  GPIO56 to 63 Qual Cycles
};

union GPBCTRL_REG {
        Uint32 all;
        struct GPBCTRL_BITS bit;
};

struct GPCCTRL_BITS {                // bits description
        Uint16 QUALPRD0 : 8;         // 7:0   GPIO68 to 71 Qual Cycles
        Uint16 rsvd1 : 8;            // 15:8  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union GPCCTRL_REG {
        Uint32 all;
        struct GPCCTRL_BITS bit;
};

struct GPECTRL_BITS {                // bits description
        Uint16 QUALPRD0 : 8;         // 7:0   GPIO128-135 Qual Cycles
        Uint16 rsvd1 : 8;            // 15:8  Reserved
        Uint16 rsvd2 : 16;           // 31:16 Reserved
};

union GPECTRL_REG {
        Uint32 all;
        struct GPECTRL_BITS bit;
};

struct GPA1_BITS {                 // bits description
        Uint16 GPIO0 : 2;          // 1:0    GPIO0
        Uint16 GPIO1 : 2;          // 3:2    GPIO1
        Uint16 GPIO2 : 2;          // 5:4    GPIO2
        Uint16 GPIO3 : 2;          // 7:6    GPIO3
        Uint16 GPIO4 : 2;          // 9:8    GPIO4
        Uint16 GPIO5 : 2;          // 11:10  GPIO5
        Uint16 GPIO6 : 2;          // 13:12  GPIO6
        Uint16 GPIO7 : 2;          // 15:14  GPIO7
        Uint16 GPIO8 : 2;          // 17:16  GPIO8
        Uint16 GPIO9 : 2;          // 19:18  GPIO9
        Uint16 GPIO10 : 2;         // 21:20  GPIO10
        Uint16 GPIO11 : 2;         // 23:22  GPIO11
        Uint16 GPIO12 : 2;         // 25:24  GPIO12
        Uint16 GPIO13 : 2;         // 27:26  GPIO13
        Uint16 GPIO14 : 2;         // 29:28  GPIO14
        Uint16 GPIO15 : 2;         // 31:30  GPIO15
};

union GPA1_REG {
        Uint32 all;
        struct GPA1_BITS bit;
};

struct GPA2_BITS {                 // bits description
        Uint16 GPIO16 : 2;         // 1:0    GPIO16
        Uint16 GPIO17 : 2;         // 3:2    GPIO17
        Uint16 GPIO18 : 2;         // 5:4    GPIO18
        Uint16 GPIO19 : 2;         // 7:6    GPIO19
        Uint16 GPIO20 : 2;         // 9:8    GPIO20
        Uint16 GPIO21 : 2;         // 11:10  GPIO21
        Uint16 GPIO22 : 2;         // 13:12  GPIO22
        Uint16 GPIO23 : 2;         // 15:14  GPIO23
        Uint16 GPIO24 : 2;         // 17:16  GPIO24
        Uint16 GPIO25 : 2;         // 19:18  GPIO25
        Uint16 GPIO26 : 2;         // 21:20  GPIO26
        Uint16 GPIO27 : 2;         // 23:22  GPIO27
        Uint16 GPIO28 : 2;         // 25:24  GPIO28
        Uint16 GPIO29 : 2;         // 27:26  GPIO29
        Uint16 GPIO30 : 2;         // 29:28  GPIO30
        Uint16 GPIO31 : 2;         // 31:30  GPIO31
};

union GPA2_REG {
        Uint32 all;
        struct GPA2_BITS bit;
};

struct GPB1_BITS {                 // bits description
        Uint16 GPIO32 : 2;         // 1:0    GPIO32
        Uint16 GPIO33 : 2;         // 3:2    GPIO33
        Uint16 GPIO34 : 2;         // 5:4    GPIO34
        Uint16 GPIO35 : 2;         // 7:6    GPIO35
        Uint16 GPIO36 : 2;         // 9:8    GPIO36
        Uint16 GPIO37 : 2;         // 11:10  GPIO37
        Uint16 GPIO38 : 2;         // 13:12  GPIO38
        Uint16 GPIO39 : 2;         // 15:14  GPIO39
        Uint16 GPIO40 : 2;         // 17:16  GPIO40
        Uint16 GPIO41 : 2;         // 19:18  GPIO41
        Uint16 GPIO42 : 2;         // 21:20  GPIO42
        Uint16 GPIO43 : 2;         // 23:22  GPIO43
        Uint16 GPIO44 : 2;         // 25:24  GPIO44
        Uint16 GPIO45 : 2;         // 27:26  GPIO45
        Uint16 GPIO46 : 2;         // 29:28  GPIO46
        Uint16 GPIO47 : 2;         // 31:30  GPIO47
};

union GPB1_REG {
        Uint32 all;
        struct GPB1_BITS bit;
};

struct GPB2_BITS {                 // bits description
        Uint16 GPIO48 : 2;         // 1:0    GPIO48
        Uint16 GPIO49 : 2;         // 3:2    GPIO49
        Uint16 GPIO50 : 2;         // 5:4    GPIO50
        Uint16 GPIO51 : 2;         // 7:6    GPIO51
        Uint16 GPIO52 : 2;         // 9:8    GPIO52
        Uint16 GPIO53 : 2;         // 11:10  GPIO53
        Uint16 GPIO54 : 2;         // 13:12  GPIO54
        Uint16 GPIO55 : 2;         // 15:14  GPIO55
        Uint16 GPIO56 : 2;         // 17:16  GPIO56
        Uint16 GPIO57 : 2;         // 19:18  GPIO57
        Uint16 GPIO58 : 2;         // 21:20  GPIO58
        Uint16 GPIO59 : 2;         // 23:22  GPIO59
        Uint16 GPIO60 : 2;         // 25:24  GPIO60
        Uint16 GPIO61 : 2;         // 27:26  GPIO61
        Uint16 GPIO62 : 2;         // 29:28  GPIO62
        Uint16 GPIO63 : 2;         // 31:30  GPIO63
};

union GPB2_REG {
        Uint32 all;
        struct GPB2_BITS bit;
};

struct GPC1_BITS {                 // bits description
        Uint16 rsvd1 : 8;          // 7:0    Reserved
        Uint16 GPIO68 : 2;         // 9:8    GPIO68
        Uint16 GPIO69 : 2;         // 11:10  GPIO69
        Uint16 GPIO70 : 2;         // 13:12  GPIO70
        Uint16 GPIO71 : 2;         // 15:14  GPIO71
        Uint16 rsvd2 : 16;         // 31:16  Reserved
};

union GPC1_REG {
        Uint32 all;
        struct GPC1_BITS bit;
};

struct GPE1_BITS {                  // bits description
        Uint16 GPIO128 : 2;         // 1:0    GPIO128
        Uint16 GPIO129 : 2;         // 3:2    GPIO129
        Uint16 GPIO130 : 2;         // 5:4    GPIO130
        Uint16 GPIO131 : 2;         // 7:6    GPIO131
        Uint16 GPIO132 : 2;         // 9:8    GPIO132
        Uint16 GPIO133 : 2;         // 11:10  GPIO133
        Uint16 GPIO134 : 2;         // 13:12  GPIO134
        Uint16 GPIO135 : 2;         // 15:14  GPIO135
        Uint16 rsvd1 : 16;          // 31:16  Reserved
};

union GPE1_REG {
        Uint32 all;
        struct GPE1_BITS bit;
};

struct GPADAT_BITS {               // bits description
        Uint16 GPIO0 : 1;          // 0    GPIO0
        Uint16 GPIO1 : 1;          // 1    GPIO1
        Uint16 GPIO2 : 1;          // 2    GPIO2
        Uint16 GPIO3 : 1;          // 3    GPIO3
        Uint16 GPIO4 : 1;          // 4    GPIO4
        Uint16 GPIO5 : 1;          // 5    GPIO5
        Uint16 GPIO6 : 1;          // 6    GPIO6
        Uint16 GPIO7 : 1;          // 7    GPIO7
        Uint16 GPIO8 : 1;          // 8    GPIO8
        Uint16 GPIO9 : 1;          // 9    GPIO9
        Uint16 GPIO10 : 1;         // 10   GPIO10
        Uint16 GPIO11 : 1;         // 11   GPIO11
        Uint16 GPIO12 : 1;         // 12   GPIO12
        Uint16 GPIO13 : 1;         // 13   GPIO13
        Uint16 GPIO14 : 1;         // 14   GPIO14
        Uint16 GPIO15 : 1;         // 15   GPIO15
        Uint16 GPIO16 : 1;         // 16   GPIO16
        Uint16 GPIO17 : 1;         // 17   GPIO17
        Uint16 GPIO18 : 1;         // 18   GPIO18
        Uint16 GPIO19 : 1;         // 19   GPIO19
        Uint16 GPIO20 : 1;         // 20   GPIO20
        Uint16 GPIO21 : 1;         // 21   GPIO21
        Uint16 GPIO22 : 1;         // 22   GPIO22
        Uint16 GPIO23 : 1;         // 23   GPIO23
        Uint16 GPIO24 : 1;         // 24   GPIO24
        Uint16 GPIO25 : 1;         // 25   GPIO25
        Uint16 GPIO26 : 1;         // 26   GPIO26
        Uint16 GPIO27 : 1;         // 27   GPIO27
        Uint16 GPIO28 : 1;         // 28   GPIO28
        Uint16 GPIO29 : 1;         // 29   GPIO29
        Uint16 GPIO30 : 1;         // 30   GPIO30
        Uint16 GPIO31 : 1;         // 31   GPIO31
};

union GPADAT_REG {
        Uint32 all;
        struct GPADAT_BITS bit;
};

struct GPBDAT_BITS {          // bits  description
        Uint16 GPIO32 : 1;    // 0    GPIO32
        Uint16 GPIO33 : 1;    // 1    GPIO33
        Uint16 GPIO34 : 1;    // 2    GPIO34
        Uint16 GPIO35 : 1;    // 3    GPIO35
        Uint16 GPIO36 : 1;    // 4    GPIO36
        Uint16 GPIO37 : 1;    // 5    GPIO37
        Uint16 GPIO38 : 1;    // 6    GPIO38
        Uint16 GPIO39 : 1;    // 7    GPIO39
        Uint16 GPIO40 : 1;    // 8    GPIO40
        Uint16 GPIO41 : 1;    // 9    GPIO41
        Uint16 GPIO42 : 1;    // 10   GPIO42
        Uint16 GPIO43 : 1;    // 11   GPIO43
        Uint16 GPIO44 : 1;    // 12   GPIO44
        Uint16 GPIO45 : 1;    // 13   GPIO45
        Uint16 GPIO46 : 1;    // 14   GPIO46
        Uint16 GPIO47 : 1;    // 15   GPIO47
        Uint16 GPIO48 : 1;    // 16   GPIO48
        Uint16 GPIO49 : 1;    // 17   GPIO49
        Uint16 GPIO50 : 1;    // 18   GPIO50
        Uint16 GPIO51 : 1;    // 19   GPIO51
        Uint16 GPIO52 : 1;    // 20   GPIO52
        Uint16 GPIO53 : 1;    // 21   GPIO53
        Uint16 GPIO54 : 1;    // 22   GPIO54
        Uint16 GPIO55 : 1;    // 23   GPIO55
        Uint16 GPIO56 : 1;    // 24   GPIO56
        Uint16 GPIO57 : 1;    // 25   GPIO57
        Uint16 GPIO58 : 1;    // 26   GPIO58
        Uint16 GPIO59 : 1;    // 27   GPIO59
        Uint16 GPIO60 : 1;    // 28   GPIO60
        Uint16 GPIO61 : 1;    // 29   GPIO61
        Uint16 GPIO62 : 1;    // 30   GPIO62
        Uint16 GPIO63 : 1;    // 31   GPIO63
};

union GPBDAT_REG {
        Uint32 all;
        struct GPBDAT_BITS bit;
};

struct GPCDAT_BITS {               // bits description
        Uint16 rsvd1 : 4;          // 3:0   Reserved
        Uint16 GPIO68 : 1;         // 4     GPIO68
        Uint16 GPIO69 : 1;         // 5     GPIO69
        Uint16 GPIO70 : 1;         // 6     GPIO70
        Uint16 GPIO71 : 1;         // 7     GPIO71
        Uint16 rsvd2 : 8;          // 15:8  Reserved
        Uint16 rsvd3 : 16;         // 31:16 Reserved
};

union GPCDAT_REG {
        Uint32 all;
        struct GPCDAT_BITS bit;
};

struct GPEDAT_BITS {                // bits description
        Uint16 GPIO128 : 1;         // 0     GPIO128
        Uint16 GPIO129 : 1;         // 1     GPIO129
        Uint16 GPIO130 : 1;         // 2     GPIO130
        Uint16 GPIO131 : 1;         // 3     GPIO131
        Uint16 GPIO132 : 1;         // 4     GPIO132
        Uint16 GPIO133 : 1;         // 5     GPIO133
        Uint16 GPIO134 : 1;         // 6     GPIO134
        Uint16 GPIO135 : 1;         // 7     GPIO135
        Uint16 rsvd1 : 8;           // 15:8  Reserved
        Uint16 rsvd2 : 16;          // 31:16 Reserved
};

union GPEDAT_REG {
        Uint32 all;
        struct GPEDAT_BITS bit;
};

struct GPTRIP1SEL_BITS {               // bits description
        Uint16 GPTRIP1SEL : 6;         // 5:0    GPTRIP1 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP1SEL_REG {
        Uint16 all;
        struct GPTRIP1SEL_BITS bit;
};

struct GPTRIP2SEL_BITS {               // bits description
        Uint16 GPTRIP2SEL : 6;         // 5:0    GPTRIP2 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP2SEL_REG {
        Uint16 all;
        struct GPTRIP2SEL_BITS bit;
};

struct GPTRIP3SEL_BITS {               // bits description
        Uint16 GPTRIP3SEL : 6;         // 5:0    GPTRIP3 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP3SEL_REG {
        Uint16 all;
        struct GPTRIP3SEL_BITS bit;
};

struct GPTRIP4SEL_BITS {               // bits description
        Uint16 GPTRIP4SEL : 6;         // 5:0    GPTRIP4 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP4SEL_REG {
        Uint16 all;
        struct GPTRIP4SEL_BITS bit;
};

struct GPTRIP5SEL_BITS {               // bits description
        Uint16 GPTRIP5SEL : 6;         // 5:0    GPTRIP5 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP5SEL_REG {
        Uint16 all;
        struct GPTRIP5SEL_BITS bit;
};

struct GPTRIP6SEL_BITS {               // bits description
        Uint16 GPTRIP6SEL : 6;         // 5:0    GPTRIP6 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP6SEL_REG {
        Uint16 all;
        struct GPTRIP6SEL_BITS bit;
};

struct GPTRIP7SEL_BITS {               // bits description
        Uint16 GPTRIP7SEL : 6;         // 5:0    GPTRIP7 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP7SEL_REG {
        Uint16 all;
        struct GPTRIP7SEL_BITS bit;
};

struct GPTRIP8SEL_BITS {               // bits description
        Uint16 GPTRIP8SEL : 6;         // 5:0    GPTRIP8 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP8SEL_REG {
        Uint16 all;
        struct GPTRIP8SEL_BITS bit;
};

struct GPTRIP9SEL_BITS {               // bits description
        Uint16 GPTRIP9SEL : 6;         // 5:0    GPTRIP9 GPIO Select
        Uint16 rsvd1 : 10;             // 15:6   Reserved
};

union GPTRIP9SEL_REG {
        Uint16 all;
        struct GPTRIP9SEL_BITS bit;
};

struct GPTRIP10SEL_BITS {               // bits description
        Uint16 GPTRIP10SEL : 6;         // 5:0    GPTRIP10 GPIO Select
        Uint16 rsvd1 : 10;              // 15:6   Reserved
};

union GPTRIP10SEL_REG {
        Uint16 all;
        struct GPTRIP10SEL_BITS bit;
};

struct GPTRIP11SEL_BITS {               // bits description
        Uint16 GPTRIP11SEL : 6;         // 5:0    GPTRIP11 GPIO Select
        Uint16 rsvd1 : 10;              // 15:6   Reserved
};

union GPTRIP11SEL_REG {
        Uint16 all;
        struct GPTRIP11SEL_BITS bit;
};

struct GPTRIP12SEL_BITS {               // bits description
        Uint16 GPTRIP12SEL : 6;         // 5:0    GPTRIP12 GPIO Select
        Uint16 rsvd1 : 10;              // 15:6   Reserved
};

union GPTRIP12SEL_REG {
        Uint16 all;
        struct GPTRIP12SEL_BITS bit;
};

struct AIODAT_BITS {                 // bits description
        Uint16 rsvd1 : 2;            // 1:0   Reserved
        Uint16 AIO2 : 1;             // 2     AIO2 Data
        Uint16 rsvd2 : 1;            // 3     Reserved
        Uint16 AIO4 : 1;             // 4     AIO4 Data
        Uint16 rsvd3 : 1;            // 5     Reserved
        Uint16 AIO6 : 1;             // 6     AIO6 Data
        Uint16 rsvd4 : 3;            // 9:7   Reserved
        Uint16 AIO10 : 1;            // 10    AIO10 Data
        Uint16 rsvd5 : 1;            // 11    Reserved
        Uint16 AIO12 : 1;            // 12    AIO12 Data
        Uint16 rsvd6 : 1;            // 13    Reserved
        Uint16 AIO14 : 1;            // 14    AIO14 Data
        Uint16 rsvd7 : 1;            // 15    Reserved
        Uint16 rsvd8 : 2;            // 17:16 Reserved
        Uint16 AIO18 : 1;            // 18    AIO18 Data
        Uint16 rsvd9 : 1;            // 19    Reserved
        Uint16 AIO20 : 1;            // 20    AIO20 Data
        Uint16 rsvd10 : 1;           // 21    Reserved
        Uint16 AIO22 : 1;            // 22    AIO22 Data
        Uint16 rsvd11 : 3;           // 25:23 Reserved
        Uint16 AIO26 : 1;            // 26    AIO26 Data
        Uint16 rsvd12 : 1;           // 27    Reserved
        Uint16 AIO28 : 1;            // 28    AIO28 Data
        Uint16 rsvd13 : 1;           // 29    Reserved
        Uint16 AIO30 : 1;            // 30    AIO30 Data
        Uint16 rsvd14 : 1;           // 31    Reserved
};

union AIODAT_REG {
        Uint32 all;
        struct AIODAT_BITS bit;
};

struct GPIOXINT_BITS {              // bits description
        Uint16 GPIOSEL : 3;         // 2:0    Select GPIO Interrupt Input Source
        Uint16 rsvd1 : 13;          // 15:3   Reserved
};

union GPIOXINT_REG {
        Uint16 all;
        struct GPIOXINT_BITS bit;
};

struct AIO1_BITS {                   // bits description
        Uint16 rsvd1 : 2;            // 1:0    Reserved
        Uint16 rsvd2 : 2;            // 3:2    Reserved
        Uint16 AIO2 : 2;             // 5:4    AIO2
        Uint16 rsvd3 : 2;            // 7:6    Reserved
        Uint16 AIO4 : 2;             // 9:8    AIO4
        Uint16 rsvd4 : 2;            // 11:10  Reserved
        Uint16 AIO6 : 2;             // 13:12  AIO6
        Uint16 rsvd5 : 2;            // 15:14  Reserved
        Uint16 rsvd6 : 2;            // 17:16  Reserved
        Uint16 rsvd7 : 2;            // 19:18  Reserved
        Uint16 AIO10 : 2;            // 21:20  AIO10
        Uint16 rsvd8 : 2;            // 23:22  Reserved
        Uint16 AIO12 : 2;            // 25:24  AIO12
        Uint16 rsvd9 : 2;            // 27:26  Reserved
        Uint16 AIO14 : 2;            // 29:28  AIO14
        Uint16 rsvd10 : 2;           // 31:30  Reserved
};

union AIO1_REG {
        Uint32 all;
        struct AIO1_BITS bit;
};

struct AIO2_BITS {                 // bits description
        Uint16 rsvd1 : 2;          // 1:0    Reserved
        Uint16 rsvd2 : 2;          // 3:2    Reserved
        Uint16 AIO18 : 2;          // 5:4    AIO18
        Uint16 rsvd3 : 2;          // 7:6    Reserved
        Uint16 AIO20 : 2;          // 9:8    AIO20
        Uint16 rsvd4 : 2;          // 11:10  Reserved
        Uint16 AIO22 : 2;          // 13:12  AIO22
        Uint16 rsvd5 : 2;          // 15:14  Reserved
        Uint16 rsvd6 : 2;          // 17:16  Reserved
        Uint16 rsvd7 : 2;          // 19:18  Reserved
        Uint16 AIO26 : 2;          // 21:20  AIO26
        Uint16 rsvd8 : 2;          // 23:22  Reserved
        Uint16 AIO28 : 2;          // 25:24  AIO28
        Uint16 rsvd9 : 2;          // 27:26  Reserved
        Uint16 AIO30 : 2;          // 29:28  AIO30
        Uint16 rsvd10 : 2;         // 31:30  Reserved
};

union AIO2_REG {
        Uint32 all;
        struct AIO2_BITS bit;
};

struct GPIOG1_CTRL_REGS {
        union    GPACTRL_REG GPACTRL;         // GPIO A Control Register (GPIO0
                                              // to 31)
        union    GPA1_REG GPAQSEL1;           // GPIO A Qualifier Select 1
                                              // Register (GPIO0 to 15)
        union    GPA2_REG GPAQSEL2;           // GPIO A Qualifier Select 2
                                              // Register (GPIO16 to 31)
        union    GPA1_REG GPAMUX1;            // GPIO A Mux 1 Register (GPIO0 to
                                              // 15)
        union    GPA2_REG GPAMUX2;            // GPIO A Mux 2 Register (GPIO16
                                              // to 31)
        union    GPADAT_REG GPADIR;           // GPIO A Direction Register
                                              // (GPIO0 to 31)
        Uint16 rsvd1[2];                      // Reserved
        Uint16 rsvd2[2];                      // Reserved
        union    GPBCTRL_REG GPBCTRL;         // GPIO B Control Register (GPIO32
                                              // to 63)
        union    GPB1_REG GPBQSEL1;           // GPIO B Qualifier Select 1
                                              // Register (GPIO32 to 47)
        union    GPB2_REG GPBQSEL2;           // GPIO B Qualifier Select 2
                                              // Register (GPIO48 to 63)
        union    GPB1_REG GPBMUX1;            // GPIO B Mux 1 Register (GPIO32
                                              // to 47)
        union    GPB2_REG GPBMUX2;            // GPIO B Mux 2 Register (GPIO48
                                              // to 63)
        union    GPBDAT_REG GPBDIR;           // GPIO B Direction Register
                                              // (GPIO32 to 63)
        Uint16 rsvd3[2];                      // Reserved
        Uint16 rsvd4[2];                      // Reserved
        union    GPCCTRL_REG GPCCTRL;         // GPIO C Control Register (GPIO68
                                              // to 71)
        union    GPC1_REG GPCQSEL1;           // GPIO C Qualifier Select 1
                                              // Register (GPIO68 to 71)
        Uint16 rsvd5[2];                      // Reserved
        union    GPC1_REG GPCMUX1;            // GPIO C Mux 1 Register (GPIO68
                                              // to 71)
        Uint16 rsvd6[2];                      // Reserved
        union    GPCDAT_REG GPCDIR;           // GPIO C Direction Register
                                              // (GPIO68 to 71)
        Uint16 rsvd7[2];                      // Reserved
        Uint16 rsvd8[2];                      // Reserved
        Uint16 rsvd9[16];                     // Reserved
};

struct GPIOG1_DATA_REGS {
        union    GPADAT_REG GPADAT;           // GPIO A Data Register (GPIO0 to
                                              // 31)
        union    GPADAT_REG GPASET;           // GPIO A Data Set Register (GPIO0
                                              // to 31)
        union    GPADAT_REG GPACLEAR;         // GPIO A Data Clear Register
                                              // (GPIO0 to 31)
        union    GPADAT_REG GPATOGGLE;        // GPIO A Data Toggle Register
                                              // (GPIO0 to 31)
        union    GPBDAT_REG GPBDAT;           // GPIO B Data Register (GPIO32 to
                                              // 63)
        union    GPBDAT_REG GPBSET;           // GPIO B Data Set Register
                                              // (GPIO32 to 63)
        union    GPBDAT_REG GPBCLEAR;         // GPIO B Data Clear Register
                                              // (GPIO32 to 63)
        union    GPBDAT_REG GPBTOGGLE;        // GPIO B Data Toggle Register
                                              // (GPIO32 to 63)
        union    GPCDAT_REG GPCDAT;           // GPIO C Data Register (GPIO68 to
                                              // 71)
        union    GPCDAT_REG GPCSET;           // GPIO C Data Set Register
                                              // (GPIO68 to 71)
        union    GPCDAT_REG GPCCLEAR;         // GPIO C Data Clear Register
                                              // (GPIO68 to 71)
        union    GPCDAT_REG GPCTOGGLE;        // GPIO C Data Toggle Register
                                              // (GPIO68 to 71)
        Uint16 rsvd1[8];                      // Reserved
};

struct GPIOG1_TRIP_REGS {
        union    GPTRIP1SEL_REG GPTRIP1SEL;       // GPTRIP1 (TZ1n) Input Select
                                                  // Register (GPIO0 to 63)
        union    GPTRIP2SEL_REG GPTRIP2SEL;       // GPTRIP2 (TZ2n) Input Select
                                                  // Register (GPIO0 to 63)
        union    GPTRIP3SEL_REG GPTRIP3SEL;       // GPTRIP3 (TZ3n) Input Select
                                                  // Register (GPIO0 to 63)
        union    GPTRIP4SEL_REG GPTRIP4SEL;       // GPTRIP4 (XINT1) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP5SEL_REG GPTRIP5SEL;       // GPTRIP5 (XINT2) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP6SEL_REG GPTRIP6SEL;       // GPTRIP6 (XINT3 & SYNCIN)
                                                  // Input Select Register
                                                  // (GPIO0 to 63)
        union    GPTRIP7SEL_REG GPTRIP7SEL;       // GPTRIP7 (ECAP1) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP8SEL_REG GPTRIP8SEL;       // GPTRIP8 (ECAP2) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPADAT_REG GPIOLPMSEL1;          // C28 LPM GPIO Select 1
                                                  // Register (GPIO0 to 31)
        union    GPBDAT_REG GPIOLPMSEL2;          // C28 LPM GPIO Select 2
                                                  // Register (GPIO32 to 63)
        Uint16 rsvd1[4];                          // Reserved
        union    GPTRIP9SEL_REG GPTRIP9SEL;       // GPTRIP9 (ECAP3) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP10SEL_REG GPTRIP10SEL;     // GPTRIP10 (ECAP4) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP11SEL_REG GPTRIP11SEL;     // GPTRIP11 (ECAP5) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        union    GPTRIP12SEL_REG GPTRIP12SEL;     // GPTRIP12 (ECAP6) Input
                                                  // Select Register (GPIO0 to
                                                  // 63)
        Uint16 rsvd2[12];                         // Reserved
};

struct GPIOG2_CTRL_REGS {
        union    GPECTRL_REG GPECTRL;         // GPIO E Control Register
                                              // (GPIO128 to 135)
        union    GPE1_REG GPEQSEL1;           // GPIO E Qualifier Select 1
                                              // Register (GPIO128 to 135)
        Uint16 rsvd1[2];                      // Reserved
        union    GPE1_REG GPEMUX1;            // GPIO E Mux 1 Register (GPIO128
                                              // to 135)
        Uint16 rsvd2[2];                      // Reserved
        union    GPEDAT_REG GPEDIR;           // GPIO E Direction Register
                                              // (GPIO128 to 135 )
        union    GPEDAT_REG GPEPUD;           // GPEPUD register for (GPIO128 to
                                              // 135)
        Uint16 rsvd3[40];                     // Reserved
        union    AIO1_REG AIOMUX1;            // Analog IO Mux 1 Register
                                              // (AIO0-15)
        union    AIO2_REG AIOMUX2;            // Analog IO Mux 2 Register
                                              // (AIO16-31)
        union    AIODAT_REG AIODIR;           // Analog IO Direction Register
                                              // (AIO0-31)
};

struct GPIOG2_DATA_REGS {
        union    GPEDAT_REG GPEDAT;           // GPIO E Data Register (GPIO128
                                              // to 135)
        union    GPEDAT_REG GPESET;           // GPIO E Data Set Register
                                              // (GPIO128 to 135)
        union    GPEDAT_REG GPECLEAR;         // GPIO E Data Clear Register
                                              // (GPIO128 to 135)
        union    GPEDAT_REG GPETOGGLE;        // GPIO E Data Toggle Register
                                              // (GPIO128 to 135)
        Uint16 rsvd1[16];                     // Reserved
        union    AIODAT_REG AIODAT;           // Analog IO Data Register (AIO0
                                              // to 31)
        union    AIODAT_REG AIOSET;           // Analog IO Data Set Register
                                              // (AIO0 to 31)
        union    AIODAT_REG AIOCLEAR;         // Analog IO Data Clear Register
                                              // (AIO0 to 31)
        union    AIODAT_REG AIOTOGGLE;        // Analog IO Data Toggle Register
                                              // (AIO0 to 31)
};

//---------------------------------------------------------------------------
// Gpio External References & Function Declarations:
extern volatile struct GPIOG1_CTRL_REGS GpioCtrlRegs;
extern volatile struct GPIOG1_DATA_REGS GpioDataRegs;
extern volatile struct GPIOG1_TRIP_REGS GpioTripRegs;
extern volatile struct GPIOG1_CTRL_REGS GpioG1CtrlRegs;
extern volatile struct GPIOG1_DATA_REGS GpioG1DataRegs;
extern volatile struct GPIOG1_TRIP_REGS GpioG1TripRegs;
extern volatile struct GPIOG2_CTRL_REGS GpioG2CtrlRegs;
extern volatile struct GPIOG2_DATA_REGS GpioG2DataRegs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of F28M35x_GPIO_H definition


