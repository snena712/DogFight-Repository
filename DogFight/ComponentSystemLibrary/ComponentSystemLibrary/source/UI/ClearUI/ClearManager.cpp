#include "ClearManager.h"
#include "../../../DX11System/Controller.h"

using namespace Component;

ClearManager::ClearManager(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("ClearManager");

}

ClearManager::~ClearManager()
{

}

void ClearManager::Start()
{
	m_fade = SceneManager::Instance()->GameObjectFindName("FadeManager")->GetComponent<Component::FadeManager>();
}

void ClearManager::Update()
{

	EndUpdate();

}


// 処理の更新
void ClearManager::EndUpdate()
{
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::CLEAR)
	{
		return;
	}

	// 一定カウント敵を見る

	// 一定カウントクリアの描画
	if (m_maxinputcnt > m_nowinputcnt)
	{
		if (m_backui == nullptr) {
			m_backui = new GameObject();
			m_backui->AddComponent<Component::SelectBackUI>();
			m_backui->SetExistState(false);
			SceneManager::Instance()->AddObject(m_backui, false);
			m_backui->GetComponent<Component::FadeComponent>()->SetFade(10);
		}
		m_nowinputcnt++;
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 200,
			Application::Instance()->CLIENT_HEIGHT / 2,
			40, 40, XMFLOAT4(0, 1.0f, 0.0f, 0.7f), "GAME CLEAR");
	}
	if (m_maxinputcnt <= m_nowinputcnt && !onceLoad) {
		onceLoad = true;
		m_nowinputcnt++;
		m_fade->SetFadeInScene(60, "SelectScene");
	}

}
