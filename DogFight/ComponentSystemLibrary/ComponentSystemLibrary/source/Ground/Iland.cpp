#include "Iland.h"
using namespace Component;

Iland::Iland(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("Iland");

	model = owner->AddComponent<Component::ModelRender>(
		"assets/Model/Iland/Low Poly Pirate Landscapes.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/Model/Iland/");						// �e�N�X�`���̑��݃t�H���_
}




