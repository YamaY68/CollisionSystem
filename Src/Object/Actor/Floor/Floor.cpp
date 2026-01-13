#include "Floor.h"
#include<memory>
#include"../Collider/ColliderBox.h"
Floor::Floor(void)
{
}

Floor::Floor(VECTOR size)
	:
	ActorBase(),
	size_(size)
{
}

Floor::~Floor(void)
{
}

void Floor::SubInit(void)
{
	isDynamic_ = false;
    trans_.pos = VGet(0.0f, size_.y * -1, 0.0f);
}

void Floor::SubUpdate(void)
{
}

void Floor::SubDraw(void)
{
}

void Floor::SubRelease(void)
{
}

void Floor::InitCollider(void)
{
    ColliderInfo info = {
SHAPE::BOX,
&trans_,
TAG::NONE,
Layer::NONE,
0,
VGet(0.0f, 0.0f, 0.0f),
VGet(0.0f, 0.0f, 0.0f),
false,
false,
true,
0.0f
    };
    std::shared_ptr<ColliderBox> collider =
        std::make_shared<ColliderBox>(info, size_);
    ownColliders_.emplace(static_cast<int>(SHAPE::BOX), collider);
}
