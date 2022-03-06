#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/Quad2DMultiRenderComponent.h"
#include "../../DX11System\dx11mathutil.h"
#include "../Enemy/EnemyManager.h"
#include "MiniMap\MiniMapIconUI.h"

namespace Component {

	class MiniMapUI : public ComponentBase
	{
	private:
		// コンポ―ネント
		Transform* tra;
		Component::QuadRender* ren;
		Component::Quad2DRender* ren2d;
		Component::StartUIColorComponent *start;

		// 変数
		GameObject* m_camera;
		GameObject* player;

		Vector3 center;							// ミニマップの中心点
		Vector3 m_pos = Vector3(100, -623, 2);	// 位置
		std::vector< GameObject*> miniiconlist;	// アイコンのリスト

		void PosUpdate();		// 場所の更新
		void RoundCheck();		// 角度から位置指定

		bool init = false;
	public:
		MiniMapUI(GameObject* _obj);
		~MiniMapUI();

		void Start();
		void Update();
	};
}