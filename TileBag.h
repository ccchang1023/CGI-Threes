#ifndef	__TILEBAG_H__
#define __TILEBAG_H__

class TileBag
{
public:
	TileBag();
	void reset();
	int getOneTileFromBag(); // return and tile value range from 1 to 3
	bool isBagEmpty();
	void shuffleTileBag();
	void showTheBag();	
private:
	int tilesInBag[12];  /*0 1 2 3 */
	int offset;
};

#endif

