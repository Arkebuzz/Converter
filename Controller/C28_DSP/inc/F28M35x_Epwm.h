//###########################################################################
//
// FILE:   F28M35x_Epwm.h
//
// TITLE:  F28M35x Device EPWM Register Definitions.
//
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#ifndef F28M35x_EPWM_H
#define F28M35x_EPWM_H

#ifdef __cplusplus
extern "C" {
#endif


//---------------------------------------------------------------------------
// EPWM Individual Register Bit Definitions:

struct TBCTL_BITS {                   // bits description
        Uint16 CTRMODE : 2;           // 1:0   Counter Mode
        Uint16 PHSEN : 1;             // 2     Phase Load Enable
        Uint16 PRDLD : 1;             // 3     Active Period Load
        Uint16 SYNCOSEL : 2;          // 5:4   Sync Output Select
        Uint16 SWFSYNC : 1;           // 6     Software Force Sync Pulse
        Uint16 HSPCLKDIV : 3;         // 9:7   High Speed TBCLK Pre-scaler
        Uint16 CLKDIV : 3;            // 12:10 Time Base Clock Pre-scaler
        Uint16 PHSDIR : 1;            // 13    Phase Direction Bit
        Uint16 FREE_SOFT : 2;         // 15:14 Emulation Mode Bits
};

union TBCTL_REG {
        Uint16 all;
        struct TBCTL_BITS bit;
};

struct TBSTS_BITS {                // bits description
        Uint16 CTRDIR : 1;         // 0    Counter Direction Status
        Uint16 SYNCI : 1;          // 1    External Input Sync Status
        Uint16 CTRMAX : 1;         // 2    Counter Max Latched Status
        Uint16 rsvd1 : 13;         // 15:3 Reserved
};

union TBSTS_REG {
        Uint16 all;
        struct TBSTS_BITS bit;
};

struct CMPCTL_BITS {                  // bits description
        Uint16 LOADAMODE : 2;         // 1:0   Active Compare A Load
        Uint16 LOADBMODE : 2;         // 3:2   Active Compare B Load
        Uint16 SHDWAMODE : 1;         // 4     Compare A Register Block Operating
                                      //       Mode
        Uint16 rsvd1 : 1;             // 5     Reserved
        Uint16 SHDWBMODE : 1;         // 6     Compare B Register Block Operating
                                      //       Mode
        Uint16 rsvd2 : 1;             // 7     Reserved
        Uint16 SHDWAFULL : 1;         // 8     Compare A Shadow Register Full
                                      //       Status
        Uint16 SHDWBFULL : 1;         // 9     Compare B Shadow Register Full
                                      //       Status
        Uint16 rsvd3 : 6;             // 15:10 Reserved
};

union CMPCTL_REG {
        Uint16 all;
        struct CMPCTL_BITS bit;
};

struct AQCTL_BITS {             // bits description
        Uint16 ZRO : 2;         // 1:0   Action Counter = Zero
        Uint16 PRD : 2;         // 3:2   Action Counter = Period
        Uint16 CAU : 2;         // 5:4   Action Counter = Compare A Up
        Uint16 CAD : 2;         // 7:6   Action Counter = Compare A Down
        Uint16 CBU : 2;         // 9:8   Action Counter = Compare B Up
        Uint16 CBD : 2;         // 11:10 Action Counter = Compare B Down
        Uint16 rsvd1 : 4;       // 15:12 Reserved
};

union AQCTL_REG {
        Uint16 all;
        struct AQCTL_BITS bit;
};

struct AQSFRC_BITS {               // bits description
        Uint16 ACTSFA : 2;         // 1:0  Action when One-time SW Force A
                                   //      Invoked
        Uint16 OTSFA : 1;          // 2    One-time SW Force A Output
        Uint16 ACTSFB : 2;         // 4:3  Action when One-time SW Force B
                                   //      Invoked
        Uint16 OTSFB : 1;          // 5    One-time SW Force A Output
        Uint16 RLDCSF : 2;         // 7:6  Reload from Shadow Options
        Uint16 rsvd1 : 8;          // 15:8 Reserved
};

union AQSFRC_REG {
        Uint16 all;
        struct AQSFRC_BITS bit;
};

struct AQCSFRC_BITS {            // bits description
        Uint16 CSFA : 2;         // 1:0    Continuous Software Force on output A
        Uint16 CSFB : 2;         // 3:2    Continuous Software Force on output B
        Uint16 rsvd1 : 12;       // 15:4   Reserved
};

union AQCSFRC_REG {
        Uint16 all;
        struct AQCSFRC_BITS bit;
};

struct DBCTL_BITS {                      // bits description
        Uint16 OUT_MODE : 2;             // 1:0   Dead Band Output Mode Control
        Uint16 POLSEL : 2;               // 3:2   Polarity Select Control
        Uint16 IN_MODE : 2;              // 5:4   Dead Band Input Select Mode
                                         //       Control
        Uint16 LOADREDMODE : 2;          // 7:6   DBRED Load from Shadow Select Mode
        Uint16 LOADFEDMODE : 2;          // 9:8   DBFED Load from Shadow Select Mode
        Uint16 SHDWDBREDMODE : 1;        // 10    RED Dead Band Load Mode
        Uint16 SHDWDBFEDMODE : 1;        // 11    FED Dead Band Load Mode
        Uint16 OUTSWAP : 2;              // 13:12 Dead Band Output Swap Control
        Uint16 DEDB_MODE : 1;            // 14    Dead Band Dual Edge B Mode 
                                         //       Control
        Uint16 HALFCYCLE : 1;            // 15    Half Cycle Clocking Enable
};

union DBCTL_REG {
        Uint16 all;
        struct DBCTL_BITS bit;
};

struct TZSEL_BITS {              // bits description
        Uint16 CBC1 : 1;         // 0    TZ1 CBC select
        Uint16 CBC2 : 1;         // 1    TZ2 CBC select
        Uint16 CBC3 : 1;         // 2    TZ3 CBC select
        Uint16 CBC4 : 1;         // 3    TZ4 CBC select
        Uint16 CBC5 : 1;         // 4    TZ5 CBC select
        Uint16 CBC6 : 1;         // 5    TZ6 CBC select
        Uint16 DCAEVT2 : 1;      // 6    DCAEVT2 CBC select
        Uint16 DCBEVT2 : 1;      // 7    DCBEVT2 CBC select
        Uint16 OSHT1 : 1;        // 8    One-shot TZ1 select
        Uint16 OSHT2 : 1;        // 9    One-shot TZ2 select
        Uint16 OSHT3 : 1;        // 10   One-shot TZ3 select
        Uint16 OSHT4 : 1;        // 11   One-shot TZ4 select
        Uint16 OSHT5 : 1;        // 12   One-shot TZ5 select
        Uint16 OSHT6 : 1;        // 13   One-shot TZ6 select
        Uint16 DCAEVT1 : 1;      // 14   One-shot DCAEVT1 select
        Uint16 DCBEVT1 : 1;      // 15   One-shot DCBEVT1 select
};

union TZSEL_REG {
        Uint16 all;
        struct TZSEL_BITS bit;
};

struct TZDCSEL_BITS {               // bits description
        Uint16 DCAEVT1 : 3;         // 2:0    Digital Compare Output A Event 1
        Uint16 DCAEVT2 : 3;         // 5:3    Digital Compare Output A Event 2
        Uint16 DCBEVT1 : 3;         // 8:6    Digital Compare Output B Event 1
        Uint16 DCBEVT2 : 3;         // 11:9   Digital Compare Output B Event 2
        Uint16 rsvd1 : 4;           // 15:12  Reserved
};

union TZDCSEL_REG {
        Uint16 all;
        struct TZDCSEL_BITS bit;
};

struct TZCTL_BITS {             // bits description
        Uint16 TZA : 2;         // 1:0    TZ1 to TZ6 Trip Action On EPWMxA
        Uint16 TZB : 2;         // 3:2    TZ1 to TZ6 Trip Action On EPWMxB
        Uint16 DCAEVT1 : 2;     // 5:4    EPWMxA action on DCAEVT1
        Uint16 DCAEVT2 : 2;     // 7:6    EPWMxA action on DCAEVT2
        Uint16 DCBEVT1 : 2;     // 9:8    EPWMxB action on DCBEVT1
        Uint16 DCBEVT2 : 2;     // 11:10  EPWMxB action on DCBEVT2
        Uint16 rsvd1 : 4;       // 15:12  Reserved
};

union TZCTL_REG {
        Uint16 all;
        struct TZCTL_BITS bit;
};

struct TZEINT_BITS {                // bits description
        Uint16 rsvd1 : 1;           // 0    Reserved
        Uint16 CBC : 1;             // 1    Trip Zones Cycle By Cycle Int Enable
        Uint16 OST : 1;             // 2    Trip Zones One Shot Int Enable
        Uint16 DCAEVT1 : 1;         // 3    Digital Compare A Event 1 Int Enable
        Uint16 DCAEVT2 : 1;         // 4    Digital Compare A Event 2 Int Enable
        Uint16 DCBEVT1 : 1;         // 5    Digital Compare B Event 1 Int Enable
        Uint16 DCBEVT2 : 1;         // 6    Digital Compare B Event 2 Int Enable
        Uint16 rsvd2 : 9;           // 15:7 Reserved
};

union TZEINT_REG {
        Uint16 all;
        struct TZEINT_BITS bit;
};

struct TZFLG_BITS {                 // bits description
        Uint16 INT : 1;             // 0    Global Int Status Flag
        Uint16 CBC : 1;             // 1    Trip Zones Cycle By Cycle Flag
        Uint16 OST : 1;             // 2    Trip Zones One Shot Flag
        Uint16 DCAEVT1 : 1;         // 3    Digital Compare A Event 1 Flag
        Uint16 DCAEVT2 : 1;         // 4    Digital Compare A Event 2 Flag
        Uint16 DCBEVT1 : 1;         // 5    Digital Compare B Event 1 Flag
        Uint16 DCBEVT2 : 1;         // 6    Digital Compare B Event 2 Flag
        Uint16 rsvd1 : 9;           // 15:7 Reserved
};

union TZFLG_REG {
        Uint16 all;
        struct TZFLG_BITS bit;
};

struct TZCLR_BITS {                  // bits description
        Uint16 INT : 1;              // 0     Global Interrupt Clear Flag
        Uint16 CBC : 1;              // 1     Cycle-By-Cycle Flag Clear
        Uint16 OST : 1;              // 2     One-Shot Flag Clear
        Uint16 DCAEVT1 : 1;          // 3     DCAVET1 Flag Clear
        Uint16 DCAEVT2 : 1;          // 4     DCAEVT2 Flag Clear
        Uint16 DCBEVT1 : 1;          // 5     DCBEVT1 Flag Clear
        Uint16 DCBEVT2 : 1;          // 6     DCBEVT2 Flag Clear
        Uint16 rsvd1 : 7;            // 13:7  Reserved
        Uint16 CBCPULSE : 2;         // 15:14 Clear Pulse for CBC Trip Latch
};

union TZCLR_REG {
        Uint16 all;
        struct TZCLR_BITS bit;
};

struct TZFRC_BITS {                 // bits description
        Uint16 rsvd1 : 1;           // 0    Reserved
        Uint16 CBC : 1;             // 1    Force Trip Zones Cycle By Cycle
                                    //      Event
        Uint16 OST : 1;             // 2    Force Trip Zones One Shot Event
        Uint16 DCAEVT1 : 1;         // 3    Force Digital Compare A Event 1
        Uint16 DCAEVT2 : 1;         // 4    Force Digital Compare A Event 2
        Uint16 DCBEVT1 : 1;         // 5    Force Digital Compare B Event 1
        Uint16 DCBEVT2 : 1;         // 6    Force Digital Compare B Event 2
        Uint16 rsvd2 : 9;           // 15:7 Reserved
};

union TZFRC_REG {
        Uint16 all;
        struct TZFRC_BITS bit;
};

struct ETSEL_BITS {                 // bits description
        Uint16 INTSEL : 3;          // 2:0   EPWMxINTn Select
        Uint16 INTEN : 1;           // 3     EPWMxINTn Enable
        Uint16 SOCASELCMP : 1;      // 4     EPWMxSOCA Compare Select
        Uint16 SOCBSELCMP : 1;      // 5     EPWMxSOCB Compare Select
        Uint16 INTSELCMP : 1;       // 6     EPWMxINT Compare Select
        Uint16 rsvd1 : 1;           // 7     Reserved
        Uint16 SOCASEL : 3;         // 10:8  Start of Conversion A Select
        Uint16 SOCAEN : 1;          // 11    Start of Conversion A Enable
        Uint16 SOCBSEL : 3;         // 14:12 Start of Conversion B Select
        Uint16 SOCBEN : 1;          // 15    Start of Conversion B Enable
};

union ETSEL_REG {
        Uint16 all;
        struct ETSEL_BITS bit;
};

struct ETPS_BITS {                   // bits description
        Uint16 INTPRD : 2;           // 1:0   EPWMxINTn Period Select
        Uint16 INTCNT : 2;           // 3:2   EPWMxINTn Counter Register
        Uint16 INTPSSEL : 1;         // 4     EPWMxINTn Pre-Scale Selection Bits
        Uint16 SOCPSSEL : 1;         // 5     EPWMxSOC A/B  Pre-Scale Selection
                                     //       Bits
        Uint16 rsvd1 : 2;            // 7:6   Reserved
        Uint16 SOCAPRD : 2;          // 9:8   EPWMxSOCA Period Select
        Uint16 SOCACNT : 2;          // 11:10 EPWMxSOCA Counter Register
        Uint16 SOCBPRD : 2;          // 13:12 EPWMxSOCB Period Select
        Uint16 SOCBCNT : 2;          // 15:14 EPWMxSOCB Counter
};

union ETPS_REG {
        Uint16 all;
        struct ETPS_BITS bit;
};

struct ETFLG_BITS {              // bits description
        Uint16 INT : 1;          // 0    EPWMxINTn Flag
        Uint16 rsvd1 : 1;        // 1    Reserved
        Uint16 SOCA : 1;         // 2    EPWMxSOCA Flag
        Uint16 SOCB : 1;         // 3    EPWMxSOCB Flag
        Uint16 rsvd2 : 12;       // 15:4 Reserved
};

union ETFLG_REG {
        Uint16 all;
        struct ETFLG_BITS bit;
};

struct ETCLR_BITS {              // bits description
        Uint16 INT : 1;          // 0    EPWMxINTn Clear
        Uint16 rsvd1 : 1;        // 1    Reserved
        Uint16 SOCA : 1;         // 2    EPWMxSOCA Clear
        Uint16 SOCB : 1;         // 3    EPWMxSOCB Clear
        Uint16 rsvd2 : 12;       // 15:4 Reserved
};

union ETCLR_REG {
        Uint16 all;
        struct ETCLR_BITS bit;
};

struct ETFRC_BITS {              // bits description
        Uint16 INT : 1;          // 0    EPWMxINTn Force
        Uint16 rsvd1 : 1;        // 1    Reserved
        Uint16 SOCA : 1;         // 2    EPWMxSOCA Force
        Uint16 SOCB : 1;         // 3    EPWMxSOCB Force
        Uint16 rsvd2 : 12;       // 15:4 Reserved
};

union ETFRC_REG {
        Uint16 all;
        struct ETFRC_BITS bit;
};

struct PCCTL_BITS {                 // bits description
        Uint16 CHPEN : 1;           // 0     PWM chopping enable
        Uint16 OSHTWTH : 4;         // 4:1   One-shot pulse width
        Uint16 CHPFREQ : 3;         // 7:5   Chopping clock frequency
        Uint16 CHPDUTY : 3;         // 10:8  Chopping clock Duty cycle
        Uint16 rsvd1 : 5;           // 15:11 Reserved
};

union PCCTL_REG {
        Uint16 all;
        struct PCCTL_BITS bit;
};

struct HRCNFG_BITS {                 // bits description
        Uint16 EDGMODE : 2;          // 1:0   ePWMxA Edge Mode Select Bits
        Uint16 CTLMODE : 1;          // 2     ePWMxA Control Mode Select Bits
        Uint16 HRLOAD : 2;           // 4:3   ePWMxA Shadow Mode Select Bits
        Uint16 SELOUTB : 1;          // 5     EPWMB Output Selection Bit
        Uint16 AUTOCONV : 1;         // 6     Autoconversion Bit
        Uint16 SWAPAB : 1;           // 7     Swap EPWMA and EPWMB Outputs Bit
        Uint16 EDGMODEB : 2;         // 9:8   ePWMxB Edge Mode Select Bits
        Uint16 CTLMODEB : 1;         // 10    ePWMxB Control Mode Select Bits
        Uint16 HRLOADB : 2;          // 12:11 ePWMxB Shadow Mode Select Bits
        Uint16 rsvd1 : 1;            // 13    Reserved
        Uint16 rsvd2 : 2;            // 15:14 Reserved
};

union HRCNFG_REG {
        Uint16 all;
        struct HRCNFG_BITS bit;
};

struct HRPCTL_BITS {                     // bits description
        Uint16 HRPE : 1;                 // 0    High Resolution Period Enable
        Uint16 rsvd1 : 1;                // 1    Reserved
        Uint16 TBPHSHRLOADE : 1;         // 2    TBPHSHR Load Enable
        Uint16 rsvd2 : 13;               // 15:3 Reserved
};

union HRPCTL_REG {
        Uint16 all;
        struct HRPCTL_BITS bit;
};

struct DCTRIPSEL_BITS {                 // bits description
        Uint16 DCAHCOMPSEL : 4;         // 3:0    Digital Compare A High COMP
                                        //        Input Select
        Uint16 DCALCOMPSEL : 4;         // 7:4    Digital Compare A Low COMP
                                        //        Input Select
        Uint16 DCBHCOMPSEL : 4;         // 11:8   Digital Compare B High COMP
                                        //        Input Select
        Uint16 DCBLCOMPSEL : 4;         // 15:12  Digital Compare B Low COMP
                                        //        Input Select
};

union DCTRIPSEL_REG {
        Uint16 all;
        struct DCTRIPSEL_BITS bit;
};

struct DCCTL_BITS {                      // bits description
        Uint16 EVT1SRCSEL : 1;           // 0     DCAEVT1 Source Signal
        Uint16 EVT1FRCSYNCSEL : 1;       // 1     DCAEVT1 Force Sync Signal
        Uint16 EVT1SOCE : 1;             // 2     DCAEVT1 SOC Enable
        Uint16 EVT1SYNCE : 1;            // 3     DCAEVT1 SYNC Enable
        Uint16 rsvd1 : 4;                // 7:4   Reserved
        Uint16 EVT2SRCSEL : 1;           // 8     DCAEVT2 Source Signal
        Uint16 EVT2FRCSYNCSEL : 1;       // 9     DCAEVT2 Force Sync Signal
        Uint16 rsvd2 : 6;                // 15:10 Reserved
};

union DCCTL_REG {
        Uint16 all;
        struct DCCTL_BITS bit;
};

struct DCFCTL_BITS {                 // bits description
        Uint16 SRCSEL : 2;           // 1:0   Filter Block Signal Source Select
        Uint16 BLANKE : 1;           // 2     Blanking Enable/Disable
        Uint16 BLANKINV : 1;         // 3     Blanking Window Inversion
        Uint16 PULSESEL : 2;         // 5:4   Pulse Select for Blanking &
                                     //       Capture Alignment
        Uint16 rsvd1 : 1;            // 6     Reserved
        Uint16 rsvd2 : 1;            // 7     Reserved
        Uint16 rsvd3 : 2;            // 9:8   Reserved
        Uint16 rsvd4 : 3;            // 12:10 Reserved
        Uint16 rsvd5 : 3;            // 15:13 Reserved
};

union DCFCTL_REG {
        Uint16 all;
        struct DCFCTL_BITS bit;
};

struct DCCAPCTL_BITS {               // bits description
        Uint16 CAPE : 1;             // 0    Counter Capture Enable
        Uint16 SHDWMODE : 1;         // 1    Counter Capture Mode
        Uint16 rsvd1 : 14;           // 15:2 Reserved
};

union DCCAPCTL_REG {
        Uint16 all;
        struct DCCAPCTL_BITS bit;
};

struct TBCTL2_BITS {               // bits description
        Uint16 rsvd1 : 14;         // 13:0    Reserved
        Uint16 PRDLDSYNC : 2;      // 15:14   Period Load on SYNC Selection
                                   //         Bits
};

union TBCTL2_REG {
        Uint16 all;
        struct TBCTL2_BITS bit;
};

struct CMPCTL2_BITS {                 // bits description
        Uint16 LOADCMODE : 2;         // 1:0  Active Compare C Load
        Uint16 LOADDMODE : 2;         // 3:2  Active Compare D load
        Uint16 SHDWCMODE : 1;         // 4    Compare C Block Operating Mode
        Uint16 rsvd1 : 1;             // 5    Reserved
        Uint16 SHDWDMODE : 1;         // 6    Compare D Block Operating Mode
        Uint16 rsvd2 : 9;             // 15:7 Reserved
};

union CMPCTL2_REG {
        Uint16 all;
        struct CMPCTL2_BITS bit;
};

struct AQCTLR_BITS {
        Uint16 LDAQAMODE : 2;         // 1:0  Active Action Qualifier A Load from Shadow Select Mode
        Uint16 LDAQBMODE : 2;         // 3:2  Active Action Qualifier B Load from Shadow Select Mode
        Uint16 SHDWAQAMODE : 1;       // 4    Action Qualifier A Register Operating Mode
        Uint16 rsvd1 : 1;             // 5    Reserved
        Uint16 SHDWAQBMODE : 1;       // 6    Action Qualifier B Register Operating Mode
        Uint16 rsvd2 : 1;             // 7    Reserved
        Uint16 LDAQASYNC : 2;         // 9:8  Shadow to Active AQCTLA Register Load on SYNC Event
        Uint16 LDAQBSYNC : 2;         // 11:10 Shadow to Active AQCTLB Register Load on SYNC Event
        Uint16 rsvd3 : 3;             // 15:12 Reserved
};

union AQCTLR_REG {
	Uint16 all;
	struct AQCTLR_BITS bit;
};

struct CMPBHR_BITS {               // bits description
        Uint16 rsvd1 : 8;          // 7:0    Reserved
        Uint16 CMPBHR : 8;         // 15:8   Compare B High Resolution Bits
};

union CMPBHR_REG {
        Uint16 all;
        struct CMPBHR_BITS bit;
};

struct DCAHTRIPSEL_BITS {               // bits description
        Uint16 TRIPINPUT1 : 1;          // 0    Trip Input 1 Select to DCAH Mux
        Uint16 TRIPINPUT2 : 1;          // 1    Trip Input 2 Select to DCAH Mux
        Uint16 TRIPINPUT3 : 1;          // 2    Trip Input 3 Select to DCAH Mux
        Uint16 TRIPINPUT4 : 1;          // 3    Trip Input 4 Select to DCAH Mux
        Uint16 TRIPINPUT5 : 1;          // 4    Trip Input 5 Select to DCAH Mux
        Uint16 TRIPINPUT6 : 1;          // 5    Trip Input 6 Select to DCAH Mux
        Uint16 TRIPINPUT7 : 1;          // 6    Trip Input 7 Select to DCAH Mux
        Uint16 TRIPINPUT8 : 1;          // 7    Trip Input 8 Select to DCAH Mux
        Uint16 TRIPINPUT9 : 1;          // 8    Trip Input 9 Select to DCAH Mux
        Uint16 TRIPINPUT10 : 1;         // 9    Trip Input 10 Select to DCAH Mux
        Uint16 TRIPINPUT11 : 1;         // 10   Trip Input 11 Select to DCAH
                                        //      Mux
        Uint16 TRIPINPUT12 : 1;         // 11   Trip Input 12 Select to DCAH
                                        //      Mux
        Uint16 TRIPINPUT13 : 1;         // 12   Trip Input 13 Select to DCAH
                                        //      Mux
        Uint16 TRIPINPUT14 : 1;         // 13   Trip Input 14 Select to DCAH
                                        //      Mux
        Uint16 TRIPINPUT15 : 1;         // 14   Trip Input 15 Select to DCAH
                                        //      Mux
        Uint16 rsvd1 : 1;               // 15   Reserved
};

union DCAHTRIPSEL_REG {
        Uint16 all;
        struct DCAHTRIPSEL_BITS bit;
};

struct DCALTRIPSEL_BITS {               // bits description
        Uint16 TRIPINPUT1 : 1;          // 0    Trip Input 1 Select to DCAL Mux
        Uint16 TRIPINPUT2 : 1;          // 1    Trip Input 2 Select to DCAL Mux
        Uint16 TRIPINPUT3 : 1;          // 2    Trip Input 3 Select to DCAL Mux
        Uint16 TRIPINPUT4 : 1;          // 3    Trip Input 4 Select to DCAL Mux
        Uint16 TRIPINPUT5 : 1;          // 4    Trip Input 5 Select to DCAL Mux
        Uint16 TRIPINPUT6 : 1;          // 5    Trip Input 6 Select to DCAL Mux
        Uint16 TRIPINPUT7 : 1;          // 6    Trip Input 7 Select to DCAL Mux
        Uint16 TRIPINPUT8 : 1;          // 7    Trip Input 8 Select to DCAL Mux
        Uint16 TRIPINPUT9 : 1;          // 8    Trip Input 9 Select to DCAL Mux
        Uint16 TRIPINPUT10 : 1;         // 9    Trip Input 10 Select to DCAL Mux
        Uint16 TRIPINPUT11 : 1;         // 10   Trip Input 11 Select to DCAL
                                        //      Mux
        Uint16 TRIPINPUT12 : 1;         // 11   Trip Input 12 Select to DCAL
                                        //      Mux
        Uint16 TRIPINPUT13 : 1;         // 12   Trip Input 13 Select to DCAL
                                        //      Mux
        Uint16 TRIPINPUT14 : 1;         // 13   Trip Input 14 Select to DCAL
                                        //      Mux
        Uint16 TRIPINPUT15 : 1;         // 14   Trip Input 15 Select to DCAL
                                        //      Mux
        Uint16 rsvd1 : 1;               // 15   Reserved
};

union DCALTRIPSEL_REG {
        Uint16 all;
        struct DCALTRIPSEL_BITS bit;
};

struct DCBHTRIPSEL_BITS {               // bits description
        Uint16 TRIPINPUT1 : 1;          // 0    Trip Input 1 Select to DCBH Mux
        Uint16 TRIPINPUT2 : 1;          // 1    Trip Input 2 Select to DCBH Mux
        Uint16 TRIPINPUT3 : 1;          // 2    Trip Input 3 Select to DCBH Mux
        Uint16 TRIPINPUT4 : 1;          // 3    Trip Input 4 Select to DCBH Mux
        Uint16 TRIPINPUT5 : 1;          // 4    Trip Input 5 Select to DCBH Mux
        Uint16 TRIPINPUT6 : 1;          // 5    Trip Input 6 Select to DCBH Mux
        Uint16 TRIPINPUT7 : 1;          // 6    Trip Input 7 Select to DCBH Mux
        Uint16 TRIPINPUT8 : 1;          // 7    Trip Input 8 Select to DCBH Mux
        Uint16 TRIPINPUT9 : 1;          // 8    Trip Input 9 Select to DCBH Mux
        Uint16 TRIPINPUT10 : 1;         // 9    Trip Input 10 Select to DCBH Mux
        Uint16 TRIPINPUT11 : 1;         // 10   Trip Input 11 Select to DCBH
                                        //      Mux
        Uint16 TRIPINPUT12 : 1;         // 11   Trip Input 12 Select to DCBH
                                        //      Mux
        Uint16 TRIPINPUT13 : 1;         // 12   Trip Input 13 Select to DCBH
                                        //      Mux
        Uint16 TRIPINPUT14 : 1;         // 13   Trip Input 14 Select to DCBH
                                        //      Mux
        Uint16 TRIPINPUT15 : 1;         // 14   Trip Input 15 Select to DCBH
                                        //      Mux
        Uint16 rsvd1 : 1;               // 15   Reserved
};

union DCBHTRIPSEL_REG {
        Uint16 all;
        struct DCBHTRIPSEL_BITS bit;
};

struct DCBLTRIPSEL_BITS {               // bits description
        Uint16 TRIPINPUT1 : 1;          // 0    Trip Input 1 Select to DCBL Mux
        Uint16 TRIPINPUT2 : 1;          // 1    Trip Input 2 Select to DCBL Mux
        Uint16 TRIPINPUT3 : 1;          // 2    Trip Input 3 Select to DCBL Mux
        Uint16 TRIPINPUT4 : 1;          // 3    Trip Input 4 Select to DCBL Mux
        Uint16 TRIPINPUT5 : 1;          // 4    Trip Input 5 Select to DCBL Mux
        Uint16 TRIPINPUT6 : 1;          // 5    Trip Input 6 Select to DCBL Mux
        Uint16 TRIPINPUT7 : 1;          // 6    Trip Input 7 Select to DCBL Mux
        Uint16 TRIPINPUT8 : 1;          // 7    Trip Input 8 Select to DCBL Mux
        Uint16 TRIPINPUT9 : 1;          // 8    Trip Input 9 Select to DCBL Mux
        Uint16 TRIPINPUT10 : 1;         // 9    Trip Input 10 Select to DCBL Mux
        Uint16 TRIPINPUT11 : 1;         // 10   Trip Input 11 Select to DCBL
                                        //      Mux
        Uint16 TRIPINPUT12 : 1;         // 11   Trip Input 12 Select to DCBL
                                        //      Mux
        Uint16 TRIPINPUT13 : 1;         // 12   Trip Input 13 Select to DCBL
                                        //      Mux
        Uint16 TRIPINPUT14 : 1;         // 13   Trip Input 14 Select to DCBL
                                        //      Mux
        Uint16 TRIPINPUT15 : 1;         // 14   Trip Input 15 Select to DCBL
                                        //      Mux
        Uint16 rsvd1 : 1;               // 15   Reserved
};

union DCBLTRIPSEL_REG {
        Uint16 all;
        struct DCBLTRIPSEL_BITS bit;
};

struct ETINTPS_BITS {               // bits description
        Uint16 INTPRD2 : 4;         // 3:0    EPWMxINTn Period Select
        Uint16 INTCNT2 : 4;         // 7:4    EPWMxINTn Counter Register
        Uint16 rsvd1 : 8;           // 15:8   Reserved
};

union ETINTPS_REG {
        Uint16 all;
        struct ETINTPS_BITS bit;
};

struct ETSOCPS_BITS {                // bits description
        Uint16 SOCAPRD2 : 4;         // 3:0    EPWMxSOCA Period Select
        Uint16 SOCACNT2 : 4;         // 7:4    EPWMxSOCA Counter Register
        Uint16 SOCBPRD2 : 4;         // 11:8   EPWMxSOCB Period Select
        Uint16 SOCBCNT2 : 4;         // 15:12  EPWMxSOCB Counter Register
};

union ETSOCPS_REG {
        Uint16 all;
        struct ETSOCPS_BITS bit;
};

struct ETCNTINITCTL_BITS {              // bits description
        Uint16 rsvd1 : 10;              // 9:0   Reserved
        Uint16 INTINITFRC : 1;          // 10    EPWMxINT Counter Initialization
                                        //       Force
        Uint16 SOCAINITFRC : 1;         // 11    EPWMxSOCA Counter
                                        //       Initialization Force
        Uint16 SOCBINITFRC : 1;         // 12    EPWMxSOCB Counter
                                        //       Initialization Force
        Uint16 INTINITEN : 1;           // 13    EPWMxINT Counter Initialization
                                        //       Enable
        Uint16 SOCAINITEN : 1;          // 14    EPWMxSOCA Counter
                                        //       Initialization Enable
        Uint16 SOCBINITEN : 1;          // 15    EPWMxSOCB Counter
                                        //       Initialization Enable
};

union ETCNTINITCTL_REG {
        Uint16 all;
        struct ETCNTINITCTL_BITS bit;
};

struct ETCNTINIT_BITS {              // bits description
        Uint16 INTINIT : 4;          // 3:0    EPWMxINT Counter Initialization
                                     //        Bits
        Uint16 SOCAINIT : 4;         // 7:4    EPWMxSOCA Counter Initialization
                                     //        Bits
        Uint16 SOCBINIT : 4;         // 11:8   EPWMxSOCB Counter Initialization
                                     //        Bits
        Uint16 rsvd1 : 4;            // 15:12  Reserved
};

union ETCNTINIT_REG {
        Uint16 all;
        struct ETCNTINIT_BITS bit;
};

struct EPWMXLINK_BITS {              // bits description
        Uint16 TBPRDLINK : 4;        // 3:0    TBPRD:TBPRDHR  Link
        Uint16 CMPALINK : 4;         // 7:4    CMPA:CMPAHR Link
        Uint16 CMPBLINK : 4;         // 11:8   CMPB:CMPBHR Link
        Uint16 CMPCLINK : 4;         // 15:12  CMPC Link
        Uint16 CMPDLINK : 4;         // 19:16  CMPD Link
        Uint16 rsvd1 : 12;           // 31:20  Reserved
};

union EPWMXLINK_REG {
        Uint32 all;
        struct EPWMXLINK_BITS bit;
};

struct ETCLRM_BITS {             // bits description
        Uint16 INT : 1;          // 0    EPWMxINTn Clear
        Uint16 rsvd1 : 1;        // 1    Reserved
        Uint16 SOCA : 1;         // 2    EPWMxSOCA Clear
        Uint16 SOCB : 1;         // 3    EPWMxSOCB Clear
        Uint16 rsvd2 : 12;       // 15:4 Reserved
};

union ETCLRM_REG {
        Uint16 all;
        struct ETCLRM_BITS bit;
};

struct TZCLRM_BITS {                 // bits description
        Uint16 INT : 1;              // 0     Global Interrupt Clear Flag
        Uint16 CBC : 1;              // 1     Cycle-By-Cycle Flag Clear
        Uint16 OST : 1;              // 2     One-Shot Flag Clear
        Uint16 DCAEVT1 : 1;          // 3     DCAVET1 Flag Clear
        Uint16 DCAEVT2 : 1;          // 4     DCAEVT2 Flag Clear
        Uint16 DCBEVT1 : 1;          // 5     DCBEVT1 Flag Clear
        Uint16 DCBEVT2 : 1;          // 6     DCBEVT2 Flag Clear
        Uint16 rsvd1 : 7;            // 13:7  Reserved
        Uint16 CBCPULSE : 2;         // 15:14 Clear Pulse for CBC Trip Latch
};

union TZCLRM_REG {
        Uint16 all;
        struct TZCLRM_BITS bit;
};

struct AQCTLAM_BITS {           // bits description
        Uint16 ZRO : 2;         // 1:0    Action Counter = Zero
        Uint16 PRD : 2;         // 3:2    Action Counter = Period
        Uint16 CAU : 2;         // 5:4    Action Counter = Compare A Up
        Uint16 CAD : 2;         // 7:6    Action Counter = Compare A Down
        Uint16 CBU : 2;         // 9:8    Action Counter = Compare B Up
        Uint16 CBD : 2;         // 11:10  Action Counter = Compare B Down
        Uint16 rsvd1 : 4;       // 15:12  Reserved
};

union AQCTLAM_REG {
        Uint16 all;
        struct AQCTLAM_BITS bit;
};

struct AQCTLBM_BITS {           // bits description
        Uint16 ZRO : 2;         // 1:0    Action Counter = Zero
        Uint16 PRD : 2;         // 3:2    Action Counter = Period
        Uint16 CAU : 2;         // 5:4    Action Counter = Compare A Up
        Uint16 CAD : 2;         // 7:6    Action Counter = Compare A Down
        Uint16 CBU : 2;         // 9:8    Action Counter = Compare B Up
        Uint16 CBD : 2;         // 11:10  Action Counter = Compare B Down
        Uint16 rsvd1 : 4;       // 15:12  Reserved
};

union AQCTLBM_REG {
        Uint16 all;
        struct AQCTLBM_BITS bit;
};

struct AQSFRCM_BITS {              // bits description
        Uint16 ACTSFA : 2;         // 1:0  Action when One-time SW Force A
                                   //      Invoked
        Uint16 OTSFA : 1;          // 2    One-time SW Force A Output
        Uint16 ACTSFB : 2;         // 4:3  Action when One-time SW Force B
                                   //      Invoked
        Uint16 OTSFB : 1;          // 5    One-time SW Force A Output
        Uint16 RLDCSF : 2;         // 7:6  Reload from Shadow Options
        Uint16 rsvd1 : 8;          // 15:8 Reserved
};

union AQSFRCM_REG {
        Uint16 all;
        struct AQSFRCM_BITS bit;
};

struct AQCSFRCM_BITS {           // bits description
        Uint16 CSFA : 2;         // 1:0    Continuous Software Force on output A
        Uint16 CSFB : 2;         // 3:2    Continuous Software Force on output B
        Uint16 rsvd1 : 12;       // 15:4   Reserved
};

union AQCSFRCM_REG {
        Uint16 all;
        struct AQCSFRCM_BITS bit;
};

struct TBPHS_HRPWM_REG {            // bits   description
        Uint16 TBPHSHR;             // 15:0   Extension register for HRPWM Phase
                                    //        (8 bits)
        Uint16 TBPHS;               // 31:16  Phase offset register
};

union TBPHS_HRPWM_GROUP {
        Uint32 all;
        struct TBPHS_HRPWM_REG half;
};

struct CMPA_HRPWM_REG {             // bits   description
        Uint16 CMPAHR;              // 15:0   Extension register for HRPWM
                                    //        compare A (8 bits)
        Uint16 CMPA;                // 31:16  Compare A reg
};

union CMPA_HRPWM_GROUP {
        Uint32 all;
        struct CMPA_HRPWM_REG half;
};

struct TBPRD_HRPWM_REG {            // bits   description
        Uint16 TBPRDHR;             // 15:0   Extension register for HRPWM
                                    //        Period (8 bits)
        Uint16 TBPRD;               // 31:16  Timebase Period Register
};

union TBPRD_HRPWM_GROUP {
        Uint32 all;
        struct TBPRD_HRPWM_REG half;
};

struct CMPB_HRPWM_REG {             // bits   description
        Uint16 CMPBHR;              // 15:0   Extension register for HRPWM
                                    //        compare B (8 bits)
        Uint16 CMPB;                // 31:16  Compare B reg
};

union CMPB_HRPWM_GROUP {
        Uint32 all;
        struct CMPB_HRPWM_REG half;
};


struct EPWM_REGS {
        union    TBCTL_REG TBCTL;                     // Time Base Control
                                                      // Register
        union    TBSTS_REG TBSTS;                     // Time Base Status
                                                      // Register
        union   TBPHS_HRPWM_GROUP TBPHS;              // Union of TBPHS:TBPHSHR
        Uint16 TBCTR;                                 // Time Base Counter
                                                      // Register
        Uint16 TBPRD;                                 // Time Base Period
                                                      // Register
        Uint16 TBPRDHR;                               // Time Base Period High
                                                      // Resolution Register
        union    CMPCTL_REG CMPCTL;                   // Counter Compare Control
                                                      // Register
        union   CMPA_HRPWM_GROUP CMPA;                // Union of CMPA:CMPAHR
        Uint16 CMPB;                                  // Counter Compare B
                                                      // Register
        union    AQCTL_REG AQCTLA;                    // Action Qualifier
                                                      // Control Register For
                                                      // Output A
        union    AQCTL_REG AQCTLB;                    // Action Qualifier
                                                      // Control Register For
                                                      // Output B
        union    AQSFRC_REG AQSFRC;                   // Action Qualifier
                                                      // Software Force Register
        union    AQCSFRC_REG AQCSFRC;                 // Action Qualifier
                                                      // Continuous S/W Force
                                                      // Register
        union    DBCTL_REG DBCTL;                     // Dead-Band Generator
                                                      // Control Register
        Uint16 DBRED;                                 // Dead-Band Generator
                                                      // Rising Edge Delay Count
                                                      // Register
        Uint16 DBFED;                                 // Dead-Band Generator
                                                      // Falling Edge Delay
                                                      // Count Register
        union    TZSEL_REG TZSEL;                     // Trip Zone Select
                                                      // Register
        union    TZDCSEL_REG TZDCSEL;                 // Trip Zone Digital
                                                      // Comparator Select
                                                      // Register
        union    TZCTL_REG TZCTL;                     // Trip Zone Control
                                                      // Register
        union    TZEINT_REG TZEINT;                   // Trip Zone Enable
                                                      // Interrupt Register
        union    TZFLG_REG TZFLG;                     // Trip Zone Flag Register
        union    TZCLR_REG TZCLR;                     // Trip Zone Clear
                                                      // Register
        union    TZFRC_REG TZFRC;                     // Trip Zone Force
                                                      // Register
        union    ETSEL_REG ETSEL;                     // Event Trigger Selection
                                                      // Register
        union    ETPS_REG ETPS;                       // Event Trigger Pre-Scale
                                                      // Register
        union    ETFLG_REG ETFLG;                     // Event Trigger Flag
                                                      // Register
        union    ETCLR_REG ETCLR;                     // Event Trigger Clear
                                                      // Register
        union    ETFRC_REG ETFRC;                     // Event Trigger Force
                                                      // Register
        union    PCCTL_REG PCCTL;                     // PWM Chopper Control
                                                      // Register
        Uint16 rsvd1;                                 // Reserved
        union    HRCNFG_REG HRCNFG;                   // HRPWM Configuration
                                                      // Register
        Uint16 rsvd2[5];                              // Reserved
        Uint16 HRMSTEP;                               // HRPWM MEP Step Register
        Uint16 rsvd3;                                 // Reserved
        union    HRPCTL_REG HRPCTL;                   // High Resolution Period
                                                      // Control Register
        Uint16 rsvd4;                                 // Reserved
        union   TBPRD_HRPWM_GROUP TBPRDM;             // Union of TBPRD:TBPRDHR
                                                      // Mirror
        union   CMPA_HRPWM_GROUP CMPAM;               // Union of CMPA:CMPAHR
                                                      // Mirror
        Uint16 rsvd5[2];                              // Reserved
        union    DCTRIPSEL_REG DCTRIPSEL;             // Digital Compare Trip
                                                      // Select Register
        union    DCCTL_REG DCACTL;                    // Digital Compare A
                                                      // Control Register
        union    DCCTL_REG DCBCTL;                    // Digital Compare B
                                                      // Control Register
        union    DCFCTL_REG DCFCTL;                   // Digital Compare Filter
                                                      // Control Register
        union    DCCAPCTL_REG DCCAPCTL;               // Digital Compare Capture
                                                      // Control Register
        Uint16 DCFOFFSET;                             // Digital Compare Filter
                                                      // Offset Register
        Uint16 DCFOFFSETCNT;                          // Digital Compare Filter
                                                      // Offset Counter Register
        Uint16 DCFWINDOW;                             // Digital Compare Filter
                                                      // Window Register
        Uint16 DCFWINDOWCNT;                          // Digital Compare Filter
                                                      // Window Counter Register
        Uint16 DCCAP;                                 // Digital Compare Counter
                                                      // Capture Register
        Uint16 rsvd6[6];                              // Reserved
        union    TBCTL2_REG TBCTL2;                   // Time Base Control
                                                      // Register 2
        union    CMPCTL2_REG CMPCTL2;                 // Counter Compare Control
                                                      // Register 2
        Uint16 rsvd7[5];                              // Reserved
        union    AQCTLR_REG AQCTLR;                   // Action Qualifier Control Register
        Uint16 rsvd8[2];
        union    CMPBHR_REG CMPBHR;                   // Compare B High
                                                      // Resolution Register
        Uint16 rsvd9;                                 // Reserved
        union    DCAHTRIPSEL_REG DCAHTRIPSEL;         // Digital Compare AH Trip
                                                      // Select
        union    DCALTRIPSEL_REG DCALTRIPSEL;         // Digital Compare AL Trip
                                                      // Select
        union    DCBHTRIPSEL_REG DCBHTRIPSEL;         // Digital Compare BH Trip
                                                      // Select
        union    DCBLTRIPSEL_REG DCBLTRIPSEL;         // Digital Compare BL Trip
                                                      // Select
        union    ETINTPS_REG ETINTPS;                 // Event-Trigger Interrupt
                                                      // Pre-Scale Register
        union    ETSOCPS_REG ETSOCPS;                 // Event-Trigger SOC
                                                      // Pre-Scale Register
        union    ETCNTINITCTL_REG ETCNTINITCTL;       // Event-Trigger Counter
                                                      // Initialization Control
                                                      // Register
        union    ETCNTINIT_REG ETCNTINIT;             // Event-Trigger Counter
                                                      // Initialization Register
        Uint16 rsvd10[10];                             // Reserved
        union    EPWMXLINK_REG EPWMXLINK;             // EPWMx Link Register
        union   TBPHS_HRPWM_GROUP TBPHSM;             // Union of TBPHS:TBPHSHR
                                                      // Mirror
        union   TBPRD_HRPWM_GROUP TBPRDM2;            // Union of TBPRD:TBPRDHR
                                                      // Mirror 2
        union   CMPA_HRPWM_GROUP CMPAM2;              // Union of CMPA:CMPAHR
                                                      // Mirror 2
        union   CMPB_HRPWM_GROUP CMPBM;               // Union of CMPB:CMPBHR
                                                      // Mirror
        Uint16 rsvd11;                                // Reserved
        Uint16 CMPC;                                  // Counter Compare C
                                                      // Register
        Uint16 rsvd12;                                // Reserved
        Uint16 CMPD;                                  // Counter Compare D
                                                      // Register
        Uint16 rsvd13[4];                             // Reserved
        union    ETCLRM_REG ETCLRM;                   // Event Trigger Clear
                                                      // Register
        union    TZCLRM_REG TZCLRM;                   // Trip Zone Clear
                                                      // Register
        Uint16 rsvd14;                                // Reserved
        union    AQCTLAM_REG AQCTLAM;                 // Action Qualifier
                                                      // Control Register For
                                                      // Output A
        union    AQCTLBM_REG AQCTLBM;                 // Action Qualifier
                                                      // Control Register For
                                                      // Output B
        union    AQSFRCM_REG AQSFRCM;                 // Action Qualifier
                                                      // Software Force Register
        union    AQCSFRCM_REG AQCSFRCM;               // Action Qualifier
                                                      // Continuous S/W Force
                                                      // Register
        Uint16 rsvd15[9];                             // Reserved
};

//---------------------------------------------------------------------------
// Epwm External References & Function Declarations:
extern volatile struct EPWM_REGS EPwm1Regs;
extern volatile struct EPWM_REGS EPwm2Regs;
extern volatile struct EPWM_REGS EPwm3Regs;
extern volatile struct EPWM_REGS EPwm4Regs;
extern volatile struct EPWM_REGS EPwm5Regs;
extern volatile struct EPWM_REGS EPwm6Regs;
extern volatile struct EPWM_REGS EPwm7Regs;
extern volatile struct EPWM_REGS EPwm8Regs;
extern volatile struct EPWM_REGS EPwm9Regs;

#ifdef __cplusplus
}
#endif  /* extern "C" */


#endif  // end of C28M35x_EPWM_H definition


