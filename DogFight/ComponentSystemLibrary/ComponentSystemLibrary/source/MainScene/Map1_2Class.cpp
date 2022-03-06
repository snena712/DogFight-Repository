#pragma once
#include "Map1_2Class.h"
#include "../MainSceneHeader.h"

void Map1_2Class::InitScene() {

	//ParticleEffect *par = new ParticleEffect();
	//AddEntityObject(par, false);

	std::vector<GameObject*> _objList;

	GameObject* player = new GameObject();
	player->AddComponent<Component::PlayerObj>();
	_objList.emplace_back(player);

	GameObject* cam = new GameObject();
	cam->AddComponent<Component::MainCamera>();
	_objList.emplace_back(cam);
	// シーン初期化用関数
	// 配列用意
	// 配列の中身の種類と場所とサイズの設定


	// 敵
	std::vector<Vector3*> pos;
	pos.emplace_back();
	pos[0] = new Vector3(0, 0, 200);
	pos.emplace_back();
	pos[1] = new Vector3(100, 0, 200);
	pos.emplace_back();
	pos[2] = new Vector3(220, -10, 150);
	pos.emplace_back();
	pos[3] = new Vector3(-150, 50, -150);
	pos.emplace_back();
	pos[4] = new Vector3(100, -100, 180);
	pos.emplace_back();
	pos[5] = new Vector3(-200, 180, 170);
	//pos.emplace_back();
	//pos[6] = new Vector3(20, -100, 10);
	//pos.emplace_back();
	//pos[7] = new Vector3(-50, 50, 100);

	// 敵の初期化
	EnemyManager::Instance()->SetList(pos);


	// 真ん中のUI
	GameObject* winui = new GameObject();
	winui->AddComponent<Component::WindowUI>();
	_objList.emplace_back(winui);

	GameObject* leftrodui = new GameObject();
	leftrodui->AddComponent<Component::LeftRodUI>();
	_objList.emplace_back(leftrodui);

	GameObject* rightrodui = new GameObject();
	rightrodui->AddComponent<Component::RightRodUI>();
	_objList.emplace_back(rightrodui);

	GameObject* speedui = new GameObject();
	speedui->AddComponent<Component::SpeedUI>();
	_objList.emplace_back(speedui);

	GameObject* enecnt = new GameObject();
	enecnt->AddComponent<Component::EnemyCountUI>();
	_objList.emplace_back(enecnt);

	GameObject* homingui = new GameObject();
	homingui->AddComponent<Component::HomingBulletUI>();
	_objList.emplace_back(homingui);

	GameObject* bulletUi = new GameObject();
	bulletUi->AddComponent<Component::BulletUI>();
	_objList.emplace_back(bulletUi);

	GameObject* minimapui = new GameObject();
	minimapui->AddComponent<Component::MiniMapUI>();
	_objList.emplace_back(minimapui);

	GameObject* minibackpui = new GameObject();
	minibackpui->AddComponent<Component::MiniMapBackUI>();
	_objList.emplace_back(minibackpui);

	GameObject* winbullet = new GameObject();
	winbullet->AddComponent<Component::WindowBullet>();
	_objList.emplace_back(winbullet);

	GameObject* ybuttun = new GameObject();
	ybuttun->AddComponent<Component::YButtunUI>();
	_objList.emplace_back(ybuttun);

	GameObject* timer = new GameObject();
	timer->AddComponent<Component::TimerUI>();
	timer->GetComponent<Component::TimerUI>()->SetTimer(300);
	_objList.emplace_back(timer);

	GameObject* score = new GameObject();
	score->AddComponent<Component::ScoreUI>();
	_objList.emplace_back(score);

	GameObject* start = new GameObject();
	start->AddComponent<Component::StartUI>();
	_objList.emplace_back(start);

	GameObject* root = new GameObject();
	root->AddComponent<Component::EnemyRoot>();
	_objList.emplace_back(root);

	GameObject* optionmana = new GameObject();
	optionmana->AddComponent<Component::OptionManager>();
	_objList.emplace_back(optionmana);

	GameObject* endmana = new GameObject();
	endmana->AddComponent<Component::GameOverManager>();
	_objList.emplace_back(endmana);

	GameObject* clearmana = new GameObject();
	clearmana->AddComponent<Component::ClearManager>();
	_objList.emplace_back(clearmana);

	//EnemyPosFrame* eneposui = new EnemyPosFrame();
	//_objList.emplace_back(eneposui);

	// パーティクル
	//TestComputeObj* efemana = new TestComputeObj(Vector3(0, 0, 0));
	//_objList.emplace_back(efemana);

	// 火
	GameObject* jet = new GameObject();
	jet->AddComponent<Component::JetFire>();
	_objList.emplace_back(jet);

	// 煙
	GameObject* smog = new GameObject();
	smog->AddComponent<Component::SmogManager>();
	_objList.emplace_back(smog);

	// フェード
	GameObject* fade = new GameObject();
	fade->AddComponent<Component::FadeManager>();
	_objList.emplace_back(fade);


	GameObject* sim = new GameObject();
	sim->AddComponent<Component::Shimmer>();
	_objList.emplace_back(sim);


	SceneManager::Instance()->AddList(&_objList);

	PlaySound(SOUND_LABEL_BGM000);
}
