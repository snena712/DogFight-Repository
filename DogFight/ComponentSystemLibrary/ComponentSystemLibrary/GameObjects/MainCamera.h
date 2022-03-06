#pragma once
#include "../Component/GameObject.h"
#include "../Component\CameraComponent.h"
#include "../DX11System/RandomClass.h"
#include "../source/Player/F1_PlayerObj.h"
#include "../source/MainGameManager.h"
#include "../source/Camera/CameraShakeComponent.h"

#define arraySiza (200)

namespace Component {

	class MainCamera : public ComponentBase
	{
	public:
		enum class CameraType {
			FPS,
			TPS,
			DELAY,
			STATIC
		};

	private:
		Transform* tra;
		Component::Camera* cam;
		Component::PlayerObj* playercompo;
		Component::OutSideControlComponent* out;
		Component::CameraShakeComponent* shake;
		CameraType camtype = CameraType::TPS;	// �J�����̎��

		GameObject* player;				// �v���C���[�I�u�W�F�N�g
		Transform*  playertra;			// �v���C���[�̃g�����X�t�H�[��

		int ControllerNum = -1;			// �R���g���[���[�ԍ�
		WORD Buttons = -1;				// �{�^���̏ڍ�
		Vector3 m_mainPos;				// �ꏊ�̃��C��

		//---------�X�^�[�g�p---------//
		int m_startcnt = 0;				// �J�n�p�J�E���g
		int m_posecnt = 0;				// �J�n�������J�E���g
		float azimuth = 70;				// �J�n�p�x
		bool firstflag = false;			// ������

		//-----------�͈͊O�̃J����-------------//
		float m_outCnt = 0;				// �͈͊O���̊Ǘ��J�E���g
		const float outMaxCnt = 60;		// �͈͊O���̊Ǘ��ő�J�E���g
		Vector3 stackOutPos;			// �͈͊O�ɍs�������̏ꏊ�ۑ�
		bool outflg = false;
		
		//--------�������p�ϐ�---------//
		int svidx = 0;					// �J�����ʒu�ŕۑ��Ŏg�p����J�E���g
		int useidx = 0;					// �J�����ʒu�Ŋ֐��Ŏg�p����J�E���g
		int svidxQt = 0;				// �J������]�ŕۑ��Ŏg�p����J�E���g
		int useidxQt = 0;				// �J������]�Ŋ֐��Ŏg�p����J�E���g

		const int _delay = 4;			// �J�����ʒu�̒x��
		const int _delayQt = 10;		// �J�����̉�]�̒x��

		// �����Ǘ��p�z��
		XMFLOAT4  qt[arraySiza];
		Vector3 forward[arraySiza];
		Vector3 up[arraySiza];
		Vector3 right[arraySiza];
		Vector3 stackpos[arraySiza];

		//-----------�N���A�J����-------------//
		Vector3 m_clearEnemyPos;		// �G�̈ʒu
		Vector3 m_clearCameraPos;		// �G�̈ʒu

		//-----------���b�N�I���p-------------//
		XMFLOAT4 m_lookRoot;			// ���b�N�I��

		void CameraPos();				// �ʒu�̍X�V
		void Input();					// ����

		void UpdateSphereCamera(
			float radius,						// ���a(����)
			float elevation,					// �p�i���́j
			const Vector3 lookat,				// �����_�i���́j
			const int _pluscnt);				// �J�E���g				

		void FPSCamera();
		void DelayCamera();											// �x��ĒǏ]����J����
		void DelayCameraUpdate();									// �Ǐ]����J�����̍X�V
		void LockOn();												// ���b�N�I��
		void StartCamera(int posecnt, int _cnt, int _pluscnt);		// �J�n�����o�p
		void StartLookCamera(Vector3 _pos, int time);				// �J�n�����o�p
		void DeadCamera();											// ���S���p
		void SetOldPos();											// �O�̈ʒu�̕ۑ�
		void ClearCamera();											// �N���A���̃J����
		void OutSideCamera();										// ��ʊO�̏���
		
		void ResetSivCnt() { if (svidx > arraySiza - 1)svidx = 0; }
		void ResetSivQtCnt() { if (svidxQt > arraySiza - 1)svidxQt = 0; }

	public:
		MainCamera(GameObject* _obj);
		~MainCamera();

		void CameraUpdate();

		void Update();
		void Start();

		XMFLOAT4 GetLookRoot() { return m_lookRoot; }				// �����̈ʒu

	};
}
