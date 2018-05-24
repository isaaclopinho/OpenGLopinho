#pragma once

class PoolObject
{
public:
	virtual PoolObject *clone(void) const = 0;
	virtual void reset(void) = 0;
};
