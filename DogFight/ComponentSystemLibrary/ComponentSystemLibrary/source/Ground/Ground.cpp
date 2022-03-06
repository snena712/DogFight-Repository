#include "Ground.h"
#include <type_traits>
#include "../../DX11System/shaderhashmap.h"
using namespace Component;

// �n�ʐ���
Ground::Ground(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("Ground");
	
	tra = owner->transform;
	owner->AddComponent<Component::ModelRender>(
		"assets/Model/Ocean/ocean.fbx",
		"shader/EnvironmentVS.hlsl",			// ���_�V�F�[�_�[
		"shader/EnvironmentPS.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/Model/Ocean/");					// �e�N�X�`���̑��݃t�H���_);)

	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �L���[�u�}�b�v�̃e�N�X�`���쐬
	HRESULT hr = CreateDDSTextureFromFileEx(_device, L"assets/Model/Ocean/CubeMap_Sky.dds", 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, nullptr, _tex.GetAddressOf());

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = _device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
}

Ground::~Ground()
{
}

void Ground::Start()
{
}

void Ground::DrawObject() {
	
}

void Ground::Update() {

	DrawSetting();
	auto devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	//devcontext->PSSetSamplers(0, 1, &CubesTexSamplerState);
}


void Ground::DrawSetting()
{
	// �f�o�C�X�R���e�L�X�g���擾
	auto devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	devcontext->PSSetShaderResources(1, 1, _tex.GetAddressOf());
}
