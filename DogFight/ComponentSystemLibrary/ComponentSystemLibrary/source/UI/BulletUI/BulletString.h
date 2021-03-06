#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../Bullet/BulletManager.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"
#include "../String/StringComponent.h"

namespace Component {
	class  BulletString : public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender *ren;
		Component::StringComponent* strcom;
		BulletManager::BulletType type;

	public:
		BulletString(GameObject* _obj);
		~BulletString() {}

		void Update();

		void DrawString();								// 文字の描画
		void SetType(BulletManager::BulletType _type)	// 状態のセット
		{ type = _type; }
	};
}

