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

