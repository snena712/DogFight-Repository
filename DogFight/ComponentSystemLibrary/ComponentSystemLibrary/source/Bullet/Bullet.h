#pragma once
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../../Component/QuadRenderComponent.h"
#include "BulletBase.h"
#include "../../Component/SceneManager.h"
#include "../Enemy/Enemy.h"
#include "../UI/WindowUI.h"

namespace Component {
	class Bullet : public ComponentBase
	{
	private:
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::QuadRender* ren;
		Component::BulletBase* bul;
		Component::SphereCollider* col;

		const float m_speed = 20;			// ���x		

		void MoveUpdate();					// �ړ��X�V
		void Collision();					// �����蔻��
		void LifeUpdate();					// �̗͂̊m�F
	public:
		Bullet(GameObject* _obj);
		~Bullet();

		void Start();
		void Update();
	};
}

