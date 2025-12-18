#pragma once
#include<vector>
#include<memory>
class ActorBase;

class MoveSystem
{
	public:
		void Update(const std::vector<std::shared_ptr<ActorBase>>&objects);

};

