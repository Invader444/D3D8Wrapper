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

// SwapChainWrapper.cpp : Implementation of CSwapChainWrapper

#include "stdafx.h"

#include "SwapChainWrapper.h"

// CSwapChainWrapper

CSwapChainWrapper::CSwapChainWrapper()
{
}

CSwapChainWrapper::~CSwapChainWrapper()
{
}

// Internal Methods
HRESULT CSwapChainWrapper::SetDirect3D8Device(CDeviceWrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDeviceWrapper == NULL)
	{
		this->pDeviceWrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CSwapChainWrapper::FinalConstruct()
{
	return S_OK;
}

void CSwapChainWrapper::FinalRelease()
{
	if (this->pSwapChain9 != NULL)
	{
		this->pDeviceWrapper->RemoveWrappedSwapChain(this->pSwapChain9);

		this->pSwapChain9->Release();
		this->pSwapChain9 = NULL;
	}
}

// IWrapper Methods
STDMETHODIMP CSwapChainWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pSwapChain9->AddRef();
	*ppvObject = this->pSwapChain9;

	return S_OK;
}

STDMETHODIMP CSwapChainWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pSwapChain9 == NULL)
	{
		this->pSwapChain9 = (IDirect3DSwapChain9*) pObject;
		result = S_OK;
	}

	return result;
}

// IDirect3DSwapChain8 Methods
STDMETHODIMP CSwapChainWrapper::Present(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return this->pSwapChain9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, 0);
}

STDMETHODIMP CSwapChainWrapper::GetBackBuffer(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer)
{
	IDirect3DSurface9* pBackBuffer9 = NULL;

	HRESULT result = this->pSwapChain9->GetBackBuffer(BackBuffer, Type, &pBackBuffer9);

	*ppBackBuffer = (IDirect3DSurface8*) this->pDeviceWrapper->GetWrappedSurface(pBackBuffer9);

	return result;
}
