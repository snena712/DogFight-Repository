#include "PlayerJatParts.h"

using namespace Component;

PlayerJatParts::PlayerJatParts(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("PlayerJatParts");

	tra = owner->transform;

	// 爆破アニメーション
	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_enjin_v5.fbx",		// モデルデータ			// カレントファイルからの読み込み
		"shader/CookTorranceVS.hlsl",			// 頂点シェーダー
		"shader/CookTorrancePS.hlsl",			// ピクセルシェーダー
		"assets/F2_v2/f2a_texture/");			// テクスチャの存在フォルダ

	
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

// 角度の設定
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
