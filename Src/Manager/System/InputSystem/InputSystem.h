#pragma once
#include<vector>
#include<memory>
class ActorBase;

class InputSystem
{
public:
		InputSystem() = default;
		~InputSystem() = default;

		void Update(const std::vector<std::shared_ptr<ActorBase>>& objects);


};

