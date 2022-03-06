#include "HommigBullet.h"
#include "../Enemy/Enemy.h"

using namespace Component;

HommingBullet::HommingBullet(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName( "HomingBullet");
	owner->SetTag("Bullet");
	tra = owner->transform;

	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2/f2a_misairu.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",				// ���_�V�F�[�_�[
		"shader/ps.hlsl",				// �s�N�Z���V�F�[�_�[
		"assets/F2/f2a_texture/");		// �e�N�X�`���̑��݃t�H���_

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	Component::SphereCollider* col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(5);
	col->SetSize(5);

	bul= owner->AddComponent < Component::BulletBase >();

}

HommingBullet::~HommingBullet()
{
}

void HommingBullet::Start()
{

}

void HommingBullet::Update()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		switch (bul->GetStatus())
		{
		case Component::BulletBase::Status::Live:

			// �ړ�
			MoveUpdate();

			// ������
			SmogUpdate();

			// �����蔻��
			Collision();

			break;

		case Component::BulletBase::Status::Dead:

			// �A�j���[�V�������I���܂ō폜�҂�
			bul->SetStatus(Component::BulletBase::Status::Dead);
			bul->SetSpeed(0);
			m_targetobj = nullptr;
			m_targetPos = Vector3(0, 0, 0);
			break;
		default:
			break;
		}
	}
}

// �`��
void HommingBullet::Draw()
{
	// bullet�Ɠ����ɂ���
}

// �ړ��̍X�V
void HommingBullet::MoveUpdate()
{
	Vector3 f = tra->forward();
	Vector3 d = m_targetPos - owner->transform->position;

	if (m_targetobj != nullptr) {
		m_targetPos = m_targetobj->transform->position;
		f = tra->forward();
		d = m_targetPos - owner->transform->position;
	}

	if (f.Dot(d) < 0)
	{
		// �^�[�Q�b�g����
	}
	else {
		if (m_targetobj != nullptr) {
			// �|�W�V�����̍X�V���Ȃ�
			owner->transform->LookSlerp(m_targetPos, owner->transform->up(), movrRotMax);
		}
	}

	bul->SetSpeed(bul->GetSpeed() + 0.1f);	// �X�s�[�h�̑��
	owner->transform->position += tra->forward() * bul->GetSpeed();

	// �e�̃��C�t�̏�ԑJ��
	if (bul->GetLife() > 0)
	{
		bul->SetLife(bul->GetLife() - 1);
	}
	else {
		
		bul->SetStatus(Component::BulletBase::Status::Dead);
		owner->SetExistState(false);
	}
}

// ���̍X�V
void HommingBullet::SmogUpdate()
{
	smog->SetActive(owner->transform->position - owner->transform->forward() * 6- owner->transform->up()/2, Vector3(2, 2, 1), XMFLOAT4(1, 1, 1, 0.7f), Vector3(0.05f, 0.05f, 0.05f), 0.01f);
}

void HommingBullet::Collision()
{
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		// ��������������
		//if (_rigidlist.obj->GetName() == "enemy")
		//{
		//	// ���݂��Ă���Ƃ�
		//	if (_rigidlist.obj->GetExistState() &&
		//		_rigidlist.obj->GetComponent<Component::Enemy>()->GetStatus())
		//	{
		//		_rigidlist.obj->GetComponent<Component::Enemy>()->LowerLife(100);
		//		m_hitflg = true;
		//	}

		//}
	}
}