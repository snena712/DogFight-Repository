#include "BulletManager.h"
#include "../InOutCheck/SimpleMath.h"
#include "../InOutCheck/insightcheck.h"
using namespace Component;

// �������̓V�[���ōs��
void BulletManager::Init()
{   
	m_bulletlist.clear();
	m_hominglist.clear();

    std::vector<GameObject*> _list;

	// �e������
    for (int i = 0; i < m_bulletnum; i++)
    {
        GameObject* _bullet = new GameObject();
        _bullet->transform->position.x = 10000;
		_bullet->AddComponent<Component::Bullet>();
        m_bulletlist.emplace_back(_bullet);
        _list.emplace_back(_bullet);
        _bullet->SetExistState(false);
    }


	// �z�[�~���O������
    for (int i = 0; i < m_homingnum; i++)
    {
        GameObject* _bullet = new GameObject();
        _bullet->transform->position.x = 10000;
        _bullet->AddComponent<Component::HommingBullet>();
        m_hominglist.emplace_back(_bullet);
        _list.emplace_back(_bullet);
        _bullet->SetExistState(false);
    }

    SceneManager::Instance()->AddList(&_list);

	// ���ˌ�������
	flash = new GameObject();
    flash->AddComponent<Component::BulletFlash>();
	SceneManager::Instance()->AddObject(flash, false);
	flash->SetExistState(false);


	// �G�t�F�N�g�p
	m_effectmana = new GameObject();
	m_effectmana->AddComponent<Component::BulletEffectManager>();
	SceneManager::Instance()->AddObject(m_effectmana, false);
}

// �폜�쐬
BulletManager::~BulletManager()
{
}

void BulletManager::Reset()
{
	// �e��ϐ��̏�����
	initFlg = false;
	m_bulletlist.clear();
	m_hominglist.clear();
}


void BulletManager::Start()
{
	if (!initFlg)
	{
		auto hoge = SceneManager::Instance()->GameObjectFindName("SmogManager");
		if (hoge == NULL)return;

		// �X���b�O�̃}�l�[�W���[�ݒ�
		SmogManager* smog = SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();
		for (auto list : m_hominglist)
		{
			list->GetComponent<Component::HommingBullet>()->SetSmogMana(smog);
		}

		window = SceneManager::Instance()->GameObjectFindName("WindowUI");
		initFlg = true;
	}
}

void BulletManager::Update()
{
	Start();

	if (initFlg) {
		BulletCntUpdate();
		GetDegree(_player,NULL);
		HitCheck();
	}
}

// �ʏ�e
void BulletManager::Fire(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str)
{
	switch (bullettype)
	{
	case BulletManager::BulletType::Bullet:
		FireBullet(_qt, _pos, _right, _up, _forward, _str);
		break;
	case BulletManager::BulletType::Homing:
		FireHoming(_qt, _pos, _right, _up, _forward);
		break;
	default:
		break;
	}
}

// �ʏ�̒e
void BulletManager::FireBullet(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str)
{
	if (bullettype == BulletType::Bullet&&m_nowbulletcnt > m_bulletcnt) {
		// �J�E���g���Z�b�g
		m_nowbulletcnt = 0;

		// �U��
		Controller::Instance()->SetShakeController(0.6f, 1, 0.6f, 1);

		// �T��
		for (auto list : m_bulletlist) {

			if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead
				&& !list->GetExistState())
			{
				list->SetExistState(true);

				list->GetComponent<Component::BulletBase>()->SetStatus(Component::BulletBase::Status::Live);


				// �E
				if (m_bulletAllCnt % 2 == 0) {
					list->transform->position = _pos + _right * 10 - _up * 3 + _forward * 8;

					// ���ˌ�
					flash->GetComponent<Component::BulletFlash>()->SetInitialPosition(_pos + _right * 8 + _forward * 3 - _up);
					flash->GetComponent<Component::BulletFlash>()->SetLife(1);
				}// ��
				else {

					list->transform->position = _pos - _right * 10 - _up * 3 + _forward * 8;

					// ���ˌ�
					flash->GetComponent<Component::BulletFlash>()->SetInitialPosition(_pos - _right * 8 + _forward * 3 - _up);
					flash->GetComponent<Component::BulletFlash>()->SetLife(1);
				}
				m_bulletAllCnt++;

				// �������֐����ׂ�
				list->transform->qt = _qt;
				list->GetComponent<Component::BulletBase>()->SetLife(60);
				PlaySound(SOUND_LABEL_SE000);

				break;
			}
		}
	}
}

