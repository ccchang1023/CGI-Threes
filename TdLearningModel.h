#ifndef __TDLEARNINGMODEL_H__
#define __TDLEARNINGMODEL_H__
#include "BitBoard.h"
#include "TupleNetwork/TupleNetwork.h"
#include <stack>
#include <vector>

using namespace std;

class TdLearningModel
{
public:
	TdLearningModel();
	void startTraining(int Round,bool is_learning_mode);
	float evaluateState(BitBoard board);
	MoveDirection getNextBestMove(BitBoard currentBoard);
	void learnEvaluation();
	void learn_evaluation_td_lamda();
	void updateStateValue(BitBoard board,float score);
	void updateLearningState();

private:
	vector<BitBoard> m_stateStacks;
	BitBoard m_learningBoard;
	TupleNetwork *tupleHandler;
	TupleNetwork *tupleHandlerStep2;
	float m_learningRate;
	int m_gameCount;
	int m_highestScore;
	int m_highestScoreNormalize;
	float m_averageScore;
	int m_maxTileValue;
};


#endif

