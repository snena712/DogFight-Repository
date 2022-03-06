#pragma once
#include "Map1_1Class.h"
#include "../MainSceneHeader.h"

void Map1_1Class::InitScene() {

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
	pos[0] = new Vector3(0, 500, 150);
	pos.emplace_back();
	pos[1] = new Vector3(100, 100, 200);
	pos.emplace_back();
	pos[2] = new Vector3(220, 100, 150);
	//pos.emplace_back();
	//pos[3] = new Vector3(-150, 50, -150);
	//pos.emplace_back();
	//pos[4] = new Vector3(100, -100, 180);
	//pos.emplace_back();
	//pos[5] = new Vector3(-100, 80, 100);
	//pos.emplace_back();
	//pos[6] = new Vector3(20, -10, 10);
	//pos.emplace_back();
	//pos[7] = new Vector3(-50, 50, 100);

	// 敵の初期化
	EnemyManager::Instance()->SetList(pos);	


	// 真ん中のUI
	GameObject* winui = new GameObject();
	winui->AddComponent<Component::WindowUI>();
	_objList.emplace_back(winui);

	/*GameObject* leftrodui = new GameObject();
	leftrodui->AddComponent<Component::LeftRodUI>();
	_objList.emplace_back(leftrodui);

	GameObject* rightrodui = new GameObject();
	rightrodui->AddComponent<Component::RightRodUI>();
	_objList.emplace_back(rightrodui);*/

	//GameObject* test = new GameObject();
	//test->AddComponent<Component::TestComputeObj>();
	//_objList.emplace_back(test);

	GameObject* speedui = new GameObject();
	speedui->AddComponent<Component::SpeedUI>();
	_objList.emplace_back(speedui);

	GameObject* enecnt = new GameObject();
	enecnt->AddComponent<Component::EnemyCountUI>();
	_objList.emplace_back(enecnt);

	GameObject* homingui = new GameObject();
	homingui->AddComponent<Component::HomingBulletUI>();
	_objList.emplace_back(homingui);

	GameObject* bulletstring = new GameObject();
	bulletstring->AddComponent<Component::BulletString>();
	_objList.emplace_back(bulletstring);

	GameObject* bulletUi = new GameObject();
	bulletUi->AddComponent<Component::BulletUI>();
	_objList.emplace_back(bulletUi);

	GameObject* minimapui = new GameObject();
	minimapui->AddComponent<Component::MiniMapUI>();
	_objList.emplace_back(minimapui);

	GameObject* minibackpui = new GameObject();
	minibackpui->AddComponent<Component::MiniMapBackUI>();
	_objList.emplace_back(minibackpui);

	// 高さUI
	GameObject* alt = new GameObject();
	alt->AddComponent<Component::AltUI>();
	_objList.emplace_back(alt);

	GameObject* score = new GameObject();
	score->AddComponent<Component::ScoreUI>();
	_objList.emplace_back(score);

	GameObject* outsideUI = new GameObject();
	outsideUI->AddComponent<Component::OutSideUI>();
	_objList.emplace_back(outsideUI);

	// ヒット時の文字
	GameObject* m_hit = new GameObject();
	m_hit->AddComponent<Component::HitUI>();
	_objList.emplace_back(m_hit);
	GameObject* m_destroy = new GameObject();
	m_destroy->AddComponent<Component::DestroyUI>();
	_objList.emplace_back(m_destroy);


	// 中心UIの初期化
	GameObject* m_centerhit = new GameObject();
	m_centerhit->AddComponent<Component::CenterHitUIManager>();
	_objList.emplace_back(m_centerhit);

	GameObject* lockArrow = new GameObject();
	lockArrow->AddComponent<Component::LockOnArrowUIParent>();
	_objList.emplace_back(lockArrow);

	// 開始UI
	GameObject* start = new GameObject();
	start->AddComponent<Component::StartUI>();
	_objList.emplace_back(start);

	GameObject* SkipStringUI = new GameObject();
	SkipStringUI->AddComponent<Component::SkipStringUI>();
	_objList.emplace_back(SkipStringUI);

	GameObject* ground = new GameObject();
	ground->AddComponent<Component::Ground>();
	_objList.emplace_back(ground);

	GameObject* iland = new GameObject();
	iland->AddComponent<Component::Iland>();
	iland->transform->position.y = -10;
	iland->transform->position.z = 3500;
	_objList.emplace_back(iland);

	GameObject* iland2 = new GameObject();
	iland2->AddComponent<Component::Iland>();
	iland2->transform->position.x= -1000;
	iland2->transform->position.y = -10;
	iland2->transform->position.z = -3500;
	_objList.emplace_back(iland2);

	GameObject* iland3 = new GameObject();
	iland3->AddComponent<Component::Iland>();
	iland3->transform->position.x = 3500;
	iland3->transform->position.y = -300;
	iland3->transform->position.z = 100;
	_objList.emplace_back(iland3);
	


	//GameObject* fog = new GameObject();
	//fog->AddComponent<Component::FogDistance>();
	//_objList.emplace_back(fog);

	GameObject* optionmana = new GameObject();
	optionmana->AddComponent<Component::OptionManager>();
	_objList.emplace_back(optionmana);

	GameObject* endmana = new GameObject();
	endmana->AddComponent<Component::GameOverManager>();
	_objList.emplace_back(endmana);

	GameObject* clearmana = new GameObject();
	clearmana->AddComponent<Component::ClearManager>();
	_objList.emplace_back(clearmana);

	GameObject* stage = new GameObject();
	//stage->AddComponent<Component::StageCollider>("assets/Model/stage/stage1.x", "assets/Model/stage/");
	stage->AddComponent<Component::StageCollider>("assets/Model/stage/stage1.x", "assets/Model/stage/");
	_objList.emplace_back(stage);

	//EnemyPosFrame* eneposui = new EnemyPosFrame();
	//_objList.emplace_back(eneposui);


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

	StopSound(SOUND_LABEL_BGM001);
	PlaySound(SOUND_LABEL_BGM000);
}

