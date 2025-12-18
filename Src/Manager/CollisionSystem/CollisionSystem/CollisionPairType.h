#pragma once
//è’ìÀéÌóﬁ
enum class CollisionPairType
{
	NONE = -1,

	SPHERE_SPHERE,
	SPHERE_CAPSULE,
	SPHERE_BOX,

	CAPSULE_CAPSULE,
	CAPSULE_BOX,

	BOX_BOX,
};