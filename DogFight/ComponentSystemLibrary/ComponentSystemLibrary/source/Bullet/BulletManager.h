#pragma once
#include "Bullet.h"
#include "HommigBullet.h"
#include "BulletFlash.h"
#include "../Smog/SmogManager.h"
#include "../Enemy\Enemy.h"
#include "../Enemy\EnemyManager.h"
#include "../../Component\SceneManager.h"
#include "../Effect/BulletEffect/BulletEffectManager.h"
#include "../../DX11System/Controller.h"
#include "../../DX11System/XAudio2.h"

constexpr int  LOCKONMAX = 12;	// ���b�N�I���ő�̏��

class BulletManager
{
public:
	// �e�̎��
	enum class BulletType
	{
		Bullet,
		Homing
	};
	// �^�O�̎��
	enum class TagType
	{
		Player,
		Enemy
	};

private:
	GameObject* _player;
	GameObject* window;
	GameObject* m_effectmana;


	bool initFlg = false;						// ������
	float pspeed = 0;							// �v���C���[�̃X�s�[�h

	BulletType bullettype = BulletType::Bullet;	// �o���b�g�̎��

	GameObject* stackEnemy = nullptr;			// �ꎞ�ۑ��ϐ�
	GameObject* m_oldStackEnemy = nullptr;		// �O�t���[���̓G��ێ�
	std::vector<GameObject*> m_bulletlist;		// �e�z��
	std::vector<GameObject*> m_hominglist;		// �z�[�~���O�z��
	GameObject* flash;							// ���ˌ�

	int m_bulletnum = 20;						// �e�̃J�E���g
	float m_nowbulletcnt = m_bulletcnt;			// �e�̔��˃J�E���g
	const float m_bulletcnt = 3;				// �e�̔��ˍő�J�E���g
	int m_bulletAllCnt;							// �ʏ�c�̎d�����ϐ�

	int m_homingnum = 5;						// �z�[�~���O�~�T�C���̐�
	const float m_maxhomingtcnt = LOCKONMAX;	// �z�[�~���O�~�T�C���e�̔��˃J�E���g
	float m_nowhomingcnt = m_maxhomingtcnt;		// �z�[�~���O�~�T�C���e�̔��ˍő�J�E���g

	//---���b�N�I���֘A---
	int m_lockoncnt = 0;						// ���b�N�I����
	bool m_lockonOk = false;					// ���b�N�I���̔��ˉ\
	bool m_hitflg = false;						// �����������̃t���O

	void HitCheck();									// �e�������������̊m�F
	void ClearLockOn();									// ���b�N�I���̑Ώۂ��N���A
	Vector3 GetDegree(GameObject* _a,GameObject* _b);	// a��b�̊p�x�̎擾
public:
	static BulletManager* Instance() {
		static BulletManager singleton;
		return &singleton;
	}

	BulletManager() {};
	~BulletManager();

	void Init();
	void Start();
	void Update();
	void Reset();

	// ���ˊ֘A
	void Fire(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str);
	void FireBullet(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward, std::string *_str);
	void FireHoming(XMFLOAT4 _qt, Vector3 _pos, Vector3 _right, Vector3 _up, Vector3 _forward);
	
	GameObject* GetTargetObj() { return stackEnemy; }
	BulletType GetBulletType() { return bullettype; }		// ��Ԏ擾
	void BulletChange();									// �e��̕ύX
	void BulletCntUpdate();									// ���˃J�E���g�̍X�V
	bool GetHitFlg() { return m_hitflg; }					// �q�b�g��Ԃ̎擾
	void SetHitFlg(bool _flg) { m_hitflg = _flg; }			// �q�b�g��Ԃ̃Z�b�g
	void InstaneEffect(Vector3 _pos, Vector3 _size);		// �G�t�F�N�g����
	bool GetLockOK() { return m_lockonOk; }					// ���b�N�I��OK
	void SetPlayerObj(GameObject* _obj) { _player = _obj; }	// �v���C���[�̃Z�b�g
	void SetPlayerSpeed(float _num) { pspeed = _num; }		// �v���C���[�̑��x

	int GetHomigNum() {	return m_homingnum;}				// �z�[�~���O�̒e�擾
	void HomingNumUP() { m_homingnum++; }					// �z�[�~���O�̒e���₷

	// �E�̎�true
	bool GetNormalBulletPos() {
		// �E
		if (m_bulletAllCnt % 2 == 1)
		{
			return true;
		}
		return false;
	}

	bool GetFire()
	{
		if (m_nowbulletcnt == 0)
		{
			return true;
		}
		return false;
	}

};
