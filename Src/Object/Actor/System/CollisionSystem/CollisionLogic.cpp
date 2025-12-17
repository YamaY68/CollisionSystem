#include "CollisionLogic.h"  
#include <type_traits>  
#include <algorithm>  

#include"../../Collider/ColliderBase.h"

#include"../../Collider/ColliderSphere.h"

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

CollisionLogic::CollisionResult CollisionLogic::SphereToSphere(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
	CollisionResult result;

	//コライダーをキャスト
	auto sphereA = std::dynamic_pointer_cast<ColliderSphere>(a);
	auto sphereB = std::dynamic_pointer_cast<ColliderSphere>(b);

	//座標取得
	VECTOR posA = a->GetFollow()->pos;
	VECTOR posB = b->GetFollow()->pos;

	//半径取得
	float radiusA = sphereA->GetRadius();
	float radiusB = sphereB->GetRadius();

	//中心間ベクトル
	VECTOR diff = VSub(posB, posA);
	//距離
	float distance = VSize(diff);

	//半径和
	float sumRadius = radiusA + radiusB;

	//衝突判定
    if (distance > sumRadius)return;
	//衝突あり
	result.isHit = true;
	//衝突法線計算
	result.normal = VScale(diff, 1.0f / distance);
    
    //めり込み量
	result.penetration = sumRadius - distance;


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
