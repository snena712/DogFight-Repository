#include "MiniMapUI.h"
#include "../../Component/SceneManager.h"

using namespace Component;

MiniMapUI::MiniMapUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("MiniMapUI");
	tra = owner->transform;

	// 爆破アニメーション
	ren2d = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/Rader.png");
	ren2d->SetAnchorType(AnchorType::LeftTop);
	ren2d->SetHeight(200);
	ren2d->SetWidth(200);
	
	ren2d->SetVertexColor(XMFLOAT4(1, 1, 1, 1.0f));

	start = owner->AddComponent<Component::StartUIColorComponent>(0.85f, Component::StartUIColorComponent::RnderState::TWO);
	start->SetColor(Vector3(1, 1, 1));
}

MiniMapUI::~MiniMapUI()
{
}

void MiniMapUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	player=SceneManager::Instance()->GameObjectFindName("player");
}

void MiniMapUI::Update()
{
	if (!init)
	{
		std::vector<GameObject*> _list;

		// アイコン作成
		// プレイヤー
		GameObject* minimapicon = new GameObject();
		minimapicon->AddComponent<Component::MiniMapIconUI>();
		minimapicon->GetComponent<Component::MiniMapIconUI>()->SetObjType(MiniMapIconUI::ObjType::Player);

		minimapicon->transform->position.x = m_pos.x;
		minimapicon->transform->position.y = m_pos.y;
		minimapicon->transform->position.z =1;

		center = minimapicon->transform->position;	
		_list.emplace_back(minimapicon);

		// エネミー
		for (auto num : EnemyManager::Instance()->GetObjectList())
		{
			GameObject* minimapicon = new GameObject();
			minimapicon->AddComponent<Component::MiniMapIconUI>();
			minimapicon->GetComponent<Component::MiniMapIconUI>()->SetObjType(MiniMapIconUI::ObjType::Enemy);
			miniiconlist.emplace_back(minimapicon);
			_list.emplace_back(minimapicon);
		}

		SceneManager::Instance()->AddList(&_list);

		init = true;
	}

	PosUpdate();

	RoundCheck();
}


// 位置設定
void MiniMapUI::PosUpdate()
{
	tra->position = m_pos;
}


// 角度から位置を計算
void MiniMapUI::RoundCheck()
{
	int i = 0;
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		for (auto radious : EnemyManager::Instance()->GetDegreeList())
		{
			float length = EnemyManager::Instance()->GetLength(i);

			Vector3 _pos;

			double radi = radious * 3.141592653589793f / 180;

			_pos.x = center.x - length / 7.5f * sin(radi);
			_pos.y = center.y - length / 7.5f * cos(radi);		// 下がマイナスになったのでマイナス処理
			_pos.z = 1;
			miniiconlist[i]->transform->position = _pos;

			i++;
		}
	}
}