#include "EnemyManager.h"


EnemyManager::~EnemyManager()
{
	m_initFlg = false;
}

void EnemyManager::Start()
{
	if (!m_initFlg) 
	{
		player = SceneManager::Instance()->GameObjectFindName("player"); if (player == NULL)return;
		EnemyDegreeUpdate();
		EnemyCameraLock();

		m_initFlg = true;
	}
}

void EnemyManager::Reset()
{
	// �e��ϐ�������
	m_initFlg = false; 
	m_objlist.clear();
	m_lengthlist.clear();
	m_degreelist.clear();
}

void EnemyManager::Update()
{
	// ������
	Start();

	if (m_initFlg) {
		// �p�x�擾
		EnemyDegreeUpdate();

		// �������̔�r
		CompareLiveCnt();

		// ���񂾓G�͂��邩
		DeadEnemyCheck();
	}

	m_deadTarget = m_objlist[0];
}

// �ʒu�̃��X�g�Ƀf�[�^�����
void EnemyManager::SetList(std::vector<Vector3*> _pos)
{
	// �e�평����
	m_objlist.clear();
	m_lengthlist.clear();
	m_degreelist.clear();

	for (auto hoge : _pos)
	{

		// ���X�g�ɒǉ�
		GameObject* ene = new GameObject();
		ene->AddComponent<Component::Enemy>();
		m_objlist.emplace_back(ene);

		// ���
		ene->transform->position = *hoge;

		// �p�x�̃��X�g�̏�����
		m_degreelist.emplace_back(0);
		m_lengthlist.emplace_back(0, nullptr);

	}
	SceneManager::Instance()->AddList(&m_objlist);

	// �T�C�Y�擾
	m_enelistsize = m_objlist.size();
	m_startlivrcnt = m_enelistsize;
}

// �߂��G�̍X�V
void EnemyManager::UpdateNearEnemy()
{
	if (m_nearcnt < m_liveCount - 1) {
		m_nearcnt++;
	}
	else {
		m_nearcnt = 0;
	}

	// ���͂��������Ƃ��̂ݎ��s
	EnemyCameraLock();
}

// �S�G�̊p�x�X�V
void EnemyManager::EnemyDegreeUpdate()
{
	_num = 0;
	m_liveCount = 0;

	// �v���C���[�ƃG�l�~�[�����ԃx�N�g�����o���܂�
	for (auto hoge : m_objlist)
	{
		SerchDegree(hoge);
	}
}


// �p�x�擾
void EnemyManager::SerchDegree(GameObject* _obj)
{

	if (_obj->GetComponent<Component::Enemy>()->GetStatus() == true)
	{
		// �����Ă���Ƃ�
		m_liveCount++;

		// �Q�_�Ԃ̋���
		Vector3 pos2 = _obj->transform->position - player->transform->position;

		// �v���C���[�̌����Ă���p�x
		double degreep = atan2(player->transform->forward().x, player->transform->forward().z);

		//�x�N�g���𐳋K�����܂�
		pos2.y = 0;
		pos2.Normalize();
		//�v���C���[�ƃG�l�~�[�����ԃx�N�g���̊p�x���v�Z���܂�
		double degree;

		degree = atan2(pos2.x, pos2.z);

		degree = degree - degreep;

		m_degreelist[_num] = degree * 180 / 3.141592653589793f + 180;

		// �ɓ]���̋���
		float pos = player->transform->position.Distance(_obj->transform->position);

		if (pos < m_lengthmax) {
			// ����
			m_lengthlist[_num] = std::make_pair(pos, _obj);
		}
		else {
			m_lengthlist[_num] = std::make_pair(m_lengthmax, _obj);
		}

	}
	else {
		// ����ł���
		m_lengthlist[_num] = std::make_pair(20000, _obj);

	}

	// ���X�g�ɕۑ����邽�߂̕ϐ�
	_num++;
}

// �J�����̃^�[�Q�b�g�擾�֐�
void EnemyManager::EnemyCameraLock()
{
	std::sort(m_lengthlist.begin(), m_lengthlist.end());//�����\�[�g

	for (int i = m_nearcnt; i >= 0; i--)
	{
		if (m_lengthlist[m_nearcnt].first < m_maxdis
			&& m_lengthlist[m_nearcnt].second->GetComponent<Component::Enemy>()->GetStatus())
		{
			m_target = m_lengthlist[m_nearcnt].second;
			break;
		}
	}
}


// ���񂾓G�͂��邩�H
void EnemyManager::DeadEnemyCheck()
{
	for (auto list : m_objlist)
	{
		if (list->GetExistState())
		{
			if (list->GetComponent<Component::Enemy>()->GetStatus()==false)
			{
				m_deadFlg = true;
				m_deadTarget = list;
				EnemyCameraLock();
				return;
			}
		}
	}	
}

// �������̔�r
void EnemyManager::CompareLiveCnt()
{
	// �|�������̎擾
	m_deadcount = m_startlivrcnt - m_liveCount;

	// �|�����G�Ə����l�������Ƃ�
	if(m_startlivrcnt==m_deadcount)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::CLEAR);
	}
}
