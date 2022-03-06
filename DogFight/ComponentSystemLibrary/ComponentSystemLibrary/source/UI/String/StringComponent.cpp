#include "StringComponent.h"

using namespace Component;

StringComponent::StringComponent(GameObject *_owner, float _alpha) : ComponentBase(_owner)
{
	m_stockAlpha = _alpha;
}

StringComponent::~StringComponent()
{}

void StringComponent::Update()
{
	switch (m_fadeStatus)
	{
	case Component::StringComponent::Fade::FadeIn:

		// �ő�l�I�����Ƃ��ύX
		if (m_fadeCnt >= m_fadeMaxCnt)
		{
			m_fadeStatus = Fade::Normal;
			m_fadeCnt = 0;
		}
		else {
			m_fadeCnt++;

			// ���̐ݒ�
			m_alpha = m_stockAlpha * m_fadeCnt / m_fadeMaxCnt;
		}
		break;

	case Component::StringComponent::Fade::FadeOut:

		// �ő�l�I�����Ƃ��ύX
		if (m_fadeCnt >= m_fadeMaxCnt)
		{
			m_fadeStatus = Fade::None;
			m_alpha = 0;
		}
		else {
			m_fadeCnt++;
			// ���̐ݒ�
			m_alpha = m_stockAlpha - m_stockAlpha * m_fadeCnt / m_fadeMaxCnt;
		}
		break;

		// �`�掞
	case Component::StringComponent::Fade::Normal:
		m_alpha = m_stockAlpha;
		break;
		// ��`�掞
	case Component::StringComponent::Fade::None:
		m_alpha = 0;
		break;

	default:
		m_fadeCnt = 0;
		break;
	}

	if (m_alpha != 0) {
		// �����̕`��
		MyString::DrawString(m_pos.x, m_pos.y,
			m_size.x, m_size.y, XMFLOAT4(m_color.x, m_color.y, m_color.z, m_alpha), m_stackString.c_str());
	}
}



