//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// �T�E���h�t�@�C��
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// �T���v��BGM	���C��
	SOUND_LABEL_BGM001,		// �T���v��BGM	�^�C�g���E�X�e�[�W�Z���N�g
	SOUND_LABEL_SE000,			// �ʏ�e���ˉ�
	SOUND_BULLET_SE001,			// �ʏ�e�ڐG��
	SOUND_BULLET_SE002,			// �~�T�C���e�ڐG��
	SOUND_BULLET_SE003,			// �~�T�C���e�ڐG��
	SOUND_BULLET_SE004,			// �~�T�C���e���b�N�I��������
	SOUND_BULLET_SE005,			// �~�T�C���e���b�N�I��������
	SOUND_OPTION_SE000,			// �ʏ�e�ڐG��
	SOUND_OPTION_SE001,			// �ʏ�e�ڐG��
	
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
bool CheckPlaying(SOUND_LABEL label);
#endif
