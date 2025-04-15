#include "D3DRenderer.h"

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
