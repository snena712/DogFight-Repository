#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//Ýè·é¼aÌæ¾æ
enum class CircleType {
	X,
	Y,
	Auto
};


namespace Component {
	class CircleCollider :public ColliderBase {

	private:

		float Size;							//{¦
		CircleType Type;					//¼aÝèÌType
		float Radius;						//¼a

		void GetRadiusSize();
		void AddColliderList();
	public:
		CircleCollider(GameObject* Owner) :ColliderBase(Owner), Type(CircleType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~CircleCollider() {}

		float		GetRadius() { return Radius; }						//¼aÌæ¾
		void		SetRadius(float radius) { Radius = radius; }		//¼aðÝè

		float		GetSize() { return Size; }							//{¦Ìæ¾
		void		SetSize(float size) { Size = size; }				//{¦ðÝè

		auto		GetCircleType() { return Type; }										//½è»è^CvÌæ¾
		void		SetCircleType(CircleType type) { Type = type; GetRadiusSize(); }		//½è»è^CvÌÝè

		void Update();
		void PhysicsUpdate();

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Circle"; }
	};
}
