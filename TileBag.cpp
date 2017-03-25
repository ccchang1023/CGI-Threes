#include "TileBag.h"
#include <iostream>
#include <time.h>
#include <cstdlib>

using namespace std;

TileBag::TileBag()
{
	for(int i=0;i<12;i++)
		tilesInBag[i] = (i%3)+1;
	reset();
}

void TileBag::reset()
{
	shuffleTileBag();
	offset = 0;
}

void TileBag::shuffleTileBag()
{
	int i,randomIndex,temp;
	for(i=0;i<12;i++)
	{
		randomIndex = rand()%12;
		temp = tilesInBag[i];
		tilesInBag[i] = tilesInBag[randomIndex];
		tilesInBag[randomIndex] = temp;
	}
}

int TileBag::getOneTileFromBag()
{
	int currentTile = tilesInBag[offset++];
	//showTheBag();
	if(isBagEmpty())
		reset();
	return currentTile;
}

bool TileBag::isBagEmpty()
{
	return (offset==12);
}

void TileBag::showTheBag()
{
	for(int i=offset;i<12;i++)
		cout<<tilesInBag[i]<<"  ";
	cout<<endl;
}

