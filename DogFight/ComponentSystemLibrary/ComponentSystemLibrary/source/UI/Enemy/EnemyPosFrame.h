#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/CameraComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../UI/Fade/StartUIColorComponent.h""
#include "../String/StringWorldScreen.h"

namespace Component {
	class EnemyPosFrame :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::Camera* cameraCompo;
		Component::StartUIColorComponent* start;

		Transform* tra;
		Component::Quad2DRender* ren;

		GameObject *Ene;			// �G�̈ʒu
		Vector3 m_screenPos;		//�X�N���[���̈ʒu
		bool m_outside = false;		// ��ʊO

		// �X�N���[���ʒu�擾
		XMVECTOR Screenpos(XMVECTOR World_Pos, Vector3 _pos);	
		
	public:
		EnemyPosFrame(GameObject* _obj);
		~EnemyPosFrame();

		void Start();
		void Update();

		void SetParent(GameObject* _obj) { Ene = _obj; }	// �Q�[���I�u�W�F�N�g�̒��g�n��
		void CheckScreen();									// �ʒu�̌v�Z
		Vector3 GetScreenPos() { return m_screenPos; }		// �X�N���[���ʒu�擾
	};
}
