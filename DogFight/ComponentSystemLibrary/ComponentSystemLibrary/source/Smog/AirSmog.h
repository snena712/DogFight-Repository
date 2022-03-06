#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"

namespace Component {

	class AirSmog : public ComponentBase
	{
	public:
		// �������
		enum class Status
		{
			LIVE,
			DEAD
		};

	private:
		Component::QuadRender* ren;
		Transform* targetTra;

		XMFLOAT4 m_color;			// �F
		float m_height;				// �c
		float m_width;				// ��
		float m_reduce = 0;			// ���l����
		Status status = Status::DEAD;

		// ���l����
		void AlphaReduce() {
			m_color.w -= m_reduce;
			m_width += 0.5f;
			m_height += 0.5f;
			if (m_color.w < 0)
			{
				status = Status::DEAD;
				owner->SetExistState(false);
			}
		}
	public:
		AirSmog(GameObject* _obj);
		~AirSmog();
		void Start();
		void Update();
		void DrawObject();

		Status GetStatus() { return status; }
		void SetStatus(Status _sta) { status = _sta; };
		void SetAlpha(XMFLOAT4 _color) { m_color = _color; }
		void SetSize(float _width, float _height) { m_width = _width; m_height = _height; }
		void SetReduce(float _num) { m_reduce = _num; }
	};

}