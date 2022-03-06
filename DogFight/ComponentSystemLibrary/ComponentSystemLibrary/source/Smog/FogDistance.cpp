#include "FogDistance.h"


using namespace Component;

// 地面生成
FogDistance::FogDistance(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("Ground");

	tra = owner->transform;

	// 各SRVの設定
	ImageResourceManager::GetInstance()->Load("assets/Image/Ocean/Ocean2.jpg");

	// Shader初期化
	InitVertexShader(CDirectXGraphics::GetInstance()->GetDXDevice(),
		"shader/Fog.hlsl",
		"VS",
		"vs_5_0",
		&m_pFogVS);

	InitPixelShader(CDirectXGraphics::GetInstance()->GetDXDevice(),
		"shader/Fog.hlsl",
		"PS",
		"ps_5_0",
		&m_pFogPS);



	RenderingType render = RenderingType::Opaque;
	SceneManager::AddDrawFunction(render, owner);


	m_oceanConBuffer = new ConstBuffer();
	m_oceanConBuffer->CreateConStructuredBuffer<OceanConStruct>(ConstBuffer::Type::Normal,1);

	// 場所指定　中心になるようにする
	tra->position = Vector3(-3000, 0, -3000);
	tra->RotateLocalXaxis(90);
	tra->scale = Vector3(500, 500, 1);
}

FogDistance::~FogDistance()
{

}

void FogDistance::Start()
{
}

void FogDistance::DrawObject() {

	DrawSetting();
}

/// <summary>
/// デストラクタ。
/// </summary>
void FogDistance::DrawSetting()
{
	// Shaderのセット

	// 各バッファの更新

	// 各バッファの設定（定数やカメラ類）

	// SRVの設定

	// 画像の設定


	//// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	//// ワールド変換行列セット

	DirectX::XMFLOAT4X4 make, mtx, scale;
	DX11MtxIdentity(mtx);	//ワールド変換行列を単位行列にする

	DX11MtxScale(tra->scale.x, tra->scale.y, tra->scale.z, scale);
	DX11MtxFromQt(mtx, tra->qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = tra->position.x;
	make._42 = tra->position.y;
	make._43 = tra->position.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);

	// バッファのセット
	m_oceanConStruct.size = 200;
	m_oceanConStruct.uv++;
	m_oceanConBuffer->NoPointerUpdate(m_oceanConStruct);
	m_oceanConBuffer->SetVSBuffer(3);
	m_oceanConBuffer->SetGSBuffer(3);

	////　初期化
	//ID3D11Buffer* ppCBNULL[2] = { NULL,NULL };
	//devcontext->VSSetConstantBuffers(0, 2, ppCBNULL);

	//// 各SRVの設定
	ID3D11ShaderResourceView* srv = ImageResourceManager::GetInstance()->GetResource("assets/Image/Ocean/Ocean2.jpg")->GetSRV();
	devcontext->PSSetShaderResources(0, 1, &srv);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得		
	devcontext->VSSetShader(m_pFogVS, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(m_pFogPS, nullptr, 0);

	//// 描画の設定
	//ID3D11Buffer* const                 g_pNullBuffer = NULL;    // Helper to Clear Buffers
	//UINT                                g_iNullUINT = 0;         // Helper to Clear Buffers
	//devcontext->IASetVertexBuffers(0, 1, &g_pNullBuffer, &g_iNullUINT, &g_iNullUINT);   //NULL
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);             // リスト

	// ドローコール発行
	devcontext->Draw(
		m_oceanConStruct.size*m_oceanConStruct.size,			// インデックス数　　（数は後で決める）
		0);														// 基準頂点インデックス

	devcontext->GSSetShader(nullptr
		, nullptr, 0);
}

