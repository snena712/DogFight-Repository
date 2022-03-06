#include "PlayerJatParts.h"

using namespace Component;

PlayerJatParts::PlayerJatParts(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("PlayerJatParts");

	tra = owner->transform;

	// ���j�A�j���[�V����
	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_enjin_v5.fbx",		// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/CookTorranceVS.hlsl",			// ���_�V�F�[�_�[
		"shader/CookTorrancePS.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/F2_v2/f2a_texture/");			// �e�N�X�`���̑��݃t�H���_

	
}

PlayerJatParts::~PlayerJatParts()
{
}


void PlayerJatParts::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");
}

void PlayerJatParts::Update()
{
}

// �p�x�̐ݒ�
void PlayerJatParts::SetRot(float _num)
{
	// Open
	if (m_rot < m_maxrot&& _num > 0)
	{
		m_rot += _num;
		owner->transform->RotateLocalXaxis(_num);
	}
	// Close
	if (m_rot > m_minrot && _num < 0)
	{
		m_rot += _num;
		owner->transform->RotateLocalXaxis(_num);
	}
}
