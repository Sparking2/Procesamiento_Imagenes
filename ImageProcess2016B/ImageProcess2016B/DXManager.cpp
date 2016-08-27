#include "stdafx.h"
#include "DXManager.h"




CDXManager::CDXManager()
{
	m_pContext = NULL;
	m_pDevice = NULL;
	m_SwapChain = NULL;
}


CDXManager::~CDXManager()
{
}


// Enumera todos los adaptadores graficos habilitados y con controlador válido. Retorna el adaptador seleccionado por el usuario
IDXGIAdapter* CDXManager::EnumAndChooseAdapter(HWND hWndOwner)
{
	IDXGIFactory* pFactory = NULL;
	HRESULT hr = CreateDXGIFactory(IID_IDXGIFactory, (void**)&pFactory);
	if (FAILED(hr))
		return NULL;

	IDXGIAdapter* pAdapter = NULL;
	for (unsigned int iAdapter = 0; 1; iAdapter++) {
		hr = pFactory->EnumAdapters(iAdapter, &pAdapter);
		if (FAILED(hr))
			break;
		DXGI_ADAPTER_DESC dad;
		memset(&dad, 0, sizeof(dad));
		pAdapter->GetDesc(&dad);
		wchar_t szMessage[4096];
		wsprintf(szMessage,L"¿Desea utilizar éste adaptador?\r\nDescripción:%s\r\nMemoria Dedicada Video:\t%dMB\r\nMemoria Dedicada de Sistema:\t%dMB\r\nMemoria Compartida de Sistema:%dMB",
			dad.Description, dad.DedicatedVideoMemory / (1024 * 1024), dad.DedicatedSystemMemory / (1024 * 1024), dad.SharedSystemMemory / (1024 * 1024));
		switch (MessageBox(hWndOwner,szMessage,L"Selección de adaptador",MB_YESNOCANCEL|MB_ICONQUESTION))
		{
		case IDYES:
			pFactory->Release();
			pFactory = NULL;
			return pAdapter;
		
		case IDNO:
			pAdapter->Release();
			pAdapter = NULL;
			break;

		case IDCANCEL:
			pAdapter->Release();
			pFactory->Release();
			return NULL;

		default:
			break;
		}
	}
	pFactory->Release();
	return pAdapter;
}


// Inicializa los recursos de dispositivo, cadena de intercambio y contexto para enviar comandos al GPU o CPU
bool CDXManager::Initialize(HWND hWnd, bool bUseWARP, IDXGIAdapter* pAdapter)
{
	DXGI_SWAP_CHAIN_DESC dscd;
	memset(&dscd, 0, sizeof(dscd));
	dscd.BufferCount = 2;
	dscd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dscd.BufferDesc.RefreshRate.Denominator = 0;
	dscd.BufferDesc.RefreshRate.Numerator = 0;
	dscd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
	dscd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
	RECT rc;
	GetClientRect(hWnd, &rc);
	dscd.BufferDesc.Height = rc.bottom;
	dscd.BufferDesc.Width = rc.right;
	dscd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	dscd.Flags = 0;
	dscd.OutputWindow = hWnd;
	dscd.SampleDesc.Count = 1;
	dscd.SampleDesc.Quality = 0;
	dscd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//Cambiar a fullSCREEEN!!! cuando termines
	dscd.Windowed = true;
	

	//dscd.BufferDesc.
		D3D_FEATURE_LEVEL Requested = D3D_FEATURE_LEVEL_11_0, Detected;
		if (bUseWARP)
		{

			D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, &Requested, 1,
				D3D11_SDK_VERSION, &dscd, &m_SwapChain, &m_pDevice, &Detected, &m_pContext);
		}
		else
			
		{
			HRESULT hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				0,
				&Requested,
				1,
				D3D11_SDK_VERSION,
				&dscd,
				&m_SwapChain,&m_pDevice,&Detected,&m_pContext
			);
			/*
			if (pAdapter) {
				HRESULT hr = D3D11CreateDeviceAndSwapChain(
					pAdapter,D3D_DRIVER_TYPE_UNKNOWN,0,0,
					&Requested,1,D3D11_SDK_VERSION,
					&dscd, &m_SwapChain, &m_pDevice, &Detected, &m_pContext
				);
				if (FAILED(hr))
					return false;
			}
			else {
					HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0,
					&Requested, 1, D3D11_SDK_VERSION,
					&dscd, &m_SwapChain, &m_pDevice, &Detected, &m_pContext);
					if (FAILED(hr))
						return false;
			}*/
		
		}
	return true;
}


// Terminar la sesion DX11
void CDXManager::Uninitialize()
{
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_SwapChain);
}
