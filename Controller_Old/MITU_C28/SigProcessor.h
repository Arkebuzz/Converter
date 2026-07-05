/*
 * SigProcessor.h
 *
 *  Created on: 09.01.2015
 *      Author: VKD
 */

#ifndef SIGPROCESSOR_H_
#define SIGPROCESSOR_H_

#include <math.h>

class SigProcessorRMS
{
	private:
	volatile float RMSSQValue;
	volatile unsigned short ValuesRMSCount;
	volatile unsigned short ValuesStored;
	volatile int CurrentPosition;
	volatile float ScaleFactor;
	volatile float SignalOffset;
	volatile float SignalFrequency;
	volatile float MinNonZeroValue;
	volatile unsigned short ZeroCrossNums;
	volatile unsigned short ZeroCrossTimeMeasuring;
	volatile unsigned short IsPositive;
	volatile unsigned short Sequencer;
	volatile float LastPositiveValue;
	volatile float LastNegativeValue;
	volatile double TransitionTime;

	public:
	volatile float MeasuredZeroCrossTime;
	volatile float Value;
	volatile float RMSValue;
	volatile unsigned short GlobalZeroCross;
	volatile double GlobalZeroCrossTime;
	float *ValuesArray;

	SigProcessorRMS()
	{}

	void SigProcessorRMSInit(float Scale, unsigned short StoredVals, unsigned short RMSCalcVals, float *ValuesStorage)
	{
	 ValuesArray = ValuesStorage;
	 IsPositive=0;
	 Sequencer=0;
	 LastPositiveValue=0;
	 LastNegativeValue=0;
	 TransitionTime=0;
	 ZeroCrossTimeMeasuring = 0;
	 GlobalZeroCrossTime = 0.0;
	 GlobalZeroCross = 0;
	 ZeroCrossNums = 0;
	 MinNonZeroValue = 0;
	 SignalOffset = 0;
     ValuesStored = StoredVals;
	 ScaleFactor = Scale;
	 ValuesRMSCount = RMSCalcVals;
	 Value = 0;
	 RMSValue = 0;
	 CurrentPosition = 0;
	}

	~SigProcessorRMS()
	{

	}

	void Input(float InpValue)
	{
		CurrentPosition++;
		if (CurrentPosition>(ValuesStored-1)) CurrentPosition=0;
		int LastRMSPosition;
		LastRMSPosition = CurrentPosition-(int)ValuesRMSCount;
		if (LastRMSPosition<0) LastRMSPosition=LastRMSPosition+ValuesStored;
		Value=InpValue*ScaleFactor+SignalOffset;
		ValuesArray[CurrentPosition]=Value;
		RMSSQValue = RMSSQValue + (Value*Value-ValuesArray[LastRMSPosition]*ValuesArray[LastRMSPosition]);
		RMSValue = sqrt(RMSSQValue/((float)ValuesRMSCount));
	}

	float GetTRMSValue(void)
	{
	int StartCountPosition = CurrentPosition - ValuesRMSCount;
	float TRMSValue = 0;
	if (StartCountPosition<0) {StartCountPosition = StartCountPosition+ValuesStored;}
	for(int i=0; i<ValuesRMSCount; i++)
		{
		TRMSValue = TRMSValue + ValuesArray[StartCountPosition]*ValuesArray[StartCountPosition];
		StartCountPosition++;
		if (StartCountPosition>(ValuesStored-1)) StartCountPosition=0;
		}
	TRMSValue = sqrt(TRMSValue/((float)ValuesRMSCount));
	return TRMSValue;
	}