// �ǔ��e
void BulletManager::FireHoming(XMFLOAT4 _qt, Vector3 _position, Vector3 _right, Vector3 _up, Vector3 _forward)
{
    // �V�[���}�l�[�W���[���X�g�ɒǉ�
    // ������
    if (bullettype == BulletType::Homing) {
        if (m_nowhomingcnt > m_maxhomingtcnt) {

			// �J�E���g���Z�b�g
			m_nowhomingcnt = 0;
            
			XMFLOAT4 _qtcopy = _qt;
            Vector3 _pos = _position;
         
			// �U��
			Controller::Instance()->SetShakeController(1, 2, 1, 2);

            for (auto list : m_hominglist)
            {
				if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead
					&& !list->GetExistState())
				{
					list->SetExistState(true);
					// ���˂̐ݒ�
					list->GetComponent<Component::BulletBase>()->SetStatus(Component::BulletBase::Status::Live);

					if (m_bulletAllCnt % 2 == 0) {
						list->transform->position = _pos + _right * 10 - _up * 3 + _forward * 8;
						// list->transform->position = _pos - _tra->up() * 10;
					}
					else {

						list->transform->position = _pos - _right * 10 - _up * 3 + _forward * 8;
					}
					m_bulletAllCnt++;

					list->transform->qt = _qtcopy;

					// �ǐՂ���G
					Vector3 _tpos;
					GameObject* _stack = nullptr;

					// �z�[�~���O�\
					if (stackEnemy != nullptr && m_lockonOk) {
						_tpos = stackEnemy->transform->position;
						_stack = stackEnemy;
					}

					PlaySound(SOUND_BULLET_SE002);
					list->GetComponent<Component::HommingBullet>()->SetTargetObj(_stack);
					list->GetComponent<Component::BulletBase>()->SetLife(150);
					//list->GetComponent<Component::BulletBase>()->SetSpeed(_player->GetSpeed() / 2);

					// �����̐ݒ�
					list->GetComponent<Component::BulletBase>()->SetSpeed(2);

					break;
				}
            }                                   

            m_homingnum = 0;
            for (auto list : m_hominglist)
            {
                if (list->GetComponent<Component::BulletBase>()->GetStatus() == Component::BulletBase::Status::Dead)
                {
                    m_homingnum++;
                }
            }
            // �X�V�|���Ă���Ƃ��ɃI�u�W�F�N�g�̃��X�Ƒ��₷�ƃo�O����
        }   
    }
}

// �e�̃J�E���g�X�V
void BulletManager::BulletCntUpdate()
{
	// �ʏ�
	if (m_nowbulletcnt <= m_maxhomingtcnt) {
		m_nowbulletcnt++;
	}

	// �z�[�~���O
	if (m_nowhomingcnt <= m_maxhomingtcnt) {
		m_nowhomingcnt++;
	}
}

