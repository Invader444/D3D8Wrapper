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

// VolumeWrapper.cpp : Implementation of CVolumeWrapper

#include "stdafx.h"

#include "VolumeWrapper.h"

// CVolumeWrapper

CVolumeWrapper::CVolumeWrapper()
{
}

CVolumeWrapper::~CVolumeWrapper()
{
}

// Internal Methods
HRESULT CVolumeWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CVolumeWrapper::SetDirect3D8VolumeTexture(CVolumeTextureWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pVolumeTextureWrapper == NULL)
	{
		this->pVolumeTextureWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CVolumeWrapper::FinalConstruct()
{
	return S_OK;
}

void CVolumeWrapper::FinalRelease()
{
}

// IWrapper Methods
STDMETHODIMP CVolumeWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pVolume9->AddRef();
	*ppvObject = this->pVolume9;

	return S_OK;
}

STDMETHODIMP CVolumeWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pVolume9 == NULL)
	{
		this->pVolume9 = (IDirect3DVolume9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CVolumeWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
{
	HRESULT result = D3DERR_INVALIDCALL;

	if (this->pDeviceWrapper != NULL)
	{
		this->pDeviceWrapper->AddRef();
		*ppDevice = this->pDeviceWrapper;
		result = D3D_OK;
	}

	return result;
}

STDMETHODIMP CVolumeWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pVolume9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CVolumeWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pVolume9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CVolumeWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pVolume9->FreePrivateData(refguid);
}

STDMETHODIMP CVolumeWrapper::GetContainer(THIS_ REFIID riid, void** ppContainer)
{
	HRESULT result = D3DERR_INVALIDCALL;

	if (this->pVolumeTextureWrapper != NULL)
	{
		this->pVolumeTextureWrapper->AddRef();
		*ppContainer = this->pVolumeTextureWrapper;
		result = D3D_OK;
	}

	return result;
}

STDMETHODIMP CVolumeWrapper::GetDesc(THIS_ D3D8VOLUME_DESC *pDesc)
{
	D3DVOLUME_DESC* pDesc9 = new D3DVOLUME_DESC();

	HRESULT result = this->pVolume9->GetDesc(pDesc9);

	pDesc->Depth = pDesc9->Depth;
	pDesc->Format = pDesc9->Format;
	pDesc->Height = pDesc9->Height;
	pDesc->Pool = pDesc9->Pool;

	//TODO: Properly calculate the size of the volume in bytes based on bits per pixel and width/height/depth...
	pDesc->Size = 0;

	pDesc->Type = pDesc9->Type;
	pDesc->Usage = pDesc9->Usage;
	pDesc->Width = pDesc9->Width;

	delete pDesc9;

	return result;
}

STDMETHODIMP CVolumeWrapper::LockBox(THIS_ D3DLOCKED_BOX * pLockedVolume, CONST D3DBOX* pBox, DWORD Flags)
{
	return this->pVolume9->LockBox(pLockedVolume, pBox, Flags);
}

STDMETHODIMP CVolumeWrapper::UnlockBox(THIS)
{
	return this->pVolume9->UnlockBox();
}
