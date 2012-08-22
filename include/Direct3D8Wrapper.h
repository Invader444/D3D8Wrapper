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

// Direct3D8Wrapper.h : Declaration of the CWrapper
#pragma once

#include "resource.h"       // main symbols

#include "D3D8Wrapper_i.h"

#include "stdafx.h"

// Forward Declaration
class CDirect3D8Wrapper;

#include <vector>
#include <map>
#include <utility>

#include "DeviceWrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CDirect3D8Wrapper

class ATL_NO_VTABLE CDirect3D8Wrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDirect3D8Wrapper, &CLSID_Direct3D8Wrapper>,
	public IDirect3D8Wrapper,
	public IDirect3D8
{
public:
	CDirect3D8Wrapper();
	~CDirect3D8Wrapper();

	DECLARE_REGISTRY_RESOURCEID(IDR_DIRECT3D8WRAPPER)

	DECLARE_NOT_AGGREGATABLE(CDirect3D8Wrapper)

	BEGIN_COM_MAP(CDirect3D8Wrapper)
		COM_INTERFACE_ENTRY(IDirect3D8Wrapper)
		COM_INTERFACE_ENTRY_IID(IID_IDirect3D8, CDirect3D8Wrapper)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

private:
	IDirect3D9* pDirect3D9;

	IClassFactory* pDeviceWrapperFactory;

	std::vector<UINT*> adapterInformation;

	std::map<IUnknown*, CDeviceWrapper*> createdDevices;
	CRITICAL_SECTION createdDevicesMutex;

	static D3DFORMAT allowedD3DFormats[];

	void* pCreateDeviceCallbackThis;
	void* pCreateDeviceCallbackFunction;

public:
	// IDirect3D8 Methods
	STDMETHOD(RegisterSoftwareDevice)(THIS_ void* pInitializeFunction);
    STDMETHOD_(UINT, GetAdapterCount)(THIS);
    STDMETHOD(GetAdapterIdentifier)(THIS_ UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER8* pIdentifier);
    STDMETHOD_(UINT, GetAdapterModeCount)(THIS_ UINT Adapter);
    STDMETHOD(EnumAdapterModes)(THIS_ UINT Adapter, UINT Mode, D3DDISPLAYMODE* pMode);
    STDMETHOD(GetAdapterDisplayMode)(THIS_ UINT Adapter, D3DDISPLAYMODE* pMode);
    STDMETHOD(CheckDeviceType)(THIS_ UINT Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);
    STDMETHOD(CheckDeviceFormat)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    STDMETHOD(CheckDeviceMultiSampleType)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
    STDMETHOD(CheckDepthStencilMatch)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    STDMETHOD(GetDeviceCaps)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS8* pCaps);
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(THIS_ UINT Adapter);
    STDMETHOD(CreateDevice)(THIS_ UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3D8PRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice8** ppReturnedDeviceInterface);

	// IWrapper Methods
	STDMETHOD(GetWrappedInterface)(THIS_ void** ppvObject);
	STDMETHOD(SetWrappedInterface)(THIS_ void* pObject);

	// IDirect3D8Wrapper Methods
	STDMETHOD(SetCreateDeviceCallback)(THIS_ void* pThis, void* pCallbackFunction);
};

OBJECT_ENTRY_AUTO(__uuidof(Direct3D8Wrapper), CDirect3D8Wrapper)
