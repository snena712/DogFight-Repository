#include "TestComputeObj.h"
#include "../../DX11System\DX11Settransform.h"
#include "../../GameObjects/MainCamera.h"
#include "../../Component/SceneManager.h"
#include <random>


using namespace Component;

TestComputeObj::TestComputeObj(GameObject* _owner) : ComponentBase(_owner)
{
	/*owner->SetName("test");
	comCompo = 
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png","assets/EfeFile/splashes.EFE");
	*/
}

TestComputeObj::~TestComputeObj()
{}

// コンピュート
void TestComputeObj::DrawObject()
{	
	
}

void TestComputeObj::Update()
{
	// 中に書き込むデータの初期化や更新をする

	// コピーやdispatch　削除の処理もする
}

