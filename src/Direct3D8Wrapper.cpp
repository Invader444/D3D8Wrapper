/*
 * This file is part of the Direct3D8Wrapper Library
 * Copyright (C) 2012  Jonathan M. Horowitz
 *
 * Direct3D8Wrapper is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Direct3D8Wrapper.cpp : Implementation of CDirect3D8Wrapper

#include "stdafx.h"

#include "Direct3D8Wrapper.h"

// CDirect3D8Wrapper

D3DFORMAT CDirect3D8Wrapper::allowedD3DFormats[] = {D3DFMT_A1R5G5B5, D3DFMT_A2R10G10B10, D3DFMT_A8R8G8B8,
                                                    D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_X8R8G8B8};

CDirect3D8Wrapper::CDirect3D8Wrapper()
{
}

CDirect3D8Wrapper::~CDirect3D8Wrapper()
{
}

HRESULT CDirect3D8Wrapper::FinalConstruct(THIS)
{
	this->pDirect3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	HRESULT hr = CoGetClassObject(IID_IDeviceWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pDeviceWrapperFactory);
	if (FAILED(hr))
	{
		this->pDeviceWrapperFactory = NULL;
	}

	// Adapter bookkeeping
	UINT adapterCount = this->pDirect3D9->GetAdapterCount();

	this->adapterInformation.resize(adapterCount);

	for (UINT i = 0; i < adapterCount; i++)
	{
		// For each device, save mode count
		UINT* formatCounts = new UINT[sizeof(CDirect3D8Wrapper::allowedD3DFormats) / sizeof(D3DFORMAT)];
		for (UINT j = 0; j < (sizeof(CDirect3D8Wrapper::allowedD3DFormats) / sizeof(D3DFORMAT)); j++)
		{
			formatCounts[j] = this->pDirect3D9->GetAdapterModeCount(i, CDirect3D8Wrapper::allowedD3DFormats[j]); 
		}
		this->adapterInformation.push_back(formatCounts);
	}

	InitializeCriticalSection(&this->createdDevicesMutex);

	return S_OK;
}

void CDirect3D8Wrapper::FinalRelease(THIS)
{
	if (this->pDirect3D9 != NULL)
	{
		this->pDirect3D9->Release();
		this->pDirect3D9 = NULL;
	}

	if (this->pDeviceWrapperFactory != NULL)
	{
		this->pDeviceWrapperFactory->Release();
		this->pDeviceWrapperFactory = NULL;
	}

	for (UINT i = 0; i < this->adapterInformation.size(); i++)
	{
		delete this->adapterInformation.at(i);
	}

	DeleteCriticalSection(&this->createdDevicesMutex);
}

// IDirect3D8Wrapper Methods
STDMETHODIMP CDirect3D8Wrapper::SetCreateDeviceCallback(void* pThis, void* pCallbackFunction)
{
	this->pCreateDeviceCallbackThis = pThis;
	this->pCreateDeviceCallbackFunction = pCallbackFunction;

	return S_OK;
}

// IWrapper Methods
STDMETHODIMP CDirect3D8Wrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pDirect3D9->AddRef();
	*ppvObject = this->pDirect3D9;

	return S_OK;
}

STDMETHODIMP CDirect3D8Wrapper::SetWrappedInterface(THIS_ void* pObject)
{
	return S_FALSE;
}

// IDirect3D8 Methods
STDMETHODIMP CDirect3D8Wrapper::RegisterSoftwareDevice(THIS_ void* pInitializeFunction)
{
	return this->pDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
}

STDMETHODIMP_(UINT) CDirect3D8Wrapper::GetAdapterCount(THIS)
{
	return this->pDirect3D9->GetAdapterCount();
}

STDMETHODIMP CDirect3D8Wrapper::GetAdapterIdentifier(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier)
{
	D3DADAPTER_IDENTIFIER9* pIdentifier9 = new D3DADAPTER_IDENTIFIER9();
	
	HRESULT result = this->pDirect3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier9);

	strncpy_s(pIdentifier->Driver, MAX_DEVICE_IDENTIFIER_STRING, pIdentifier9->Driver, MAX_DEVICE_IDENTIFIER_STRING);
	strncpy_s(pIdentifier->Description, MAX_DEVICE_IDENTIFIER_STRING, pIdentifier9->Description, MAX_DEVICE_IDENTIFIER_STRING);

	pIdentifier->VendorId = pIdentifier9->VendorId;
	pIdentifier->DeviceId = pIdentifier9->DeviceId;
	pIdentifier->SubSysId = pIdentifier9->SubSysId;
	pIdentifier->Revision = pIdentifier9->Revision;

	memcpy(&pIdentifier->DeviceIdentifier, &pIdentifier9->DeviceIdentifier, sizeof(GUID));

	pIdentifier->WHQLLevel = pIdentifier9->WHQLLevel;

#ifdef _WIN32
	memcpy(&pIdentifier->DriverVersion, &pIdentifier9->DriverVersion, sizeof(LARGE_INTEGER));
#else
	pIdentifier->DriverVersionLowPart = pIdentifier9->DriverVersionLowPart;
	pIdentifier->DriverVersionHighPart = pIdentifier9->DriverVersionHighPart;
#endif

	delete pIdentifier9;
	
	return result;
}

STDMETHODIMP_(UINT) CDirect3D8Wrapper::GetAdapterModeCount(THIS_ UINT Adapter)
{
	UINT result = 0;

	UINT* formatCounts = this->adapterInformation.at(Adapter);

	for (UINT i = 0; i < (sizeof(CDirect3D8Wrapper::allowedD3DFormats) / sizeof(D3DFORMAT)); i++)
	{
		result += formatCounts[i];
	}

	return result;
}

STDMETHODIMP CDirect3D8Wrapper::EnumAdapterModes(THIS_ UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode)
{
	HRESULT result = D3D_OK;

	UINT* formatCounts = this->adapterInformation.at(Adapter);

	UINT curModeRange = 0;
	for (UINT i = 0; i < (sizeof(CDirect3D8Wrapper::allowedD3DFormats) / sizeof(D3DFORMAT)); i++)
	{
		curModeRange += formatCounts[i];
		if (Mode < curModeRange)
		{
			result = this->pDirect3D9->EnumAdapterModes(Adapter,
			                                            CDirect3D8Wrapper::allowedD3DFormats[i],
			                                            Mode - (curModeRange - formatCounts[i]),
			                                            pMode);
			break;
		}
	}

	return result;
}

STDMETHODIMP CDirect3D8Wrapper::GetAdapterDisplayMode(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode)
{
	return this->pDirect3D9->GetAdapterDisplayMode(Adapter, pMode);
}

STDMETHODIMP CDirect3D8Wrapper::CheckDeviceType(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed)
{
	return this->pDirect3D9->CheckDeviceType(Adapter, CheckType, DisplayFormat, BackBufferFormat, Windowed);
}

STDMETHODIMP CDirect3D8Wrapper::CheckDeviceFormat(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
{
	return this->pDirect3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

STDMETHODIMP CDirect3D8Wrapper::CheckDeviceMultiSampleType(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType)
{
	return this->pDirect3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, NULL);
}

STDMETHODIMP CDirect3D8Wrapper::CheckDepthStencilMatch(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
{
	return this->pDirect3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

STDMETHODIMP CDirect3D8Wrapper::GetDeviceCaps(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps)
{
	D3DCAPS9* pCaps9 = new D3DCAPS9();

	HRESULT result = this->pDirect3D9->GetDeviceCaps(Adapter, DeviceType, pCaps9);

	memcpy(pCaps, pCaps9, sizeof(D3DCAPS8));

	delete pCaps9;

	return result;
}

STDMETHODIMP_(HMONITOR) CDirect3D8Wrapper::GetAdapterMonitor(THIS_ UINT Adapter)
{
	return this->pDirect3D9->GetAdapterMonitor(Adapter);
}

STDMETHODIMP CDirect3D8Wrapper::CreateDevice(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3D8PRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface)
{
	HRESULT result = D3DERR_NOTAVAILABLE;

	IDirect3DDevice9* pReturnedDeviceInterface9 = NULL;

	D3DPRESENT_PARAMETERS* pPresentationParameters9 = new D3DPRESENT_PARAMETERS();
	pPresentationParameters9->AutoDepthStencilFormat = pPresentationParameters->AutoDepthStencilFormat;
	pPresentationParameters9->BackBufferCount = pPresentationParameters->BackBufferCount;
	pPresentationParameters9->BackBufferFormat = pPresentationParameters->BackBufferFormat;
	pPresentationParameters9->BackBufferHeight = pPresentationParameters->BackBufferHeight;
	pPresentationParameters9->BackBufferWidth = pPresentationParameters->BackBufferWidth;
	pPresentationParameters9->EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
	pPresentationParameters9->Flags = pPresentationParameters->Flags;
	pPresentationParameters9->FullScreen_RefreshRateInHz = pPresentationParameters->FullScreen_RefreshRateInHz;
	pPresentationParameters9->hDeviceWindow = pPresentationParameters->hDeviceWindow;

	// Will always select best quality...
	this->pDirect3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, pPresentationParameters->BackBufferFormat, pPresentationParameters->Windowed, pPresentationParameters->MultiSampleType, &pPresentationParameters9->MultiSampleQuality);

	pPresentationParameters9->MultiSampleType = pPresentationParameters->MultiSampleType;
	pPresentationParameters9->PresentationInterval = pPresentationParameters->FullScreen_PresentationInterval;
	pPresentationParameters9->SwapEffect = pPresentationParameters->SwapEffect;
	pPresentationParameters9->Windowed = pPresentationParameters->Windowed;

	HRESULT tmpResult = this->pDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters9, &pReturnedDeviceInterface9);

	// Check if we already have a pointer to this device... and if we do, re-use it
	CDeviceWrapper* deviceWrapper = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pReturnedDeviceInterface9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdDevicesMutex);
		std::map<IUnknown*, CDeviceWrapper*>::iterator storedDevice = this->createdDevices.find(pUnknown);
		if (storedDevice == this->createdDevices.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pDeviceWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pDeviceWrapperFactory->CreateInstance(NULL, IID_IDeviceWrapper, (void**) &deviceWrapper)))
				{
					deviceWrapper->SetWrappedInterface((void**) &pReturnedDeviceInterface9);
					deviceWrapper->SetDirect3D8(this);
					deviceWrapper->SetZBufferDiscarding((pPresentationParameters->Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL) != 0);

					this->createdDevices.insert(std::pair<IUnknown*, CDeviceWrapper*>(pUnknown, deviceWrapper));

					result = tmpResult;
				}
			}
		}
		else
		{
			// Exists, so re-use
			deviceWrapper = storedDevice->second;
			deviceWrapper->AddRef();

			// Can this change with a different call to the same interface?
			deviceWrapper->SetZBufferDiscarding((pPresentationParameters->Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL) != 0);

			result = tmpResult;
		}
		LeaveCriticalSection(&this->createdDevicesMutex);

		pUnknown->Release();
	}
	else
	{
		// Assume we need to create a new device... which won't be tracked or re-used :(
		if (this->pDeviceWrapperFactory != NULL)
		{
			if (SUCCEEDED(this->pDeviceWrapperFactory->CreateInstance(NULL, IID_IDeviceWrapper, (void**) &deviceWrapper)))
			{
				deviceWrapper->SetWrappedInterface((void**) &pReturnedDeviceInterface9);
				deviceWrapper->SetDirect3D8(this);
				deviceWrapper->SetZBufferDiscarding((pPresentationParameters->Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL) != 0);

				result = tmpResult;
			}
		}
	}

	if (this->pCreateDeviceCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		deviceWrapper->AddRef();
		if (this->pCreateDeviceCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateDeviceCallback_t) this->pCreateDeviceCallbackFunction)((IDeviceWrapper*) deviceWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateDeviceCallback_t) this->pCreateDeviceCallbackFunction)(this->pCreateDeviceCallbackThis, (IDeviceWrapper*) deviceWrapper);
		}
	}

	*ppReturnedDeviceInterface = deviceWrapper;

	delete pPresentationParameters9;

	return result;
}
