#pragma once
#include<cstdint>
#include<DxLib.h>
#include<memory>
#include"CollisionPairType.h"
#include"../../../Object/Actor/Collider/ColliderShape.h"
class ColliderBase;

class CollisionLogic
{
public:
	struct CollisionResult
	{
		bool isHit = false;

		// 接触情報（共通）
		VECTOR normal;         // A → B 方向の法線
		
		float penetration;    // めり込み量

		float pushA = 0.0f;     // A に適用する押し出し量
		float pushB = 0.0f;     // B に適用する押し出し量
	};
public:
	//コンストラクタ
	CollisionLogic();
	//デストラクタ
	~CollisionLogic();

	//衝突ペア種類取得
	static CollisionPairType GetCollisionPairType(SHAPE shapeA, SHAPE shapeB);
	
	//衝突処理
	static CollisionResult DispatchCollision(
		CollisionPairType pairType,
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);

private:
	//衝突処理用関数群
	//球×球
	static CollisionResult SphereToSphere(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//球×カプセル
	static CollisionResult SphereToCapsule(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//球×ボックス
	static CollisionResult SphereToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//カプセル×カプセル
	static CollisionResult CapsuleToCapsule(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//カプセル×ボックス
	static CollisionResult CapsuleToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);
	//ボックス×ボックス
	static CollisionResult BoxToBox(
		const std::shared_ptr<ColliderBase>& a,
		const std::shared_ptr<ColliderBase>& b);

private:

};

