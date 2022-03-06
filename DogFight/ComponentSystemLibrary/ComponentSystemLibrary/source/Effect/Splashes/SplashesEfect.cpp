#include "SplashesEfect.h"

using namespace Component;

SplashesEfect::SplashesEfect(GameObject* _owner) : ComponentBase(_owner)
{
	tra = owner->transform;
	
	efemana =
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png", "assets/EfeFile/splashes.EFE");
}

SplashesEfect::~SplashesEfect()
{

}

void SplashesEfect::Start()
{
}

void SplashesEfect::Update()
{
	tra->position.y = 10;
	efemana->SetSpawnPos(tra->position);
}


