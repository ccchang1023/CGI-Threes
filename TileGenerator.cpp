#include "tileGenerator.h"
#include "time.h"
#include <cstdlib>

int tileGenerator::getNextTile(int maxValueInBoard)
{
	if(isBonusState(maxValueInBoard))
	{
		double probability = (1.0/21);	// BONUS RATIO
		double result = static_cast<double>(rand()) / RAND_MAX;
		if(result <= probability)
			return GetNextTileInBonus(maxValueInBoard);;
	}
	return GetNextTileInNormal();
}

int tileGenerator::GetNextTileInNormal()
{
	return bag.getOneTileFromBag();
}

int tileGenerator::GetNextTileInBonus(int maxValueInBoard)
{
	int maxTileValue = maxValueInBoard-3;   //valueTable from 4 to 11 (6,12,24~768)
	int randIndex = rand()%(maxTileValue-3) + 4;	
	return randIndex;
}


