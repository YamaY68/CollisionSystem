#include "RigidBody.h"

void RigidBody::SetBodyType(BodyType type)
{
	type_ = type;
}

void RigidBody::SetMass(float mass)
{
	mass_ = mass;
	invMass_ = (mass_ != 0.0f) ? 1.0f / mass_ : 0.0f;
}

void RigidBody::SetUseGravity(bool useGravity)
{
	useGravity_ = useGravity;
}

void RigidBody::SetVelocity(const VECTOR& velocity)
{
	velocity_ = velocity;
}


RigidBody::BodyType RigidBody::GetBodyType() const
{
	return type_;
}

float RigidBody::GetMass() const
{
	return mass_;
}

float RigidBody::GetInvMass() const
{
	return invMass_;
}

bool RigidBody::IsUseGravity() const
{
	return useGravity_;
}

VECTOR RigidBody::GetVelocity() const
{
	return velocity_;
}

void RigidBody::AddForce(const VECTOR& addForce)
{
	force_ = VAdd(force_, addForce);
}

void RigidBody::ClearForce()
{
	force_ = VGet(0.0f, 0.0f, 0.0f);
}
