#include "Ground.h"
#include <type_traits>
#include "../../DX11System/shaderhashmap.h"
using namespace Component;

// 地面生成
Ground::Ground(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("Ground");
	
	tra = owner->transform;
	owner->AddComponent<Component::ModelRender>(
		"assets/Model/Ocean/ocean.fbx",
		"shader/EnvironmentVS.hlsl",			// 頂点シェーダー
		"shader/EnvironmentPS.hlsl",			// ピクセルシェーダー
		"assets/Model/Ocean/");					// テクスチャの存在フォルダ);)

	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// キューブマップのテクスチャ作成
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
	// デバイスコンテキストを取得
	auto devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	devcontext->PSSetShaderResources(1, 1, _tex.GetAddressOf());
}
