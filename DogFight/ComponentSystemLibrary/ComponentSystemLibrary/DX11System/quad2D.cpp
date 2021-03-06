#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"../Application.h"
#include	"quad2D.h"
#include	"ImageResourceManager.h"

// comptr
using Microsoft::WRL::ComPtr;

// 使用する頂点シェーダー名
const char* Quad2D::vsfilename[] = {
	"shader/quad2dvs.hlsl"	,	//通常（2D）
};

// 使用するピクセルシェーダー名
const char* Quad2D::psfilename[] = {
	"shader/quad2dps.hlsl",		//通常（2D）
	"shader/multips.hlsl"
};

// 矩形の初期化
bool Quad2D::Init(
	int width,
	int height,
	DirectX::XMFLOAT4 color,
	DirectX::XMFLOAT2* uv)
{
	DX11MtxIdentity(m_worldmtx);	// 初期姿勢

	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(quad2dps.hlsl)", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(multips.hlsl)", "error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数

	// 頂点初期化
	updateVertex(width, height, color, uv);

	// デバイスを取得
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Quad2D::Vertex),		// １頂点当たりバイト数
		4,							// 頂点数
		m_vertex,					// 頂点データ格納メモリ先頭アドレス
		&m_vertexbuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// インデックスバッファを生成
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// デバイスオブジェクト
		4,							// インデックス数
		index,						// インデックス格納メモリ先頭アドレス
		&m_indexbuffer				// インデックスバッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	// ビューポートサイズセット
	SetViewPortSize(Application::Instance()->CLIENT_WIDTH, Application::Instance()->CLIENT_HEIGHT);

	return true;
}

// 描画
void Quad2D::Draw() {

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// テクスチャをセット
	ID3D11ShaderResourceView* srv[] = { ImageResourceManager::GetInstance()->GetResource(m_texFileName)->GetSRV() };
	devcontext->PSSetShaderResources(0, 1, srv);

	// ワールド変換行列をセット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	//シェーダーセット
	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}

// マルチテクスチャの描画
void Quad2D::MultiDraw()
{
	DirectX::XMFLOAT4X4 mtx;

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ワールド変換行列をセット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad2D::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[1]);
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}

void Quad2D::Exit() {

}

bool Quad2D::SetViewPortSize(uint32_t width, uint32_t height)
{
	// デバイスコンテキストを取得
	ID3D11Device* device = CDirectXGraphics::GetInstance()->GetDXDevice();
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	ComPtr<ID3D11Buffer> CBViewPort;

	// ビューポート用の定数バッファ
	struct ConstantBufferViewPort {
		uint32_t	Width[4];			// ビューポート幅
		uint32_t	Height[4];			// ビューポート高さ
	};

	// ビューポート定数バッファに値をセット
	bool sts = CreateConstantBufferWrite(
		device,
		sizeof(ConstantBufferViewPort),
		&CBViewPort);
	if (!sts) {
		return false;
	}

	ConstantBufferViewPort cb;
	cb.Width[0] = width;
	cb.Height[0] = height;

	D3D11_MAPPED_SUBRESOURCE pData;

	HRESULT hr = devcontext->Map(CBViewPort.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(ConstantBufferViewPort));
		devcontext->Unmap(CBViewPort.Get(), 0);
	}
	else {
		return false;
	}

	// 頂点シェーダーにセット
	devcontext->VSSetConstantBuffers(5, 1, CBViewPort.GetAddressOf());

	return true;
}

// 拡大、縮小
void Quad2D::SetScale(float sx, float sy, float sz) {
	DX11MtxScale(sx, sy, sz, m_worldmtx);
}

// 位置をセット
void Quad2D::SetPosition(float x, float y, float z) {
	m_worldmtx._41 = x;
	m_worldmtx._42 = y;
	m_worldmtx._43 = z;
}

// 位置と回転
void Quad2D::SetPosRot(Vector3 pos, Vector3 rot) {
	Vector3 zero = Vector3::zero;
	DX11MakeWorldMatrix(m_worldmtx, zero, rot, pos);
}

void Quad2D::updateVertex(
	float width,
	float height,
	DirectX::XMFLOAT4 color,
	DirectX::XMFLOAT2* uv)
{

	m_width = width;
	m_height = height;

	Quad2D::Vertex	v[4] = {
		// 座標													// カラー	// UV	
		XMFLOAT3(-m_width / 2.0f,	-m_height / 2.0f, 0.0f),	color,		uv[0],
		XMFLOAT3(m_width / 2.0f,	-m_height / 2.0f, 0.0f),	color,		uv[1],
		XMFLOAT3(-m_width / 2.0f,	 m_height / 2.0f, 0.0f),	color,		uv[2],
		XMFLOAT3(m_width / 2.0f,	 m_height / 2.0f, 0.0f),	color,		uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}


// 頂点バッファ更新
void Quad2D::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Quad2D::Vertex) * 4);
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}