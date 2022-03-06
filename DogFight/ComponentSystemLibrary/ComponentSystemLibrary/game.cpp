//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	ゲーム処理
//!	@author	
//*****************************************************************************

#include "systemInclude.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
FILE* m_fp;							// デバッグ用コンソール

bool once;
bool test;
//==============================================================================
//!	@fn		GameInit
//!	@brief	ゲーム初期処理
//!	@param	インスタンス値
//!	@param	ウインドウハンドル値
//!	@param	ウインドウサイズ幅（補正済みの値）
//!	@param	ウインドウサイズ高さ（補正済みの値）
//!	@param　フルスクリーンフラグ　true ; フルスクリーン　false : ウインドウ
//!	@retval	true 成功　false 失敗
//==============================================================================
bool GameInit()
{
	// DX 初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// アルファブレンディングを有効にする　
	TurnOnAlphablend();

	// XAudio初期化
	InitSound();

	bool sts = false;

	// CDirectInput初期化
	sts = CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "CDirectInput", "error", MB_OK);
	}

	// コントローラーの初期化
	Controller::Instance()->Init();

	// 平行光源初期化
	DX11LightInit(DirectX::XMFLOAT4(0, 1, 1, 0));		// 平行光源の方向をセット

	// スカイドームの初期化
	Skydome::GetInstance()->Init(
		"assets/Skydome_RR6/Dome_RR601.x",
		"shader/vs.hlsl",
		"shader/pstexcol.hlsl",
		"assets/Skydome_RR6/");

	PostEffect::PostEffectManager::Instance()->Init();

	ManagerControl::Instance()->Start();

	Skydome::GetInstance()->SetScale(100);

	// シーンの初期化
	Scene* t = new Map1_1Class("Map1_1");

	Scene* select = new SelectScene("SelectScene");

	// タイトル
	Scene* title = new TitleScene("TitleScene");
	if (!SceneManager::Instance()->ActiveScene("TitleScene")) {
		exit(EXIT_FAILURE);
	}

	// 2Dシステム初期化
	Init2D();

	MyString::InitMyString();
	MyQuad::InitMyQuad();

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	ゲーム入力検知処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameInput(){

	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();

	// コントローラー
	Controller::Instance()->CheckConnect();
	Controller::Instance()->Update();

}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameUpdate() {

#ifdef DEBUG
	ImGuiManager::Instance()->ImGuiUpdate();		// ゲームオブジェクトにする？
#endif
	if (!test) {
		test = true;
	}
	else {
	
		// 敵の更新
		ManagerControl::Instance()->Update();
		// コンピュートマネージャー
	}
			
	DX11LightUpdate(XMFLOAT4(0, 10000, 0, 0));
	SceneManager::Instance()->ManagerUpdate();	
	
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	ゲーム更新処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameRender() {

	// ターゲットバッファクリア	
	float ClearColor[4] = { 0,0,0, 1.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）
	DX11BeforeRender(ClearColor);

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::SELECT
		&& MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::TITLE)
	{
		//スカイドームの描画
		Skydome::GetInstance()->Draw();
	}

	//Sceneの描画
	SceneManager::Instance()->ManagerDraw();


#ifdef DEBUG
	auto _deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
	auto d = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	auto m_RTVCombine = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	_deviceContex->OMSetRenderTargets(1, &m_RTVCombine, d);

	// ImGuiの描画
	ImGuiManager::Instance()->ImGuiRender();
#endif

	PostEffect::PostEffectManager().Instance()->AfterRenderClear();

	// 描画後処理
	DX11AfterRender();
}


//==============================================================================
//!	@fn		GameDispose
//!	@brief	ゲーム終了処理
//!	@param　	
//!	@retval	なし
//==============================================================================
void GameDispose() {

	Skydome::GetInstance()->Exit();

	ImageResourceManager::GetInstance()->Exit();
	ModelResourceManager::GetInstance()->Exit();

	Controller::Instance()->Exit();

	SceneManager::Instance()->UninitScene();

#ifdef DEBUG
	ImGuiManager::Instance()->ImGuiEnd();
#endif

	UninitSound();

	CDirectInput::GetInstance().Exit();

	// DX終了処理
	DX11Uninit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
