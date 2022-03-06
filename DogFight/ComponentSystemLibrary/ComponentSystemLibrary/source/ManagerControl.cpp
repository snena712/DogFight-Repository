#include "ManagerControl.h"


void ManagerControl::Start()
{
	EnemyManager::Instance()->Start();
	// マネージャー 場所は追々考える
	BulletManager::Instance()->Init();
	ComputeManager::Instance()->Init();
}

void ManagerControl::Reset()
{
	EnemyManager::Instance()->Reset();
	BulletManager::Instance()->Reset();
}

void ManagerControl::Update()
{
	string _name = SceneManager::Instance()->GetActiveScene()->GetSceneName();
	auto state = MainGameManager::GetInstance()->GetSceneState();

	if (state == MainGameManager::SceneState::START)
	{
		// 異なるときの初期化
		if (_name != oldstr)
		{
			Start();
			oldstr = _name;
		}
	}
	else {
		// 文字列のクリア
		oldstr = "";
	}

	if (state != MainGameManager::SceneState::SELECT
		&&state != MainGameManager::SceneState::AWAKE
		&&state != MainGameManager::SceneState::START)
	{
		if (_name != "SelectScene"&&_name != "TitleScene") {

			EnemyManager::Instance()->Update();
			// マネージャー 場所は追々考える
			BulletManager::Instance()->Update();	
		}
	}
	ComputeManager::Instance()->Update();
}