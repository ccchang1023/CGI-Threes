#include "BitBoard.h"
#include "TileBag.h"
#include "tileGenerator.h"
#include "TdLearningModel.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <fstream>


//#define GRID_LENGTH 4

using namespace std;
int main()
{
	int test;
	unsigned int rand_seed = time(0);
	srand(rand_seed);
	
	ofstream seedFp("30_1000w_rate_lamda_seed",ios::out|ios::app);
	seedFp << rand_seed << endl;
	seedFp.close();
	
 	TdLearningModel td;
 	td.startTraining(5000000,true); // reduce learning rate after 500M training
	cout <<"ver : 30_1000w_rate_lamda after reduce rate"<<endl;
	cin >>test;

	return 0;
}


