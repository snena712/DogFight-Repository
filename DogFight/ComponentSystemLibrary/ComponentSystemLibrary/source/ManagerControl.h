#pragma once
#include "Enemy/EnemyManager.h"
#include "Compute/ComputeManager.h"

class ManagerControl
{
private:
	std::string oldstr;
	bool first=false;
public:
	static ManagerControl* Instance() {
		static ManagerControl singleton;
		return &singleton;
	}
	ManagerControl() {};
	~ManagerControl() {};

	void Start();
	void Update();
	void Draw();
	void Reset();

};

