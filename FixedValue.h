#ifndef __FIXEDVALUE_H__
#define __FIXEDVALUE_H__
#include <iostream>
using namespace std;

class FixedValue
{
public:
	FixedValue();
	static int valueTable[15];
	static int scoreTable[65536];
	static int afterMovingTable[65536];
	static unsigned long long andMask[16];
	static int moveRightMaskOrder[16];
	static int moveLeftMaskOrder[16];
	static int moveDownMaskOrder[16];
	static int moveUpMaskOrder[16];
	static int moveRightShiftTable[16];
	static int moveLeftShiftTable[16];
	static int moveDownShiftTable[16];
	static int moveUpShiftTable[16];
private:
};

#endif

