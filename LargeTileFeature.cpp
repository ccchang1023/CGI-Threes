#include "LargeTileFeature.h"

LargeTileFeature::LargeTileFeature(float weight):
TileTuple(weight)
{
	score_table_ = new float[1024];  // 6144 ~ 384, 0~3 tiles
	memset(score_table_, 0, sizeof(float) * 1024);
}

LargeTileFeature::~LargeTileFeature()
{
	delete[] score_table_;
}

unsigned long long LargeTileFeature::get_index(const BitBoard& game_board) const
{
	static int shift_pos[5] = {0, 2, 4, 6, 8};
	int index = 0;
	int count = 0;
	int tile = 0;
	for(unsigned long long mask = 0xf;mask != 0;mask <<= 4) {
		if((tile = (game_board.getBoard() & mask) >> count) >= 9)
			index += 0x1ull << shift_pos[tile - 9];
		count += 4;
	}
	return index;
}

/*int LargeTileFeature::get_isomorphic_indexes(const board_t& original_index, board_t* isomorphic_indexes) const
{
	isomorphic_indexes[0] = original_index;
	return 1;
}*/

void LargeTileFeature::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(float) * 1024);
}

void LargeTileFeature::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(float) * 1024);
}


