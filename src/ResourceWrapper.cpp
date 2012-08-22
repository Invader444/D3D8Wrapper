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

// ResourceWrapper.cpp : Implementation of CResourceWrapper

#include "stdafx.h"

#include "ResourceWrapper.h"

// CResourceWrapper

CResourceWrapper::CResourceWrapper()
{
}

CResourceWrapper::~CResourceWrapper()
{
}

// Internal Methods
HRESULT CResourceWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CResourceWrapper::FinalConstruct()
{
	return S_OK;
}

void CResourceWrapper::FinalRelease()
{
	if (this->pResource9 != NULL)
	{
		this->pResource9->Release();
		this->pResource9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CResourceWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pResource9->AddRef();
	*ppvObject = this->pResource9;

	return S_OK;
}

STDMETHODIMP CResourceWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pResource9 == NULL)
	{
		this->pResource9 = (IDirect3DResource9*) pObject;
		result = S_OK;
	}

	return result;
}

// IDirect3DResource8 Methods
STDMETHODIMP CResourceWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CResourceWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pResource9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CResourceWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pResource9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CResourceWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pResource9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CResourceWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pResource9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CResourceWrapper::GetPriority(THIS)
{
	return this->pResource9->GetPriority();
}

STDMETHODIMP_(void) CResourceWrapper::PreLoad(THIS)
{
	this->pResource9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CResourceWrapper::GetType(THIS)
{
	return this->pResource9->GetType();
}
