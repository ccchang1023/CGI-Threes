#ifndef __SEVENTUPLE_H__
#define __SEVENTUPLE_H__

#include "TileTuple.h"

class SevenTuple : public TileTuple
{
public:
	SevenTuple(float weight=1);
	~SevenTuple();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	virtual unsigned long long get_index(const BitBoard& game_board) const = 0;
};

#endif

