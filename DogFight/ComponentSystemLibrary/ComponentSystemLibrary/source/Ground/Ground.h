#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/Shader.h"
#include "../../DX11System/DX11Settransform.h"


namespace Component {
	class Ground :public ComponentBase
	{
	private:
		
		Transform* tra;		
	
		ComPtr < ID3D11ShaderResourceView> _tex;			// キューブマップテクスチャ
		ID3D11SamplerState* CubesTexSamplerState;			// キューブマップサンプラー

		void DrawSetting();

	public:
		Ground(GameObject* _obj);
		~Ground();

		void Start();
		void DrawObject();
		void Update();

	};
}