#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/CameraComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../UI/Fade/StartUIColorComponent.h""
#include "../String/StringWorldScreen.h"

namespace Component {
	class EnemyPosFrame :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::Camera* cameraCompo;
		Component::StartUIColorComponent* start;

		Transform* tra;
		Component::Quad2DRender* ren;

		GameObject *Ene;			// 敵の位置
		Vector3 m_screenPos;		//スクリーンの位置
		bool m_outside = false;		// 画面外

		// スクリーン位置取得
		XMVECTOR Screenpos(XMVECTOR World_Pos, Vector3 _pos);	
		
	public:
		EnemyPosFrame(GameObject* _obj);
		~EnemyPosFrame();

		void Start();
		void Update();

		void SetParent(GameObject* _obj) { Ene = _obj; }	// ゲームオブジェクトの中身渡す
		void CheckScreen();									// 位置の計算
		Vector3 GetScreenPos() { return m_screenPos; }		// スクリーン位置取得
	};
}
