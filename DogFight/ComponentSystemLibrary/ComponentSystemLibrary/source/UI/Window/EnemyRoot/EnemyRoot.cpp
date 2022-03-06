#include "EnemyRoot.h"
#include "../../../../GameObjects/MainCamera.h"
using namespace Component;

EnemyRoot::EnemyRoot(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("EnemyRoot");

	tra = owner->transform;
	tra->scale = Vector3(0.5f, 0.5f, 0.5f);
	 
	model = owner->AddComponent<Component::ModelRender>(
		"assets/Root/yajirusi_color.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"");						// テクスチャの存在フォルダ


}

EnemyRoot::~EnemyRoot()
{

}

void EnemyRoot::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");

}

void EnemyRoot::Update()
{
	PosUpdate();
}

void EnemyRoot::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 4.5f + m_camera->transform->up() * 1;
	
	tra->qt = m_camera->GetComponent<Component::MainCamera>()->GetLookRoot();
	tra->RotateLocalXaxis(180);
}


