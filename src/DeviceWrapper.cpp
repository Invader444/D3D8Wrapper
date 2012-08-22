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

// DeviceWrapper.cpp : Implementation of CDeviceWrapper

#include "stdafx.h"

#include "DeviceWrapper.h"

// CDeviceWrapper

CDeviceWrapper::CDeviceWrapper()
{
}

CDeviceWrapper::~CDeviceWrapper()
{
}

// Internal Methods
HRESULT CDeviceWrapper::SetDirect3D8(CDirect3D8Wrapper* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDirect3D8Wrapper == NULL)
	{
		this->pDirect3D8Wrapper = pObject;
		result = S_OK;
	}

	return result;
}

HRESULT CDeviceWrapper::SetZBufferDiscarding(bool IsEnabled)
{
	if (IsEnabled)
	{
		this->zBufferDiscardingEnabled = TRUE;
	}
	else
	{
		this->zBufferDiscardingEnabled = FALSE;
	}

	return S_OK;
}

CSwapChainWrapper* CDeviceWrapper::GetWrappedSwapChain(IDirect3DSwapChain9* pSwapChain9)
{
	CSwapChainWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pSwapChain9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdSwapChainsMutex);
		std::map<IUnknown*, CSwapChainWrapper*>::iterator storedSwapChain = this->createdSwapChains.find(pUnknown);
		if (storedSwapChain == this->createdSwapChains.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pSwapChainWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pSwapChainWrapperFactory->CreateInstance(NULL, IID_ISwapChainWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pSwapChain9);
					result->SetDirect3D8Device(this);

					this->createdSwapChains.insert(std::pair<IUnknown*, CSwapChainWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedSwapChain->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pSwapChain9->Release();
		}
		LeaveCriticalSection(&this->createdSwapChainsMutex);

		pUnknown->Release();
	}

	return result;
}

CTextureWrapper* CDeviceWrapper::GetWrappedTexture(IDirect3DTexture9* pTexture9)
{
	CTextureWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdTexturesMutex);
		std::map<IUnknown*, CTextureWrapper*>::iterator storedTexture = this->createdTextures.find(pUnknown);
		if (storedTexture == this->createdTextures.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pTextureWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pTextureWrapperFactory->CreateInstance(NULL, IID_ITextureWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pTexture9);
					result->SetDirect3D8Device(this);

					this->createdTextures.insert(std::pair<IUnknown*, CTextureWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedTexture->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pTexture9->Release();
		}
		LeaveCriticalSection(&this->createdTexturesMutex);

		pUnknown->Release();
	}

	return result;
}

CVolumeTextureWrapper* CDeviceWrapper::GetWrappedVolumeTexture(IDirect3DVolumeTexture9* pVolumeTexture9)
{
	CVolumeTextureWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pVolumeTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVolumeTexturesMutex);
		std::map<IUnknown*, CVolumeTextureWrapper*>::iterator storedVolumeTexture = this->createdVolumeTextures.find(pUnknown);
		if (storedVolumeTexture == this->createdVolumeTextures.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pVolumeTextureWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pVolumeTextureWrapperFactory->CreateInstance(NULL, IID_IVolumeTextureWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pVolumeTexture9);
					result->SetDirect3D8Device(this);

					this->createdVolumeTextures.insert(std::pair<IUnknown*, CVolumeTextureWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedVolumeTexture->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pVolumeTexture9->Release();
		}
		LeaveCriticalSection(&this->createdVolumeTexturesMutex);

		pUnknown->Release();
	}

	return result;
}

CCubeTextureWrapper* CDeviceWrapper::GetWrappedCubeTexture(IDirect3DCubeTexture9* pCubeTexture9)
{
	CCubeTextureWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pCubeTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdCubeTexturesMutex);
		std::map<IUnknown*, CCubeTextureWrapper*>::iterator storedCubeTexture = this->createdCubeTextures.find(pUnknown);
		if (storedCubeTexture == this->createdCubeTextures.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pCubeTextureWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pCubeTextureWrapperFactory->CreateInstance(NULL, IID_ICubeTextureWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pCubeTexture9);
					result->SetDirect3D8Device(this);

					this->createdCubeTextures.insert(std::pair<IUnknown*, CCubeTextureWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedCubeTexture->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pCubeTexture9->Release();
		}
		LeaveCriticalSection(&this->createdCubeTexturesMutex);

		pUnknown->Release();
	}

	return result;
}

CVertexBufferWrapper* CDeviceWrapper::GetWrappedVertexBuffer(IDirect3DVertexBuffer9* pVertexBuffer9)
{
	CVertexBufferWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pVertexBuffer9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVertexBuffersMutex);
		std::map<IUnknown*, CVertexBufferWrapper*>::iterator storedVertexBuffer = this->createdVertexBuffers.find(pUnknown);
		if (storedVertexBuffer == this->createdVertexBuffers.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pVertexBufferWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pVertexBufferWrapperFactory->CreateInstance(NULL, IID_IVertexBufferWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pVertexBuffer9);
					result->SetDirect3D8Device(this);

					this->createdVertexBuffers.insert(std::pair<IUnknown*, CVertexBufferWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedVertexBuffer->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pVertexBuffer9->Release();
		}
		LeaveCriticalSection(&this->createdVertexBuffersMutex);

		pUnknown->Release();
	}

	return result;
}

CIndexBufferWrapper* CDeviceWrapper::GetWrappedIndexBuffer(IDirect3DIndexBuffer9* pIndexBuffer9)
{
	CIndexBufferWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pIndexBuffer9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdIndexBuffersMutex);
		std::map<IUnknown*, CIndexBufferWrapper*>::iterator storedIndexBuffer = this->createdIndexBuffers.find(pUnknown);
		if (storedIndexBuffer == this->createdIndexBuffers.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pIndexBufferWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pIndexBufferWrapperFactory->CreateInstance(NULL, IID_IIndexBufferWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pIndexBuffer9);
					result->SetDirect3D8Device(this);

					this->createdIndexBuffers.insert(std::pair<IUnknown*, CIndexBufferWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedIndexBuffer->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pIndexBuffer9->Release();
		}
		LeaveCriticalSection(&this->createdIndexBuffersMutex);

		pUnknown->Release();
	}

	return result;
}

