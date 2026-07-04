//###########################################################################
// FILE:    F28M35x_Adc.c
// TITLE:   F28M35x ADC Initialization & Support Functions.
//###########################################################################
// $TI Release: F28M35x Support Library v202 $
// $Release Date: Tue Apr  8 12:35:30 CDT 2014 $
//###########################################################################

#include "F28M35x_Device.h"     // F28M35x Headerfile Include File
#include "F28M35x_Examples.h"   // F28M35x Examples Include File

#define ADC_usDELAY  1000L

//---------------------------------------------------------------------------
// InitAdc1:
//---------------------------------------------------------------------------
// This function initializes ADC1 to a known state.
void InitAdc1(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    // To powerup the ADC1 the ADC1ENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and
    // ADC1 core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the F28M35xA_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    Adc1Regs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC1 BG
    Adc1Regs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    Adc1Regs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC1
    Adc1Regs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC1
    Adc1Regs.ADCCTL1.bit.ADCREFSEL = 1;      // Select external reference
    EDIS;

    DELAY_US(ADC_usDELAY);         // Delay before converting ADC1 channels
}

void InitAdc1Aio()
{
    EALLOW;

/* Configure ADC1 pins using AIO regs*/

// This specifies which of the possible AIO pins will be Analog input pins.
// Comment out other unwanted lines.

    GpioG2CtrlRegs.AIOMUX1.bit.AIO2 = 2;   // Configure AIO2 for A2 (analog
                                           // input) operation
    GpioG2CtrlRegs.AIOMUX1.bit.AIO4 = 2;   // Configure AIO4 for A4 (analog
                                           // input) operation
    GpioG2CtrlRegs.AIOMUX1.bit.AIO6 = 2;   // Configure AIO6 for A6 (analog
                                           // input) operation
    GpioG2CtrlRegs.AIOMUX1.bit.AIO10 = 2;  // Configure AIO10 for B2 (analog
                                           // input) operation
    GpioG2CtrlRegs.AIOMUX1.bit.AIO12 = 2;  // Configure AIO12 for B4 (analog
                                           // input) operation
    GpioG2CtrlRegs.AIOMUX1.bit.AIO14 = 2;  // Configure AIO14 for B6 (analog
                                           // input) operation

    EDIS;
}

/* Adc1offsetSelfCal-
   This function re-calibrates the ADC1 zero offset error by converting the
   VREFLO reference with
   the ADC1 and modifying the ADCOFFTRIM register. VREFLO is sampled by the ADC1
   using an internal
   MUX select which connects VREFLO to A5 without sacrificing an external ADC1
   pin. This
   function calls two other functions:
   - Adc1ChanSelect(channel) selects the ADC1 channel to convert
   - Adc1Conversion() initiates several ADC1 conversions and returns the average
 */
void Adc1OffsetSelfCal()
{
    Uint16 AdcConvMean;

    EALLOW;
    Adc1Regs.ADCCTL1.bit.ADCREFSEL = 0;                     //Select internal
                                                            // reference mode
    Adc1Regs.ADCCTL1.bit.VREFLOCONV = 1;                    //Select VREFLO
                                                            // internal
                                                            // connection on B5
    Adc1ChanSelect(13);                                     //Select channel B5
                                                            // for all SOC
    Adc1Regs.ADCOFFTRIM.bit.OFFTRIM = 80;                   //Apply artificial
                                                            // offset (+80) to
                                                            // account for a
                                                            // negative offset
                                                            // that may reside
                                                            // in the ADC1 core
    AdcConvMean = Adc1Conversion();                         //Capture ADC1
                                                            // conversion on
                                                            // VREFLO
    Adc1Regs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;     //Set offtrim
                                                            // register with new
                                                            // value (i.e remove
                                                            // artical offset
                                                            // (+80) and create
                                                            // a two's
                                                            // compliment of the
                                                            // offset error)
    Adc1Regs.ADCCTL1.bit.VREFLOCONV = 0;                    //Select external
                                                            // ADCIN5 input pin
                                                            // on B5
    Adc1Regs.ADCCTL1.bit.ADCREFSEL = 1;

    EDIS;
}

/*  Adc1ChanSelect-
    This function selects the ADC1 channel to convert by setting all SOC channel
   selects to a single channel.

 * IMPORTANT * This function will overwrite previous SOC channel select
 *settings.
 * Recommend saving the previous settings.
 */
void Adc1ChanSelect(Uint16 ch_no)
{
    Adc1Regs.ADCSOC0CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC1CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC2CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC3CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC4CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC5CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC6CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC7CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC8CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC9CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC10CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC11CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC12CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC13CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC14CTL.bit.CHSEL= ch_no;
    Adc1Regs.ADCSOC15CTL.bit.CHSEL= ch_no;
}

