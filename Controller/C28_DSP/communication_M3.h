#ifndef COMMUNICATION_M3_H
#define COMMUNICATION_M3_H

short WriteTo_CTOM_MSGRAM_Float(unsigned short offset, float value);
short WriteTo_CTOM_MSGRAM(unsigned short offset, short value);
short ReadFrom_MTOC_MSGRAM(short offset);
float ReadFrom_MTOC_MSGRAM_Float(short offset);

#endif // COMMUNICATION_M3_H
