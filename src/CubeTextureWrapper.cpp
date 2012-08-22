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

// CubeTextureWrapper.cpp : Implementation of CCubeTextureWrapper

#include "stdafx.h"

#include "CubeTextureWrapper.h"

// CCubeTextureWrapper

CCubeTextureWrapper::CCubeTextureWrapper()
{
}

CCubeTextureWrapper::~CCubeTextureWrapper()
{
}

// Internal Methods
HRESULT CCubeTextureWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CCubeTextureWrapper::FinalConstruct()
{
	return S_OK;
}

void CCubeTextureWrapper::FinalRelease()
{
	if (this->pCubeTexture9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedCubeTexture(this->pCubeTexture9);

		this->pCubeTexture9->Release();
		this->pCubeTexture9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CCubeTextureWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pCubeTexture9->AddRef();
	*ppvObject = this->pCubeTexture9;

	return S_OK;
}

STDMETHODIMP CCubeTextureWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pCubeTexture9 == NULL)
	{
		this->pCubeTexture9 = (IDirect3DCubeTexture9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CCubeTextureWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CCubeTextureWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pCubeTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CCubeTextureWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pCubeTexture9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CCubeTextureWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pCubeTexture9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CCubeTextureWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pCubeTexture9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CCubeTextureWrapper::GetPriority(THIS)
{
	return this->pCubeTexture9->GetPriority();
}

STDMETHODIMP_(void) CCubeTextureWrapper::PreLoad(THIS)
{
	this->pCubeTexture9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CCubeTextureWrapper::GetType(THIS)
{
	return this->pCubeTexture9->GetType();
}

STDMETHODIMP_(DWORD) CCubeTextureWrapper::SetLOD(THIS_ DWORD LODNew)
{
	return this->pCubeTexture9->SetLOD(LODNew);
}

STDMETHODIMP_(DWORD) CCubeTextureWrapper::GetLOD(THIS)
{
	return this->pCubeTexture9->GetLOD();
}

STDMETHODIMP_(DWORD) CCubeTextureWrapper::GetLevelCount(THIS)
{
	return this->pCubeTexture9->GetLevelCount();
}

STDMETHODIMP CCubeTextureWrapper::GetLevelDesc(THIS_ UINT Level, D3D8SURFACE_DESC *pDesc)
{
	D3DSURFACE_DESC* pDesc9 = new D3DSURFACE_DESC();

	HRESULT result = this->pCubeTexture9->GetLevelDesc(Level, pDesc9);

	pDesc->Format = pDesc9->Format;
	pDesc->Height = pDesc9->Height;
	pDesc->MultiSampleType = pDesc9->MultiSampleType;
	pDesc->Pool = pDesc9->Pool;

	//TODO: Properly calculate the size of the texture in bytes based on bits per pixel and width/height...
	pDesc->Size = 0;

	pDesc->Type = pDesc9->Type;
	pDesc->Usage = pDesc9->Usage;
	pDesc->Width = pDesc9->Width;

	delete pDesc9;

	return result;
}

STDMETHODIMP CCubeTextureWrapper::GetCubeMapSurface(THIS_ D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface8** ppCubeMapSurface)
{
	IDirect3DSurface9* pCubeMapSurface9 = NULL;

	HRESULT result = this->pCubeTexture9->GetCubeMapSurface(FaceType, Level, &pCubeMapSurface9);

	*ppCubeMapSurface = (IDirect3DSurface8*) this->pDeviceWrapper->GetWrappedSurface(pCubeMapSurface9);

	return result;
}

STDMETHODIMP CCubeTextureWrapper::LockRect(THIS_ D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return this->pCubeTexture9->LockRect(FaceType, Level, pLockedRect, pRect, Flags);
}

STDMETHODIMP CCubeTextureWrapper::UnlockRect(THIS_ D3DCUBEMAP_FACES FaceType, UINT Level)
{
	return this->pCubeTexture9->UnlockRect(FaceType, Level);
}

STDMETHODIMP CCubeTextureWrapper::AddDirtyRect(THIS_ D3DCUBEMAP_FACES FaceType, CONST RECT* pDirtyRect)
{
	return this->pCubeTexture9->AddDirtyRect(FaceType, pDirtyRect);
}
