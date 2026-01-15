#pragma once
#include<vector>
#include<memory>

class ActorBase;

class PhysicsSystem
{
public:

	PhysicsSystem() = default;
	~PhysicsSystem() = default;
	void Update(const std::vector<std::shared_ptr<ActorBase>>& objects);

	//ÅIˆÊ’uŒˆ’è
	void Resolve(const std::vector<std::shared_ptr<ActorBase>>& objects);

private:


};

