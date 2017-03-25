#ifndef __LARGETILEFEATURE_H__
#define __LARGETILEFEATURE_H__

#include "TileTuple.h"

class LargeTileFeature : public TileTuple
{
public:
	LargeTileFeature(float weight=1);
	~LargeTileFeature();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	unsigned long long get_index(const BitBoard& game_board) const;
	int get_isomorphic_indexes(const unsigned long long& original_index, unsigned long long* isomorphic_indexes) const;
};

#endif

