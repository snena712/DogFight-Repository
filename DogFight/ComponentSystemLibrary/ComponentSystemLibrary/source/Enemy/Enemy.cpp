#include "Enemy.h"

using namespace Component;

Enemy::Enemy(GameObject *_owner):ComponentBase(_owner)
{
	owner->SetName("enemy");
	owner->SetTag("Enemy");

	// �ʒu
	tra = owner->transform;
	tra->scale = 2;

	// ���f���̐ݒ�
	model = owner->AddComponent<Component::ModelRender>(
		"assets/f1/f1.x",			// ���f���f�[�^			
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/f1/");				// �e�N�X�`���̑��݃t�H���_

	// �����蔻��Əd��
	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();
	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(10);

	// ���
	status = Status::LIVE;

	// �ړ�����
	RandMove();

	// �����̃t���[���ǉ�
	GameObject* _frame = new GameObject();
	frame=_frame->AddComponent<Component::EnemyPosFrame>();  //�R���|�[�l���g�ł��̃G�l�~�[�n��
	_frame->GetComponent< Component::EnemyPosFrame>()->SetParent(owner);
	posFrameCom=_frame->GetComponent< Component::EnemyPosFrame>();
	SceneManager::Instance()->AddObject(_frame, false);

	auto bombobj = new GameObject();
	bomb=bombobj->AddComponent<Component::BombAnim>();  //�R���|�[�l���g�ł��̃G�l�~�[�n��
	SceneManager::Instance()->AddObject(bombobj, false);
	bombobj->SetExistState(false);

	lockui=owner->AddComponent<Component::LockOnUI>();
	lockStr = owner->AddComponent<Component::LockOnString>();
}

Enemy::~Enemy()
{
}

void Enemy::Start()
{
	smogmana= SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();
}

void Enemy::Update()
{
	if (status == Status::LIVE) {

		// �ړ��͈̔͊O�̔���
		OutSide();

		// �ړ�
		MoveUpdate();

		// ��]
		SpinMove();

		//�ۑ����ꂽ		
		frame->CheckScreen();

		// ��ʈʒu
		m_ScreenPos = posFrameCom->GetScreenPos();
		lockStr->SetLockOnPos(m_ScreenPos);
	}

	StatusUpdate();
}

void Enemy::DrawObject()
{
	if (status == Status::LIVE) {
		//GameObject::Draw();
	}
}

// �ړ������̍X�V
void Enemy::RandMove()
{
	switch (RandomClass::GetRandom(0, 7))
	{
	case 0:
		movedir = MoveDir::UP;
		break;
	case 1:
		movedir = MoveDir::DOWN;
		break;
	case 2:
		movedir = MoveDir::UP;
		break;
	case 3:
		movedir = MoveDir::DOWN;
		break;

	case 7:
		if (!turnFlg) {
			movedir = MoveDir::TURUN;
		}
		else {
			turnFlg = true;

			// ���O�o����͕��ʂ̍s���̑j�~
			switch (RandomClass::GetRandom(0, 2))
			{
			case 0:
				movedir = MoveDir::UP;
				break;
			case 1:
				movedir = MoveDir::DOWN;
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}

}

void Enemy::SpinMove()
{
	if (movedir != MoveDir::TURUN)
	{
		if (m_spincnt < m_nowspincnt) {

			m_nowspincnt = 0;
			m_spincnt = RandomClass::GetRandom(30, 60);
			//��]�̐ݒ�
			switch (RandomClass::GetRandom(0, 2))
			{
			case 0:
				spin = Spin::Right;
				break;
			case 1:
				spin = Spin::Right;
				break;
			case 2:
				spin = Spin::None;
				break;
			default:
				break;
			}
		}
		else {
			m_nowspincnt++;

			switch (spin)
			{
			case Component::Enemy::Spin::Right:
				tra->RotateLocalZaxis(1.0f);
				break;
			case Component::Enemy::Spin::Left:
				tra->RotateLocalZaxis(-1.0f);
				break;
			case Component::Enemy::Spin::None:
				break;
			default:
				break;
			}
		}
	}
}

//�ړ������ɑ΂���X�V
void Enemy::MoveUpdate()
{
	switch (movedir)
	{
	case Enemy::MoveDir::UP:
		tra->RotateLocalXaxis(-0.5f);
		break;
	case Enemy::MoveDir::DOWN:
		tra->RotateLocalXaxis(0.5f);
		break;
	case Enemy::MoveDir::RIGHT:
		//tra->RotateYaxis(1.0f);
		break;
	case Enemy::MoveDir::LEFT:
		//tra->RotateLocalYaxis(-1.0f);
		break;
	case Enemy::MoveDir::STRREAT:
		straightSpeed += speed;
		break;
	case Enemy::MoveDir::SLOW:
		straightSpeed -= speed;
		break;

	case MoveDir::TURUN:
		tra->RotateLocalXaxis(-outsidePlusCnt);
		turnCnt += outsidePlusCnt;
		if (turnCnt > turnMaxCnt)
		{
			// �e�t���O�̏�����
			turnCnt = 0;
		}
		break;

	case MoveDir::OUTSIDE:

		if (outsideFlg)
		{
			float t = (float)outsideCnt / 360.0f / 4.0f;
			outsideCnt++;
			tra->LookSlerp(m_centerPos, Vector3(0,1,0), t);
			if (t == 1)
			{
				outsideCnt = 0;
				outsideFlg = false;
				RandMove();
			}
		}

		break;

	default:
		break;
	}

	// �͈͊O����Ȃ���
	if (movedir != MoveDir::OUTSIDE)
	{
		// �ړ��̍X�V
		if (m_movecnt < m_nowmovecnt)
		{
			RandMove();
			m_nowmovecnt = 0;
		}
		else {
			m_nowmovecnt++;
		}
	}

	// �����ړ�
	AutoGo(tra->forward());
}

// �����蔻��
void Enemy::Collision()
{
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		if (_rigidlist.obj->GetTag()== "Bullet")
		{
			// �A�N�e�B�u���̂�
			if (_rigidlist.obj->GetExistState()) {
				_rigidlist.obj->GetComponent<Component::BulletBase>()->SetStatus(BulletBase::Status::Dead);
				_rigidlist.obj->SetExistState(false);
					
				// �e�̃_���[�W
				if (_rigidlist.obj->GetName() == "Bullet")
				{
					LowerLife(2);

					// �G�t�F�N�g
					BulletManager::Instance()->InstaneEffect(_rigidlist.obj->transform->position, Vector3(40, 40, 0));
					BulletManager::Instance()->SetHitFlg(true);
					PlaySound(SOUND_LABEL_SE000);
				}
				else {
					LowerLife(5);

					// �G�t�F�N�g
					BulletManager::Instance()->InstaneEffect(_rigidlist.obj->transform->position, Vector3(50, 50, 0));
					BulletManager::Instance()->SetHitFlg(true);
				}
			}
		}
	}
}

// ��Ԃ̍X�V
void Enemy::StatusUpdate()
{
	switch (status)
	{
	case Enemy::LIVE:
		// �����蔻��
		Collision();
		
		// ��
		SmogFly();

		// ���C�t
		LifeCheck();
		break;

	case Enemy::DEAD:
		// ���S���̍X�V
		DeadUpdate();
		break;
	default:
		break;
	}
}

// ���S�̏���
void Enemy::DeadUpdate()
{
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		if (m_nowdeadcnt == 0) {
			// �t���[���폜�@���������@
			bomb->GetOwner()->SetExistState(true);
			frame->GetOwner()->SetExistState(false);
			lockui->SetAlpha(0);
		}
		m_nowdeadcnt++;
		DeadFly(tra->forward());
	}
	else {
		owner->SetExistState(false);
	}
	// �����̏ꏊ�̍X�V
	bomb->GetOwner()->transform->position = owner->transform->position - owner->transform->forward() * 2;
}

// �����ړ�
void Enemy::AutoGo(XMFLOAT4 _axisZ)
{
	// �ړ��̐���
	if (straightSpeed < m_minSpeed)
	{
		straightSpeed = m_minSpeed;
	}
	else if (straightSpeed > m_maxSpeed) {
		straightSpeed = m_maxSpeed;
	}

	// �ړ�
	if (0 < straightSpeed)
	{
		tra->position.x += _axisZ.x * straightSpeed;
		tra->position.y += _axisZ.y * straightSpeed;
		tra->position.z += _axisZ.z * straightSpeed;
	}
	else
	{
		tra->position.x += _axisZ.x * straightSpeed / 5;
		tra->position.y += _axisZ.y * straightSpeed / 5;
		tra->position.z += _axisZ.z * straightSpeed / 5;
	}
}

// �O�Ƃ̗͔̑�r�Ǝ��S����
void Enemy::LifeCheck()
{
	if (life != oldlife) {
		oldlife = life;
	}

	if (life <= 0)
	{
		status = Status::DEAD;
	}
}

// ���S����
void Enemy::DeadFly(XMFLOAT4 _axisZ)
{
	// �����Ă�����ɗ�����
	straightSpeed = 1;		// ���x�͌��݂̃X�s�[�h
	tra->position.x += _axisZ.x * straightSpeed;
	tra->position.y += _axisZ.y * straightSpeed;
	tra->position.z += _axisZ.z * straightSpeed;

	tra->RotateXaxis(0.5f);

	const int hoge = 10;
	for (int i = 1; i < hoge; i++)
	{// �傫��5�ŉ��ۂ�����
		smogmana->SetEnemyDeadSmog(tra->position - tra->forward() * (straightSpeed) * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
		smogmana->SetEnemyDeadSmog(tra->position - tra->forward() * (straightSpeed) * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
	}
}

// ���ł鎞�̉�
void Enemy::SmogFly()
{
	if (m_maxsmogcnt < m_nowsmogcnt)
	{
		//���J�E���g���Ƃɐ���
		// �傫��5�ŉ��ۂ�����
		m_nowsmogcnt = 0;

		smogmana->SetEnemySmog(tra->position + tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
		smogmana->SetEnemySmog(tra->position - tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
	}
	else {
		m_nowsmogcnt++;
	}
}

// ��ʊO�̏���
void Enemy::OutSide()
{
	// �͈͊O
	if (tra->position.Distance(m_centerPos) > outsideDistance)
	{
		movedir = MoveDir::OUTSIDE;
		outsideFlg = true;
	}
	else {
		outsideCnt = 0;
		outsideFlg = false;
		RandMove();
	}
}

void Enemy::Atack()
{
	BulletManager::Instance()->Fire(tra->qt, tra->position, tra->up(), tra->right(), tra->forward(), &owner->GetTag());
}
