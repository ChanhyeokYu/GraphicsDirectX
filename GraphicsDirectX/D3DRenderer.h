#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <string>
#include "Inc//WICTextureLoader.h"


// �߿����� ���� ü���� �̿��� ���������� ���۹� -> ����Ʈ ������ ��ü�ϸ鼭 ���� ���� ȭ�鿡 ����ϴ� ��

struct Vertex
{
	float x, y, z; // ��ġ ��
	float u, v;
};

class D3DRenderer
{
public:
	bool Init(HWND hWnd); // �ʱ�ȭ
	void ClearScreen(float r, float g, float b, float a); // ��� ���� Ŭ����
	void Present(); // ���
	bool CreateTriangleResources();
	bool CreateConstantBuffer();
	bool CreateSampler();

	void DrawTriangle();
	bool LoadTexture(const std::wstring& filename);

	void SetTransform(const DirectX::XMMATRIX& matrix);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device; // GPU ����̽�
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context; // ����̽� ���ؽ�Ʈ
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; // ����ü��
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView; // ���� Ÿ��
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

};

