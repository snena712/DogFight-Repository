#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "Enemy.h"
#include "../../Component/ComponentBaseClass.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/CDirectInput.h"
#include <utility>
#include "../MainGameManager.h"
#include "../../DX11System/Controller.h"


class EnemyManager
{
private:

	// ���X�g
	std::vector< GameObject*> m_objlist;						// �S�G�̃��X�g
	std::vector< float> m_degreelist;							// �S�G�̊p�x���X�g
	std::vector<std::pair<float, GameObject*>> m_lengthlist;	// �S�G�̒������X�g

	GameObject* player;

	// �ϐ�
	int _num = 0;							// �G�̃J�E���g
	int m_startlivrcnt = 0;					// �ŏ��ɂ��Ă��鐔
	int m_liveCount = 0;					// �������Ă���G�̐�
	int m_deadcount = 0;					// �|������
	float m_enelistsize;					// ���X�g�̃T�C�Y
	const int m_lengthmax = 700;			// �����ő�l
	bool m_deadFlg = false;					// ���S�Ǘ��t���O
	bool m_initFlg = false;					// ���������m

	const float m_maxdis = 10000;			// �Ώ۔͈�
	GameObject* m_target;					// �^�[�Q�b�g
	GameObject* m_deadTarget;				// �^�[�Q�b�g
	int m_nearcnt;							// ���݂̉��Ԗڂ̋߂���\�����̃J�E���g
	const int m_maxnearcnt = 3;				// ���Ԗڂ��̍ő�l

	// �֐�
	void EnemyDegreeUpdate();				// �S�G�̊p�x�X�V
	void SerchDegree(GameObject* _obj);		// �p�x�擾�֐�
	void EnemyCameraLock();					// �J�����̃^�[�Q�b�g�擾�֐�
	void CompareLiveCnt();					// �������̔�r
	void DeadEnemyCheck();					// ���S�����G�̊m�F
	
public:
	static EnemyManager* Instance() {
		static EnemyManager singleton;
		return &singleton;
	}
	EnemyManager() {};
	~EnemyManager();

	void Start();
	void Update();
	void Reset();

	void SetList(std::vector<Vector3*> _pos);						// �ʒu�̃��X�g�Ƀf�[�^�����
	std::vector< GameObject*> GetObjectList() { return m_objlist; }	// �G�l�~�[���X�g�擾
	GameObject* GetEnemy(int _num) { return  m_objlist[_num]; }		// �G�l�~�[�̎擾
	std::vector< float> GetDegreeList() { return m_degreelist; }	// �p�x���X�g�擾
	float GetLength(int _num) { return m_lengthlist[_num].first; }	// �������X�g�擾

	int GetLiveCount() { return m_liveCount; }						// �������擾	
	void LiveCountReduce() { m_liveCount--; };						// �������̌���		
	int GetDeadEnemyCnt() { return m_deadcount; }					// �|������

	bool GetDeadFlg() { return m_deadFlg; }							// ���񂾓G�͂��Ă邩
	void ResetDeadFlg() { m_deadFlg = false; }						// �t���O�̃��Z�b�g

	GameObject* GetLookObj() { return m_target; }					// ���b�N�I������^�[�Q�b�g
	GameObject* GetDeadObj() { return m_deadTarget; }				// ���S�����I�u�W�F�N�g

	void UpdateNearEnemy();											// �߂��G�̍X�V
};

