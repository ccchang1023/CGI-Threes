#ifndef __TUPLE_H__
#define __TUPLE_H__

#include "../BitBoard.h"
#include <fstream>
#include <cstdlib>
#include <cstring>

using std::fstream;

class Tuple
{
public:
	Tuple(float weight=1) { weight_ = weight; }
	virtual ~Tuple() {}
	virtual float query_tuple_value(const BitBoard& game_board) const = 0;
	virtual void update_tuple_value(const BitBoard& game_board, const float& value) = 0;
	virtual void save_tuple(ofstream& fout) const = 0;
	virtual void load_tuple(ifstream& fin) = 0;

protected:
	float weight_;
};

#endif