	void UpdateRMSSQValue(void)
	{
	int StartCountPosition = CurrentPosition - ValuesRMSCount;
	float TRMSValue = 0;
	if (StartCountPosition<0) {StartCountPosition = StartCountPosition+ValuesStored;}
	for(int i=0; i<ValuesRMSCount; i++)
		{
		TRMSValue = TRMSValue + ValuesArray[StartCountPosition]*ValuesArray[StartCountPosition];
		StartCountPosition++;
		if (StartCountPosition>(ValuesStored-1)) StartCountPosition=0;
		}
	RMSSQValue = TRMSValue;
	}

void InlineZeroCrossCalc(void)
	{
		if (CurrentPosition==0)
		{
		GlobalZeroCross = ZeroCrossNums;
		ZeroCrossNums = 0;
		if (ValuesArray[CurrentPosition]>=0) {IsPositive=1;}
		else {IsPositive=0;}
		}

		if (IsPositive==1)
		{
			if (ValuesArray[CurrentPosition]<(-1*MinNonZeroValue))
			{IsPositive=0;}
		}
		else
		{
			if (ValuesArray[CurrentPosition]>MinNonZeroValue)
			{ZeroCrossNums++;IsPositive=1;}
		}
	}

short int InlineZeroCrossTimeCalc(float StepTime)
	{
		int CurrPos =  CurrentPosition;
		int PrevPos = CurrPos-1;
		if (PrevPos<0) {PrevPos=PrevPos+ValuesStored;}
    	short int IsZeroCross=0;

    	if( ((ValuesArray[CurrPos]> 0) && (ValuesArray[PrevPos]<=0)) || ((ValuesArray[PrevPos]< 0) && (ValuesArray[CurrPos]>=0)))
			{
				if (MeasuredZeroCrossTime>0.013 && MeasuredZeroCrossTime<0.02 && ZeroCrossTimeMeasuring!=0)
				{IsZeroCross = 1;}
			}

    	if (IsPositive==1)
    		{
    			if (ValuesArray[CurrentPosition]>MinNonZeroValue)
    			{LastPositiveValue = ValuesArray[CurrentPosition];TransitionTime=0;}
    			else if (ValuesArray[CurrentPosition]<(-1*MinNonZeroValue))
					{
						IsPositive=0;
					}
				else
				{TransitionTime=TransitionTime+StepTime;}

				if(ZeroCrossTimeMeasuring!=0)
				{MeasuredZeroCrossTime=MeasuredZeroCrossTime+StepTime;}

    		}
    	else
			{
				if (ValuesArray[CurrentPosition]<(-1*MinNonZeroValue))
    			{
    			LastNegativeValue = ValuesArray[CurrentPosition];
    			TransitionTime=0;
				if(ZeroCrossTimeMeasuring!=0) {MeasuredZeroCrossTime=MeasuredZeroCrossTime+StepTime;}
				}
				else if (ValuesArray[CurrentPosition]>MinNonZeroValue)
					{
							if(ZeroCrossTimeMeasuring==0)
							{
							ZeroCrossTimeMeasuring = 1;
							MeasuredZeroCrossTime = (TransitionTime+StepTime)*(ValuesArray[CurrPos]/(ValuesArray[CurrPos]-LastNegativeValue));
							}
							else if (MeasuredZeroCrossTime>0.013 && MeasuredZeroCrossTime<0.02)
							{
							MeasuredZeroCrossTime = MeasuredZeroCrossTime + (TransitionTime+StepTime)*(LastNegativeValue/(LastNegativeValue-ValuesArray[CurrPos]));
							GlobalZeroCrossTime = MeasuredZeroCrossTime;
							MeasuredZeroCrossTime = (TransitionTime+StepTime)*(ValuesArray[CurrPos]/(ValuesArray[CurrPos]-LastNegativeValue));
							}
							else
							{
							IsZeroCross=-1;
							ZeroCrossTimeMeasuring=0;
							MeasuredZeroCrossTime=0;
							}
							IsPositive=1;
					}
				else
				{TransitionTime=TransitionTime+StepTime;}
			}

		if (MeasuredZeroCrossTime>0.1)
						{
							MeasuredZeroCrossTime=0;
							ZeroCrossTimeMeasuring = 0;
							GlobalZeroCrossTime = 0;
						}

		return IsZeroCross;
	}

	double GetGlobalZeroCrossTime()
	{
	return GlobalZeroCrossTime;
	}

	float GetInlineCalcFrequency(float StepTime)
	{
	 	float MeasuredFrequency = GlobalZeroCross / (ValuesStored * StepTime);
		return MeasuredFrequency;
	}

	double FrequencyCalcZeroCrossTime(double StepTime)
	{
    unsigned short ZeroCross=0;
    unsigned short Measured=0;
    float ZeroCrossTime=0;
    float MeasuredFrequency=0;
    int CurrPos =  CurrentPosition;
    int PrevPos = CurrPos - 1;
    int i = 0;
    if (PrevPos<0) PrevPos = PrevPos + ValuesStored;
	while (Measured!=1 && i<ValuesStored);
		{
		if (ZeroCross == 1)
			{
				if(((ValuesArray[CurrPos]> MinNonZeroValue) && ValuesArray[PrevPos]<0) || ((ValuesArray[PrevPos]< -1.0*MinNonZeroValue) && (ValuesArray[CurrPos]>0)))
					{
						Measured = 1;
						ZeroCrossTime = ZeroCrossTime + StepTime*(ValuesArray[CurrPos]/(ValuesArray[CurrPos]-ValuesArray[PrevPos]));
						MeasuredFrequency = 1.0/ZeroCrossTime;
					}
				else
					{
					ZeroCrossTime = ZeroCrossTime+StepTime;
					}
			}
		else if (ZeroCross == 0)
			{
			if(((ValuesArray[CurrPos]> MinNonZeroValue) && ValuesArray[PrevPos]<0) || ((ValuesArray[PrevPos]< -1.0*MinNonZeroValue) && (ValuesArray[CurrPos]>0)))
				{
					ZeroCross = 1;
					ZeroCrossTime = ZeroCrossTime + StepTime*(ValuesArray[PrevPos]/(ValuesArray[PrevPos]-ValuesArray[CurrPos]));
				}
			}
		CurrPos = CurrPos - 1;
		if (CurrPos<0) CurrPos = CurrPos + ValuesStored;
		PrevPos = CurrPos - 1;
		if (PrevPos<0) PrevPos = PrevPos + ValuesStored;
		i++;
		}

	return MeasuredFrequency;
	}

