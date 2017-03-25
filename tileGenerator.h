#ifndef __TILEGENERATOR_H__
#define __TILEGENERATOR_H__
#include "TileBag.h"
#include <iostream>
using namespace std;

class tileGenerator
{
public:
	int getNextTile(int maxValueInBoard);
	int GetNextTileInNormal();
	int GetNextTileInBonus(int maxValueInBoard);
	bool isBonusState(int maxValueInBoard) {return (maxValueInBoard>=7);} //if tileValue >= 48
private:
	TileBag bag;	//normal bag
};


#endif

