#ifndef __EXPECTIMAX_H__
#define __EXPECTIMAX_H__

#include "BitBoard.h"
#include "TupleNetwork.h"
#include "ZHashTable.h"
#include <iostream>
#include <string>

using namespace std;

class ExpectiMax
{
public:
	ExpectiMax(int search_depth, string tuple_network_file);
	MoveDirection find_best_move(BitBoard board);
	void set_search_depth(int search_depth) { search_depth_ = search_depth; }
	int get_search_depth() const { return search_depth_; }

private:
	float get_best_successor_and_score(BitBoard board, int depth, MoveDirection& best_successor, bool& has_dead_board);
	float expecti_max_ai_turn(BitBoard board, int depth, bool& has_dead_board);
	float expecti_max_random_turn(BitBoard board, int depth, bool& has_dead_board);
	float evaluate(BitBoard board);

private:
	int search_depth_;
	TupleNetwork tuple_network_;
	TupleNetwork tuple_network_step2;
	TupleNetwork tuple_network_step3;
	// TupleNetwork tuple_network_step4;
	// TupleNetwork tuple_network_step5;
};

#endif
