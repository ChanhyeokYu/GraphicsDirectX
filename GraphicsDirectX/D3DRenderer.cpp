#include "D3DRenderer.h"

#pragma comment(lib, "d3dcompiler.lib")


bool D3DRenderer::Init(HWND hWnd)
{
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context);

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
		{ 0.0f,   0.5f,  0.0f,   1.0f, 0.0f, 0.0f},
		{ 0.5f,  -0.5f,  0.0f,   1.0f, 1.0f, 0.0f},
		{-0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f},
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

	Microsoft::WRL::ComPtr<ID3DBlob> vsBolob;
	hr = D3DCompilefromFile

	return SUCCEEDED(hr);
}

void D3DRenderer::DrawTriangle()
{
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

	context->Draw(3, 0);

}
