//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
//!	@author	
//*****************************************************************************

#include "systemInclude.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
FILE* m_fp;							// �f�o�b�O�p�R���\�[��

bool once;
bool test;
//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit()
{
	// DX ������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// �A���t�@�u�����f�B���O��L���ɂ���@
	TurnOnAlphablend();

	// XAudio������
	InitSound();

	bool sts = false;

	// CDirectInput������
	sts = CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "CDirectInput", "error", MB_OK);
	}

	// �R���g���[���[�̏�����
	Controller::Instance()->Init();

	// ���s����������
	DX11LightInit(DirectX::XMFLOAT4(0, 1, 1, 0));		// ���s�����̕������Z�b�g

	// �X�J�C�h�[���̏�����
	Skydome::GetInstance()->Init(
		"assets/Skydome_RR6/Dome_RR601.x",
		"shader/vs.hlsl",
		"shader/pstexcol.hlsl",
		"assets/Skydome_RR6/");

	PostEffect::PostEffectManager::Instance()->Init();

	ManagerControl::Instance()->Start();

	Skydome::GetInstance()->SetScale(100);

	// �V�[���̏�����
	Scene* t = new Map1_1Class("Map1_1");

	Scene* select = new SelectScene("SelectScene");

	// �^�C�g��
	Scene* title = new TitleScene("TitleScene");
	if (!SceneManager::Instance()->ActiveScene("TitleScene")) {
		exit(EXIT_FAILURE);
	}

	// 2D�V�X�e��������
	Init2D();

	MyString::InitMyString();
	MyQuad::InitMyQuad();

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput(){

	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();

	// �R���g���[���[
	Controller::Instance()->CheckConnect();
	Controller::Instance()->Update();

}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate() {

#ifdef DEBUG
	ImGuiManager::Instance()->ImGuiUpdate();		// �Q�[���I�u�W�F�N�g�ɂ���H
#endif
	if (!test) {
		test = true;
	}
	else {
	
		// �G�̍X�V
		ManagerControl::Instance()->Update();
		// �R���s���[�g�}�l�[�W���[
	}
			
	DX11LightUpdate(XMFLOAT4(0, 10000, 0, 0));
	SceneManager::Instance()->ManagerUpdate();	
	
}

//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender() {

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0,0,0, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j
	DX11BeforeRender(ClearColor);

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	
	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::SELECT
		&& MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::TITLE)
	{
		//�X�J�C�h�[���̕`��
		Skydome::GetInstance()->Draw();
	}

	//Scene�̕`��
	SceneManager::Instance()->ManagerDraw();


#ifdef DEBUG
	auto _deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
	auto d = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	auto m_RTVCombine = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	_deviceContex->OMSetRenderTargets(1, &m_RTVCombine, d);

	// ImGui�̕`��
	ImGuiManager::Instance()->ImGuiRender();
#endif

	PostEffect::PostEffectManager().Instance()->AfterRenderClear();

	// �`��㏈��
	DX11AfterRender();
}


//==============================================================================
//!	@fn		GameDispose
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
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

	// DX�I������
	DX11Uninit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
