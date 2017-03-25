#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "time.h"
#include "FixedValue.h"
#include "BitBoard.h"



#define FIRST_BYTE  0x000F
#define SECOND_BYTE  0x00F0
#define THIRD_BYTE  0x0F00
#define FOURTH_BYTE  0xF000
#define INITIAL_TILE_NUM 9
#define GRID_SIZE 16



using namespace std;

BitBoard::BitBoard()
{
	//FixedValue::afterMovingTable.reserve(65536);
	//showHex(m_board);
	m_gameScore = 0;
	m_board = 0ull;
	m_maxValueInBoard = 0;
	m_possibleTileCount = 0;
	m_hintTile = 0;
}

BitBoard::BitBoard(unsigned long long sBoard)
{
	m_gameScore = 0;
	m_board = sBoard;
	m_maxValueInBoard = getMaxValueInBoard();
	m_possibleTileCount = 0;
	m_hintTile = 0;
}

BitBoard::~BitBoard()
{
}

void BitBoard::initailBoard()
{
	/*Initialize bitboard*/
	//srand (time(NULL));
	//m_gameScore = 0;
	//m_board = 0ull;
	vector<int> initialTileIndex;
	unsigned long long randNum;
	int walker = 0;

	initialTileIndex.clear();
	while(initialTileIndex.size()!=INITIAL_TILE_NUM)
	{
		do 
		{
			randNum = rand() % 16;
		}while( find(initialTileIndex.begin(), initialTileIndex.end(), randNum)!=initialTileIndex.end() ); // repeat until vector was not contained randNum
		initialTileIndex.push_back(randNum);
	}
	sort(initialTileIndex.begin(),initialTileIndex.end());
	// 	for(int i=0;i<initialTileIndex.size();i++)
	// 		cout<<initialTileIndex[i]<<endl;
	for(int i=0;i<16;i++)
	{
		if(walker<INITIAL_TILE_NUM)
			if(i==initialTileIndex[walker])
			{
				//randNum = getValueFromTable(rand()%3+1);	// initail tiles should be 1 or 2 or 3
				randNum = m_tileGenerator.getNextTile(0);

				if(randNum == 3)
					m_gameScore += 3;
				//cout<<"rand num is"<<randNum<<endl;
				m_board = m_board|randNum;
				walker++;
			}
			if(i!=15)
				m_board = (m_board << 4);
	}
	m_maxValueInBoard = getMaxValueInBoard();
	m_hintTile = m_tileGenerator.getNextTile(0);
}

BitBoard BitBoard::copyCurrentBoard()
{
	BitBoard b_new;
	b_new.m_gameScore = m_gameScore;
	b_new.m_board = m_board;
	b_new.m_maxValueInBoard = m_maxValueInBoard;
	b_new.m_possibleTileCount = m_possibleTileCount;
	b_new.m_tileGenerator = m_tileGenerator;
	b_new.m_hintTile = m_hintTile;
	b_new.m_lastMove = m_lastMove;
	for(int i=0;i<m_possibleTileCount;i++)
	{
		b_new.m_possibleNextTile[i] = m_possibleNextTile[i];
		b_new.m_addInTileValue[i] = m_addInTileValue[i];
	}

	return b_new;
}

