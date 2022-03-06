#include "ManagerControl.h"


void ManagerControl::Start()
{
	EnemyManager::Instance()->Start();
	// �}�l�[�W���[ �ꏊ�͒ǁX�l����
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
		// �قȂ�Ƃ��̏�����
		if (_name != oldstr)
		{
			Start();
			oldstr = _name;
		}
	}
	else {
		// ������̃N���A
		oldstr = "";
	}

	if (state != MainGameManager::SceneState::SELECT
		&&state != MainGameManager::SceneState::AWAKE
		&&state != MainGameManager::SceneState::START)
	{
		if (_name != "SelectScene"&&_name != "TitleScene") {

			EnemyManager::Instance()->Update();
			// �}�l�[�W���[ �ꏊ�͒ǁX�l����
			BulletManager::Instance()->Update();	
		}
	}
	ComputeManager::Instance()->Update();
}