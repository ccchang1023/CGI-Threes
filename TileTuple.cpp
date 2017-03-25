#include "TileTuple.h"

TileTuple::TileTuple(float weight):
Tuple(weight),
score_table_(NULL)
{
}

TileTuple::~TileTuple()
{
}

float TileTuple::query_tuple_value(const BitBoard& game_board) const
{
	return weight_ * score_table_[get_index(game_board)];
}

void TileTuple::update_tuple_value(const BitBoard& game_board, const float& value)
{
	score_table_[get_index(game_board)] += value;
}


