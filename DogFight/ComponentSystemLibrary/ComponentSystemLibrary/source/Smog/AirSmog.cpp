#include "AirSmog.h"
// todo ‰Á‘¬‚Æi‚ñ‚¾‹——£‚Å¶¬—Ê•Ï‚¦‚é

using namespace Component;

AirSmog::AirSmog(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("AirSmog");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/RealSmog.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.9f));
	ren->SetWidth(1);
	ren->SetHeight(1);

}

AirSmog::~AirSmog()
{
}

void AirSmog::Start()
{

}

void AirSmog::Update()
{
	if (status == Status::LIVE) {

		AlphaReduce();

		ren->SetVertexColor(m_color);
		ren->SetHeight(m_height);
		ren->SetWidth(m_width);
	}
}

void AirSmog::DrawObject()
{

}
