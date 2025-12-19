#include "CollisionLogic.h"  
#include <type_traits>  
#include <algorithm>  

#include"../../../Object/Actor/Collider/ColliderInfo.h"
#include"../../../Object/Actor/Collider/ColliderBase.h"

#include"../../../Object/Actor/Collider/ColliderSphere.h"
#include"../../../Object/Actor/Collider/ColliderBox.h"

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
    VECTOR diff = VSub(posA, posB);

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
	result.pushA = result.penetration * (weightA / totalWeight);
	result.pushB = result.penetration * (weightB / totalWeight);


    return result;
}


CollisionLogic::CollisionResult CollisionLogic::SphereToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult
CollisionLogic::SphereToBox(
    const std::shared_ptr<ColliderBase>& a,
    const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;

    auto sphere = std::dynamic_pointer_cast<ColliderSphere>(a);
    auto box = std::dynamic_pointer_cast<ColliderBox>(b);
	// 型変換に失敗したら衝突判定を行わない
    if (!sphere || !box) return result;

	// 球の位置と半径
    VECTOR spherePos = a->GetFollow()->pos;
    float  radius = sphere->GetRadius();
	// 箱の位置と半サイズ
    VECTOR boxPos = b->GetFollow()->pos;
    VECTOR half = box->GetHalfSize();

	// ===== ローカル座標系変換 =====
    VECTOR local = VSub(spherePos, boxPos);

	// ===== 最近接点計算 =====
    VECTOR closest = VGet(
        std::clamp(local.x, -half.x, half.x),
        std::clamp(local.y, -half.y, half.y),
        std::clamp(local.z, -half.z, half.z)
    );

	// ===== 距離計算 =====
    VECTOR diff = VSub(local, closest);
    float distSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

	//衝突していなければ終了
    if (distSq > radius * radius)
        return result;

    result.isHit = true;

	//特殊ケース：箱の中に球の中心がある場合
    if (distSq < 1e-6f)
    {
        float dx = half.x - fabsf(local.x);
        float dy = half.y - fabsf(local.y);
        float dz = half.z - fabsf(local.z);

        if (dx < dy && dx < dz)
        {
            result.normal = (local.x > 0) ? VGet(1, 0, 0) : VGet(-1, 0, 0);
            result.penetration = radius + dx;
        }
        else if (dy < dz)
        {
            result.normal = (local.y > 0) ? VGet(0, 1, 0) : VGet(0, -1, 0);
            result.penetration = radius + dy;
        }
        else
        {
            result.normal = (local.z > 0) ? VGet(0, 0, 1) : VGet(0, 0, -1);
            result.penetration = radius + dz;
        }
    }
	// 通常ケース
    else
    {
        float dist = sqrtf(distSq);
        result.normal = VScale(diff, 1.0f / dist);
        result.penetration = radius - dist;
    }

	// 押し出し計算
    const ColliderInfo& infoA = a->GetColliderInfo();
    const ColliderInfo& infoB = b->GetColliderInfo();
	// トリガー判定
    if (infoA.isTrigger || infoB.isTrigger)
        return result;
	// お互いが静的なら押し出し計算を行わない
    if (!infoA.isDynamic && !infoB.isDynamic)
        return result;
	// どちらかが静的ならもう片方を全押し出し
    float wA = infoA.isDynamic ? infoA.weight : 0.0f;
    float wB = infoB.isDynamic ? infoB.weight : 0.0f;
	// 正規化用比重
    float total = wA + wB;
	// 押し出し量計算
    if (total <= 0.0f)
    {
        result.pushA = result.penetration;
        result.pushB = 0.0f;
    }
    else
    {
        result.pushA = result.penetration * (wA / total);
        result.pushB = result.penetration * (wB / total);
    }

    return result;
}

CollisionLogic::CollisionResult CollisionLogic::CapsuleToCapsule(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult CollisionLogic::CapsuleToBox(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
    return CollisionResult();
}

CollisionLogic::CollisionResult
CollisionLogic::BoxToBox(
    const std::shared_ptr<ColliderBase>& a,
    const std::shared_ptr<ColliderBase>& b)
{
    CollisionResult result;

    auto boxA = std::dynamic_pointer_cast<ColliderBox>(a);
    auto boxB = std::dynamic_pointer_cast<ColliderBox>(b);
	// 型変換に失敗したら衝突判定を行わない
    if (!boxA || !boxB) return result;
    
	// 箱の位置と半サイズ
    VECTOR posA = a->GetFollow()->pos;
    VECTOR posB = b->GetFollow()->pos;
    VECTOR halfA = boxA->GetHalfSize();
    VECTOR halfB = boxB->GetHalfSize();

    //軸平行判定
    VECTOR diff = VSub(posB, posA);

	// X軸方向の重なり判定
    float overlapX = (halfA.x + halfB.x) - fabsf(diff.x);
    if (overlapX <= 0.0f) return result;

	// Y軸方向の重なり判定
    float overlapY = (halfA.y + halfB.y) - fabsf(diff.y);
    if (overlapY <= 0.0f) return result;

	// Z軸方向の重なり判定
    float overlapZ = (halfA.z + halfB.z) - fabsf(diff.z);
    if (overlapZ <= 0.0f) return result;

    result.isHit = true;

	// 最小の重なり軸を見つける
    if (overlapX < overlapY && overlapX < overlapZ)
    {
        result.normal = (diff.x > 0) ? VGet(-1, 0, 0) : VGet(1, 0, 0);
        result.penetration = overlapX;
    }
    else if (overlapY < overlapZ)
    {
        result.normal = (diff.y > 0) ? VGet(0, -1, 0) : VGet(0, 1, 0);
        result.penetration = overlapY;
    }
    else
    {
        result.normal = (diff.z > 0) ? VGet(0, 0, -1) : VGet(0, 0, 1);
        result.penetration = overlapZ;
    }

    const ColliderInfo& infoA = a->GetColliderInfo();
    const ColliderInfo& infoB = b->GetColliderInfo();

	// トリガー判定
    if (infoA.isTrigger || infoB.isTrigger)
        return result;

	// お互いが静的なら押し出し計算を行わない
    if (!infoA.isDynamic && !infoB.isDynamic)
        return result;

	// どちらかが静的ならもう片方を全押し出し
    float wA = infoA.isDynamic ? infoA.weight : 0.0f;
    float wB = infoB.isDynamic ? infoB.weight : 0.0f;
	// 正規化用比重
    float total = wA + wB;

	// 押し出し量計算
    if (total <= 0.0f)
    {
        result.pushA = result.penetration;
        result.pushB = 0.0f;
    }
    else
    {
        result.pushA = result.penetration * (wA / total);
        result.pushB = result.penetration * (wB / total);
    }

    return result;
}

