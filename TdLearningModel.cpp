#include "TdLearningModel.h"
#include <limits>

int largeValueTile[10] = {0};
ofstream training_record_output_fp("no_split_1000w_rate_lamda.txt",ios::out|ios::app);
bool save_the_record = true;
bool save_collection_board = false;
string save_tuple_name = "no_split_1000w_rate_lamda.tuple";
string start_board_name = "3072.board";
string collection_board_name = "3072.board";
int maxScore_normailize_round = 1000;

TdLearningModel::TdLearningModel()
{
	m_learningBoard.initailBoard();
	m_learningBoard.generateMovingTable();
	m_learningRate = 0.0025;
	m_gameCount = 0;
	m_highestScore = 0;
	m_highestScoreNormalize = 0;
	m_averageScore = 0;
	m_maxTileValue = 0;
	tupleHandler = new TupleNetwork();
	tupleHandler->load_tuple_network(save_tuple_name);
}

void TdLearningModel::startTraining(int Round,bool is_learning_mode)
{	
	// BitBoard* start_board= new BitBoard[110000];
	// ifstream fin(start_board_name.c_str());
	// fin.read((char*)start_board, sizeof(BitBoard) * 110000);
	// m_learningBoard = start_board[0].copyCurrentBoard();
	
	BitBoard* collection_board = new BitBoard[110000];
	int collection_count = 0;
	
	MoveDirection nextMove;
	double average = 0;
	
	BitBoard newBoard;
	newBoard.initailBoard();
	m_learningBoard = newBoard.copyCurrentBoard();
	
	for(int i=0;i<Round;i++)
	{
		while(!m_learningBoard.isGameOver())
		{
			nextMove = getNextBestMove(m_learningBoard);
			m_learningBoard.move(nextMove);
			m_stateStacks.push_back(m_learningBoard);
			m_learningBoard.addTile();
			//m_stateStacks.push_back(m_learningBoard);
			// if(first3072And1536And768Tag == true && Board_3072And1536And768_count < 500000)
			// {
				// if(m_learningBoard.getMaxValueInBoard()>=13 && m_learningBoard.getScore()>= 255879) // >= 177147 + 59049 +19683
				// {
					// Board_start_from_3072And1536And768[Board_3072And1536And768_count++] = m_learningBoard.copyCurrentBoard();
					// first3072And1536And768Tag = false;
				// }
			// }
			// if(m_learningBoard.getMaxValueInBoard()>=13 && !m_learningBoard.isGameOver())	//12 -> 1536, 13-> 3072
			// {
				// collection_board[collection_count++] = m_learningBoard.copyCurrentBoard();
				// break;
			// }			
		}
		average += m_learningBoard.getScore();
		
		/*learning mode on or off*/		
		if(is_learning_mode)
		{
			learn_evaluation_td_lamda();
			//learnEvaluation();
		}	
		
		updateLearningState();		
		
		// /*reset board*/
		// int newIndex = i%100000;
		// m_learningBoard =  start_board[newIndex].copyCurrentBoard();
		
		BitBoard newBoard;
		newBoard.initailBoard();
		m_learningBoard = newBoard.copyCurrentBoard();
		
		if(save_collection_board)
		{
			if(i%1000 == 0)
				cout << "Collection board count " << collection_count << endl;
			if(collection_count>=100000)
				break;
		}

		m_stateStacks.clear();
		
	}
	// cout << "average of 3072 board is "<< average/10000<<endl;
	
	training_record_output_fp.close();
	if(is_learning_mode)
		tupleHandler->save_tuple_network(save_tuple_name);
		
	if(save_collection_board)
	{
		ofstream fout(collection_board_name.c_str());
		fout.write((char*)collection_board, sizeof(BitBoard) * 110000);
	}

}