/* Adc1Conversion -
   This function initiates several ADC1 conversions and returns the average. It
   uses ADCINT1 and ADCINT2
   to "ping-pong" between SOC0-7 and SOC8-15 and is referred to as "ping-pong"
   sampling.

 * IMPORTANT * This function will overwrite previous ADC1 settings. Recommend
 *saving previous settings.
 */
Uint16 Adc1Conversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            //initialize index to 0
    SampleSize  = 256;          //set sample size to 256 (**NOTE: Sample size
                                // must be multiples of 2^x where is an integer
                                // >= 4)
    Sum         = 0;            //set sum to 0
    Mean        = 999;          //initialize mean to known value

    //Set the ADC1 sample window to the desired value (Sample window = ACQPS +
    // 1)
    ACQPS_Value = 6;
    Adc1Regs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    Adc1Regs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    Adc1Regs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    Adc1Regs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    Adc1Regs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    Adc1Regs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    Adc1Regs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;

    //Enable ping-pong sampling

    // Enabled ADCINT1 and ADCINT2
    Adc1Regs.INTSEL1N2.bit.INT1E = 1;
    Adc1Regs.INTSEL1N2.bit.INT2E = 1;

    // Disable continuous sampling for ADCINT1 and ADCINT2
    Adc1Regs.INTSEL1N2.bit.INT1CONT = 0;
    Adc1Regs.INTSEL1N2.bit.INT2CONT = 0;

    Adc1Regs.ADCCTL1.bit.INTPULSEPOS = 1;    //ADCINTs trigger at end of
                                             // conversion

    // Setup ADCINT1 and ADCINT2 trigger source
    Adc1Regs.INTSEL1N2.bit.INT1SEL = 6;      //EOC6 triggers ADCINT1
    Adc1Regs.INTSEL1N2.bit.INT2SEL = 14;     //EOC14 triggers ADCINT2

    // Setup each SOC's ADCINT trigger source
    Adc1Regs.ADCINTSOCSEL1.bit.SOC0  = 2;    //ADCINT2 starts SOC0-7
    Adc1Regs.ADCINTSOCSEL1.bit.SOC1  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC2  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC3  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC4  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC5  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC6  = 2;
    Adc1Regs.ADCINTSOCSEL1.bit.SOC7  = 2;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC8  = 1;    //ADCINT1 starts SOC8-15
    Adc1Regs.ADCINTSOCSEL2.bit.SOC9  = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC10 = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC11 = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC12 = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC13 = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC14 = 1;
    Adc1Regs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // Delay before converting ADC1
                                            // channels

    //ADC1 Conversion

    Adc1Regs.ADCSOCFRC1.all = 0x00FF;  // Force Start SOC0-7 to begin ping-pong
                                       // sampling

    while( index < SampleSize ) {

        //Wait for ADCINT1 to trigger, then add ADCRESULT0-7 registers to sum
        while (Adc1Regs.ADCINTFLG.bit.ADCINT1 == 0) {}
        Adc1Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;   //Must clear ADCINT1 flag since
                                                 // INT1CONT = 0
        Sum += Adc1Result.ADCRESULT0;
        Sum += Adc1Result.ADCRESULT1;
        Sum += Adc1Result.ADCRESULT2;
        Sum += Adc1Result.ADCRESULT3;
        Sum += Adc1Result.ADCRESULT4;
        Sum += Adc1Result.ADCRESULT5;
        Sum += Adc1Result.ADCRESULT6;
        Sum += Adc1Result.ADCRESULT7;

        //Wait for ADCINT2 to trigger, then add ADCRESULT8-15 registers to sum
        while (Adc1Regs.ADCINTFLG.bit.ADCINT2 == 0) {}
        Adc1Regs.ADCINTFLGCLR.bit.ADCINT2 = 1;   //Must clear ADCINT2 flag since
                                                 // INT2CONT = 0
        Sum += Adc1Result.ADCRESULT8;
        Sum += Adc1Result.ADCRESULT9;
        Sum += Adc1Result.ADCRESULT10;
        Sum += Adc1Result.ADCRESULT11;
        Sum += Adc1Result.ADCRESULT12;
        Sum += Adc1Result.ADCRESULT13;
        Sum += Adc1Result.ADCRESULT14;
        Sum += Adc1Result.ADCRESULT15;

        index+=16;

    } // end data collection

    //Disable ADCINT1 and ADCINT2 to STOP the ping-pong sampling
    Adc1Regs.INTSEL1N2.bit.INT1E = 0;
    Adc1Regs.INTSEL1N2.bit.INT2E = 0;

    Mean = Sum / SampleSize;    //Calculate average ADC1 sample value

    return Mean;                //return the average

}

