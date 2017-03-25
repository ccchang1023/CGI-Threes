#include "NewTilePredictor.h"

NewTilePredictor::NewTilePredictor()
{
	for(int i=0;i<3;i++)
		m_normalTileCount[i] = 4;
	m_totalCount =12;
}

NewTilePredictor::~NewTilePredictor()
{

}

void NewTilePredictor::takeOffTile(int Tile)
{
	if(Tile>=4)
		return ;
	m_normalTileCount[Tile-1]--;
	m_totalCount--;
	if(m_totalCount == 0)
		resetCount();
	return ;
}

float NewTilePredictor::getProbability(int predictTile,int maxTileInBoard) //return the probability of predictTile be chosen in bag
{
	float probabiblity;
	int tileCount;
	if(predictTile>=4)
	{
		probabiblity = 0.0;
		if(maxTileInBoard>=7)	//bonus state
			probabiblity = (1.0) / ((maxTileInBoard-6) * (21.0));
	}
	else
	{
		tileCount = m_normalTileCount[predictTile-1];
		probabiblity = static_cast<float>(tileCount) / m_totalCount;
	}
	return probabiblity;
}

void NewTilePredictor::resetCount()
{
	for(int i=0;i<3;i++)
		m_normalTileCount[i] = 4;
	m_totalCount =12;
}

