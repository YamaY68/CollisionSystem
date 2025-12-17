#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(void)
{
}

CollisionSystem::~CollisionSystem(void)
{
}

void CollisionSystem::AddDynamicCollider(const std::shared_ptr<ColliderBase>& collider)
{
	dynamicColliders_.push_back(collider);
}

void CollisionSystem::AddStaticCollider(const std::shared_ptr<ColliderBase>& collider)
{
	staticColliders_.push_back(collider);
}

void CollisionSystem::Check(void)
{

}

void CollisionSystem::CollisionCheck(const std::shared_ptr<ColliderBase>& a, const std::shared_ptr<ColliderBase>& b)
{
}
