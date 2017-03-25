#ifndef __TUPLECOLLECTION_H__
#define __TUPLECOLLECTION_H__

#include "FourTuple.h"
#include "SixTuple.h"
#include "SevenTuple.h"

////////////////////////
// line-tuple outside
// O O O O	
// O O O O
// O O O O
// X X X X
class LineTupleOutside : public FourTuple
{
public:
	LineTupleOutside(float weight=1):FourTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return game_board.getBoard() & 0xffff;
	}
};

////////////////////////
// line-tuple inside
// O O O O	
// O O O O
// X X X X
// O O O O
class LineTupleInside : public FourTuple
{
public:
	LineTupleInside(float weight=1):FourTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xffff0000) >> 16;
	}
};

////////////////////////
// square-tuple outside
// O O O O	
// O O O O
// O X X X
// O X X X
class SquareTupleOutside : public SixTuple
{
public:
	SquareTupleOutside(float weight=1):SixTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xfff0000) >> 4 | (game_board.getBoard() & 0xfff);
	}
};

////////////////////////
// square-tuple inside
// O O O O	
// O X X X
// O X X X
// O O O O
class SquareTupleInside : public SixTuple
{
public:
	SquareTupleInside(float weight=1):SixTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xfff00000000) >> 20 | (game_board.getBoard() & 0xfff0000) >> 16;
	}
};

////////////////////////
// small axe-tuple outside
// O O O O	
// O O O O
// O O X X
// X X X X
class SmallAxeTupleOutside : public SixTuple
{
public:
	SmallAxeTupleOutside(float weight=1):SixTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return game_board.getBoard() & 0xffffff;
	}
};

////////////////////////
// small axe-tuple inside
// O O O O	
// O O X X
// X X X X
// O O O O
class SmallAxeTupleInside : public SixTuple
{
public:
	SmallAxeTupleInside(float weight=1):SixTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xffffff0000) >> 16;
	}
};

////////////////////////
// big axe-tuple outside
// O O O O	
// O O O O
// O X X X
// X X X X
class BigAxeTupleOutside : public SevenTuple
{
public:
	BigAxeTupleOutside(float weight=1):SevenTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return game_board.getBoard() & 0xfffffff;
	}
};

////////////////////////
// big axe-tuple inside
// O O O O	
// O X X X
// X X X X
// O O O O
class BigAxeTupleInside : public SevenTuple
{
public:
	BigAxeTupleInside(float weight=1):SevenTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xfffffff0000) >> 16;
	}
};

////////////////////////
// triangle-tuple
// O O O O	
// O O O X
// O O X X
// O X X X
class TriangleTuple : public SevenTuple
{
public:
	TriangleTuple(float weight=1):SevenTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		return (game_board.getBoard() & 0xf00000000) >> 12 | (game_board.getBoard() & 0xff0000) >> 4 | (game_board.getBoard() & 0xfff);
	}
};


////////////////////////
// LHint -tuple
//
// O O O O	
// O O O O  +   1 hint number (1~11)
// O O O X
// X X X X
class LHintTuple : public SixTuple
{
public:
	LHintTuple(float weight=1):SixTuple(weight) { }

protected:
	unsigned long long get_index(const BitBoard& game_board) const 
	{
		int hintTile = game_board.getHintTile();
		hintTile = (hintTile >= 4)?4:hintTile;	// if bonus tile > 6 , only know with '+' symbol
		return (game_board.getBoard() & 0xfffff) | (hintTile<<5);
	}
};


#endif

