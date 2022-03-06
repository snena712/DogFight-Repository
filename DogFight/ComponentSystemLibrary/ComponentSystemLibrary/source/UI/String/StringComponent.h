#pragma once
#include "../../../DX11System/mystring.h"
#include "../../../Component/ComponentBaseClass.h"

namespace Component {
	class  StringComponent :public ComponentBase
	{
	public:
		enum class Fade {
			FadeIn,		// �t�F�[�h�C��
			FadeOut,	// �t�F�[�h�A�E�g
			Normal,		// �`��
			None,		// ��`�掞
		};

	private:

		float m_fadeCnt = 0;			// �t�F�[�h���̃J�E���g
		const float m_fadeMaxCnt = 20;	// �t�F�[�h���̃J�E���g
		float m_alpha = 1.0f;			// �������ߐF
		float m_stockAlpha = 1.0f;		// �������ߐF
		std::string m_stackString;		// �`�敶����
		Vector3 m_pos;					// �傫��
		Vector3 m_size;					// �T�C�Y
		Vector3 m_color;				// �F
		Fade m_fadeStatus= Fade::None;	// �t�F�[�h�̏��
	public:
		StringComponent(GameObject* _obj, float _alpha);
		~StringComponent();

		// �����̊֐�
		void Update();

		// �`��ݒ�p�̊֐�
		void SetString(std::string _str) { m_stackString = _str; }
		void SetColor(Vector3 _col) { m_color = _col; }
		void SetBaseAlpha(float _alpha) { m_alpha = _alpha; }
		void SetPosSize(Vector3 _pos, Vector3 _size) { m_pos = _pos; m_size = _size; }
		
		// �t�F�[�h�̃Z�b�g
		void SetFadeStatus(Fade _fade) { 
			if (_fade == Fade::FadeIn) {
				if (m_fadeStatus != Fade::Normal&&m_fadeStatus != Fade::FadeIn) {
					m_fadeStatus = _fade;
				}
			}
			if (_fade == Fade::FadeOut) {
				if (m_fadeStatus != Fade::None&&m_fadeStatus != Fade::FadeOut) {
					m_fadeStatus = _fade;
				}
			}
		}
		Fade GetFadeStatus() {return m_fadeStatus;}
	};
}

