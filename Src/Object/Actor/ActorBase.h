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

	const Transform& GetTransform(void) { return trans_; }

	// 自身の衝突情報取得
	const std::map<int, std::shared_ptr<ColliderBase>>& GetOwnColliders(void) const
	{
		return ownColliders_;
	}
	// 特定の自身の衝突情報取得
	const ColliderBase* GetOwnCollider(int key) const;

	void SetEntityId(int id) { entityId_ = id; }
	int GetEntityId(void) const { return entityId_; }

	VECTOR& GetVelocity(void) {return vel_; }

	bool IsDynamic(void) {return isDynamic_; }
	// 衝突対象のコライダーを追加
	void AddHitCollider(const ColliderBase* hitCollider);

	//衝突対象のコライダーをクリア
	void ClearHitColliders(void);
protected:
	virtual void SubLoad(void) {};
	virtual void SubInit(void) {};
	virtual void SubUpdate(void) {};
	virtual void SubDraw(void) {};
	virtual void SubRelease(void) {};

	virtual void ColliderInit() {};
	virtual void SensorColliderInit() {};

protected:
	AnimationController* animationController_ = nullptr;
	Transform trans_;

	// 自身の衝突情報
	std::map<int, std::shared_ptr<ColliderBase>> ownColliders_;

	//衝突対象の情報
	std::vector<const ColliderBase*> hitColliders_;
	
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
	bool isDynamic_=false;

protected:

};