// Functions for legacy purposes
// When legacy ADC functions are called new ADC1 functions are called
void InitAdc(){
    InitAdc1();
}

void InitAdcAio(){
    InitAdc1Aio();
}

void AdcOffsetSelfCal(){
    Adc1OffsetSelfCal();
}

void AdcChanSelect(Uint16 ch_no){
    Adc1ChanSelect(ch_no);
}

Uint16 AdcConversion(){
    Uint16 mean;
    mean = Adc1Conversion();
    return mean;
}

//---------------------------------------------------------------------------
// InitAdc2:
//---------------------------------------------------------------------------
// This function initializes ADC2 to a known state.
void InitAdc2(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    // To powerup the ADC2 the ADC2ENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and
    // ADC2 core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the F28M35xA_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    Adc2Regs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC2 BG
    Adc2Regs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    Adc2Regs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC2
    Adc2Regs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC2
    Adc2Regs.ADCCTL1.bit.ADCREFSEL = 1;      // Select external reference
    EDIS;

    DELAY_US(ADC_usDELAY);         // Delay before converting ADC2 channels
}

void InitAdc2Aio()
{
    EALLOW;

/* Configure ADC2 pins using AIO regs*/

// This specifies which of the possible AIO pins will be Analog input pins.
// Comment out other unwanted lines.

    GpioG2CtrlRegs.AIOMUX2.bit.AIO18 = 2;   // Configure AIO18 for A2 (analog
                                            // input) operation
    GpioG2CtrlRegs.AIOMUX2.bit.AIO20 = 2;   // Configure AIO20 for A4 (analog
                                            // input) operation
    GpioG2CtrlRegs.AIOMUX2.bit.AIO22 = 2;   // Configure AIO22 for A6 (analog
                                            // input) operation
    GpioG2CtrlRegs.AIOMUX2.bit.AIO26 = 2;   // Configure AIO26 for B2 (analog
                                            // input) operation
    GpioG2CtrlRegs.AIOMUX2.bit.AIO28 = 2;   // Configure AIO28 for B4 (analog
                                            // input) operation
    GpioG2CtrlRegs.AIOMUX2.bit.AIO30 = 2;   // Configure AIO30 for B6 (analog
                                            // input) operation

    EDIS;
}

/* Adc2offsetSelfCal-
   This function re-calibrates the ADC2 zero offset error by converting the
   VREFLO reference with
   the ADC2 and modifying the ADCOFFTRIM register. VREFLO is sampled by the ADC2
   using an internal
   MUX select which connects VREFLO to A5 without sacrificing an external ADC2
   pin. This
   function calls two other functions:
   - Adc2ChanSelect(channel) selects the ADC2 channel to convert
   - Adc2Conversion() initiates several ADC2 conversions and returns the average
 */
void Adc2OffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    Adc2Regs.ADCCTL1.bit.ADCREFSEL = 0;                  //Select internal
                                                         // reference mode
    Adc2Regs.ADCCTL1.bit.VREFLOCONV = 1;                 //Select VREFLO
                                                         // internal connection
                                                         // on B5
    Adc2ChanSelect(13);                                  //Select channel B5 for
                                                         // all SOC
    Adc2Regs.ADCOFFTRIM.bit.OFFTRIM = 80;                //Apply artificial
                                                         // offset (+80) to
                                                         // account for a
                                                         // negative offset that
                                                         // may reside in the
                                                         // ADC2 core
    AdcConvMean = Adc2Conversion();                      //Capture ADC2
                                                         // conversion on VREFLO
    Adc2Regs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;  //Set offtrim register
                                                         // with new value (i.e
                                                         // remove artical
                                                         // offset (+80) and
                                                         // create a two's
                                                         // compliment of the
                                                         // offset error)
    Adc2Regs.ADCCTL1.bit.VREFLOCONV = 0;                 //Select external
                                                         // ADCIN5 input pin on
                                                         // B5
    Adc2Regs.ADCCTL1.bit.ADCREFSEL = 1;
    EDIS;
}

/*  Adc2ChanSelect-
    This function selects the ADC2 channel to convert by setting all SOC channel
   selects to a single channel.

 * IMPORTANT * This function will overwrite previous SOC channel select
 *settings.
 * Recommend saving the previous settings.
 */
void Adc2ChanSelect(Uint16 ch_no)
{
    Adc2Regs.ADCSOC0CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC1CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC2CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC3CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC4CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC5CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC6CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC7CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC8CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC9CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC10CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC11CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC12CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC13CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC14CTL.bit.CHSEL= ch_no;
    Adc2Regs.ADCSOC15CTL.bit.CHSEL= ch_no;
}

