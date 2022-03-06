#pragma once

#include "../../Component\RigidbodyComponent.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../source/ObjectShape/Cone.h"
#include "../../DX11System/RandomClass.h"
#include "../../DX11System\quad.h"
#include "../../DX11System\CDirectxGraphics.h"
#include "../../DX11System\Shader.h"
#include "../../DX11System/CDirectInput.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SphereColliderComponent.h"
#include "../../Component/BoxColliderComponent.h"
#include "XAudio2.h"
#include "../../DX11System/XAudio2.h"
#include "../../Component\ImGuiComponent.h"
#include "../../Component/SceneManager.h"
#include "../drawaxis.h"
#include "../../DX11System/Controller.h"
#include "../../DX11System/mystring.h"
#include "../../DX11System/XAudio2.h"
#include "../MainGameManager.h"
#include "../Bullet/BulletManager.h"
#include "../Smog/SmogManager.h"
#include "../Effect/Bomb/BombAnim.h"
#include "../Player/PlayerWingParent.h"
#include "../Player/PlayerJatPartsParent.h"
#include "../Move/FallObjectComponent.h"
#include "../Move/OutSideControlComponent.h"
#include "../UI/Window/EnemyRoot/EnemyRoot.h"
#include "../Bullet/FireBulletPlace.h"
#include "../Stage/StageCollider.h"
#include "../../Config.h"
#include "../Effect/Splashes/SplashesEfect.h"

enum  Status
{
	LIVE,
	DEAD
};

namespace Component {
	class PlayerObj : public ComponentBase
	{

	private:
		//----�R���|�[�l���g------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;
		GameObject* bomb;
		Component::FallObjectComponent* fall ;
		Component::OutSideControlComponent* out;
		GameObject* root;
		GameObject* splashes;
		Component::ComputeComponent* efemana;
		Component::SmogManager* smog;
		RandomClass* randam;
		Component::PlayerWingParent*pwing;
		Component::PlayerJatPartsParent* pjet;
		Component::FireBulletPlace* bulletRPlace;
		Component::FireBulletPlace* bulletLPlace;
		GameObject* bulletRPlaceObj;
		GameObject* bulletLPlaceObj;

		int ControllerNum = -1;						// �R���g���[���[

		Status m_status = Status::LIVE;				// ���
		int m_nowdeadcnt = 0;						// ���S���̃J�E���g
		const int m_maxdeadcnt = 60;				// ���S���̍ő�J�E���g

		//----�p�x----//
		DirectX::XMFLOAT4X4 g_mtxframe;				// ����̃t���[���ł̉�]�ʒu��\���s��
		DirectX::XMFLOAT3   g_angle = { 0,0,0 };	// ��]�p�x

		// �ړ�
		const float speed = 0.016;					// �ړ����x
		const float Lside = 2;						// ���X�e�B�b�N��X�̈ړ�����
		const float m_maxstraightStackSpeed = 0.5f;	// �ړ��ő�l
		const float m_maxSpeed = 5.0f;				// �ړ��ő�l
		//const float m_minSpeed = 1.0f;			// �ړ��ŏ��l
		const float m_minSpeed = 0;					// �ړ��ŏ��l
		float straightSpeed = m_minSpeed+2;			// ���݂̑��x
		float straightStackSpeed;					// ���݂̑��x
		float m_oldspeed = 0;						// �O�̑��x
		const float m_compareSpeed = 0.005f;		// �U���p�̉����n�̌Œ�l

		// ��]
		Vector3 m_stackRot;							// ��]���̈ړ��ʕۑ�
		const float deadZone = 20;					// ���͂̓���Ȃ��͈�
		const float m_maxRot = 55;					// �ړ��ő�l
		const float m_minRot = 0;					// �ړ��ŏ��l
		const float m_decay = 1;					// ������()

		// �ʒu
		DirectX::XMFLOAT3 g_startpos;				// �L�[���͂��ꂽ�ۂ̊J�n�ʒu
		
		// ���x�̕ϐ�
		bool m_streatflg = false;					// �O�ɉ������Ă��邩
		bool m_downflg = false;						// ����������
		float m_speedShakeNowCnt;					// ���x�̌��݃J�E���g
		float m_speedShakeMaxCnt=10;				// ���x�̍ő�J�E���g

		const float winglInstValue = 0.5f;				// ���𐶐�����
		float m_perimetrySmogCnt = 0;					// �������Ȃ��Ƃ��̃J�E���g
		const float m_perimetrySmogMaxCnt = m_maxSpeed;	// �������Ȃ��Ƃ��̍ő�J�E���g


		//-------�֐�--------//
		void Collision();						// �����蔻�� 
		void Keybord();							// ��]
		void Controller();						// �R���g���[���[
		void DrawSpeed();						// �X�s�[�h�̎擾
		void AutoGo(XMFLOAT4 _axisZ);			// �ړ�
		void RotUpdate();						// ��]�̌v�Z
		void SmogInstanse();					// ��ȉ�����
		void InstancePerimetrySmog(bool _flg);	// ��s���̎���̉_
		void SpeedControllerShake();			// ���x�ɉ������U��
		void DeadUpdate();						// ���S���̍X�V
		void DeadFly(XMFLOAT4 _axisZ);			// ���S���̎����ړ�

		// �t���[�g�̒l����肢�Ȃ��ł��邩�𔻒f
		bool floatcheck(float _checkvalue, float _value, float _minimum);	

		void BulletEffect();					// ���˃G�t�F�N�g
	public:
		PlayerObj(GameObject* _obj);
		~PlayerObj();

		//----��{�֐�----//
		void Start();
		void Update();
		void DrawObject();

		float GetSpeed() { return straightSpeed; }				// ���x�擾
		float GetMaxSpeed() { return m_maxSpeed; }				// �ő呬�x�擾

		bool GetStreatFlg() { return m_streatflg; }				// �����������̔���
		bool GetDownFlg() { return m_downflg; }					// �����������̔���

		Status GetStatus() { return m_status; }					// ��Ԏ擾
		void SetStatus(Status _status) { m_status = _status; }	// ��Ԃ̃Z�b�g

	};
}