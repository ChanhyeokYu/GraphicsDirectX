#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include "Inc//WICTextureLoader.h"


// 중요점은 스왑 체인을 이용해 지속적으로 백퍼버 -> 프론트 버퍼을 교체하면서 끊김 없이 화면에 출력하는 것

struct Vertex
{
	float x, y, z; // 위치 값
	float u, v;
};

class D3DRenderer
{
public:
	bool Init(HWND hWnd); // 초기화
	void ClearScreen(float r, float g, float b, float a); // 배경 색상 클리어
	void Present(); // 출력
	bool CreateTriangleResources();
	bool CreateConstantBuffer();
	bool CreateSampler();

	void DrawTriangle();
	bool LoadTexture(const std::wstring& filename);

	void SetTransform(const DirectX::XMMATRIX& matrix);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device; // GPU 디바이스
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context; // 디바이스 컨텍스트
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; // 스왑체인
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView; // 렌더 타겟
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

};

