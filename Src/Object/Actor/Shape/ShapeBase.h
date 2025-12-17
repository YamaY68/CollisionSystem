#pragma once
#include "../ActorBase.h"
#include<memory>
class ShapeBase :
    public ActorBase
{
public:
	ShapeBase(void);
	~ShapeBase(void) override;

private:
	void SubInit(void) override;
	void SubUpdate(void) override;
	void SubDraw(void) override;
	void SubRelease(void) override;

private:

};

