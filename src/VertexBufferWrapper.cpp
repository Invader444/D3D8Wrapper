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

// VertexBufferWrapper.cpp : Implementation of CVertexBufferWrapper

#include "stdafx.h"

#include "VertexBufferWrapper.h"

// CVertexBufferWrapper

CVertexBufferWrapper::CVertexBufferWrapper()
{
}

CVertexBufferWrapper::~CVertexBufferWrapper()
{
}

// Internal Methods
HRESULT CVertexBufferWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CVertexBufferWrapper::FinalConstruct()
{
	return S_OK;
}

void CVertexBufferWrapper::FinalRelease()
{
	if (this->pVertexBuffer9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedVertexBuffer(this->pVertexBuffer9);

		this->pVertexBuffer9->Release();
		this->pVertexBuffer9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CVertexBufferWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pVertexBuffer9->AddRef();
	*ppvObject = this->pVertexBuffer9;

	return S_OK;
}

STDMETHODIMP CVertexBufferWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pVertexBuffer9 == NULL)
	{
		this->pVertexBuffer9 = (IDirect3DVertexBuffer9*) pObject;
		result = S_OK;
	}

	return result;
}

STDMETHODIMP CVertexBufferWrapper::GetDevice(THIS_ IDirect3DDevice8** ppDevice)
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

STDMETHODIMP CVertexBufferWrapper::SetPrivateData(THIS_ REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags)
{
	return this->pVertexBuffer9->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

STDMETHODIMP CVertexBufferWrapper::GetPrivateData(THIS_ REFGUID refguid, void* pData, DWORD* pSizeOfData)
{
	return this->pVertexBuffer9->GetPrivateData(refguid, pData, pSizeOfData);
}

STDMETHODIMP CVertexBufferWrapper::FreePrivateData(THIS_ REFGUID refguid)
{
	return this->pVertexBuffer9->FreePrivateData(refguid);
}

STDMETHODIMP_(DWORD) CVertexBufferWrapper::SetPriority(THIS_ DWORD PriorityNew)
{
	return this->pVertexBuffer9->SetPriority(PriorityNew);
}

STDMETHODIMP_(DWORD) CVertexBufferWrapper::GetPriority(THIS)
{
	return this->pVertexBuffer9->GetPriority();
}

STDMETHODIMP_(void) CVertexBufferWrapper::PreLoad(THIS)
{
	this->pVertexBuffer9->PreLoad();
}

STDMETHODIMP_(D3DRESOURCETYPE) CVertexBufferWrapper::GetType(THIS)
{
	return this->pVertexBuffer9->GetType();
}

STDMETHODIMP CVertexBufferWrapper::Lock(THIS_ UINT OffsetToLock, UINT SizeToLock, BYTE** ppbData, DWORD Flags)
{
	return this->pVertexBuffer9->Lock(OffsetToLock, SizeToLock, (void**) ppbData, Flags);
}

STDMETHODIMP CVertexBufferWrapper::Unlock(THIS)
{
	return this->pVertexBuffer9->Unlock();
}

STDMETHODIMP CVertexBufferWrapper::GetDesc(THIS_ D3DVERTEXBUFFER_DESC *pDesc)
{
	return this->pVertexBuffer9->GetDesc(pDesc);
}
