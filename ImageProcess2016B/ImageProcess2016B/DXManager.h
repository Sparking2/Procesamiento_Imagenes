#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#ifndef  SAFE_RELEASE
#define SAFE_RELEASE(x) if((x)){(x)->Release();(x)=0;}
#endif // ! SAFE_RELEASE

class CDXManager
{
protected:
	ID3D11Device* m_pDevice; //Factory of Device Resources
	ID3D11DeviceContext* m_pContext; //Command Queue and Command Record Execution
	IDXGISwapChain* m_SwapChain; //Buffers Chain (Backbuffer)


public:
	CDXManager();
	~CDXManager();

	// Enumera todos los adaptadores graficos habilitados y con controlador válido. Retorna el adaptador seleccionado por el usuario
	static IDXGIAdapter* EnumAndChooseAdapter(HWND hWndOwner);
	// Inicializa los recursos de dispositivo, cadena de intercambio y contexto para enviar comandos al GPU o CPU
	bool Initialize(HWND hWnd, bool bUseWARP, IDXGIAdapter* pAdapter);
	// Terminar la sesion DX11
	void Uninitialize();
};