CSurfaceWrapper* CDeviceWrapper::GetWrappedSurface(IDirect3DSurface9* pSurface9)
{
	CSurfaceWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pSurface9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdSurfacesMutex);
		std::map<IUnknown*, CSurfaceWrapper*>::iterator storedSurface = this->createdSurfaces.find(pUnknown);
		if (storedSurface == this->createdSurfaces.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pSurfaceWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pSurfaceWrapperFactory->CreateInstance(NULL, IID_ISurfaceWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pSurface9);
					result->SetDirect3D8Device(this);

					this->createdSurfaces.insert(std::pair<IUnknown*, CSurfaceWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedSurface->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pSurface9->Release();
		}
		LeaveCriticalSection(&this->createdSurfacesMutex);

		pUnknown->Release();
	}

	return result;
}

CVolumeWrapper* CDeviceWrapper::GetWrappedVolume(IDirect3DVolume9* pVolume9)
{
	CVolumeWrapper* result = NULL;

	IUnknown* pUnknown = NULL;
	if (SUCCEEDED(pVolume9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVolumesMutex);
		std::map<IUnknown*, CVolumeWrapper*>::iterator storedVolume = this->createdVolumes.find(pUnknown);
		if (storedVolume == this->createdVolumes.end())
		{
			// Does not exist, so create a wrapper and store it
			if (this->pVolumeWrapperFactory != NULL)
			{
				if (SUCCEEDED(this->pVolumeWrapperFactory->CreateInstance(NULL, IID_IVolumeWrapper, (void**) &result)))
				{
					result->SetWrappedInterface((void**) &pVolume9);
					result->SetDirect3D8Device(this);

					this->createdVolumes.insert(std::pair<IUnknown*, CVolumeWrapper*>(pUnknown, result));
				}
			}
		}
		else
		{
			// Exists, so re-use
			result = storedVolume->second;
			result->AddRef();

			// ... and release the D3D9 interface since the re-used wrapper already owns a reference
			pVolume9->Release();
		}
		LeaveCriticalSection(&this->createdVolumesMutex);

		pUnknown->Release();
	}

	return result;
}

