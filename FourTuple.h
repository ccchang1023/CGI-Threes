#ifndef __FOURTUPLE_H__
#define __FOURTUPLE_H__

#include "TileTuple.h"

class FourTuple : public TileTuple
{
public:
	FourTuple(float weight=1);
	~FourTuple();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	virtual unsigned long long get_index(const BitBoard& game_board) const = 0;
};

#endif

