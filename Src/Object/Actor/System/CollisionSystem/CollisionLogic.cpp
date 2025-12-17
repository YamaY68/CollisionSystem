#include "CollisionLogic.h"  
#include <type_traits>  
#include <algorithm>  
#include"../../Collider/ColliderBase.h"

CollisionLogic::CollisionLogic()  
{  
}  

CollisionLogic::~CollisionLogic()  
{  
}  

CollisionPairType CollisionLogic::GetCollisionPairType(SHAPE shapeA, SHAPE shapeB)  
{  
   // ìØÇ∂å`èÛìØém  
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

   // èáèòÇãzé˚Åiè¨Ç≥Ç¢ÇŸÇ§ÇAÇ…Åj  
   if (shapeA > shapeB)  
       std::swap(shapeA, shapeB);  

   // àŸÇ»ÇÈå`èÛìØém  
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
    return CollisionResult();
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
