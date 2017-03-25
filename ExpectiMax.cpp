#include "ExpectiMax.h"

ExpectiMax::ExpectiMax(int search_depth, string tuple_network_file):
search_depth_(search_depth),
	tuple_network_()
{
	tuple_network_.load_tuple_network(tuple_network_file);
	tuple_network_step2.load_tuple_network("./15_1000w_rate_lamda.tuple");
	tuple_network_step3.load_tuple_network("./30_1000w_rate_lamda.tuple");
}


MoveDirection ExpectiMax::find_best_move( BitBoard board)
{
	MoveDirection best_successor;
	bool has_dead_board = false;
	get_best_successor_and_score(board, search_depth_, best_successor, has_dead_board);
	return best_successor;
}

float ExpectiMax::get_best_successor_and_score( BitBoard board, int depth, MoveDirection& best_successor, bool& has_dead_board )
{
	MoveDirection successors[4];
	int successor_count = board.get_successor(successors);
	float best_evaluation_score = 0.0;
	bool is_first_direction = true;
	// handle child node has dead board
	MoveDirection best_successor_with_dead = Right;
	float best_evaluation_score_with_dead = 0.0;
	bool is_first_direction_with_dead = true;

	//int moveReward[4]={0};
	//int moveOrder[4]={0};
	//int bestMoveIndex = 0;


	for(int i = 0;i < successor_count;i++) {
		BitBoard next_board = board.copyCurrentBoard();
		//bool is_legal_move;
		next_board.move(successors[i]);
		int reward = next_board.getScore() - board.getScore();

// 		//////////////////////////
// 		if(depth == search_depth_)
// 		{
// 			moveOrder[i] = i;
// 			moveReward[i] = reward;
// 		}
// 		//////////////////////////


		bool next_tern_has_dead_board = false;
		float evaluation_score = reward + expecti_max_random_turn(next_board, depth - 1, next_tern_has_dead_board);
		if(next_tern_has_dead_board == true) {
			if(is_first_direction_with_dead || evaluation_score > best_evaluation_score_with_dead) {
				best_evaluation_score_with_dead = evaluation_score;
				best_successor_with_dead = successors[i];

				//bestMoveIndex = i;

				is_first_direction_with_dead = false;
			}
			continue;
		}
		if(is_first_direction || evaluation_score > best_evaluation_score) {
			best_evaluation_score = evaluation_score;
			best_successor = successors[i];

			//bestMoveIndex = i;

			is_first_direction = false;
		}
	}
	if(is_first_direction == true) { // all child has dead board
		best_evaluation_score = best_evaluation_score_with_dead;
		best_successor = best_successor_with_dead;
		has_dead_board = true;
	}



// 	if(depth == search_depth_)
// 	{
// 		int temp;
// 		int diffCount;
// 		for(int i=0;i<successor_count;i++)
// 			for(int j=successor_count-1;j>i;j--)
// 			{
// 				if(moveReward[i] < moveReward[j])
// 				{
// 					temp = moveReward[i];
// 					moveReward[i] = moveReward[j];
// 					moveReward[j] = temp;
// 
// 					temp = moveOrder[i];
// 					moveOrder[i] = moveOrder[j];
// 					moveOrder[j] = temp;
// 				}
// 			}
// 		ofstream outputFile("searchObservation.txt",ios::out|ios::app);
// 		if(outputFile.is_open())
// 		{
// 			outputFile<<"\n===========================================\n";
// 			outputFile<<"Best move order in depth one: ";
// 			for(int i=0;i<successor_count;i++)
// 				outputFile<<moveOrder[i]<<" ";
// 
// 			outputFile<<"\nBest move at the end: "<<bestMoveIndex<<endl;
// 
// 			for(diffCount=0;moveOrder[diffCount]!=bestMoveIndex;diffCount++);
// 
// 			outputFile<<"Difference count: "<<diffCount<<endl;
// 		}
// 	}


	return best_evaluation_score;
}

float ExpectiMax::expecti_max_ai_turn( BitBoard board, int depth, bool& has_dead_board )
{
	if(board.isGameOver()) {
		has_dead_board = true;
		return -20000000.0;
		// return 0;
	}
	MoveDirection best_successor;
	return get_best_successor_and_score(board, depth, best_successor, has_dead_board);
}

float ExpectiMax::expecti_max_random_turn( BitBoard board, int depth, bool& has_dead_board )
{
	bool find_in_hash_table = false;
	float evaluate_value = ZHashTable::z_hash_table.get_value(board.getBoard(), depth, has_dead_board, find_in_hash_table);
	if(find_in_hash_table)
		return evaluate_value;

	if(depth <= 0) {
		evaluate_value = evaluate(board);
		ZHashTable::z_hash_table.set_value(board.getBoard(), depth, evaluate_value, has_dead_board);
		return evaluate_value;
	}
	BitBoard possible_boards[45];
	float boards_probability[45];
	int possible_boards_count;
	if(depth==search_depth_-1)
		possible_boards_count =	board.get_all_possible_board(possible_boards,boards_probability,true);
	else
		possible_boards_count =	board.get_all_possible_board(possible_boards,boards_probability,false);


	//int possible_boards_count = 0;
	float total_evaluation_score = 0.0;
	float worst_score = 0;
	for(int i = 0;i < possible_boards_count;i++) {
		bool next_tern_has_dead_board = false;
		float evaluation_score = expecti_max_ai_turn(possible_boards[i], depth - 1, next_tern_has_dead_board);
		if(next_tern_has_dead_board == true)
			has_dead_board = true;
		if(total_evaluation_score == 0 || evaluation_score < worst_score)
			worst_score = evaluation_score;
			//worst_score = evaluation_score * boards_probability[i];
		total_evaluation_score += evaluation_score * boards_probability[i];
	}
	static float alpha = 0.95; //have tried 0.5~0.9
	total_evaluation_score = alpha * total_evaluation_score + (1.0 - alpha) * worst_score;

	ZHashTable::z_hash_table.set_value(board.getBoard(), depth, total_evaluation_score, has_dead_board);
	return total_evaluation_score;
}

float ExpectiMax::evaluate( BitBoard board )
{
	if(board.getMaxValueInBoard()>=13)	// >= 3072
		return tuple_network_step3.getBoard_value(board);
	else if(board.getMaxValueInBoard()>=12)	// >= 1536
		return tuple_network_step2.getBoard_value(board);		
		
	return tuple_network_.getBoard_value(board);
}