MoveDirection TdLearningModel::getNextBestMove(BitBoard currentBoard)
{
	BitBoard boardAfterMoving = currentBoard.copyCurrentBoard();
	MoveDirection direction;
	float score = 0;
	float maxScore;
	int maxIndex;
	bool firstLegalDirection = true;
	for(int i=0;i<4;i++)
	{
		direction = static_cast<MoveDirection> (i);
		boardAfterMoving.move(direction);
		score = boardAfterMoving.getScore() - currentBoard.getScore();
		score += evaluateState(boardAfterMoving);
		if(boardAfterMoving.getBoard()==currentBoard.getBoard())
		{
			boardAfterMoving = currentBoard.copyCurrentBoard();
			continue;
		}
		if (firstLegalDirection || maxScore < score)
		{
			maxScore = score;
			maxIndex = i;
			//cout<<"here"<<i<<"score is "<<maxScore<<endl;
		}
		boardAfterMoving = currentBoard.copyCurrentBoard();
		firstLegalDirection = false;
	}
	return static_cast<MoveDirection> (maxIndex);
}

float TdLearningModel::evaluateState(BitBoard board)
{
	return tupleHandler->getBoard_value(board);

	//return 0;
	if(board.getMaxValueInBoard() >= 12) // if tile >= 1536
		return tupleHandlerStep2->getBoard_value(board);
	else
		return tupleHandler->getBoard_value(board);
}

void TdLearningModel::learnEvaluation()
{
	/* (s+1)' - s' */
	BitBoard currentBoard , nextBoard;
	MoveDirection nextMove;
	bool startTrackingBack = false;
	float newScore;
	int reward;
	while(!m_stateStacks.empty())
	{
		currentBoard = m_stateStacks.back();
		m_stateStacks.pop_back();
		if(!startTrackingBack)
		{
			newScore = 0 - evaluateState(currentBoard);
			startTrackingBack = true;
		}
		else
		{
			reward = nextBoard.getScore() - currentBoard.getScore();
			newScore = reward;
			newScore += evaluateState(nextBoard) - evaluateState(currentBoard);	//v(s'next) - v(s')
		}
		updateStateValue(currentBoard, newScore);
		nextBoard = currentBoard;
	}
}

void TdLearningModel::learn_evaluation_td_lamda()
{
	/* (s+1)' - s' */
	BitBoard currentBoard;
	float difference;
	int reward;
	
	//td lamda
	double *nextFiveStateScore = new double[5];
	double *nextFiveR = new double[5];
	int backwardMoveCount = 0;
	int latestIndex = m_stateStacks.size() - 1;
	for(int i=0;i<5;i++)
		nextFiveStateScore[i] = 0;	
	while(latestIndex>=0)
	{		
		currentBoard = m_stateStacks[latestIndex];
		if(backwardMoveCount>0)
		{	
		    nextFiveR[0] = m_stateStacks[latestIndex+1].getScore() - currentBoard.getScore();
			nextFiveStateScore[0] = evaluateState(m_stateStacks[latestIndex+1]) + nextFiveR[0];
		}
		if(backwardMoveCount>1)
		{
			nextFiveR[1] = m_stateStacks[latestIndex+2].getScore() - m_stateStacks[latestIndex+1].getScore();
			nextFiveStateScore[1] = evaluateState(m_stateStacks[latestIndex+2]) + nextFiveR[0] + nextFiveR[1];	
		}			
		if(backwardMoveCount>2)			
		{
			nextFiveR[2] = m_stateStacks[latestIndex+3].getScore() - m_stateStacks[latestIndex+2].getScore();
			nextFiveStateScore[2] = evaluateState(m_stateStacks[latestIndex+3]) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2];
		}			
		if(backwardMoveCount>3)
		{
			nextFiveR[3] = m_stateStacks[latestIndex+4].getScore() - m_stateStacks[latestIndex+3].getScore();
			nextFiveStateScore[3] = evaluateState(m_stateStacks[latestIndex+4]) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2] + nextFiveR[3];
		}			
		if(backwardMoveCount>4)
		{
			nextFiveR[4] = m_stateStacks[latestIndex+5].getScore() - m_stateStacks[latestIndex+4].getScore();
			nextFiveStateScore[4] = evaluateState(m_stateStacks[latestIndex+5]) + nextFiveR[0] + nextFiveR[1] + nextFiveR[2] + nextFiveR[3] + nextFiveR[4];
		}				
		
		double expected_reward = 0.5 * nextFiveStateScore[0] + 0.25 * nextFiveStateScore[1] + 0.125 * nextFiveStateScore[2] +
		                 		 0.0625 * nextFiveStateScore[3] + 0.0625 * nextFiveStateScore[4];	
		difference = expected_reward - evaluateState(currentBoard);		
		updateStateValue(currentBoard, difference);
		backwardMoveCount++;
		latestIndex--;		
	}	
	m_stateStacks.clear();
	delete [] nextFiveStateScore;
	delete [] nextFiveR;	
}


