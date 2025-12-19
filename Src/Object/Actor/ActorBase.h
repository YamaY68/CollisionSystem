#pragma once
#include <map>
#include <vector>
#include<memory>
#include<unordered_map>
#include<typeindex>
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
	void SetVelocity(const VECTOR& vel) { vel_ = vel; }
	//最大測度を超えないように加算
	void AddVelocity(const VECTOR& vel);

	//最大速度を設定取得
	void SetMaxVelocity(float maxVel) { maxVelocity_ = maxVel; }
	float GetMaxVelocity(void) const { return maxVelocity_; }

	bool IsDynamic(void) {return isDynamic_; }

	//自身のコライダーリスト取得
	const std::map<int, std::shared_ptr<ColliderBase>>& GetOwnColliders(void) const { return ownColliders_; }

	//コンポーネント追加
	template<class T>
	void AddComponent(std::shared_ptr<T>component);
	//コンポーネントがあるか
	template<class T>
	bool HasComponent(void);
	//コンポーネント取得
	template<class T>
	T& GetComponent(void);

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
	float maxVelocity_ = 10.0f;
	//動的か静的か
	bool isDynamic_=true;

	//自身のコライダーリスト
	std::map<int, std::shared_ptr<ColliderBase>> ownColliders_;
	//コンポーネントリスト
	std::unordered_map<std::type_index, std::shared_ptr<void>> components_;
};

template<class T>
inline void ActorBase::AddComponent(std::shared_ptr<T> component)
{
	components_[std::type_index(typeid(T))] = component;
}

template<class T>
inline bool ActorBase::HasComponent(void)
{
	
	return components_.find(std::type_index(typeid(T))) != components_.end();
}

template<class T>
inline T& ActorBase::GetComponent(void)
{
	return *std::static_pointer_cast<T>(components_[std::type_index(typeid(T))]);
}
