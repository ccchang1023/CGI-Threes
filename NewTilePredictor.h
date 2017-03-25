#ifndef	__NEWTILEPREDICTOR_H__
#define __NEWTILEPREDICTOR_H__



class NewTilePredictor
{
public:
	NewTilePredictor();
	~NewTilePredictor();
	void takeOffTile(int Tile);
	float getProbability(int predictTile,int maxTileInBoard); //return the probability of predictTile be chosen in bag
	void resetCount();
	int getTotalCount() {return m_totalCount;}
	int getCountOfTile(int Tile) {return m_normalTileCount[Tile-1];}
private:
	int m_normalTileCount[3];	//3 elements represent 1,2,3 tile's count
	int m_totalCount;

};


#endif

