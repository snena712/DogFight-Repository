#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

namespace Component {
	class BoxCollider :public ColliderBase {

	private:
		Vector3 Size;		//倍率
		Vector3 Scale;		//大きさ

		bool Type2D;		//２Dフラグ　// trueにするとBox同氏の当たり判定が２D上で判定されます

		void AddColliderList();

	public:
		BoxCollider(GameObject* Owner) :ColliderBase(Owner), Size(1.0f, 1.0f, 1.0f), Scale(-1.0f, -1.0f, 1.0f), Type2D(false) {}
		~BoxCollider() {}


		Vector3		GetSize() { return Size; }					//倍率の取得
		void		SetSize(Vector3 size) { Size = size; }		//倍率を設定

		bool		Get2DFlg() { return Type2D; }				//２Dフラグの取得
		void		Set2Dflg(bool flg) { Type2D = flg; }		//２D フラグの設定

		Vector3		GetScale() { return Scale; }				//大きさの取得
		void		SetScale(Vector3 scale) { Scale = scale; }	//大きさを設定

		void		Update();
		void		PhysicsUpdate();
		void		Draw() {}

#ifdef DEBUG
		void		ImGuiDebug();
#endif

		std::string GetType() { return "Box"; }
	};
}