void TdLearningModel::updateStateValue(BitBoard board , float score)
{
	return tupleHandler->update_board_value(board,m_learningRate * score);

	if(board.getMaxValueInBoard() >= 12) // if tile >= 1536
		return tupleHandlerStep2->update_board_value(board,m_learningRate * score);
	else
		return tupleHandler->update_board_value(board,m_learningRate * score);
}

void TdLearningModel::updateLearningState()
{
	m_gameCount++;
	int currentScore = m_learningBoard.getScore();
	int maxTileValue = m_learningBoard.getMaxValueInBoard();
	m_averageScore += currentScore;

	if(currentScore > m_highestScore)
		m_highestScore = currentScore;
		
	if(m_gameCount%1000 < maxScore_normailize_round && currentScore > m_highestScoreNormalize)
		m_highestScoreNormalize = currentScore;
		
	if(maxTileValue > m_maxTileValue)
		m_maxTileValue = maxTileValue;

	if(maxTileValue >= 14)
		largeValueTile[7]++;
	if(maxTileValue >= 13 && currentScore>= 255879)
		largeValueTile[6]++;
	if(maxTileValue >= 13 && currentScore>= 236196)
		largeValueTile[5]++;
	if(maxTileValue >= 13)
		largeValueTile[4]++;
	if(maxTileValue >= 12)
		largeValueTile[3]++;
	if(maxTileValue >= 11)
		largeValueTile[2]++;
	if(maxTileValue >= 10)
		largeValueTile[1]++;
	if(maxTileValue >= 9)
		largeValueTile[0]++;

	if(m_gameCount%1000 == 0)
	{
		m_averageScore /= 1000.0;
		cout<<"\n===========================================\n";
		cout<<"Games Count:  "<<m_gameCount<<endl;
		cout<<"highest score is "<<m_highestScore<<endl;
		cout<<"highest score normalize is "<<m_highestScoreNormalize<<endl;
		cout<<"average score in 1000 games is "<<m_averageScore<<endl;
		cout<<"count of 192 in 1000 games :"<<largeValueTile[0]<<endl;
		cout<<"count of 384 in 1000 games :"<<largeValueTile[1]<<endl;
		cout<<"count of 768 in 1000 games :"<<largeValueTile[2]<<endl;
		cout<<"count of 1536 in 1000 games :"<<largeValueTile[3]<<endl;
		cout<<"count of 3072 in 1000 games :"<<largeValueTile[4]<<endl;
		cout<<"count of 3072 + 1536 in 1000 games :"<<largeValueTile[5]<<endl;
		cout<<"count of 3072 + 1536 + 786 in 1000 games :"<<largeValueTile[6]<<endl;
		cout<<"count of 6144 in 1000 games :"<<largeValueTile[7]<<endl;
		
		if(save_the_record)
			training_record_output_fp << m_averageScore << "\t" << m_highestScore << "\t" << m_highestScoreNormalize << endl;
		
		/*reset counter*/
		m_averageScore = 0;
		m_maxTileValue = 0;
		m_highestScore = 0;
		m_highestScoreNormalize = 0;
		for(int j=0;j<10;j++)
			largeValueTile[j] = 0;
	}
	
}


