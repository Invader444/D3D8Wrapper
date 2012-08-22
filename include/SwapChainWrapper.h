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

// SwapChainWrapper.h : Declaration of the CSwapChainWrapper
#pragma once

#include "resource.h"       // main symbols

#include "D3D8Wrapper_i.h"

#include "stdafx.h"

// Forward Declaration
class CSwapChainWrapper;

#include "DeviceWrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CSwapChainWrapper

class ATL_NO_VTABLE CSwapChainWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSwapChainWrapper, &CLSID_SwapChainWrapper>,
	public ISwapChainWrapper,
	public IDirect3DSwapChain8
{
public:
	CSwapChainWrapper();
	~CSwapChainWrapper();

	DECLARE_REGISTRY_RESOURCEID(IDR_SWAPCHAINWRAPPER)

	DECLARE_NOT_AGGREGATABLE(CSwapChainWrapper)

	BEGIN_COM_MAP(CSwapChainWrapper)
		COM_INTERFACE_ENTRY(ISwapChainWrapper)
		COM_INTERFACE_ENTRY_IID(IID_IDirect3DSwapChain8, CSwapChainWrapper)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

	// Internal Methods
	HRESULT SetDirect3D8Device(CDeviceWrapper* pObject);

private:
	IDirect3DSwapChain9* pSwapChain9;

	CDeviceWrapper* pDeviceWrapper;

public:
	// IDirect3DSwapChain8 Methods
	STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	STDMETHOD(GetBackBuffer)(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer);

	// IWrapper Methods
	STDMETHOD(GetWrappedInterface)(THIS_ void** ppvObject);
	STDMETHOD(SetWrappedInterface)(THIS_ void* pObject);
};

OBJECT_ENTRY_AUTO(__uuidof(SwapChainWrapper), CSwapChainWrapper)
