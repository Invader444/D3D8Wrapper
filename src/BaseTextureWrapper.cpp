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

// BaseTextureWrapper.cpp : Implementation of CBaseTextureWrapper

#include "stdafx.h"

#include "BaseTextureWrapper.h"

// CBaseTextureWrapper

CBaseTextureWrapper::CBaseTextureWrapper()
{
}

CBaseTextureWrapper::~CBaseTextureWrapper()
{
}

// Internal Methods
HRESULT CBaseTextureWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CBaseTextureWrapper::FinalConstruct()
{
	return S_OK;
}

void CBaseTextureWrapper::FinalRelease()
{
	if (this->pBaseTexture9 != NULL)
	{
		this->pBaseTexture9->Release();
		this->pBaseTexture9 = NULL;
	}
}

STDMETHODIMP CBaseTextureWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	*ppvObject = this->pBaseTexture9;
	this->pBaseTexture9->AddRef();

	return S_OK;
}

STDMETHODIMP CBaseTextureWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pBaseTexture9 == NULL)
	{
		this->pBaseTexture9 = (IDirect3DBaseTexture9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CBaseTextureWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CBaseTextureWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pBaseTexture9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CBaseTextureWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pBaseTexture9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CBaseTextureWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pBaseTexture9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CBaseTextureWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pBaseTexture9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CBaseTextureWrapper::GetPriority(THIS)
{
	return this->pBaseTexture9->GetPriority();
}

STDMETHODIMP_(void) CBaseTextureWrapper::PreLoad(THIS)
{
	this->pBaseTexture9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CBaseTextureWrapper::GetType(THIS)
{
	return this->pBaseTexture9->GetType();
}

STDMETHODIMP_(DWORD) CBaseTextureWrapper::SetLOD(THIS_ DWORD LODNew)
{
	return this->pBaseTexture9->SetLOD(LODNew);
}

STDMETHODIMP_(DWORD) CBaseTextureWrapper::GetLOD(THIS)
{
	return this->pBaseTexture9->GetLOD();
}

STDMETHODIMP_(DWORD) CBaseTextureWrapper::GetLevelCount(THIS)
{
	return this->pBaseTexture9->GetLevelCount();
}