void BitBoard::generateMovingTable()
{
 	//vector<unsigned int> resultAfterMoving;
 	//resultAfterMoving.reserve(65535); // totally 15^4 combinations for one row or one column
	unsigned int index;
	unsigned int scoreTableIndex = 0;
	unsigned int previosResult,result;
	int walker,score;
	unsigned int tile[4];
	bool moveTag;

	
	/*calculate all possible results after move right with one row*/
	for(index=0;index<65536;index++)
	{
		tile[3] = (index&FIRST_BYTE);
		tile[2] = (index&SECOND_BYTE)>>4;
		tile[1] = (index&THIRD_BYTE)>>8;
		tile[0] = (index&FOURTH_BYTE)>>12;

		//previosResult = tile[0]|tile[1]|tile[2]|tile[3];
		moveTag = false;
		score = 0;
		//slotTag = false;
		for(int walker=3;walker>0;walker--)
		{
			switch(tile[walker])
			{
			case 0:
				moveTag = true;
				break;
			case 1:case 2:
					if(tile[walker-1] + tile[walker]==3)
					{
						tile[walker] = 3;
						score = 3;
						walker--;
						moveTag = true;
						//slotTag = true;
					}
				break;
			default:	// 3 6 12 ......6144 (tile values)
					if(tile[walker-1] == tile[walker] && tile[walker]<14)
					{
						tile[walker]++;
						//score = FixedValue::valueTable[tile[walker]];
						score = power3(tile[walker]-2) - 2*power3(tile[walker]-3);
						walker--;
						moveTag = true;
						//slotTag = true;
					}
			}

			if(moveTag)
			{
				for(;walker>0;walker--)
					tile[walker] = tile[walker-1];
				tile[0] = 0u;
			}
		}
		FixedValue::scoreTable[index] = score;
		result = (tile[0]<<12)|(tile[1]<<8)|(tile[2]<<4)|(tile[3]);
		FixedValue::afterMovingTable[index] = result;
	}
// 
// 	ofstream outputFile("scoreTable.txt");
// 	if(!outputFile.is_open())
// 	{
// 		cout<<"Fail to open moveTable.txt: "<<endl;
// 		exit(1);
// 	}
//  	for(int i=0;i<resultAfterMoving.size();i++)
// 		outputFile << "0x" << hex << i << " " <<FixedValue::scoreTable[i] << endl;
// 		//outputFile << resultAfterMoving[i] << endl;
// 	outputFile.close();
// 	cout<<"done"<<endl;
}

