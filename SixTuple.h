#ifndef __SIXTUPLE_H__
#define __SIXTUPLE_H__

#include "TileTuple.h"

class SixTuple : public TileTuple
{
public:
	SixTuple(float weight=1);
	~SixTuple();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	virtual unsigned long long get_index(const BitBoard& game_board) const = 0;
};

#endif