void CDeviceWrapper::RemoveWrappedSwapChain(IDirect3DSwapChain9* pSwapChain9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pSwapChain9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdSwapChainsMutex);

		this->createdSwapChains.erase(pUnknown);

		LeaveCriticalSection(&this->createdSwapChainsMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedTexture(IDirect3DTexture9* pTexture9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdTexturesMutex);

		this->createdTextures.erase(pUnknown);

		LeaveCriticalSection(&this->createdTexturesMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedVolumeTexture(IDirect3DVolumeTexture9* pVolumeTexture9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pVolumeTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVolumeTexturesMutex);

		this->createdVolumeTextures.erase(pUnknown);

		LeaveCriticalSection(&this->createdVolumeTexturesMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedCubeTexture(IDirect3DCubeTexture9* pCubeTexture9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pCubeTexture9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdCubeTexturesMutex);

		this->createdCubeTextures.erase(pUnknown);

		LeaveCriticalSection(&this->createdCubeTexturesMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedVertexBuffer(IDirect3DVertexBuffer9* pVertexBuffer9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pVertexBuffer9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVertexBuffersMutex);

		this->createdVertexBuffers.erase(pUnknown);

		LeaveCriticalSection(&this->createdVertexBuffersMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedIndexBuffer(IDirect3DIndexBuffer9* pIndexBuffer9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pIndexBuffer9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdIndexBuffersMutex);

		this->createdIndexBuffers.erase(pUnknown);

		LeaveCriticalSection(&this->createdIndexBuffersMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedSurface(IDirect3DSurface9* pSurface9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pSurface9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdSurfacesMutex);

		this->createdSurfaces.erase(pUnknown);

		LeaveCriticalSection(&this->createdSurfacesMutex);

		pUnknown->Release();
	}
}

void CDeviceWrapper::RemoveWrappedVolume(IDirect3DVolume9* pVolume9)
{
	IUnknown* pUnknown = NULL;

	if (SUCCEEDED(pVolume9->QueryInterface(IID_IUnknown, (void**) &pUnknown)))
	{
		EnterCriticalSection(&this->createdVolumesMutex);

		this->createdVolumes.erase(pUnknown);

		LeaveCriticalSection(&this->createdVolumesMutex);

		pUnknown->Release();
	}
}

HRESULT CDeviceWrapper::FinalConstruct()
{
	HRESULT hr = CoGetClassObject(IID_ISwapChainWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pSwapChainWrapperFactory);
	if (FAILED(hr))
	{
		this->pSwapChainWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_ITextureWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pTextureWrapperFactory);
	if (FAILED(hr))
	{
		this->pTextureWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_IVolumeTextureWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pVolumeTextureWrapperFactory);
	if (FAILED(hr))
	{
		this->pVolumeTextureWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_ICubeTextureWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pCubeTextureWrapperFactory);
	if (FAILED(hr))
	{
		this->pCubeTextureWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_IVertexBufferWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pVertexBufferWrapperFactory);
	if (FAILED(hr))
	{
		this->pVertexBufferWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_IIndexBufferWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pIndexBufferWrapperFactory);
	if (FAILED(hr))
	{
		this->pIndexBufferWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_ISurfaceWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pSurfaceWrapperFactory);
	if (FAILED(hr))
	{
		this->pSurfaceWrapperFactory = NULL;
	}

	hr = CoGetClassObject(IID_IVolumeWrapper, CLSCTX_INPROC, NULL, IID_IClassFactory, (LPVOID*) &this->pVolumeWrapperFactory);
	if (FAILED(hr))
	{
		this->pVolumeWrapperFactory = NULL;
	}

	InitializeCriticalSection(&this->createdSwapChainsMutex);
	InitializeCriticalSection(&this->createdTexturesMutex);
	InitializeCriticalSection(&this->createdVolumeTexturesMutex);
	InitializeCriticalSection(&this->createdCubeTexturesMutex);
	InitializeCriticalSection(&this->createdVertexBuffersMutex);
	InitializeCriticalSection(&this->createdIndexBuffersMutex);
	InitializeCriticalSection(&this->createdSurfacesMutex);
	InitializeCriticalSection(&this->createdVolumesMutex);

	return S_OK;
}

void CDeviceWrapper::FinalRelease()
{
	if (this->pSwapChainWrapperFactory != NULL)
	{
		this->pSwapChainWrapperFactory->Release();
		this->pSwapChainWrapperFactory = NULL;
	}

	if (this->pTextureWrapperFactory != NULL)
	{
		this->pTextureWrapperFactory->Release();
		this->pTextureWrapperFactory = NULL;
	}

	if (this->pVolumeTextureWrapperFactory != NULL)
	{
		this->pVolumeTextureWrapperFactory->Release();
		this->pVolumeTextureWrapperFactory = NULL;
	}

	if (this->pCubeTextureWrapperFactory != NULL)
	{
		this->pCubeTextureWrapperFactory->Release();
		this->pCubeTextureWrapperFactory = NULL;
	}

	if (this->pVertexBufferWrapperFactory != NULL)
	{
		this->pVertexBufferWrapperFactory->Release();
		this->pVertexBufferWrapperFactory = NULL;
	}

	if (this->pIndexBufferWrapperFactory != NULL)
	{
		this->pIndexBufferWrapperFactory->Release();
		this->pIndexBufferWrapperFactory = NULL;
	}

	if (this->pSurfaceWrapperFactory != NULL)
	{
		this->pSurfaceWrapperFactory->Release();
		this->pSurfaceWrapperFactory = NULL;
	}

	if (this->pVolumeWrapperFactory != NULL)
	{
		this->pVolumeWrapperFactory->Release();
		this->pVolumeWrapperFactory = NULL;
	}

	DeleteCriticalSection(&this->createdSwapChainsMutex);
	DeleteCriticalSection(&this->createdTexturesMutex);
	DeleteCriticalSection(&this->createdVolumeTexturesMutex);
	DeleteCriticalSection(&this->createdCubeTexturesMutex);
	DeleteCriticalSection(&this->createdVertexBuffersMutex);
	DeleteCriticalSection(&this->createdIndexBuffersMutex);
	DeleteCriticalSection(&this->createdSurfacesMutex);
	DeleteCriticalSection(&this->createdVolumesMutex);
}

// IDeviceWrapper Methods
STDMETHODIMP CDeviceWrapper::SetCreateAdditionalSwapChainCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateAdditionalSwapChainCallbackThis = pThis;
	this->pCreateAdditionalSwapChainCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateTextureCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateTextureCallbackThis = pThis;
	this->pCreateTextureCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateVolumeTextureCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateVolumeTextureCallbackThis = pThis;
	this->pCreateVolumeTextureCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateCubeTextureCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateCubeTextureCallbackThis = pThis;
	this->pCreateCubeTextureCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateVertexBufferCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateVertexBufferCallbackThis = pThis;
	this->pCreateVertexBufferCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateIndexBufferCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateIndexBufferCallbackThis = pThis;
	this->pCreateIndexBufferCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateRenderTargetCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateRenderTargetCallbackThis = pThis;
	this->pCreateRenderTargetCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateDepthStencilSurfaceCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateDepthStencilSurfaceCallbackThis = pThis;
	this->pCreateDepthStencilSurfaceCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetCreateImageSurfaceCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pCreateImageSurfaceCallbackThis = pThis;
	this->pCreateImageSurfaceCallbackFunction = pCallbackFunction;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetGetBackBufferCallback(THIS_ void* pThis, void* pCallbackFunction)
{
	this->pGetBackBufferCallbackThis = pThis;
	this->pGetBackBufferCallbackFunction = pCallbackFunction;

	return S_OK;
}

// IDirect3DDevice8 Methods
STDMETHODIMP CDeviceWrapper::GetWrappedInterface(THIS_ void** ppvObject)
{
	this->pDevice9->AddRef();
	*ppvObject = this->pDevice9;

	return S_OK;
}

STDMETHODIMP CDeviceWrapper::SetWrappedInterface(THIS_ void* pObject)
{
	HRESULT result = S_FALSE;

	if (this->pDevice9 == NULL)
	{
		this->pDevice9 = (IDirect3DDevice9*) pObject;

		// Get some bookkeeping out of the way for later function calls...
		if (this->pDevice9 != NULL)
		{
			this->pDevice9->GetCreationParameters(&this->deviceCreationParameters);
		}

		result = S_OK;
	}

	return result;
}

// IDirect3DDevice8 Methods
STDMETHODIMP CDeviceWrapper::TestCooperativeLevel(THIS)
{
	return this->pDevice9->TestCooperativeLevel();
}

STDMETHODIMP_(UINT) CDeviceWrapper::GetAvailableTextureMem(THIS)
{
	return this->pDevice9->GetAvailableTextureMem();
}

STDMETHODIMP CDeviceWrapper::ResourceManagerDiscardBytes(THIS_ DWORD Bytes)
{
	return D3D_OK;
}

STDMETHODIMP CDeviceWrapper::GetDirect3D(THIS_ IDirect3D8** ppD3D8)
{
	HRESULT result = D3DERR_INVALIDCALL;

	if (this->pDirect3D8Wrapper != NULL)
	{
		this->pDirect3D8Wrapper->AddRef();
		*ppD3D8 = this->pDirect3D8Wrapper;
		result = D3D_OK;
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::GetDeviceCaps(THIS_ D3DCAPS8* pCaps)
{
	D3DCAPS9* pCaps9 = new D3DCAPS9();
	HRESULT result = this->pDevice9->GetDeviceCaps(pCaps9);

	memcpy(pCaps, pCaps9, sizeof(D3DCAPS8));

	delete pCaps9;

	return result;
}

STDMETHODIMP CDeviceWrapper::GetDisplayMode(THIS_ D3DDISPLAYMODE* pMode)
{
	HRESULT result = this->pDevice9->GetDisplayMode(this->deviceCreationParameters.AdapterOrdinal, pMode);

	return result;
}

STDMETHODIMP CDeviceWrapper::GetCreationParameters(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return this->pDevice9->GetCreationParameters(pParameters);
}

STDMETHODIMP CDeviceWrapper::SetCursorProperties(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DSurface9* pCursorBitmap9 = NULL;
	if (SUCCEEDED(((ISurfaceWrapper*) pCursorBitmap)->GetWrappedInterface((void**) &pCursorBitmap9)))
	{
		result = this->pDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap9);
		pCursorBitmap9->Release();
	}

	return result;
}

STDMETHODIMP_(void) CDeviceWrapper::SetCursorPosition(THIS_ int X, int Y, DWORD Flags)
{
	this->pDevice9->SetCursorPosition(X, Y, Flags);
}

STDMETHODIMP_(BOOL) CDeviceWrapper::ShowCursor(THIS_ BOOL bShow)
{
	return this->pDevice9->ShowCursor(bShow);
}

STDMETHODIMP CDeviceWrapper::CreateAdditionalSwapChain(THIS_ D3D8PRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** pSwapChain)
{
	HRESULT result = D3DERR_NOTAVAILABLE;

	IDirect3D9* pDirect3D9 = NULL;
	if (SUCCEEDED(this->pDevice9->GetDirect3D(&pDirect3D9)))
	{
		IDirect3DSwapChain9* pSwapChain9 = NULL;

		D3DPRESENT_PARAMETERS* pPresentationParameters9 = new D3DPRESENT_PARAMETERS();
		pPresentationParameters9->AutoDepthStencilFormat = pPresentationParameters->AutoDepthStencilFormat;
		pPresentationParameters9->BackBufferCount = pPresentationParameters->BackBufferCount;
		pPresentationParameters9->BackBufferFormat = pPresentationParameters->BackBufferFormat;
		pPresentationParameters9->BackBufferHeight = pPresentationParameters->BackBufferHeight;
		pPresentationParameters9->BackBufferWidth = pPresentationParameters->BackBufferWidth;
		pPresentationParameters9->EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
		pPresentationParameters9->Flags = pPresentationParameters->Flags;
		pPresentationParameters9->FullScreen_RefreshRateInHz = pPresentationParameters->FullScreen_RefreshRateInHz;
		pPresentationParameters9->hDeviceWindow = pPresentationParameters->hDeviceWindow;

		// Will always select best quality...
		pDirect3D9->CheckDeviceMultiSampleType(this->deviceCreationParameters.AdapterOrdinal, this->deviceCreationParameters.DeviceType, pPresentationParameters->BackBufferFormat, pPresentationParameters->Windowed, pPresentationParameters->MultiSampleType, &pPresentationParameters9->MultiSampleQuality);

		pPresentationParameters9->MultiSampleType = pPresentationParameters->MultiSampleType;
		pPresentationParameters9->PresentationInterval = pPresentationParameters->FullScreen_PresentationInterval;
		pPresentationParameters9->SwapEffect = pPresentationParameters->SwapEffect;
		pPresentationParameters9->Windowed = pPresentationParameters->Windowed;

		result = this->pDevice9->CreateAdditionalSwapChain(pPresentationParameters9, &pSwapChain9);

		// Check if we already have a pointer to this swap chain... and if we do, re-use it
		CSwapChainWrapper* swapChainWrapper = this->GetWrappedSwapChain(pSwapChain9);

		if (this->pCreateAdditionalSwapChainCallbackFunction != NULL)
		{
			// Callback function gets an owning reference...
			swapChainWrapper->AddRef();
			if (this->pCreateAdditionalSwapChainCallbackThis == NULL)
			{
				// C-style function was set
				((CCreateAdditionalSwapChainCallback_t) this->pCreateAdditionalSwapChainCallbackFunction)((ISwapChainWrapper*) swapChainWrapper);
			}
			else
			{
				// C++ class member function was set
				((CreateAdditionalSwapChainCallback_t) this->pCreateAdditionalSwapChainCallbackFunction)(this->pCreateAdditionalSwapChainCallbackThis, (ISwapChainWrapper*) swapChainWrapper);
			}
		}

		*pSwapChain = swapChainWrapper;

		pDirect3D9->Release();

		delete pPresentationParameters9;
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::Reset(THIS_ D3D8PRESENT_PARAMETERS* pPresentationParameters)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3D9* pDirect3D9 = NULL;
	if (SUCCEEDED(this->pDevice9->GetDirect3D(&pDirect3D9)))
	{
		D3DPRESENT_PARAMETERS* pPresentationParameters9 = new D3DPRESENT_PARAMETERS();
		pPresentationParameters9->AutoDepthStencilFormat = pPresentationParameters->AutoDepthStencilFormat;
		pPresentationParameters9->BackBufferCount = pPresentationParameters->BackBufferCount;
		pPresentationParameters9->BackBufferFormat = pPresentationParameters->BackBufferFormat;
		pPresentationParameters9->BackBufferHeight = pPresentationParameters->BackBufferHeight;
		pPresentationParameters9->BackBufferWidth = pPresentationParameters->BackBufferWidth;
		pPresentationParameters9->EnableAutoDepthStencil = pPresentationParameters->EnableAutoDepthStencil;
		pPresentationParameters9->Flags = pPresentationParameters->Flags;
		pPresentationParameters9->FullScreen_RefreshRateInHz = pPresentationParameters->FullScreen_RefreshRateInHz;
		pPresentationParameters9->hDeviceWindow = pPresentationParameters->hDeviceWindow;

		// Will always select best quality...
		pDirect3D9->CheckDeviceMultiSampleType(this->deviceCreationParameters.AdapterOrdinal, this->deviceCreationParameters.DeviceType, pPresentationParameters->BackBufferFormat, pPresentationParameters->Windowed, pPresentationParameters->MultiSampleType, &pPresentationParameters9->MultiSampleQuality);

		pPresentationParameters9->MultiSampleType = pPresentationParameters->MultiSampleType;
		pPresentationParameters9->PresentationInterval = pPresentationParameters->FullScreen_PresentationInterval;
		pPresentationParameters9->SwapEffect = pPresentationParameters->SwapEffect;
		pPresentationParameters9->Windowed = pPresentationParameters->Windowed;

		result = this->pDevice9->Reset(pPresentationParameters9);

		pDirect3D9->Release();

		delete pPresentationParameters9;
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::Present(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return this->pDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

STDMETHODIMP CDeviceWrapper::GetBackBuffer(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer)
{
	IDirect3DSurface9* pBackBuffer9 = NULL;
	HRESULT result = this->pDevice9->GetBackBuffer(this->deviceCreationParameters.AdapterOrdinal, BackBuffer, Type, &pBackBuffer9);

	// Check if we already have a pointer to this surface... and if we do, re-use it
	CSurfaceWrapper* surfaceWrapper = this->GetWrappedSurface(pBackBuffer9);

	if (this->pGetBackBufferCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		surfaceWrapper->AddRef();
		if (this->pGetBackBufferCallbackThis == NULL)
		{
			// C-style function was set
			((CGetBackBufferCallback_t) this->pGetBackBufferCallbackFunction)((ISurfaceWrapper*) surfaceWrapper);
		}
		else
		{
			// C++ class member function was set
			((GetBackBufferCallback_t) this->pGetBackBufferCallbackFunction)(this->pGetBackBufferCallbackThis, (ISurfaceWrapper*) surfaceWrapper);
		}
	}

	*ppBackBuffer = surfaceWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::GetRasterStatus(THIS_ D3DRASTER_STATUS* pRasterStatus)
{
	return this->pDevice9->GetRasterStatus(this->deviceCreationParameters.AdapterOrdinal, pRasterStatus);
}

STDMETHODIMP_(void) CDeviceWrapper::SetGammaRamp(THIS_ DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	this->pDevice9->SetGammaRamp(this->deviceCreationParameters.AdapterOrdinal, Flags, pRamp);
}

STDMETHODIMP_(void) CDeviceWrapper::GetGammaRamp(THIS_ D3DGAMMARAMP* pRamp)
{
	this->pDevice9->GetGammaRamp(this->deviceCreationParameters.AdapterOrdinal, pRamp);
}

STDMETHODIMP CDeviceWrapper::CreateTexture(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture)
{
	IDirect3DTexture9* pTexture9 = NULL;
	HRESULT result = this->pDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, &pTexture9, NULL);

	// Check if we already have a pointer to this texture... and if we do, re-use it
	CTextureWrapper* textureWrapper = this->GetWrappedTexture(pTexture9);

	if (this->pCreateTextureCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		textureWrapper->AddRef();
		if (this->pCreateTextureCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateTextureCallback_t) this->pCreateTextureCallbackFunction)((ITextureWrapper*) textureWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateTextureCallback_t) this->pCreateTextureCallbackFunction)(this->pCreateTextureCallbackThis, (ITextureWrapper*) textureWrapper);
		}
	}

	*ppTexture = textureWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateVolumeTexture(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture)
{
	IDirect3DVolumeTexture9* pVolumeTexture9 = NULL;
	HRESULT result = this->pDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, &pVolumeTexture9, NULL);

	// Check if we already have a pointer to this volume texture... and if we do, re-use it
	CVolumeTextureWrapper* volumeTextureWrapper = this->GetWrappedVolumeTexture(pVolumeTexture9);

	if (this->pCreateVolumeTextureCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		volumeTextureWrapper->AddRef();
		if (this->pCreateVolumeTextureCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateVolumeTextureCallback_t) this->pCreateVolumeTextureCallbackFunction)((IVolumeTextureWrapper*) volumeTextureWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateVolumeTextureCallback_t) this->pCreateVolumeTextureCallbackFunction)(this->pCreateVolumeTextureCallbackThis, (IVolumeTextureWrapper*) volumeTextureWrapper);
		}
	}

	*ppVolumeTexture = volumeTextureWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateCubeTexture(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture)
{
	IDirect3DCubeTexture9* pCubeTexture9 = NULL;
	HRESULT result = this->pDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, &pCubeTexture9, NULL);

	// Check if we already have a pointer to this cube texture... and if we do, re-use it
	CCubeTextureWrapper* cubeTextureWrapper = this->GetWrappedCubeTexture(pCubeTexture9);

	if (this->pCreateCubeTextureCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		cubeTextureWrapper->AddRef();
		if (this->pCreateCubeTextureCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateCubeTextureCallback_t) this->pCreateCubeTextureCallbackFunction)((ICubeTextureWrapper*) cubeTextureWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateCubeTextureCallback_t) this->pCreateCubeTextureCallbackFunction)(this->pCreateCubeTextureCallbackThis, (ICubeTextureWrapper*) cubeTextureWrapper);
		}
	}

	*ppCubeTexture = cubeTextureWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateVertexBuffer(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer)
{
	IDirect3DVertexBuffer9* pVertexBuffer9 = NULL;
	HRESULT result = this->pDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, &pVertexBuffer9, NULL);

	// Check if we already have a pointer to this vertex buffer... and if we do, re-use it
	CVertexBufferWrapper* vertexBufferWrapper = this->GetWrappedVertexBuffer(pVertexBuffer9);

	if (this->pCreateVertexBufferCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		vertexBufferWrapper->AddRef();
		if (this->pCreateVertexBufferCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateVertexBufferCallback_t) this->pCreateVertexBufferCallbackFunction)((IVertexBufferWrapper*) vertexBufferWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateVertexBufferCallback_t) this->pCreateVertexBufferCallbackFunction)(this->pCreateVertexBufferCallbackThis, (IVertexBufferWrapper*) vertexBufferWrapper);
		}
	}

	*ppVertexBuffer = vertexBufferWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateIndexBuffer(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer)
{
	IDirect3DIndexBuffer9* pIndexBuffer9 = NULL;
	HRESULT result = this->pDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, &pIndexBuffer9, NULL);

	// Check if we already have a pointer to this index buffer... and if we do, re-use it
	CIndexBufferWrapper* indexBufferWrapper = this->GetWrappedIndexBuffer(pIndexBuffer9);

	if (this->pCreateIndexBufferCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		indexBufferWrapper->AddRef();
		if (this->pCreateIndexBufferCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateIndexBufferCallback_t) this->pCreateIndexBufferCallbackFunction)((IIndexBufferWrapper*) indexBufferWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateIndexBufferCallback_t) this->pCreateIndexBufferCallbackFunction)(this->pCreateIndexBufferCallbackThis, (IIndexBufferWrapper*) indexBufferWrapper);
		}
	}

	*ppIndexBuffer = indexBufferWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateRenderTarget(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface)
{
	HRESULT result = D3DERR_NOTAVAILABLE;

	IDirect3D9* pDirect3D9 = NULL;
	if (SUCCEEDED(this->pDevice9->GetDirect3D(&pDirect3D9)))
	{
		IDirect3DSurface9* pSurface9 = NULL;

		DWORD multiSampleQuality;
		pDirect3D9->CheckDeviceMultiSampleType(this->deviceCreationParameters.AdapterOrdinal, this->deviceCreationParameters.DeviceType, Format, TRUE, MultiSample, &multiSampleQuality);

		result = this->pDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, multiSampleQuality, Lockable, &pSurface9, NULL);

		// Check if we already have a pointer to this surface... and if we do, re-use it
		CSurfaceWrapper* surfaceWrapper = this->GetWrappedSurface(pSurface9);

		if (this->pCreateRenderTargetCallbackFunction != NULL)
		{
			// Callback function gets an owning reference...
			surfaceWrapper->AddRef();
			if (this->pCreateRenderTargetCallbackThis == NULL)
			{
				// C-style function was set
				((CCreateRenderTargetCallback_t) this->pCreateRenderTargetCallbackFunction)((ISurfaceWrapper*) surfaceWrapper);
			}
			else
			{
				// C++ class member function was set
				((CreateRenderTargetCallback_t) this->pCreateRenderTargetCallbackFunction)(this->pCreateRenderTargetCallbackThis, (ISurfaceWrapper*) surfaceWrapper);
			}
		}

		*ppSurface = surfaceWrapper;

		pDirect3D9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateDepthStencilSurface(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface)
{
	HRESULT result = D3DERR_NOTAVAILABLE;

	IDirect3D9* pDirect3D9 = NULL;
	if (SUCCEEDED(this->pDevice9->GetDirect3D(&pDirect3D9)))
	{
		IDirect3DSurface9* pSurface9 = NULL;

		DWORD multiSampleQuality;
		pDirect3D9->CheckDeviceMultiSampleType(this->deviceCreationParameters.AdapterOrdinal, this->deviceCreationParameters.DeviceType, Format, TRUE, MultiSample, &multiSampleQuality);

		result = this->pDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, multiSampleQuality, this->zBufferDiscardingEnabled, &pSurface9, NULL);

		// Check if we already have a pointer to this surface... and if we do, re-use it
		CSurfaceWrapper* surfaceWrapper = this->GetWrappedSurface(pSurface9);

		if (this->pCreateDepthStencilSurfaceCallbackFunction != NULL)
		{
			// Callback function gets an owning reference...
			surfaceWrapper->AddRef();
			if (this->pCreateDepthStencilSurfaceCallbackThis == NULL)
			{
				// C-style function was set
				((CCreateDepthStencilSurfaceCallback_t) this->pCreateDepthStencilSurfaceCallbackFunction)((ISurfaceWrapper*) surfaceWrapper);
			}
			else
			{
				// C++ class member function was set
				((CreateDepthStencilSurfaceCallback_t) this->pCreateDepthStencilSurfaceCallbackFunction)(this->pCreateDepthStencilSurfaceCallbackThis, (ISurfaceWrapper*) surfaceWrapper);
			}
		}

		*ppSurface = surfaceWrapper;

		pDirect3D9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateImageSurface(THIS_ UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface)
{
	IDirect3DSurface9* pSurface9 = NULL;
	HRESULT result = this->pDevice9->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SCRATCH, &pSurface9, NULL);

	// Check if we already have a pointer to this surface... and if we do, re-use it
	CSurfaceWrapper* surfaceWrapper = this->GetWrappedSurface(pSurface9);

	if (this->pCreateImageSurfaceCallbackFunction != NULL)
	{
		// Callback function gets an owning reference...
		surfaceWrapper->AddRef();
		if (this->pCreateImageSurfaceCallbackThis == NULL)
		{
			// C-style function was set
			((CCreateImageSurfaceCallback_t) this->pCreateImageSurfaceCallbackFunction)((ISurfaceWrapper*) surfaceWrapper);
		}
		else
		{
			// C++ class member function was set
			((CreateImageSurfaceCallback_t) this->pCreateImageSurfaceCallbackFunction)(this->pCreateImageSurfaceCallbackThis, (ISurfaceWrapper*) surfaceWrapper);
		}
	}

	*ppSurface = surfaceWrapper;

	return result;
}

STDMETHODIMP CDeviceWrapper::CopyRects(THIS_ IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DSurface9* pSourceSurface9 = NULL;
	if (SUCCEEDED(((ISurfaceWrapper*) pSourceSurface)->GetWrappedInterface((void**) &pSourceSurface9)))
	{
		IDirect3DSurface9* pDestinationSurface9 = NULL;
		if (SUCCEEDED(((ISurfaceWrapper*) pDestinationSurface)->GetWrappedInterface((void**) &pDestinationSurface9)))
		{
			if (pSourceRectsArray == NULL)
			{
				// Copy entire surface...
				if (pDestPointsArray == NULL)
				{
					POINT* originPoint = new POINT();
					originPoint->x = 0;
					originPoint->y = 0;
					// ...to upper left corner of destination surface
					result = this->pDevice9->UpdateSurface(pSourceSurface9, NULL, pDestinationSurface9, originPoint);

					delete originPoint;
				}
				else
				{
					// ...to destination point
					result = this->pDevice9->UpdateSurface(pSourceSurface9, NULL, pDestinationSurface9, pDestPointsArray);
				}
			}
			else
			{
				// Copy each RECT in pSourceRectsArray...
				if (pDestPointsArray == NULL)
				{
					// ... to upper left corner of destination surface
					POINT* originPoint = new POINT();
					originPoint->x = 0;
					originPoint->y = 0;

					for (UINT i = 0; i < cRects; i++)
					{
						result = this->pDevice9->UpdateSurface(pSourceSurface9, &pSourceRectsArray[i], pDestinationSurface9, originPoint);
						if (!SUCCEEDED(result))
						{
							break;
						}
					}

					delete originPoint;
				}
				else
				{
					// ...to destination point in pDestPointsArray
					for (UINT i = 0; i < cRects; i++)
					{
						result = this->pDevice9->UpdateSurface(pSourceSurface9, &pSourceRectsArray[i], pDestinationSurface9, &pDestPointsArray[i]);
						if (!SUCCEEDED(result))
						{
							break;
						}
					}
				}
			}
			pDestinationSurface9->Release();
		}

		pSourceSurface9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::UpdateTexture(THIS_ IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DBaseTexture9* pSourceTexture9 = NULL;
	if (SUCCEEDED(((IBaseTextureWrapper*) pSourceTexture)->GetWrappedInterface((void**) &pSourceTexture9)))
	{
		IDirect3DBaseTexture9* pDestinationTexture9 = NULL;
		if (SUCCEEDED(((IBaseTextureWrapper*) pDestinationTexture)->GetWrappedInterface((void**) &pDestinationTexture9)))
		{
			result = this->pDevice9->UpdateTexture(pSourceTexture9, pDestinationTexture9);

			pDestinationTexture9->Release();
		}

		pSourceTexture9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::GetFrontBuffer(THIS_ IDirect3DSurface8* pDestSurface)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DSurface9* pDestSurface9 = NULL;
	if (SUCCEEDED(((ISurfaceWrapper*) pDestSurface)->GetWrappedInterface((void**) &pDestSurface9)))
	{
		result = this->pDevice9->GetFrontBufferData(this->deviceCreationParameters.AdapterOrdinal, pDestSurface9);

		pDestSurface9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::SetRenderTarget(THIS_ IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil)
{
	HRESULT result = D3DERR_INVALIDCALL;

	if (pRenderTarget == NULL)
	{
		// We are setting a new Z Stencil...
		IDirect3DSurface9* pNewZStencil9 = NULL;
		if (SUCCEEDED(((ISurfaceWrapper*) pNewZStencil)->GetWrappedInterface((void**) &pNewZStencil9)))
		{
			result = this->pDevice9->SetDepthStencilSurface(pNewZStencil9);
			pNewZStencil9->Release();
		}
	}
	else
	{
		// We are setting a new Render Target...
		IDirect3DSurface9* pRenderTarget9 = NULL;
		if (SUCCEEDED(((ISurfaceWrapper*) pRenderTarget)->GetWrappedInterface((void**) &pRenderTarget9)))
		{
			result = this->pDevice9->SetRenderTarget(0, pRenderTarget9);
			pRenderTarget9->Release();
		}

		if (pNewZStencil != NULL)
		{
			// ...and a new Z stencil
			IDirect3DSurface9* pNewZStencil9 = NULL;
			if (SUCCEEDED(((ISurfaceWrapper*) pNewZStencil)->GetWrappedInterface((void**) &pNewZStencil9)))
			{
				result = this->pDevice9->SetDepthStencilSurface(pNewZStencil9);
				pNewZStencil9->Release();
			}
		}
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::GetRenderTarget(THIS_ IDirect3DSurface8** ppRenderTarget)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetDepthStencilSurface(THIS_ IDirect3DSurface8** ppZStencilSurface)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::BeginScene(THIS)
{
	return this->pDevice9->BeginScene();
}

STDMETHODIMP CDeviceWrapper::EndScene(THIS)
{
	return this->pDevice9->EndScene();
}

STDMETHODIMP CDeviceWrapper::Clear(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return this->pDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

STDMETHODIMP CDeviceWrapper::SetTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return this->pDevice9->SetTransform(State, pMatrix);
}

STDMETHODIMP CDeviceWrapper::GetTransform(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return this->pDevice9->GetTransform(State, pMatrix);
}

STDMETHODIMP CDeviceWrapper::MultiplyTransform(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return this->pDevice9->MultiplyTransform(State, pMatrix);
}

STDMETHODIMP CDeviceWrapper::SetViewport(THIS_ CONST D3DVIEWPORT8* pViewport)
{
	return this->pDevice9->SetViewport(pViewport);
}

STDMETHODIMP CDeviceWrapper::GetViewport(THIS_ D3DVIEWPORT8* pViewport)
{
	return this->pDevice9->GetViewport(pViewport);
}

STDMETHODIMP CDeviceWrapper::SetMaterial(THIS_ CONST D3DMATERIAL8* pMaterial)
{
	return this->pDevice9->SetMaterial(pMaterial);
}

STDMETHODIMP CDeviceWrapper::GetMaterial(THIS_ D3DMATERIAL8* pMaterial)
{
	return this->pDevice9->GetMaterial(pMaterial);
}

STDMETHODIMP CDeviceWrapper::SetLight(THIS_ DWORD Index, CONST D3DLIGHT8* pLight)
{
	return this->pDevice9->SetLight(Index, pLight);
}

STDMETHODIMP CDeviceWrapper::GetLight(THIS_ DWORD Index, D3DLIGHT8* pLight)
{
	return this->pDevice9->GetLight(Index, pLight);
}

STDMETHODIMP CDeviceWrapper::LightEnable(THIS_ DWORD Index, BOOL Enable)
{
	return this->pDevice9->LightEnable(Index, Enable);
}

STDMETHODIMP CDeviceWrapper::GetLightEnable(THIS_ DWORD Index, BOOL* pEnable)
{
	return this->pDevice9->GetLightEnable(Index, pEnable);
}

STDMETHODIMP CDeviceWrapper::SetClipPlane(THIS_ DWORD Index, CONST float* pPlane)
{
	return this->pDevice9->SetClipPlane(Index, pPlane);
}

STDMETHODIMP CDeviceWrapper::GetClipPlane(THIS_ DWORD Index, float* pPlane)
{
	return this->pDevice9->GetClipPlane(Index, pPlane);
}

STDMETHODIMP CDeviceWrapper::SetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD Value)
{
	return this->pDevice9->SetRenderState(State, Value);
}

STDMETHODIMP CDeviceWrapper::GetRenderState(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return this->pDevice9->GetRenderState(State, pValue);
}

STDMETHODIMP CDeviceWrapper::BeginStateBlock(THIS)
{
	return this->pDevice9->BeginStateBlock();
}

STDMETHODIMP CDeviceWrapper::EndStateBlock(THIS_ DWORD* pToken)
{
	return this->pDevice9->EndStateBlock((IDirect3DStateBlock9**) pToken);
}

STDMETHODIMP CDeviceWrapper::ApplyStateBlock(THIS_ DWORD Token)
{
	IDirect3DStateBlock9* pBlock = (IDirect3DStateBlock9*) Token;
	return pBlock->Apply();
}

STDMETHODIMP CDeviceWrapper::CaptureStateBlock(THIS_ DWORD Token)
{
	IDirect3DStateBlock9* pBlock = (IDirect3DStateBlock9*) Token;
	return pBlock->Capture();
}

STDMETHODIMP CDeviceWrapper::DeleteStateBlock(THIS_ DWORD Token)
{
	IDirect3DStateBlock9* pBlock = (IDirect3DStateBlock9*) Token;
	pBlock->Release();

	return D3D_OK;
}

STDMETHODIMP CDeviceWrapper::CreateStateBlock(THIS_ D3DSTATEBLOCKTYPE Type, DWORD* pToken)
{
	return this->pDevice9->CreateStateBlock(Type, (IDirect3DStateBlock9**) pToken);
}

STDMETHODIMP CDeviceWrapper::SetClipStatus(THIS_ CONST D3DCLIPSTATUS8* pClipStatus)
{
	return this->pDevice9->SetClipStatus(pClipStatus);
}

STDMETHODIMP CDeviceWrapper::GetClipStatus(THIS_ D3DCLIPSTATUS8* pClipStatus)
{
	return this->pDevice9->GetClipStatus(pClipStatus);
}

STDMETHODIMP CDeviceWrapper::GetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture8** ppTexture)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetTexture(THIS_ DWORD Stage, IDirect3DBaseTexture8* pTexture)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DBaseTexture9* pTexture9 = NULL;
	if (SUCCEEDED(((IBaseTextureWrapper*) pTexture)->GetWrappedInterface((void**) &pTexture9)))
	{
		result = this->pDevice9->SetTexture(Stage, pTexture9);
		pTexture9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::GetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return this->pDevice9->GetTextureStageState(Stage, Type, pValue);
}

STDMETHODIMP CDeviceWrapper::SetTextureStageState(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return this->pDevice9->SetTextureStageState(Stage, Type, Value);
}

STDMETHODIMP CDeviceWrapper::ValidateDevice(THIS_ DWORD* pNumPasses)
{
	return this->pDevice9->ValidateDevice(pNumPasses);
}

STDMETHODIMP CDeviceWrapper::GetInfo(THIS_ DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetPaletteEntries(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return this->pDevice9->SetPaletteEntries(PaletteNumber, pEntries);
}

STDMETHODIMP CDeviceWrapper::GetPaletteEntries(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return this->pDevice9->GetPaletteEntries(PaletteNumber, pEntries);
}

STDMETHODIMP CDeviceWrapper::SetCurrentTexturePalette(THIS_ UINT PaletteNumber)
{
	return this->pDevice9->SetCurrentTexturePalette(PaletteNumber);
}

STDMETHODIMP CDeviceWrapper::GetCurrentTexturePalette(THIS_ UINT *PaletteNumber)
{
	return this->pDevice9->GetCurrentTexturePalette(PaletteNumber);
}

STDMETHODIMP CDeviceWrapper::DrawPrimitive(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return this->pDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

STDMETHODIMP CDeviceWrapper::DrawIndexedPrimitive(THIS_ D3DPRIMITIVETYPE Type, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	//return this->pDevice9->DrawIndexedPrimitive(Type, ?, minIndex, NumVertices, startIndex, primCount);
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::DrawPrimitiveUP(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return this->pDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

STDMETHODIMP CDeviceWrapper::DrawIndexedPrimitiveUP(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return this->pDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertexIndices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

STDMETHODIMP CDeviceWrapper::ProcessVertices(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags)
{
	HRESULT result = D3DERR_INVALIDCALL;

	IDirect3DVertexBuffer9* pDestBuffer9 = NULL;
	if (SUCCEEDED(((IVertexBufferWrapper*) pDestBuffer)->GetWrappedInterface((void**) &pDestBuffer9)))
	{
		//result = this->pDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer9, ?, Flags);
		result = E_NOTIMPL;
		pDestBuffer9->Release();
	}

	return result;
}

STDMETHODIMP CDeviceWrapper::CreateVertexShader(THIS_ CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetVertexShader(THIS_ DWORD Handle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetVertexShader(THIS_ DWORD* pHandle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::DeleteVertexShader(THIS_ DWORD Handle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetVertexShaderConstant(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetVertexShaderConstant(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetVertexShaderDeclaration(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetVertexShaderFunction(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetStreamSource(THIS_ UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetStreamSource(THIS_ UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetIndices(THIS_ IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetIndices(THIS_ IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::CreatePixelShader(THIS_ CONST DWORD* pFunction, DWORD* pHandle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetPixelShader(THIS_ DWORD Handle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetPixelShader(THIS_ DWORD* pHandle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::DeletePixelShader(THIS_ DWORD Handle)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::SetPixelShaderConstant(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetPixelShaderConstant(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::GetPixelShaderFunction(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceWrapper::DrawRectPatch(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return this->pDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

STDMETHODIMP CDeviceWrapper::DrawTriPatch(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return this->pDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

STDMETHODIMP CDeviceWrapper::DeletePatch(THIS_ UINT Handle)
{
	return this->pDevice9->DeletePatch(Handle);
}
