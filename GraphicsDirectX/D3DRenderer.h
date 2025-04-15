#pragma once
#include <d3d11.h>
#include <wrl.h>

// �߿����� ���� ü���� �̿��� ���������� ���۹� -> ����Ʈ ������ ��ü�ϸ鼭 ���� ���� ȭ�鿡 ����ϴ� ��

class D3DRenderer
{
public:
	bool Init(HWND hWnd); // �ʱ�ȭ
	void ClearScreen(float r, float g, float b, float a); // ��� ���� Ŭ����
	void Present(); // ���

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device; // GPU ����̽�
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context; // ����̽� ���ؽ�Ʈ
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; // ����ü��
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView; // ���� Ÿ��

};

