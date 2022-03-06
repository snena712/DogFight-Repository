#include "Iland.h"
using namespace Component;

Iland::Iland(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("Iland");

	model = owner->AddComponent<Component::ModelRender>(
		"assets/Model/Iland/Low Poly Pirate Landscapes.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/Model/Iland/");						// テクスチャの存在フォルダ
}




