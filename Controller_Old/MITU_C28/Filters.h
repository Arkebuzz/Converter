/*
 * Filters.h
 *
 *  Created on: 09.01.2015
 *      Author: VKD
 */

#ifndef FILTERS_H_
#define FILTERS_H_


class SecOrdFilter
{
public:
	volatile float CurrentOutput;

	//constructor
	SecOrdFilter();
	void SecOrdFilterInit(float theA0, float theA1, float theA2, float theA3, float theB0, float theB1, float theC0, float theC1, float theD);
	void Input(float theInput);
	void ResetData();

private:

	volatile float A0;
	volatile float A1;
	volatile float A2;
	volatile float A3;

	volatile float B0;
	volatile float B1;

	volatile float C0;
	volatile float C1;

	volatile float D;

	volatile float X0;
	volatile float X1;

};


#endif /* FILTERS_H_ */
