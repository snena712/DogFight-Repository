#pragma once
#include "../../Component\ModelComponent.h"
#include "../../Component\SphereColliderComponent.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component\RigidbodyComponent.h"
#include "../Compute/EffectManager.h"
#include "../UI/Enemy/EnemyPosFrame.h"
#include "../../Component/SceneManager.h"
#include "../Smog/SmogManager.h"
#include "../Effect/Bomb/BombAnim.h"
#include "../UI/Window/LockOn/LockOnUI.h"
#include "../UI/String/LockOnString.h"
#include "../Bullet/BulletManager.h"
#include <random>

namespace Component {
	class Enemy :public ComponentBase
	{
	public:
		// �ړ�
		enum class MoveDir
		{
			UP,
			DOWN,
			RIGHT,
			LEFT,
			STRREAT,
			SLOW,
			CHASE,
			TURUN,
			STOP,
			OUTSIDE
		};

		// ��]
		enum class Spin
		{
			Right,
			Left,
			None,
		};
		// ���
		enum Status
		{
			LIVE,
			DEAD
		};

	private:
		MoveDir movedir;
		Spin spin;
		Status status;
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		Component::SphereCollider* col;
		Component::SmogManager* smogmana;
		Component::EnemyPosFrame* posFrameCom;
		EffectManager* efemana;
		Component::LockOnString* lockStr;
		Component::LockOnUI* lockui;
	
		Component::EnemyPosFrame *frame;		// �g
		Component::BombAnim *bomb;				// ����
	
		float life = 20;						// ���C�t
		float oldlife = life;					// �O�t���[���̃��C�t

		int m_nowdeadcnt = 0;					// ���݂̎��S�J�E���g
		const int m_maxdeadcnt = 60;			// ���S���̍ő�J�E���g
	
		//-----�ړ�----
		const float speed = 0.01;				// �ړ����x
		const float Lside = 3;					// ���X�e�B�b�N��X�̈ړ�����
		const float m_maxSpeed = 3;				// �ړ��ő�l
		const float m_minSpeed = 2.5f;			// �ړ��ŏ��l

		//-----�ړ�----//
		int m_movecnt = 60;						// �ړ��p�̃J�E���g
		int m_nowmovecnt;						// ���݂̃J�E���g
		//const float speed = 0.00;				// �ړ����x
		//const float Lside = 0;				// ���X�e�B�b�N��X�̈ړ�����
		//const float m_maxSpeed = 0;			// �ړ��ő�l
		//const float m_minSpeed = 0.00f;		// �ړ��ŏ��l
		int m_spincnt = 60;						// �ړ��p�̃J�E���g
		int m_nowspincnt;						// ���݂̃J�E���g
		float straightSpeed = m_maxSpeed / 2;	// ���x

		//-----��]�Ɣ͈͊O----//
		float outsideDistance = 800;			// �s���ω��̋����i����ɂȂ�j
		int turnCnt = 0;						// ��]�̃J�E���g
		int turnMaxCnt = 180;					// ��]�̃J�E���g�ő�
		int outsideCnt = 0;						// ��ʊO�̃J�E���g
		const int outsidePlusCnt = 1;			// �J�E���g�̑���
		bool outsideFlg= false;					// �����̍s������
		bool turnFlg= false;					// �����̍s������
		const Vector3 m_centerPos
			= Vector3(0, outsideDistance, 0);	// �ړ��̒��S�_

		int m_nowsmogcnt = 0;					// ���̌��݂̃J�E���g
		int m_maxsmogcnt = 1;					// ���̌��݂̍ő�J�E���g

		Vector3 m_ScreenPos;					// �X�N���[�����W

		void MoveUpdate();						// �ړ��ʂ̌v�Z
		void AutoGo(XMFLOAT4 _axisZ);			// �ړ�
		void Collision();						// �����蔻��
		void StatusUpdate();					// ��ԕύX
		void RandMove();						// ��Ԃ������_���ɕύX
		void SpinMove();						// ���̉�]
		void LifeCheck();						// �̗͂̊m�F
		void DeadUpdate();						// ���S����
		void DeadFly(XMFLOAT4 _axisZ);			// ���S���̔�s
		void SmogFly();							// ��s���̉_
		void OutSide();							// �͈͊O�̍X�V
		void Atack();							// �U��
	public:
		Enemy(GameObject *_owner);
		~Enemy();

		void Start();
		void Update();
		void DrawObject();

		void LowerLife(int _num) { life -= _num; }	// �̗͂̌���
		bool GetStatus() { if (status == LIVE)		// �����Ă���Ƃ�true�Ԃ�
		{ return true; } else { return false; } }

	
		// �^�[�Q�b�g�̃X�N���[�����W
		void SetScreenPos(Vector3 _pos) { m_ScreenPos = _pos; }
		Vector3 GetScreenPos() { return m_ScreenPos; }
	};

}