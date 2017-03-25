#include "IsomorphicTupleContainer.h"

IsomorphicTupleContainer::IsomorphicTupleContainer(float weight):
TupleContainer(weight)
{
}

float IsomorphicTupleContainer::query_tuple_value(const BitBoard& game_board) const
{
	float sum_of_values = 0;
	BitBoard isomorphic_boards[8];
	game_board.get_isomorphic_boards(isomorphic_boards);
	for(int isomorphic_num = 0;isomorphic_num < 8;isomorphic_num++) {
		for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it) {
			sum_of_values += (*it)->query_tuple_value(isomorphic_boards[isomorphic_num]);
		}
	}
	return weight_ * sum_of_values;
}

void IsomorphicTupleContainer::update_tuple_value(const BitBoard& game_board, const float& value)
{
	BitBoard isomorphic_boards[8];
	game_board.get_isomorphic_boards(isomorphic_boards);
	for(int isomorphic_num = 0;isomorphic_num < 8;isomorphic_num++) {
		for(vector<Tuple*>::iterator it = tuples_.begin(); it != tuples_.end(); ++it) {
			(*it)->update_tuple_value(isomorphic_boards[isomorphic_num], value);
		}
	}
}

