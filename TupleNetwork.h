#ifndef __TUPLENETWORK_H__
#define __TUPLENETWORK_H__

#include <fstream>
#include <string>
#include "../BitBoard.h"
#include "Tuple.h"
#include "TupleContainer.h"
#include "IsomorphicTupleContainer.h"
#include "TupleCollection.h"
#include "LargeTileFeature.h"
#include "BlankTileFeature.h"
#include "MergeFeature.h"

using std::fstream;
using std::string;

class TupleNetwork
{
public:
	TupleNetwork();
	TupleNetwork(int stepNum);
	~TupleNetwork();
	float getBoard_value(const BitBoard& board) const;
	void update_board_value(const BitBoard& board, const float& value);
	void save_tuple_network(const string& filename) const;
	void load_tuple_network(const string& filename);
	//GameBoard get_masquerade_board(const GameBoard& board) const;
	//static int compare_tile (const void *a, const void *b);

private:
	Tuple* create_isomorphic_tuple_collection() const;

private:
	Tuple* tuples;
};

#endif

