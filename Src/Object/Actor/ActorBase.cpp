#include "ActorBase.h"
#include"../Common/AnimationController.h"

ActorBase::ActorBase(void) :
	animationController_(nullptr)
{
}

void ActorBase::Load(void)
{
	SubLoad();
}

void ActorBase::Init(void)
{
	SubInit();
	InitCollider();
}

void ActorBase::Update(void)
{
	SubUpdate();
}

void ActorBase::Draw(void)
{
	SubDraw();
}

void ActorBase::Release(void)
{
	SubRelease();

}

void ActorBase::AddVelocity(const VECTOR& vel)
{
	VECTOR newVel = VAdd(vel_, vel);
	float len = VSize(newVel);
	if (len > maxVelocity_)
	{
		newVel = VScale(newVel, maxVelocity_ / len);
	}
	vel_ = newVel;
}


