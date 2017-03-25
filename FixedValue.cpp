#include "FixedValue.h"

using namespace std;
int FixedValue::scoreTable[65536];
int FixedValue::afterMovingTable[65536];
int FixedValue::valueTable[15] = {0,1,2,3,6,12,24,48,96,192,384,768,1536,3072,6144};
unsigned long long FixedValue::andMask[16] =
{
	0xF000000000000000,
	0x0F00000000000000,
	0x00F0000000000000,
	0x000F000000000000,
	0x0000F00000000000,
	0x00000F0000000000,
	0x000000F000000000,
	0x0000000F00000000,
	0x00000000F0000000,
	0x000000000F000000,
	0x0000000000F00000,
	0x00000000000F0000,
	0x000000000000F000,
	0x0000000000000F00,
	0x00000000000000F0,
	0x000000000000000F,
};
int FixedValue::moveRightMaskOrder[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int FixedValue::moveLeftMaskOrder[16] = {3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12};
int FixedValue::moveDownMaskOrder[16] = {0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
int FixedValue::moveUpMaskOrder[16] = {12,8,4,0,13,9,5,1,14,10,6,2,15,11,7,3};
int FixedValue::moveRightShiftTable[16] = {48,48,48,48,32,32,32,32,16,16,16,16,0,0,0,0};
int FixedValue::moveLeftShiftTable[16] = {36,44,52,60,20,28,36,44,4,12,20,28,-12,-4,4,12};
int FixedValue::moveDownShiftTable[16] = {48,36,24,12,44,32,20,8,40,28,16,4,36,24,12,0};
int FixedValue::moveUpShiftTable[16] = {0,20,40,60,-4,16,36,56,-8,12,32,52,-12,8,28,48};


FixedValue::FixedValue()
{
}