void BitBoard::move(MoveDirection Direction)
{
	//cout<<"next num is "<<m_tileGenerator.getNextTile(0)<<endl;
	int *maskOrder,*shiftTable;
	unsigned long long tileValue[4],rowValue,boardAfterMoving=0,temp;
	unsigned long long possibleNewTile[4];
	switch(Direction)
	{
	case RIGHT:
		maskOrder = FixedValue::moveRightMaskOrder;
		shiftTable = FixedValue::moveRightShiftTable;
		break;
	case LEFT:
		maskOrder = FixedValue::moveLeftMaskOrder;
		shiftTable = FixedValue::moveLeftShiftTable;
		break;
	case DOWN:
		maskOrder = FixedValue::moveDownMaskOrder;
		shiftTable = FixedValue::moveDownShiftTable;
		break;
	case UP:
		maskOrder = FixedValue::moveUpMaskOrder;
		shiftTable = FixedValue::moveUpShiftTable;
		break;
		
	}

	m_possibleTileCount = temp = 0;
	for(int i=0;i<16;i+=4)
	{
		/*And mask to get one row tile value */
		tileValue[0] = m_board & FixedValue::andMask[maskOrder[i]];
		tileValue[1] = m_board & FixedValue::andMask[maskOrder[i+1]];
		tileValue[2] = m_board & FixedValue::andMask[maskOrder[i+2]];
		tileValue[3] = m_board & FixedValue::andMask[maskOrder[i+3]];

		/*Shift correspond bits number to combine an unsigned int (2^16)*/
	    tileValue[0] = (shiftTable[i]>0)?(tileValue[0]>>shiftTable[i]):(tileValue[0]<<(-1*shiftTable[i]));
		tileValue[1] = (shiftTable[i+1]>0)?(tileValue[1]>>shiftTable[i+1]):(tileValue[1]<<(-1*shiftTable[i+1]));
		tileValue[2] = (tileValue[2]>>shiftTable[i+2]);
		tileValue[3] = (tileValue[3]>>shiftTable[i+3]);

		rowValue = tileValue[0] | tileValue[1] | tileValue[2] | tileValue[3];
		//showHex(rowValue);
		//cout<<"after moving:"<<endl;
		//showHex(resultAfterMoving[rowValue]);
		m_gameScore += FixedValue::scoreTable[rowValue];

		tileValue[3] = FixedValue::afterMovingTable[rowValue] & FIRST_BYTE;
		tileValue[2] = FixedValue::afterMovingTable[rowValue] & SECOND_BYTE;
		tileValue[1] = FixedValue::afterMovingTable[rowValue] & THIRD_BYTE;
		tileValue[0] = FixedValue::afterMovingTable[rowValue] & FOURTH_BYTE;

		if(FixedValue::afterMovingTable[rowValue]!=rowValue)	//have a slot
		{
			//cout<<"slot in "<<(i/4)<<" row "<<endl;
			if(temp==0)
			{
				//temp = m_tileGenerator.getNextTile(m_maxValueInBoard);
				temp = m_hintTile;
//				refreshHintTile();
				temp =temp << 12;
			}
			if((temp>>12) >= 3)
				m_addInTileValue[m_possibleTileCount] = (temp>>12);
			else
				m_addInTileValue[m_possibleTileCount] = 0;

			m_possibleNextTile[m_possibleTileCount++] = (shiftTable[i]>0)?(temp<<shiftTable[i]):(temp>>(-1*shiftTable[i]));
		}

		tileValue[0] = (shiftTable[i]>0)?(tileValue[0]<<shiftTable[i]):(tileValue[0]>>(-1*shiftTable[i]));
		tileValue[1] = (shiftTable[i+1]>0)?(tileValue[1]<<shiftTable[i+1]):(tileValue[1]>>(-1*shiftTable[i+1]));
		tileValue[2] = (tileValue[2]<<shiftTable[i+2]);
		tileValue[3] = (tileValue[3]<<shiftTable[i+3]);

		boardAfterMoving = boardAfterMoving | tileValue[0] | tileValue[1] | tileValue[2] | tileValue[3];
	}

// 	cout<<"possible tile will be ";
// 	for(int i=0;i<possibleCount;i++)
// 		showHex(possibleNextTile[i]);


	m_board = boardAfterMoving;
	m_maxValueInBoard = getMaxValueInBoard();
	m_lastMove = Direction;

// 	showBoard();
// 	cout<<"done"<<endl<<endl;
}

void BitBoard::addTile()
{
	int randIndex;
	if(m_possibleTileCount > 0)
	{
		randIndex = rand() % m_possibleTileCount;
//		cout<<"rand num is "<<randIndex<<endl;
		m_board = m_board | m_possibleNextTile[randIndex];
		if(m_addInTileValue[randIndex]>=3)
			m_gameScore += power3(m_addInTileValue[randIndex]-2);

		refreshHintTile();
	}
	m_maxValueInBoard = getMaxValueInBoard();
	return ;
}

int BitBoard::getMaxValueInBoard()
{
	int max = 0;
	int shiftNum;
	unsigned long long tileValue;
	unsigned long long mask = 0xf000000000000000;
	for(shiftNum = 60 ;shiftNum >= 0;shiftNum-=4,mask=mask>>4)
	{
		tileValue = (m_board&mask)>>shiftNum;
		if(max < tileValue)
			max = tileValue;
	}
	return max;		// 0~14
}

bool BitBoard::isGameOver()
{
	unsigned long long mask = 0xf000000000000000;
	for(;mask!=0;mask=mask>>4)
		if((m_board&mask) == 0)
			return false;

	//boardBeforeMoving = m_board;
	BitBoard boardAfterMoving = copyCurrentBoard();
	MoveDirection direction;
	for(int i=0;i<4;i++)
	{
		direction = static_cast<MoveDirection> (i);
		boardAfterMoving.move(direction);
		//boardAfterMoving.addTile();
		if(boardAfterMoving.m_board!=m_board)
			return false;
		boardAfterMoving = copyCurrentBoard();
	}	
	return true;
}

