#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../../DX11System/Shader.h"
#include "../../DX11System/DX11Settransform.h"
#include "../../DX11System/ImageResourceManager.h"
#include "../Buffer/ConstBuffer.h"
#include "../Buffer/StructuredBuffer.h"

namespace Component {
	class FogDistance :public ComponentBase
	{
		ALIGN16 struct OceanConStruct
		{
			float size;
			float uv;
			float dummy;
			float dummy2;
		};

	private:
		// shader
		ID3D11VertexShader* m_pFogVS = NULL;
		ID3D11PixelShader* m_pFogPS = NULL;

		Transform* tra;

		OceanConStruct m_oceanConStruct;
		ConstBuffer* m_oceanConBuffer;

		void DrawSetting();
	public:
		FogDistance(GameObject* _obj);
		~FogDistance();

		void Start();
		void DrawObject();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		// PS
		bool InitPixelShader(ID3D11Device* device,
			const char* szFileName,
			LPCSTR szEntryPoint,
			LPCSTR szShaderModel,
			ID3D11PixelShader** _ps)
		{
			bool sts = CreatePixelShader(CDirectXGraphics::GetInstance()->GetDXDevice(),		// �f�o�C�X�I�u�W�F�N�g
				szFileName,
				szEntryPoint,
				szShaderModel,
				_ps);

			return sts;
		}

		
		// �V�F�_�[�̈����ς���
		bool InitVertexShader(ID3D11Device* device,
			const char* szFileName,
			LPCSTR szEntryPoint,
			LPCSTR szShaderModel,
			ID3D11VertexShader** _vs)
		{
			HRESULT hr;

			ID3DBlob* pBlob = nullptr;

			void* ShaderObject;
			size_t	ShaderObjectSize;

			// �t�@�C���̊g���q�ɍ��킹�ăR���p�C��
			hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &ShaderObject, ShaderObjectSize, &pBlob);
			if (FAILED(hr))
			{
				if (pBlob)pBlob->Release();
				return false;
			}

			// ���_�V�F�[�_�[�𐶐�
			hr = device->CreateVertexShader(ShaderObject, ShaderObjectSize, nullptr, _vs);
			if (FAILED(hr))
			{
				if (pBlob)pBlob->Release();
				return false;
			}

			return true;
		}
	};
}