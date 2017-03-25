#ifndef __TILETUPLE_H__
#define __TILETUPLE_H__

#include "Tuple.h"

class TileTuple : public Tuple
{
public:
	TileTuple(float weight=1);
	virtual ~TileTuple();
	// implement interface
	virtual float query_tuple_value(const BitBoard& game_board) const;
	virtual void update_tuple_value(const BitBoard& game_board, const float& value);
	virtual void save_tuple(ofstream& fout) const = 0;
	virtual void load_tuple(ifstream& fin) = 0;

protected:
	virtual unsigned long long get_index(const BitBoard& game_board) const = 0;

protected:
	float* score_table_;
};

#endif