void BitBoard::showBoard()
{
	ofstream outputFile("boardInfo.txt",ios::out|ios::app);
	int shiftNum;
	unsigned long long tileValue;
	unsigned long long mask = 0xf000000000000000;
	for(shiftNum = 60 ;shiftNum >= 0;shiftNum-=4,mask=mask>>4)
	{
		tileValue = getValueFromTable((m_board&mask)>>shiftNum);
		cout<<tileValue<<"	";
		//outputFile<<tileValue<<"	";
		if(shiftNum%16==0)
			cout<<"\n\n";
			//outputFile<<"\n\n";
			
	}
}

void BitBoard::showHex2(unsigned int input)
{
	stringstream stream;
	stream << std::hex << input;
	std::string result( stream.str() );
	cout <<"0x"<<result<<endl;

	// 	for (int i = 15; i >= 0; i--)
	// 		std::cout << ((input >> i) & 1);
	// 	cout<<endl;
}

void BitBoard::showHex(unsigned long long input)
{
	stringstream stream;
	stream << std::hex << input;
	std::string result( stream.str() );
	cout <<"0x"<<result<<endl;

// 	for (int i = 15; i >= 0; i--)
// 		std::cout << ((input >> i) & 1);
// 	cout<<endl;
}

int BitBoard::power3(int exponent)
{
	if(exponent<0)
		return 0;
	int result = 1;
	for(int i = 0;i < exponent;i++)
		result *= 3;
	return result;
}

void BitBoard::get_isomorphic_boards(BitBoard isomorphic_boards[]) const
{
	BitBoard flip_board = get_flip_board();
	BitBoard mirror_board = get_mirror_board();
	BitBoard flip_diagonal_3_12_board = get_flip_diagonal_3_12_board();
	BitBoard flip_diagonal_0_15_board = get_flip_diagonal_0_15_board();
	isomorphic_boards[0] = *this;
	isomorphic_boards[1] = flip_board;
	isomorphic_boards[2] = mirror_board;
	isomorphic_boards[3] = flip_board.get_mirror_board();
	isomorphic_boards[4] = flip_diagonal_3_12_board;
	isomorphic_boards[5] = flip_diagonal_0_15_board;
	isomorphic_boards[6] = flip_diagonal_3_12_board.get_flip_board();
	isomorphic_boards[7] = flip_diagonal_0_15_board.get_flip_board();
}

BitBoard BitBoard::get_flip_board() const
{
	return (m_board & 0xffff000000000000) >> 48 |
		(m_board & 0x0000ffff00000000) >> 16 |
		(m_board & 0x00000000ffff0000) << 16 |
		(m_board & 0x000000000000ffff) << 48;
}

BitBoard BitBoard::get_mirror_board() const
{
	return (m_board & 0xf000f000f000f000) >> 12 |
		(m_board & 0x0f000f000f000f00) >> 4 |
		(m_board & 0x00f000f000f000f0) << 4 |
		(m_board & 0x000f000f000f000f) << 12;
}

BitBoard BitBoard::get_flip_diagonal_3_12_board() const
{
	return (m_board & 0x000f00f00f00f000) |
		(m_board & 0x00f00f00f0000000) >> 20 |
		(m_board & 0x0000000f00f00f00) << 20 |
		(m_board & 0x0f00f00000000000) >> 40 |
		(m_board & 0x00000000000f00f0) << 40 |
		(m_board & 0xf000000000000000) >> 60 |
		(m_board & 0x000000000000000f) << 60;
}

BitBoard BitBoard::get_flip_diagonal_0_15_board() const
{
	return (m_board & 0xf0000f0000f0000f) |
		(m_board & 0x0f0000f0000f0000) >> 12 |
		(m_board & 0x0000f0000f0000f0) << 12 |
		(m_board & 0x00f0000f00000000) >> 24 |
		(m_board & 0x00000000f0000f00) << 24 |
		(m_board & 0x000f000000000000) >> 36 |
		(m_board & 0x000000000000f000) << 36;
}

