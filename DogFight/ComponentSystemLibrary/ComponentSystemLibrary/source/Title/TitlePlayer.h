#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../DX11System/DX11util.h"
#include "../Bullet/BulletManager.h"
#include "../../source/ObjectShape/Cone.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../DX11System/mystring.h"

namespace Component {
	class TitlePlayer :public ComponentBase
	{
	private:
		//----�R���|�[�l���g------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::ComputeComponent* comCompo;
		GameObject* fade;

		// �R���g���[���[
		int ControllerNum = -1;
		bool m_inputflg = false;		// �A�����͒�~

		int m_inputcnt = 0;				// �X�V
		const int m_inputmax = 30;		// ����J�E���g

		// �����̐F�̊Ǘ��ϐ�
		const  XMFLOAT4 _base= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);
		XMFLOAT4 m_col2= XMFLOAT4(0.7f, 0.7f, 0.7f, 1);

		//-------�֐�--------//
		void KeyBord();				// �L�[�{�[�h
		void Controller();			// �R���g���[���[	
		void InputCnt();			// ���͂ɉ�����J�E���g�Ǘ�

		void String();
	public:
		TitlePlayer(GameObject* _obj);
		~TitlePlayer();

		//----��{�֐�----//
		void Start();
		void Update();
		void Input();
	};
}