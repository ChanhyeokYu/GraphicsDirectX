#include "D3DRenderer.h"
#include <d3dcompiler.h>
#include <assert.h>

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler")

using namespace DirectX;


bool D3DRenderer::Init(HWND hWnd)
{
	UINT flags = 0;
#if defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // defined(_DEBUG)

	DXGI_SWAP_CHAIN_DESC scd = {}; // ���� ü���� ������ ���� ����ü ����
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 800;
	scd.BufferDesc.Height = 600;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32��Ʈ �÷�
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1; // ��Ƽ ���ø� ������
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ���� �� Ȱ��. �ش� Ȱ���� ���� �� ������� ������ ����

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context);

	if (FAILED(hr))
	{
		return false;
	}
	// ����� ���
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	//����� -> ���� Ÿ�� ��� ��ȯ
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView); // ����̽��� �� Ÿ���� ����

	//���� Ÿ�� ���ε�
	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr); //����̽��� ���� �ִ� Ÿ�ٿ� ������ ���

	// D11 ���� ����Ʈ�� �־������
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);

	CreateConstantBuffer();

	return true;
}

void D3DRenderer::ClearScreen(float r, float g, float b, float a)
{
	float color[4] = { r,g,b,a };
	context->ClearRenderTargetView(renderTargetView.Get(), color);
}

void D3DRenderer::Present()
{
	swapChain->Present(1, 0);// ȭ�鿡 ǥ��
}

bool D3DRenderer::CreateTriangleResources()
{
	// ���� ������ ����
	Vertex vertices[] = {
		{ 0.0f,   0.5f,  0.0f,   0.5f, 0.0f},
		{ 0.5f,  -0.5f,  0.0f,   1.0f, 1.0f},
		{-0.5f,  -0.5f,  0.0f,   0.0f, 1.0f},
	};

	// ���� ����
	CD3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ����
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	HRESULT hr = device->CreateBuffer(&bd, &initData, &vertexBuffer);
	
	if (FAILED(hr))
	{
		return false;	
	}

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	hr = D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, nullptr);
	if (FAILED(hr))
	{
		return false;
	}

	device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);

	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	hr = D3DCompileFromFile(L"Shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, nullptr);
	if (FAILED(hr))
	{
		return false;
	}

	device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0} ,
		{ "TEXTCODE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = device->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);
	if (FAILED(hr))
	{
		return false;
	}

	return true;

}

bool D3DRenderer::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(XMMATRIX);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr = device->CreateBuffer(&cbd, nullptr, &constantBuffer);

	return SUCCEEDED(hr);
}

bool D3DRenderer::CreateSampler()
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	HRESULT hr = device->CreateSamplerState(&desc, &samplerState);
	return SUCCEEDED(hr);
}

void D3DRenderer::DrawTriangle()
{
	// �ؽ�ó ���ε�
	context->PSSetShaderResources(0, 1, textureView.GetAddressOf());
	context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	// ���� ���� ���ε�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// �Է� ���̾ƿ� ����
	context->IASetInputLayout(inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���̴� ���ε�
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);

	assert(vertexBuffer.Get() != nullptr);
	assert(vertexShader.Get() != nullptr);
	assert(pixelShader.Get() != nullptr);
	assert(inputLayout.Get() != nullptr);


	context->Draw(3, 0);

}

bool D3DRenderer::LoadTexture(const std::wstring& filename)
{
	HRESULT hr = DirectX::CreateWICTextureFromFile(device.Get(), context.Get(), filename.c_str(), nullptr, &textureView);
	std::wstring error = std::to_wstring(hr);
	OutputDebugString(error.c_str());
	return SUCCEEDED(hr);
	//return false;
}

void D3DRenderer::SetTransform(const DirectX::XMMATRIX& matrix)
{
	context->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &matrix, 0, 0);
	context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
}
