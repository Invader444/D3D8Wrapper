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

// VolumeTextureWrapper.cpp : Implementation of CVolumeTextureWrapper

#include "stdafx.h"

#include "VolumeTextureWrapper.h"

// CVolumeTextureWrapper

CVolumeTextureWrapper::CVolumeTextureWrapper()
{
}

CVolumeTextureWrapper::~CVolumeTextureWrapper()
{
}

// Internal Methods
HRESULT CVolumeTextureWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CVolumeTextureWrapper::FinalConstruct()
{
	return S_OK;
}

void CVolumeTextureWrapper::FinalRelease()
{
	if (this->pVolumeTexture9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedVolumeTexture(this->pVolumeTexture9);

		this->pVolumeTexture9->Release();
		this->pVolumeTexture9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CVolumeTextureWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pVolumeTexture9->AddRef();
	*ppvObject = this->pVolumeTexture9;

	return S_OK;
}

STDMETHODIMP CVolumeTextureWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pVolumeTexture9 == NULL)
	{
		this->pVolumeTexture9 = (IDirect3DVolumeTexture9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CVolumeTextureWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CVolumeTextureWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pVolumeTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CVolumeTextureWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pVolumeTexture9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CVolumeTextureWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pVolumeTexture9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CVolumeTextureWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pVolumeTexture9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CVolumeTextureWrapper::GetPriority(THIS)
{
	return this->pVolumeTexture9->GetPriority();
}

STDMETHODIMP_(void) CVolumeTextureWrapper::PreLoad(THIS)
{
	this->pVolumeTexture9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CVolumeTextureWrapper::GetType(THIS)
{
	return this->pVolumeTexture9->GetType();
}

STDMETHODIMP_(DWORD) CVolumeTextureWrapper::SetLOD(THIS_ DWORD LODNew)
{
	return this->pVolumeTexture9->SetLOD(LODNew);
}

STDMETHODIMP_(DWORD) CVolumeTextureWrapper::GetLOD(THIS)
{
	return this->pVolumeTexture9->GetLOD();
}

STDMETHODIMP_(DWORD) CVolumeTextureWrapper::GetLevelCount(THIS)
{
	return this->pVolumeTexture9->GetLevelCount();
}

STDMETHODIMP CVolumeTextureWrapper::GetLevelDesc(THIS_ UINT Level, D3D8VOLUME_DESC *pDesc)
{
	D3DVOLUME_DESC* pDesc9 = new D3DVOLUME_DESC();

	HRESULT result = this->pVolumeTexture9->GetLevelDesc(Level, pDesc9);

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

STDMETHODIMP CVolumeTextureWrapper::GetVolumeLevel(THIS_ UINT Level, IDirect3DVolume8** ppVolumeLevel)
{
	IDirect3DVolume9* pVolumeLevel9 = NULL;

	HRESULT result = this->pVolumeTexture9->GetVolumeLevel(Level, &pVolumeLevel9);
	
	CVolumeWrapper* volumeWrapper = this->pDeviceWrapper->GetWrappedVolume(pVolumeLevel9);
	volumeWrapper->SetDirect3D8VolumeTexture(this);

	*ppVolumeLevel = (IDirect3DVolume8*) volumeWrapper;

	return result;
}

STDMETHODIMP CVolumeTextureWrapper::LockBox(THIS_ UINT Level, D3DLOCKED_BOX* pLockedVolume, CONST D3DBOX* pBox, DWORD Flags)
{
	return this->pVolumeTexture9->LockBox(Level, pLockedVolume, pBox, Flags);
}

STDMETHODIMP CVolumeTextureWrapper::UnlockBox(THIS_ UINT Level)
{
	return this->pVolumeTexture9->UnlockBox(Level);
}

STDMETHODIMP CVolumeTextureWrapper::AddDirtyBox(THIS_ CONST D3DBOX* pDirtyBox)
{
	return this->pVolumeTexture9->AddDirtyBox(pDirtyBox);
}
