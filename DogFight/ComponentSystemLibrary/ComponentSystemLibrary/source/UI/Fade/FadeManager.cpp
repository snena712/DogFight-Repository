#include "FadeManager.h"

using namespace Component;

// gameに置く　instanceする
FadeManager::FadeManager(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("FadeManager");

	tra = owner->transform;
	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;
	tra->position = Vector3(Application::CLIENT_WIDTH / 2, -hei / 2, 1);

	ren = owner->AddComponent<Component::Quad2DRender>("assets/white.bmp");
	ren->SetVertexColor(XMFLOAT4( 0, 0, 0, m_alpha));
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(hei);
	ren->SetWidth(wid);
}

FadeManager::~FadeManager()
{

}

void FadeManager::Start()
{}

void FadeManager::Update()
{
	switch (m_state)
	{
	case Component::FadeManager::FadeState::NONE:
		
		break;
	case Component::FadeManager::FadeState::FADEIN:
		StartFade();
		break;
	case Component::FadeManager::FadeState::FADEOUT:
		EndFade();
		break;
	default:
		break;
	}


	ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
}


bool FadeManager::StartFade()
{
	if (m_alpha < 1)
	{
		m_alpha += 1.0f / m_fadecnt;
	}
	else {
		m_alpha = 1;
		m_state = FadeState::NONE;
		owner->SetExistState(false);

		// 黒くなるとシーン読み込み
		if (!SceneManager::Instance()->ActiveScene(m_scenename)) {
			exit(EXIT_FAILURE);
		}
	}
	ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
}


bool FadeManager::EndFade()
{
	if (m_alpha >0)
	{
		m_alpha -= 1.0f / m_fadecnt;
		ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
		return false;
	}
	else {
		m_alpha = 0;
		ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
		owner->SetExistState(false);

		m_state = FadeState::NONE;
		return true;
	}
}

