#ifndef __ISOMORPHICTUPLECONTAINER_H__
#define __ISOMORPHICTUPLECONTAINER_H__

#include "TupleContainer.h"

class IsomorphicTupleContainer : public TupleContainer
{
public:
	IsomorphicTupleContainer(float weight=1);
	// implement interface
	virtual float query_tuple_value(const BitBoard& game_board) const;
	virtual void update_tuple_value(const BitBoard& game_board, const float& value);
};

#endif

