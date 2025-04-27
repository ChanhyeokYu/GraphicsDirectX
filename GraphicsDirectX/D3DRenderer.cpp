#include "D3DRenderer.h"

#pragma comment(lib, "d3dcompiler.lib")


bool D3DRenderer::Init(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd = {}; // 스왑 체인의 정보를 담을 구조체 선언
	scd.BufferCount = 1;
	scd.BufferDesc.Width = 800;
	scd.BufferDesc.Height = 600;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32비트 컬러
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1; // 멀티 샘플링 사용안함
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 스왑 후 활동. 해당 활동은 스왑 후 백버퍼의 내용을 버림

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &swapChain, &device, nullptr, &context);

	if (FAILED(hr))
	{
		return false;
	}
	// 백버퍼 얻기
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	//백버퍼 -> 렌더 타겟 뷰로 전환
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView); // 디바이스가 볼 타겟을 얻음

	//랜더 타켓 바인딩
	context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr); //디바이스가 보고 있는 타겟에 랜더링 명령

	return true;
}

void D3DRenderer::ClearScreen(float r, float g, float b, float a)
{
	float color[4] = { r,g,b,a };
	context->ClearRenderTargetView(renderTargetView.Get(), color);
}

void D3DRenderer::Present()
{
	swapChain->Present(1, 0);// 화면에 표시
}

bool D3DRenderer::CreateTriangleResources()
{
	// 정점 데이터 정의
	Vertex vertices[] = {
		{ 0.0f,   0.5f,  0.0f,   1.0f, 0.0f, 0.0f},
		{ 0.5f,  -0.5f,  0.0f,   1.0f, 1.0f, 0.0f},
		{-0.5f,  -0.5f,  0.0f,   0.0f, 0.0f, 1.0f},
	};

	// 버퍼 설정
	CD3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 버퍼 데이터 지정
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
	// 정점 버퍼 바인딩
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// 입력 레이아웃 설정
	context->IASetInputLayout(inputLayout.Get());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 셰이더 바인딩
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);

	context->Draw(3, 0);

}