	float FrequencyCalcZeroCrossNum(float StepTime)
	{
		unsigned short ZeroCross=0;
		float MeasuredFrequency=0;
		for (int i=1; i<ValuesStored; i++)
			{
			 int CurrPos =  i;
    		 int PrevPos = CurrPos - 1;
			 if(((ValuesArray[CurrPos]> MinNonZeroValue) && ValuesArray[PrevPos]<0) || ((ValuesArray[PrevPos]< -1.0*MinNonZeroValue) && (ValuesArray[CurrPos]>0)))
				{
					ZeroCross = ZeroCross+1;
				}
			}
		MeasuredFrequency = ZeroCross / (ValuesStored * StepTime);
		return MeasuredFrequency;
	}


	int GetCurrentPos()
	{
	return CurrentPosition;
	}

	void SetCurrentPos(int theCurrentPosition)
	{
	CurrentPosition = theCurrentPosition;
	}

	float GetCurrentValue()
	{
	int LastMeasuredPos = CurrentPosition;
	if (LastMeasuredPos<0) {LastMeasuredPos = LastMeasuredPos+ValuesStored;}
	return ValuesArray[LastMeasuredPos];
	}

	float GetPrevValue()
	{
	int LastMeasuredPos = CurrentPosition - 1;
	if (LastMeasuredPos<0) {LastMeasuredPos = LastMeasuredPos+ValuesStored;}
	return ValuesArray[LastMeasuredPos];
	}


	void SetOffset(float offset)
	{
	SignalOffset = offset;
	}

	void SetMinNonZeroValue(float theValue)
	{
	 MinNonZeroValue = fabs(theValue);
	}

	float GetValue(int position)
	{
	return ValuesArray[position];
	}

	void SetAllValues(float SetValue)
	{
		for (int i=0;i<ValuesStored;i++)
		{
		ValuesArray[i] = SetValue;
		}
	Value = SetValue;
	RMSValue = SetValue;
	RMSSQValue = RMSValue*RMSValue*ValuesRMSCount;
	}

};

class SigProcessorMean
{
	private:
	volatile unsigned short ValuesMeanCount;
	volatile unsigned short ValuesStored;
	volatile unsigned short RecalcCycleNum;
	volatile unsigned short RecalcCounter;
	volatile float StepScaleFactor;
	volatile float AccumulatedSum;

	public:
	volatile float Value;
	volatile float MeanValue;
	volatile float SignalOffset;
	volatile float ScaleFactor;
	float *ValuesArray;

	SigProcessorMean()
	{}

	void SigProcessorMeanInit(float Scale, unsigned short StoredVals, unsigned short MeanCalcVals, float *ValuesStorage, unsigned short theRecalcCycleNum)
	{
	 ValuesArray = ValuesStorage;
	 SignalOffset=0;
     ValuesStored = StoredVals;
	 ScaleFactor = Scale;
	 ValuesMeanCount = MeanCalcVals;
	 RecalcCycleNum = theRecalcCycleNum;
	 Value = 0;
	 MeanValue = 0;
	 AccumulatedSum = 0;
	 CurrentPosition = 0;
	 StepScaleFactor = 1.0/ValuesMeanCount;
	 RecalcCounter=theRecalcCycleNum;
	}

	~SigProcessorMean()
	{

	}

	void Input(float InpValue, unsigned short CurrentPosition, unsigned short LastMeanPosition)
	{
		Value=(InpValue)*ScaleFactor+SignalOffset;
		ValuesArray[CurrentPosition]=Value;
		if (RecalcCounter>=ValuesMeanCount)
			{
			AccumulatedSum = AccumulatedSum*StepScaleFactor;
			MeanValue = AccumulatedSum;
			RecalcCounter=0;
			AccumulatedSum = Value;
			}
		else
			{
			RecalcCounter++;
			MeanValue = MeanValue + (Value-ValuesArray[LastMeanPosition])*StepScaleFactor;
			AccumulatedSum = AccumulatedSum + Value;
			}
	}

};


#endif /* SIGPROCESSOR_H_ */
