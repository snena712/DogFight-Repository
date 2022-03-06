#pragma once
#include "../../Component\GameObject.h"
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../../DX11System\dx11mathutil.h"
#include "BulletBase.h"
#include "../Smog/SmogManager.h"

namespace Component {

	class HommingBullet :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::BulletBase* bul;
		Component::SmogManager* smog;

		Vector3 m_targetPos;			// �^�[�Q�b�g�̈ʒu
		GameObject* m_targetobj;		// �^�[�Q�b�g�̃I�u�W�F�N�g
		bool m_hitflg = false;			// �q�b�g�̃t���O
		const float movrRotMax = 0.1f;	// ��]�̏��

		void MoveUpdate();				// �ړ�
		void Collision();				// �����蔻��
		void SmogUpdate();				// ���̍X�V

	public:
		HommingBullet(GameObject* _obj);
		~HommingBullet();

		void Start();
		void Update();
		void Draw();

		// �����ʒu
		void SetInitialPosition(XMFLOAT3 _pos){ owner->transform->position = _pos; }

		// �p�x�̃Z�b�g
		void SetDirection(XMFLOAT4 _qt){ owner->transform->qt = _qt; }

		// �^�[�Q�b�g�̈ʒu�Z�b�g
		void SetTargetPos(Vector3 _pos){ m_targetPos = _pos; };
		
		//�^�[�Q�b�g�̃I�u�W�F�N�g
		void SetTargetObj(GameObject* _obj){ m_targetobj = _obj; };

		// ���̃Z�b�g
		void SetSmogMana(Component::SmogManager* _smog) { smog = _smog; }

		// �q�b�g�Ǘ�
		bool GetHitFlg() { return m_hitflg; }
		void SetHitFlg(bool _flg) { m_hitflg = _flg; }
	};
}


