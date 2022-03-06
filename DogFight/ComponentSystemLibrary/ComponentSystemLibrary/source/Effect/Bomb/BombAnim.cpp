#include "BombAnim.h"

using namespace Component;

BombAnim::BombAnim(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("BombAnim");

	ren=owner->AddComponent<Component::QuadRender>("assets/Image/Bomb/BON.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetHeight(120);
	ren->SetWidth(120);

	// 爆破アニメーション
	anim = owner->AddComponent<Component::Animation>();
	anim->fileread("Anim/Bomb/original.csv");
	anim->SetSpeed(2);
	anim->SetAnim(anim->GetAnim("bomb"));

	owner->transform->position = Vector3(-10, 0, 10);
}

BombAnim::~BombAnim()
{

}

void BombAnim::Update()
{
	// 終了フラグにより非表示にする
	if (anim->GetEndFlg())
	{
		owner->SetExistState(false);
	}
}



