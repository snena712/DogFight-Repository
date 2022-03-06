#include "Bullet.h"

using namespace Component;

Bullet::Bullet(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "Bullet");
	owner->SetTag("Bullet");

	tra = owner->transform;  
	
	// ���j�A�j���[�V����
	model = owner->AddComponent<Component::ModelRender>(
		"assets/Bullet/bullet.fbx",
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps_yellow.hlsl",			// �s�N�Z���V�F�[�_�[
		"");			// �e�N�X�`���̑��݃t�H���_);

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(5);
	col->SetSize(1);

	bul = owner->AddComponent<Component::BulletBase>();
}

Bullet::~Bullet()
{
}

void Bullet::Start() 
{
}

void Bullet::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		switch (bul->GetStatus())
		{
		case Component::BulletBase::Status::Live:

			// �ړ��X�V
			MoveUpdate();

			// �̗͂̊m�F
			LifeUpdate();

			// �����蔻��
			Collision();

			break;

		case Component::BulletBase::Status::Dead:
			break;
		default:
			break;
		}
	}

}

// �ړ��X�V
void Bullet::MoveUpdate()
{
	tra->position.x += m_speed * tra->forward().x;
	tra->position.y += m_speed * tra->forward().y;
	tra->position.z += m_speed * tra->forward().z;
}

// �̗͂̍X�V
void Bullet::LifeUpdate()
{
	// ����
	if (bul->GetLife() > 0)
	{
		bul->SetLife(bul->GetLife() - 1);
	}
	else {

		bul->SetStatus(Component::BulletBase::Status::Dead);
		owner->SetExistState(false);
	}
}

// �����蔻��
void Bullet::Collision()
{
	//for (auto& _rigidlist : *rb->GetCollisionObject())
	//{
	//	//// ��������������
	//	//if (_rigidlist.obj->GetName() == "enemy")
	//	//{
	//	//	// ���݂��Ă���Ƃ�
	//	//	if (_rigidlist.obj->GetExistState() &&
	//	//		_rigidlist.obj->GetComponent<Component::Enemy>()->GetStatus()) 
	//	//	{
	//	//		_rigidlist.obj->GetComponent<Component::Enemy>()->LowerLife(1);
	//	//		m_hitflg = true;
	//	//	}
	//	//	
	//	//}
	//}
}