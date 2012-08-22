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

// IndexBufferWrapper.cpp : Implementation of CIndexBufferWrapper

#include "stdafx.h"

#include "IndexBufferWrapper.h"

// CIndexBufferWrapper

CIndexBufferWrapper::CIndexBufferWrapper()
{
}

CIndexBufferWrapper::~CIndexBufferWrapper()
{
}

// Internal Methods
HRESULT CIndexBufferWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CIndexBufferWrapper::FinalConstruct()
{
	return S_OK;
}

void CIndexBufferWrapper::FinalRelease()
{
	if (this->pIndexBuffer9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedIndexBuffer(this->pIndexBuffer9);

		this->pIndexBuffer9->Release();
		this->pIndexBuffer9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CIndexBufferWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pIndexBuffer9->AddRef();
	*ppvObject = this->pIndexBuffer9;

	return S_OK;
}

STDMETHODIMP CIndexBufferWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pIndexBuffer9 == NULL)
	{
		this->pIndexBuffer9 = (IDirect3DIndexBuffer9*) pObject;
		result = S_OK;
	}

	return result;
}

// IDirect3DIndexBuffer8 Methods
STDMETHODIMP CIndexBufferWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CIndexBufferWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pIndexBuffer9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CIndexBufferWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pIndexBuffer9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CIndexBufferWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pIndexBuffer9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CIndexBufferWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pIndexBuffer9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CIndexBufferWrapper::GetPriority(THIS)
{
	return this->pIndexBuffer9->GetPriority();
}

STDMETHODIMP_(void) CIndexBufferWrapper::PreLoad(THIS)
{
	this->pIndexBuffer9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CIndexBufferWrapper::GetType(THIS)
{
	return this->pIndexBuffer9->GetType();
}

STDMETHODIMP CIndexBufferWrapper::Lock(THIS_ UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags)
{
	return this->pIndexBuffer9->Lock(OffsetToLock, SizeToLock, (void**) ppbData, Flags);
}

STDMETHODIMP CIndexBufferWrapper::Unlock(THIS)
{
	return this->pIndexBuffer9->Unlock();
}

STDMETHODIMP CIndexBufferWrapper::GetDesc(THIS_ D3DINDEXBUFFER_DESC *pDesc)
{
	return this->pIndexBuffer9->GetDesc(pDesc);
}
