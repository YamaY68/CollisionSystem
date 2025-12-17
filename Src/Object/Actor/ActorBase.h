#pragma once
#include <map>
#include <vector>
#include"../Common/Transform.h"
#include"Collider/ColliderBase.h"

class AnimationController;

class ActorBase
{
public:
	ActorBase(void);
	virtual ~ActorBase(void) {};

	//読み込み
	void Load(void);
	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);

	Transform& GetTransform(void) { return trans_; }

	void SetEntityId(int id) { entityId_ = id; }
	int GetEntityId(void) const { return entityId_; }

	VECTOR& GetVelocity(void) {return vel_; }

	bool IsDynamic(void) {return isDynamic_; }

	//自身のコライダーリスト取得
	const std::map<int, std::shared_ptr<ColliderBase>>& GetOwnColliders(void) const { return ownColliders_; }

protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void InitCollider(void) {};



protected:
	AnimationController* animationController_ = nullptr;
	Transform trans_;
	
	int entityId_ = -1;
	// 移動前の座標
	VECTOR prevPos_ = { 0,0,0 };
	//移動量
	VECTOR vel_ = { 0,0,0 };
	//向き
	VECTOR dir_ = { 0,0,0 };
	//移動測度
	float speed_=0;

	//動的か静的か
	bool isDynamic_=true;

	//自身のコライダーリスト
	std::map<int, std::shared_ptr<ColliderBase>> ownColliders_;
};

