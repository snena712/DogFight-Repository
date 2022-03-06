#include "StartUIColorComponent.h"
#include "../../../Component/SceneManager.h"

using namespace Component;
StartUIColorComponent::StartUIColorComponent(GameObject *_owner, float _alpha, RnderState _state) : ComponentBase(_owner)
{
	m_stockAlpha = _alpha;

	switch (_state)
	{
	case Component::StartUIColorComponent::RnderState::NORMAL:
		if (ren == nullptr)
		{
			ren = owner->GetComponent<Component::QuadRender>();
		}

		break;
	case Component::StartUIColorComponent::RnderState::TWO:
		if (ren2d == nullptr)
		{
			ren2d = owner->GetComponent<Component::Quad2DRender>();
		}

		break;
	default:
		break;
	}
}

StartUIColorComponent::~StartUIColorComponent()
{}

void StartUIColorComponent::Update()
{
	GameStateUpdate();

	switch (m_fadeStatus)
	{
	case Fade::FadeIn:

		// 最大値終えたとき変更
		if (m_fadeCnt >= m_fadeMaxCnt)
		{
			m_fadeStatus = Fade::Normal;
			m_fadeCnt = 0;
		}
		else {
			m_fadeCnt++;

			// αの設定
			m_alpha = m_stockAlpha * m_fadeCnt / m_fadeMaxCnt;
		}
		break;

	case Fade::FadeOut:

		// 最大値終えたとき変更
		if (m_fadeCnt >= m_fadeMaxCnt)
		{
			m_fadeStatus = Fade::None;
		}
		else {
			m_fadeCnt++;
			// αの設定
			m_alpha = m_stockAlpha - m_stockAlpha * m_fadeCnt / m_fadeMaxCnt;
		}
		break;

	case Fade::Normal:
		m_alpha = m_stockAlpha;
		break;

	case Fade::None:
		m_alpha = 0;
		break;

	default:
		m_fadeCnt = 0;
		break;
	}

	ColorSet();
}

// 色の設定
void StartUIColorComponent::ColorSet()
{
	if (ren != nullptr)
	{
		ren->SetVertexColor(XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha));
	}
	else if (ren2d != nullptr)
	{
		ren2d->SetVertexColor(XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha));
	}
}

void StartUIColorComponent::GameStateUpdate()
{
	if (!m_fadeFlg) 
	{
		switch (MainGameManager::GetInstance()->GetSceneState())
		{
		case MainGameManager::SceneState::NORMAL:

			SetFadeStatus(Fade::FadeIn);

			break;
		case MainGameManager::SceneState::END:
			SetFadeStatus(Fade::FadeOut);
			break;
		default:
			break;
		}
	}
}

