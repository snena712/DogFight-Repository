#pragma once
#include	<unordered_map>
#include	<d3d11.h>
#include	<string>
#include	"CDirectxGraphics.h"
#include	"shader.h"

class ShaderHashmap {
public:
	ShaderHashmap(const ShaderHashmap&) = delete;
	ShaderHashmap& operator=(const ShaderHashmap&) = delete;
	ShaderHashmap(ShaderHashmap&&) = delete;
	ShaderHashmap& operator=(ShaderHashmap&&) = delete;

	static ShaderHashmap* GetInstance() {
		static ShaderHashmap instance;
		return &instance;
	}

public:
	// ピクセルシェーダーセット
	bool SetPixelShader(std::string filename) {
		
		// 存在するかを確かめる
		auto it = m_pshashmap.find(filename);
		if (it != m_pshashmap.end()) {
			return true;	//既に存在するのでtrueを返す
		}
		
		ID3D11PixelShader* psh;

		// デバイス取得
		ID3D11Device* device;
		device = CDirectXGraphics::GetInstance()->GetDXDevice();

		// ピクセルシェーダーオブジェクトを生成
		bool sts = CreatePixelShader(device,
			filename.c_str(),
			"main",
			"ps_5_0",
			&psh);

		if (!sts) {
			MessageBox(nullptr, "CreatePixel Shader error", "error", MB_OK);
			return false;
		}

		// unordered_mapコンテナに格納
		m_pshashmap[filename] = psh;

		return true;
	}

	// ジオメトリシェーダーセット
	bool SetGeometryShader(std::string filename) {
		
		// 存在するかを確かめる
		auto it = m_gshashmap.find(filename);
		if (it != m_gshashmap.end()) {
			return true;	//既に存在するのでtrueを返す
		}
		
		ID3D11GeometryShader* gsh;

		// デバイス取得
		ID3D11Device* device;
		device = CDirectXGraphics::GetInstance()->GetDXDevice();

		// ピクセルシェーダーオブジェクトを生成
		bool sts = CreateGeometryShader(device,
			filename.c_str(),
			"main",
			"gs_5_0",
			&gsh);

		if (!sts) {
			MessageBox(nullptr, "CreateGeometry Shader error", "error", MB_OK);
			return false;
		}

		// unordered_mapコンテナに格納
		m_gshashmap[filename] = gsh;

		return true;
	}

	// 頂点シェーダーセット
	bool SetVertexShader(std::string filename,
		D3D11_INPUT_ELEMENT_DESC* layout,
		unsigned int numElements) {

		// 存在するかを確かめる
		auto it = m_vshashmap.find(filename);
		if (it != m_vshashmap.end()) {
			return true;	//既に存在するのでtrueを返す
		}

		ID3D11VertexShader* vsh;
		ID3D11InputLayout*  vlayout;

		// デバイス取得
		ID3D11Device* device;
		device = CDirectXGraphics::GetInstance()->GetDXDevice(); 

		ID3DBlob* pBlob = nullptr;

		void* ShaderObject;
		size_t	ShaderObjectSize;

		// ファイルの拡張子に合わせてコンパイル
		HRESULT hr = CompileShader(
			filename.c_str(),
			"main",
			"vs_5_0",
			&ShaderObject,
			ShaderObjectSize,
			&pBlob);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// 頂点シェーダーを生成
		hr = device->CreateVertexShader(
			ShaderObject,
			ShaderObjectSize,
			nullptr,
			&vsh);
		if (FAILED(hr))
		{
			if (pBlob)pBlob->Release();
			return false;
		}

		// 頂点データ定義生成
		hr = device->CreateInputLayout(
			layout,
			numElements,
			ShaderObject,
			ShaderObjectSize,
			&vlayout);
		if (FAILED(hr)) {
			MessageBox(nullptr, "CreateInputLayout error", "error", MB_OK);
			pBlob->Release();
			return false;
		}

		// unordered_mapコンテナに格納
		m_vshashmap[filename] = vsh;
		m_layouthashmap[filename] = vlayout;

		// メモリ解放
		if (pBlob)pBlob->Release();

		return true;
	}

	// 頂点シェーダー取得
	ID3D11VertexShader* GetVertexShader(std::string filenamekey) {
		// 存在するかを確かめる
		auto it = m_vshashmap.find(filenamekey);
		if (it == m_vshashmap.end()) {
			return nullptr;
		}
		return m_vshashmap[filenamekey];
	}

	// ジオメトリシェーダー取得
	ID3D11GeometryShader* GetGeometryShader(std::string filenamekey) {
		// 存在するかを確かめる
		auto it = m_gshashmap.find(filenamekey);
		if (it == m_gshashmap.end()) {
			return nullptr;
		}
		return m_gshashmap[filenamekey];
	}

	// ピクセルシェーダー取得
	ID3D11PixelShader* GetPixelShader(std::string filenamekey) {
		// 存在するかを確かめる
		auto it = m_pshashmap.find(filenamekey);
		if (it == m_pshashmap.end()) {
			return nullptr;
		}
		return m_pshashmap[filenamekey];
	}

	// 頂点レイアウト取得
	ID3D11InputLayout* GetVertexLayout(std::string filenamekey) {
		// 存在するかを確かめる
		auto it = m_layouthashmap.find(filenamekey);
		if (it == m_layouthashmap.end()) {
			return nullptr;
		}
		return m_layouthashmap[filenamekey];
	}

	// 終了処理
	void Exit() {

		// 解放
		for (auto data : m_vshashmap) {
			data.second->Release();
		}

		m_vshashmap.clear();

		// 解放
		for (auto data : m_gshashmap) {
			data.second->Release();
		}

		m_gshashmap.clear();

		// 解放
		for (auto data : m_pshashmap) {
			data.second->Release();
		}

		m_pshashmap.clear();

		// 解放
		for (auto data : m_layouthashmap) {
			data.second->Release();
		}

		m_layouthashmap.clear();
	}
private:
	ShaderHashmap() {}
	std::unordered_map<std::string, ID3D11VertexShader*> m_vshashmap;
	std::unordered_map<std::string, ID3D11GeometryShader*> m_gshashmap;
	std::unordered_map<std::string, ID3D11PixelShader*> m_pshashmap;
	std::unordered_map<std::string, ID3D11InputLayout*> m_layouthashmap;
};