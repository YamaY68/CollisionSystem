#include "CollisionLogic.h"  
#include <type_traits>  
#include <algorithm>  

#include"../../../Object/Actor/Collider/ColliderInfo.h"
#include"../../../Object/Actor/Collider/ColliderBase.h"

#include"../../../Object/Actor/Collider/ColliderSphere.h"

CollisionLogic::CollisionLogic()  
{  
}  

CollisionLogic::~CollisionLogic()  
{  
}  

CollisionPairType CollisionLogic::GetCollisionPairType(SHAPE shapeA, SHAPE shapeB)  
{  
   // 同じ形状同士  
   if (shapeA == shapeB)  
   {  
       switch (shapeA)  
       {  
       case SHAPE::SPHERE:   return CollisionPairType::SPHERE_SPHERE;  
       case SHAPE::CAPSULE: return CollisionPairType::CAPSULE_CAPSULE;  
       case SHAPE::BOX:     return CollisionPairType::BOX_BOX;  
       default:             return CollisionPairType::NONE;  
       }  
   }  

   // 順序を吸収（小さいほうをAに）  
   if (shapeA > shapeB)  
       std::swap(shapeA, shapeB);  

   // 異なる形状同士  
   if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::CAPSULE)  
       return CollisionPairType::SPHERE_CAPSULE;  

   if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::BOX)  
       return CollisionPairType::SPHERE_BOX;  

   if (shapeA == SHAPE::CAPSULE && shapeB == SHAPE::BOX)  
       return CollisionPairType::CAPSULE_BOX;  

   return CollisionPairType::NONE;  
}  

CollisionLogic::CollisionResult CollisionLogic::DispatchCollision(CollisionPairType pairType, const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)  
{  
	CollisionResult result;
    switch (pairType)
    {
    case CollisionPairType::NONE:
        break;
    case CollisionPairType::SPHERE_SPHERE:
		result=SphereToSphere(a, b);
        break;
    case CollisionPairType::SPHERE_CAPSULE:
		result=SphereToCapsule(a, b);
        break;
    case CollisionPairType::SPHERE_BOX:
		result=SphereToBox(a, b);
        break;
    case CollisionPairType::CAPSULE_CAPSULE:
		result = CapsuleToCapsule(a, b);
        break;
    case CollisionPairType::CAPSULE_BOX:
		result = CapsuleToBox(a, b);
        break;
    case CollisionPairType::BOX_BOX:
		result = BoxToBox(a, b);
        break;
    default:
        break;
    }
	return result;
}

CollisionLogic::CollisionResult
CollisionLogic::SphereToSphere(
    const std::shared_ptr<ColliderBase>& a,
    const std::shared_ptr<ColliderBase>& b)
{
	// 2つの球の衝突判定
    CollisionResult result;

	//念のため型変換
    auto sphereA = std::dynamic_pointer_cast<ColliderSphere>(a);
    auto sphereB = std::dynamic_pointer_cast<ColliderSphere>(b);
	// 型変換に失敗したら衝突判定を行わない
    if (!sphereA || !sphereB) return result;

	// 球の中心位置
    VECTOR posA = a->GetFollow()->pos;
    VECTOR posB = b->GetFollow()->pos;

	// 球の半径
    float rA = sphereA->GetRadius();
    float rB = sphereB->GetRadius();

	// 2つの球の中心間のベクトル差
    VECTOR diff = VSub(posB, posA);

	// 中心間の距離の2乗
    float distSq = diff.x * diff.x + diff.y * diff.y+diff.z*diff.z;
	// 半径の和
    float sumR = rA + rB;
	// 衝突していなければ終了
    if (distSq > sumR * sumR) return result;
	// 衝突している場合の情報を設定
    result.isHit = true;
	// 特殊ケース：中心が重なっている場合
    if (distSq < 0.001f)
    {
        result.normal = VGet(1, 0, 0);
        result.penetration = sumR;
        return result;
    }
	// 通常ケース
    else
    {
		// 押し出し方向と深さの計算
		// 距離の計算
        float dist = sqrtf(distSq);
		// 押し出し方向（法線ベクトル）の計算
        result.normal = VScale(diff, 1.0f / dist);
		// 押し出し深さの計算
        result.penetration = sumR - dist;
    }
    //押し出し計算
    const ColliderInfo& infoA = a->GetColliderInfo();
	const ColliderInfo& infoB = b->GetColliderInfo();

	// トリガー判定
    if (infoA.isTrigger || infoB.isTrigger)
        return result;
	//お互いが静的なら押し出し計算を行わない
    if (!infoA.isDynamic && !infoB.isDynamic)
		return result;
    
	//どちらかが静的ならもう片方を全押し出し
	float weightA = infoA.isDynamic ? infoA.weight : 0.0f;
	float weightB = infoB.isDynamic ? infoB.weight : 0.0f;

    //正規化用比重
	float totalWeight = weightA + weightB;
    result.penetration *= 0.8f;
	result.pushA = result.penetration * (weightB / totalWeight);
	result.pushB = result.penetration * (weightA / totalWeight);


    return result;
}


CollisionLogic::CollisionResult CollisionLogic::SphereToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult CollisionLogic::SphereToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult CollisionLogic::CapsuleToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult CollisionLogic::CapsuleToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult CollisionLogic::BoxToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}
