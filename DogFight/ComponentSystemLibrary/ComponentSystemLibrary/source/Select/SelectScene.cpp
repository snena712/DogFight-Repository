#pragma once
#include "SelectScene.h"
#include "../../Component/SceneManager.h"
#include <vector>
#include "SelectPlayer.h"
#include "SelectSceneCamera.h"
#include "SelectBackUI.h"
#include "SelectUnderbarUI.h"
#include "../Map/TileLine.h"
#include "../Compute/ComputeManager.h"

void SelectScene::InitScene() {

	std::vector<GameObject*> _objList;

	GameObject* player = new GameObject();
	player->AddComponent<Component::SelectPlayer>();
	_objList.emplace_back(player);

	GameObject* camera = new GameObject();
	camera->AddComponent<Component::SelectSceneCamera>();
	_objList.emplace_back(camera);

	GameObject* back = new GameObject();
	back->AddComponent<Component::SelectBackUI>();
	_objList.emplace_back(back);

	GameObject* under = new GameObject();
	under->AddComponent<Component::SelectUnderbarUI>();
	_objList.emplace_back(under);

	//升目
	GameObject* tile = new GameObject();
	tile->AddComponent<Component::TileLine>();
	_objList.emplace_back(tile);

	SceneManager::Instance()->AddList(&_objList);

	ComputeManager::Instance()->ResetDate();

	// サウンドの確認
	if (!CheckPlaying(SOUND_LABEL_BGM001))
	{
		StopSound(SOUND_LABEL_BGM000);
		PlaySound(SOUND_LABEL_BGM001);
	}
}






