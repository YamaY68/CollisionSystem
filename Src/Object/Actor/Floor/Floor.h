#pragma once
#include "../ActorBase.h"
class Floor :
    public ActorBase
{
public:
	Floor(void);
	Floor(VECTOR size);
	~Floor(void) override;

private:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;
	void InitCollider(void) override;

private:
	VECTOR size_;
};

