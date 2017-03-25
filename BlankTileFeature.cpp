#include "BlankTileFeature.h"

BlankTileFeature::BlankTileFeature(float weight):
TileTuple(weight)
{
	score_table_ = new float[16];  // max 16 blank tiles
	memset(score_table_, 0, sizeof(float) * 16);
}

BlankTileFeature::~BlankTileFeature()
{
	delete[] score_table_;
}

unsigned long long BlankTileFeature::get_index(const BitBoard& game_board) const
{
	int index = 0;
	int count = 0;

	int blankCount = 0;
	for(unsigned long long mask = 0xf;mask != 0;mask <<= 4) {
		if(((game_board.getBoard() & mask) >> count) == 0)
			blankCount++;
		count += 4;
	}

	return blankCount;
}

/*int LargeTileFeature::get_isomorphic_indexes(const board_t& original_index, board_t* isomorphic_indexes) const
{
	isomorphic_indexes[0] = original_index;
	return 1;
}*/

void BlankTileFeature::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(float) * 16);
}

void BlankTileFeature::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(float) * 16);
}


