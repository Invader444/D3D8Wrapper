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

// TextureWrapper.cpp : Implementation of CTextureWrapper

#include "stdafx.h"

#include "TextureWrapper.h"

// CTextureWrapper

CTextureWrapper::CTextureWrapper()
{
}

CTextureWrapper::~CTextureWrapper()
{
}

// Internal Methods
HRESULT CTextureWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CTextureWrapper::FinalConstruct()
{
	return S_OK;
}

void CTextureWrapper::FinalRelease()
{
	if (this->pTexture9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedTexture(this->pTexture9);

		this->pTexture9->Release();
		this->pTexture9 = NULL;
	}
}

STDMETHODIMP CTextureWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pTexture9->AddRef();
	*ppvObject = this->pTexture9;

	return S_OK;
}

STDMETHODIMP CTextureWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pTexture9 == NULL)
	{
		this->pTexture9 = (IDirect3DTexture9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CTextureWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CTextureWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CTextureWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pTexture9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CTextureWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pTexture9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CTextureWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pTexture9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CTextureWrapper::GetPriority(THIS)
{
	return this->pTexture9->GetPriority();
}

STDMETHODIMP_(void) CTextureWrapper::PreLoad(THIS)
{
	this->pTexture9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CTextureWrapper::GetType(THIS)
{
	return this->pTexture9->GetType();
}

STDMETHODIMP_(DWORD) CTextureWrapper::SetLOD(THIS_ DWORD LODNew)
{
	return this->pTexture9->SetLOD(LODNew);
}

STDMETHODIMP_(DWORD) CTextureWrapper::GetLOD(THIS)
{
	return this->pTexture9->GetLOD();
}

STDMETHODIMP_(DWORD) CTextureWrapper::GetLevelCount(THIS)
{
	return this->pTexture9->GetLevelCount();
}

STDMETHODIMP CTextureWrapper::GetLevelDesc(THIS_ UINT Level, D3D8SURFACE_DESC *pDesc)
{
	D3DSURFACE_DESC* pDesc9 = new D3DSURFACE_DESC();

	HRESULT result = this->pTexture9->GetLevelDesc(Level, pDesc9);

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

STDMETHODIMP CTextureWrapper::GetSurfaceLevel(THIS_ UINT Level, IDirect3DSurface8** ppSurfaceLevel)
{
	IDirect3DSurface9* pSurfaceLevel9 = NULL;

	HRESULT result = this->pTexture9->GetSurfaceLevel(Level, &pSurfaceLevel9);

	*ppSurfaceLevel = (IDirect3DSurface8*) this->pDeviceWrapper->GetWrappedSurface(pSurfaceLevel9);

	return result;
}

STDMETHODIMP CTextureWrapper::LockRect(THIS_ UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return this->pTexture9->LockRect(Level, pLockedRect, pRect, Flags);
}

STDMETHODIMP CTextureWrapper::UnlockRect(THIS_ UINT Level)
{
	return this->pTexture9->UnlockRect(Level);
}

STDMETHODIMP CTextureWrapper::AddDirtyRect(THIS_ CONST RECT* pDirtyRect)
{
	return this->pTexture9->AddDirtyRect(pDirtyRect);
}
