//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// サンプルBGM	メイン
	SOUND_LABEL_BGM001,		// サンプルBGM	タイトル・ステージセレクト
	SOUND_LABEL_SE000,			// 通常弾発射音
	SOUND_BULLET_SE001,			// 通常弾接触音
	SOUND_BULLET_SE002,			// ミサイル弾接触音
	SOUND_BULLET_SE003,			// ミサイル弾接触音
	SOUND_BULLET_SE004,			// ミサイル弾ロックオン準備音
	SOUND_BULLET_SE005,			// ミサイル弾ロックオン完了音
	SOUND_OPTION_SE000,			// 通常弾接触音
	SOUND_OPTION_SE001,			// 通常弾接触音
	
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
bool CheckPlaying(SOUND_LABEL label);
#endif