void BitBoard::refreshHintTile()
{
	m_hintTile = m_tileGenerator.getNextTile(m_maxValueInBoard);
// 	if(m_hintTile>3)
// 		cout<<m_hintTile<<endl;
}

void BitBoard::getBoardTileInArrayForm(int destinationArray[16])
{
	int shiftNum;
	unsigned long long mask = 0xf000000000000000;
	int count = 0;
	for(shiftNum = 60 ;shiftNum >= 0;shiftNum-=4,mask=mask>>4)
		destinationArray[count++] = (m_board&mask)>>shiftNum;
	return ;
}

int BitBoard::get_successor(MoveDirection successors[])
{
	int successor_count = 0;
	BitBoard boardAfterMoving = copyCurrentBoard();
	MoveDirection direction;
	for(int i=0;i<4;i++)
	{
		direction = static_cast<MoveDirection> (i);
		boardAfterMoving.move(direction);
		if(boardAfterMoving.m_board!=m_board)
			successors[successor_count++] = static_cast<MoveDirection>(direction);
		boardAfterMoving = copyCurrentBoard();
	}	
	return successor_count;
}

int BitBoard::get_all_possible_board(BitBoard boards[], float boardsPossibility[], bool has_hint)
{
	int possible_board_size = 0;
	unsigned long long addInTileMask;
	unsigned long long mask ;
	int rightShift[4] = {12,28,44,60};
	int leftShift[4] = {0,16,32,48};
	int upShift[4] = {0,4,8,12};
	int downShift[4] = {48,52,56,60};
	int *shiftTable;
	int maxTileCount;
	int minTile;
	int addInScore;

	if(m_lastMove == RIGHT)
		shiftTable = rightShift;
	else if(m_lastMove == LEFT)
		shiftTable = leftShift;
	else if(m_lastMove == UP)
		shiftTable = upShift;
	else if(m_lastMove == DOWN)
		shiftTable = downShift;

	if(m_maxValueInBoard>=7)
	{
		maxTileCount = m_maxValueInBoard - 3;
		minTile = 4;
	}
	else
	{
		maxTileCount = 3;
		minTile = 1;
	}

	if(has_hint)
	{
		int hintTile = getHintTile();
		for(int i=0;i<4;i++)
		{
			mask = 0xfull << shiftTable[i];
			if((m_board&mask) != 0)
				continue;
			boards[possible_board_size] = copyCurrentBoard();
			addInTileMask = static_cast<unsigned long long>(hintTile)<<shiftTable[i];
			boards[possible_board_size].m_board |= addInTileMask;
			if(hintTile>=3)
				boards[possible_board_size].setScore(m_gameScore + power3(hintTile-2));
			possible_board_size++;
		}
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			mask = 0xfull << shiftTable[i];
			if((m_board&mask) != 0)
				continue;
			for(int j=minTile;j<=maxTileCount;j++)
			{
				boards[possible_board_size] = copyCurrentBoard();
				addInTileMask = static_cast<unsigned long long>(j)<<shiftTable[i];
				boards[possible_board_size].m_board |= addInTileMask;
				if(j>=3)
					boards[possible_board_size].setScore(m_gameScore + power3(j-2));
				possible_board_size++;
			}
		}
	}
	

 	return possible_board_size;
}



// Face Value:	3	6	12	24	48	 96	  192	384	  768	  1536	  3072	   6144
// Point Value:	3	9	27	81	243	 729  2187	6561  19,683  59,049  177,147  531,441

/*
bool BitBoard::has_1536()
{
	return (m_gameScore >= 59049);
}

bool BitBoard::has_1536_and_768()
{
	return (m_gameScore >= 78732);
}

bool BitBoard::has_3072()
{
	return (m_gameScore >= 177147);
}

bool BitBoard::has_3072_and_1536()
{
	return (m_gameScore >= 236196);
}

bool BitBoard::has_3072_and_1536_and_768()
{
	return (m_gameScore >= 255879);
}
*/











