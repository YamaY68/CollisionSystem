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
	ColliderInit();
	SensorColliderInit();
	SubInit();
}

void ActorBase::Update(void)
{
	SubUpdate();
}

void ActorBase::Draw(void)
{
	SubDraw();
#ifdef _DEBUG
	// 所有しているコライダの描画
	for (const auto& own : ownColliders_)
	{
		own.second->Draw();
	}
#endif // _DEBUG
}

void ActorBase::Release(void)
{
	SubRelease();
	// 自身のコライダ解放
	ownColliders_.clear();
}


const ColliderBase* ActorBase::GetOwnCollider(int key) const  
{  
   if (ownColliders_.count(key) == 0)  
   {  
       return nullptr;  
   }  
   return ownColliders_.at(key).get();
}
void ActorBase::AddHitCollider(const ColliderBase* hitCollider)
{
	for (const auto& c : hitColliders_)
	{
		if (c == hitCollider)
		{
			return;
		}
	}
	hitColliders_.emplace_back(hitCollider);
}

void ActorBase::ClearHitColliders(void)
{
	hitColliders_.clear();
}