// XZ���̃v���C���[�ƓG�̊p�x�擾
Vector3 BulletManager::GetDegree(GameObject* _a, GameObject* _b)
{
    const float M_PI = 3.14f;

    Vector3 m_position = _a->transform->position;	//�v���C���[�̍��W

    //�v�Z���ďo���b��I�Ɉ�ԏ������p�x���L������ϐ�
    float degreemum = 3.14f * 2;
    double degreeHei;
    double degreeVer;
	float _stackdis = 1000;

    auto m_enemyList = EnemyManager::Instance()->GetObjectList();
	auto _camera = Camera::GetMainCamera()->GetOwner();

	bool lock = false;
	bool _lockClear = true;
	static bool _sound = false;
    for (auto* _enemy : m_enemyList)
    {
		if (_enemy->GetComponent<Component::Enemy>()->GetStatus())
		{


			SimpleMath::Vector3 _campos;
			_campos.x = _camera->transform->position.x;
			_campos.y = _camera->transform->position.y;
			_campos.z = _camera->transform->position.z;

			//SimpleMath::Vector3 _lookat;
			//auto _forward = _camera->transform->forward();
			//_lookat.x = _forward.x;
			//_lookat.y = _forward.y;
			//_lookat.z = _forward.z;

			SimpleMath::Vector3 _enemyPos;
			_enemyPos.x = _enemy->transform->position.x;
			_enemyPos.y = _enemy->transform->position.y;
			_enemyPos.z = _enemy->transform->position.z;

			Vector3 _lookat = _a->transform->position + _a->transform->forward() * 100;
			SimpleMath::Vector3 rotlookat;
			rotlookat.x = _lookat.x;
			rotlookat.y = _lookat.y;
			rotlookat.z = _lookat.z;

			static int hogent = 0;
			// �Q�_�Ԃ̋���
			if (InsightCheckXZ(
				_campos,
				rotlookat,
				Camera::GetMainCamera()->GetFov(),                          // FOV
				_enemyPos,                     // check point
				_stackdis))                                       // view length
			{
				printf("�͂�%d\n", hogent);
				hogent++;

				// �����̍X�V
				_stackdis = _a->transform->position.Distance(_enemy->transform->position);
				stackEnemy = _enemy;
				_lockClear = false;
			}
		}
    }

	if (_lockClear)
	{
		stackEnemy = nullptr;
		if (!_sound)
		{
			_sound = true;
			StopSound(SOUND_BULLET_SE004);
			StopSound(SOUND_BULLET_SE005);
		}
	}

	if (BulletManager::Instance()->bullettype == BulletManager::BulletType::Homing)
	{	
		// ���b�N�I���̏�ԕێ�
		if (stackEnemy != nullptr) {
			
			
			// �^�[�Q�b�g�������Ƃ�
			if (m_oldStackEnemy == stackEnemy) {

				stackEnemy->GetComponent<Component::LockOnUI>()->SetLockOnCnt(m_lockoncnt);
				stackEnemy->GetComponent < Component::LockOnUI >()->SetLoclOnFlg(true);
				// �J�E���g�̍X�V
				if (LOCKONMAX > m_lockoncnt)
				{
					m_lockoncnt++;

					// ���b�N�I��������
					if (!CheckPlaying(SOUND_BULLET_SE004))
					{
						PlaySound(SOUND_BULLET_SE004);
					}
				}
				else
				{
					// �ǔ��\
					m_lockonOk = true;
					stackEnemy->GetComponent < Component::LockOnString >()->SetlockOnFlg(true);

					// ���b�N�I���������̒�~�ƃ��b�N�I����
					StopSound(SOUND_BULLET_SE004);
					if (!CheckPlaying(SOUND_BULLET_SE005))
					{
						PlaySound(SOUND_BULLET_SE005);
						_sound = false;
					}
				}
			}
			else {

				ClearLockOn();

				// �^�[�Q�b�g���قȂ�Ƃ�
				m_oldStackEnemy = stackEnemy;
				m_lockoncnt = 0;
			}
		}// �^�[�Q�b�g���Ȃ��Ƃ�
		else
		{
			ClearLockOn();
		}
	}
	else {
		ClearLockOn();
	}
	

    // �v�C��
    return Vector3(0, 0, 0);
}

// �e�ύX
void BulletManager::ClearLockOn()
{
	// ������
	if (m_oldStackEnemy != nullptr)
	{
		m_lockoncnt = 0;
		// ui�̃R���|�[�l���g�̕\������
		m_oldStackEnemy->GetComponent < Component::LockOnUI >()->SetLoclOnFlg(false);
		m_oldStackEnemy->GetComponent < Component::LockOnString >()->SetlockOnFlg(false);
		m_oldStackEnemy = nullptr;
		m_lockonOk = false;
	}
}


void BulletManager::BulletChange()
{
    switch (bullettype)
    {
    case BulletManager::BulletType::Bullet:
        bullettype = BulletType::Homing;
		break;
    case BulletManager::BulletType::Homing:
        bullettype = BulletType::Bullet;
        break;

    default:
        break;
    }
}

// �����������̔���
void BulletManager::HitCheck()
{
	m_hitflg = false;
}

// �G�t�F�N�g����
void BulletManager::InstaneEffect(Vector3 _pos, Vector3 _size)
{
	// �G�t�F�N�g����
	m_effectmana->GetComponent<Component::BulletEffectManager>()->Fire(_pos, _size);
}
