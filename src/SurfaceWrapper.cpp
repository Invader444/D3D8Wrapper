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

// SurfaceWrapper.cpp : Implementation of CSurfaceWrapper

#include "stdafx.h"

#include "SurfaceWrapper.h"

// CSurfaceWrapper

CSurfaceWrapper::CSurfaceWrapper()
{
}

CSurfaceWrapper::~CSurfaceWrapper()
{
}

// Internal Methods
HRESULT CSurfaceWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CSurfaceWrapper::FinalConstruct()
{
	return S_OK;
}

void CSurfaceWrapper::FinalRelease()
{
	if (this->pSurface9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedSurface(this->pSurface9);

		this->pSurface9->Release();
		this->pSurface9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CSurfaceWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pSurface9->AddRef();
	*ppvObject = this->pSurface9;

	return S_OK;
}

STDMETHODIMP CSurfaceWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pSurface9 == NULL)
	{
		this->pSurface9 = (IDirect3DSurface9*) pObject;
		result = S_OK;
	}

	return result;
}

// IDirect3DSurface8 Methods
STDMETHODIMP CSurfaceWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CSurfaceWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pSurface9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CSurfaceWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pSurface9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CSurfaceWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pSurface9->FreePrivateData(refguid);
}

STDMETHODIMP CSurfaceWrapper::GetContainer(THIS_ REFIID riid, void** ppContainer)
{
	return this->pSurface9->GetContainer(riid, ppContainer);
}

STDMETHODIMP CSurfaceWrapper::GetDesc(THIS_ D3D8SURFACE_DESC *pDesc)
{
	D3DSURFACE_DESC* pDesc9 = new D3DSURFACE_DESC();

	HRESULT result = this->pSurface9->GetDesc(pDesc9);

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

STDMETHODIMP CSurfaceWrapper::LockRect(THIS_ D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags)
{
	return this->pSurface9->LockRect(pLockedRect, pRect, Flags);
}

STDMETHODIMP CSurfaceWrapper::UnlockRect(THIS)
{
	return this->pSurface9->UnlockRect();
}
