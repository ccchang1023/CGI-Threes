#include "SevenTuple.h"

SevenTuple::SevenTuple(float weight):
TileTuple(weight)
{
	score_table_ = new float[0x10000000];
	memset(score_table_, 0, sizeof(float) * 0x10000000);
}

SevenTuple::~SevenTuple()
{
	delete[] score_table_;
}

void SevenTuple::save_tuple(ofstream& fout) const
{
	fout.write((char*)score_table_, sizeof(float) * 0x10000000);
}

void SevenTuple::load_tuple(ifstream& fin)
{
	fin.read((char*)score_table_, sizeof(float) * 0x10000000);
}

