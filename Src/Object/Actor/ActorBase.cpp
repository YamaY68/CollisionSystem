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
	SetOwnerActor();
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
	float yVel = newVel.y;
	newVel.y = 0.0f;
	float len = VSize(newVel);
	if (len > maxVelocity_)
	{
		newVel = VScale(newVel, maxVelocity_ / len);
	}
	if (yVel > maxVelocity_)
	{
		yVel = maxVelocity_;
	}
	newVel.y = yVel;
	vel_ = newVel;
}

void ActorBase::SetOwnerActor(void)
{
	for (auto collider : ownColliders_)
	{
		collider.second->SetFollowActor(this);
	}
}