/* Adc2Conversion -
   This function initiates several ADC2 conversions and returns the average. It
   uses ADCINT1 and ADCINT2
   to "ping-pong" between SOC0-7 and SOC8-15 and is referred to as "ping-pong"
   sampling.

 * IMPORTANT * This function will overwrite previous ADC2 settings. Recommend
 *saving previous settings.
 */
Uint16 Adc2Conversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            //initialize index to 0
    SampleSize  = 256;          //set sample size to 256 (**NOTE: Sample size
                                // must be multiples of 2^x where is an integer
                                // >= 4)
    Sum         = 0;            //set sum to 0
    Mean        = 999;          //initialize mean to known value

    //Set the ADC2 sample window to the desired value (Sample window = ACQPS +
    // 1)
    ACQPS_Value = 6;
    Adc2Regs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    Adc2Regs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    Adc2Regs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    Adc2Regs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    Adc2Regs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    Adc2Regs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    Adc2Regs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;

    //Enable ping-pong sampling

    // Enabled ADCINT1 and ADCINT2
    Adc2Regs.INTSEL1N2.bit.INT1E = 1;
    Adc2Regs.INTSEL1N2.bit.INT2E = 1;

    // Disable continuous sampling for ADCINT1 and ADCINT2
    Adc2Regs.INTSEL1N2.bit.INT1CONT = 0;
    Adc2Regs.INTSEL1N2.bit.INT2CONT = 0;

    Adc2Regs.ADCCTL1.bit.INTPULSEPOS = 1;    //ADCINTs trigger at end of
                                             // conversion

    // Setup ADCINT1 and ADCINT2 trigger source
    Adc2Regs.INTSEL1N2.bit.INT1SEL = 6;      //EOC6 triggers ADCINT1
    Adc2Regs.INTSEL1N2.bit.INT2SEL = 14;     //EOC14 triggers ADCINT2

    // Setup each SOC's ADCINT trigger source
    Adc2Regs.ADCINTSOCSEL1.bit.SOC0  = 2;    //ADCINT2 starts SOC0-7
    Adc2Regs.ADCINTSOCSEL1.bit.SOC1  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC2  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC3  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC4  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC5  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC6  = 2;
    Adc2Regs.ADCINTSOCSEL1.bit.SOC7  = 2;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC8  = 1;    //ADCINT1 starts SOC8-15
    Adc2Regs.ADCINTSOCSEL2.bit.SOC9  = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC10 = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC11 = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC12 = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC13 = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC14 = 1;
    Adc2Regs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // Delay before converting ADC2
                                            // channels

    //ADC2 Conversion

    Adc2Regs.ADCSOCFRC1.all = 0x00FF;  // Force Start SOC0-7 to begin ping-pong
                                       // sampling

    while( index < SampleSize ) {

        //Wait for ADCINT1 to trigger, then add ADCRESULT0-7 registers to sum
        while (Adc2Regs.ADCINTFLG.bit.ADCINT1 == 0) {}
        Adc2Regs.ADCINTFLGCLR.bit.ADCINT1 = 1;   //Must clear ADCINT1 flag since
                                                 // INT1CONT = 0
        Sum += Adc2Result.ADCRESULT0;
        Sum += Adc2Result.ADCRESULT1;
        Sum += Adc2Result.ADCRESULT2;
        Sum += Adc2Result.ADCRESULT3;
        Sum += Adc2Result.ADCRESULT4;
        Sum += Adc2Result.ADCRESULT5;
        Sum += Adc2Result.ADCRESULT6;
        Sum += Adc2Result.ADCRESULT7;

        //Wait for ADCINT2 to trigger, then add ADCRESULT8-15 registers to sum
        while (Adc2Regs.ADCINTFLG.bit.ADCINT2 == 0) {}
        Adc2Regs.ADCINTFLGCLR.bit.ADCINT2 = 1;   //Must clear ADCINT2 flag since
                                                 // INT2CONT = 0
        Sum += Adc2Result.ADCRESULT8;
        Sum += Adc2Result.ADCRESULT9;
        Sum += Adc2Result.ADCRESULT10;
        Sum += Adc2Result.ADCRESULT11;
        Sum += Adc2Result.ADCRESULT12;
        Sum += Adc2Result.ADCRESULT13;
        Sum += Adc2Result.ADCRESULT14;
        Sum += Adc2Result.ADCRESULT15;

        index+=16;

    } // end data collection

    //Disable ADCINT1 and ADCINT2 to STOP the ping-pong sampling
    Adc2Regs.INTSEL1N2.bit.INT1E = 0;
    Adc2Regs.INTSEL1N2.bit.INT2E = 0;

    Mean = Sum / SampleSize;    //Calculate average ADC2 sample value

    return Mean;                //return the average

}



