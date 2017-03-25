#ifndef __MERGEFEATURE_H__
#define __MERGEFEATURE_H__

#include "TileTuple.h"

class MergeFeature : public TileTuple
{
public:
	MergeFeature(float weight=1);
	~MergeFeature();
	// implement interface
	virtual void save_tuple(ofstream& fout) const;
	virtual void load_tuple(ifstream& fin);

protected:
	unsigned long long get_index(const BitBoard& game_board) const;
	int get_isomorphic_indexes(const unsigned long long& original_index, unsigned long long* isomorphic_indexes) const;
};

#endif

