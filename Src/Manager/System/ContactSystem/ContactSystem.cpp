#include "ContactSystem.h"

void ContactSystem::OnBeginContact(EntityID a, EntityID b,CollisionResult result)
{
	touching_[a].insert(b);
	touching_[b].insert(a);

	beginEvents_.push_back({ a,b });
}

void ContactSystem::OnEndContact(EntityID a, EntityID b,CollisionResult result)
{
	touching_[a].erase(b);
	touching_[b].erase(a);

 	endEvents_.push_back({ a,b });
}
