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

// VolumeTextureWrapper.h : Declaration of the CVolumeTextureWrapper
#pragma once

#include "resource.h"       // main symbols

#include "D3D8Wrapper_i.h"

#include "stdafx.h"

// Forward Declaration
class CVolumeTextureWrapper;

#include "DeviceWrapper.h"
#include "VolumeWrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CVolumeTextureWrapper

class ATL_NO_VTABLE CVolumeTextureWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CVolumeTextureWrapper, &CLSID_VolumeTextureWrapper>,
	public IVolumeTextureWrapper,
	public IDirect3DVolumeTexture8
{
public:
	CVolumeTextureWrapper();
	~CVolumeTextureWrapper();

	DECLARE_REGISTRY_RESOURCEID(IDR_VOLUMETEXTUREWRAPPER)

	DECLARE_NOT_AGGREGATABLE(CVolumeTextureWrapper)

	BEGIN_COM_MAP(CVolumeTextureWrapper)
		COM_INTERFACE_ENTRY(IVolumeTextureWrapper)
		COM_INTERFACE_ENTRY_IID(IID_IDirect3DVolumeTexture8, CVolumeTextureWrapper)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

	// Internal Methods
	HRESULT SetDirect3D8Device(CDeviceWrapper* pObject);

private:
	IDirect3DVolumeTexture9* pVolumeTexture9;

	CDeviceWrapper* pDeviceWrapper;

public:
	// IDirect3DVolumeTexture8 Methods
	STDMETHOD(GetDevice)(THIS_ IDirect3DDevice8** ppDevice);
    STDMETHOD(SetPrivateData)(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags);
    STDMETHOD(GetPrivateData)(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData);
    STDMETHOD(FreePrivateData)(THIS_ REFGUID refguid);
    STDMETHOD_(DWORD,  SetPriority)(THIS_ DWORD PriorityNew);
    STDMETHOD_(DWORD,  GetPriority)(THIS);
    STDMETHOD_(void,  PreLoad)(THIS);
    STDMETHOD_(D3DRESOURCETYPE,  GetType)(THIS);
    STDMETHOD_(DWORD,  SetLOD)(THIS_ DWORD LODNew);
    STDMETHOD_(DWORD,  GetLOD)(THIS);
    STDMETHOD_(DWORD,  GetLevelCount)(THIS);
    STDMETHOD(GetLevelDesc)(THIS_ UINT Level, D3D8VOLUME_DESC *pDesc);
    STDMETHOD(GetVolumeLevel)(THIS_ UINT Level, IDirect3DVolume8** ppVolumeLevel);
    STDMETHOD(LockBox)(THIS_ UINT Level, D3DLOCKED_BOX* pLockedVolume, CONST D3DBOX* pBox, DWORD Flags);
    STDMETHOD(UnlockBox)(THIS_ UINT Level);
    STDMETHOD(AddDirtyBox)(THIS_ CONST D3DBOX* pDirtyBox);

	// IWrapper Methods
	STDMETHOD(GetWrappedInterface)(THIS_ void** ppvObject);
	STDMETHOD(SetWrappedInterface)(THIS_ void* pObject);
};

OBJECT_ENTRY_AUTO(__uuidof(VolumeTextureWrapper), CVolumeTextureWrapper)
