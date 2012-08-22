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

// DeviceWrapper.h : Declaration of the CDeviceWrapper
#pragma once

#include "resource.h"       // main symbols

#include "D3D8Wrapper_i.h"

#include "stdafx.h"

// Forward Declaration
class CDeviceWrapper;

#include <map>

#include "Direct3D8Wrapper.h"
#include "SwapChainWrapper.h"
#include "TextureWrapper.h"
#include "VolumeTextureWrapper.h"
#include "CubeTextureWrapper.h"
#include "VertexBufferWrapper.h"
#include "IndexBufferWrapper.h"
#include "SurfaceWrapper.h"

#include "VolumeWrapper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform,  such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// CDeviceWrapper

class ATL_NO_VTABLE CDeviceWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDeviceWrapper, &CLSID_DeviceWrapper>,
	public IDeviceWrapper,
	public IDirect3DDevice8
{
public:
	CDeviceWrapper();
	~CDeviceWrapper();

	DECLARE_REGISTRY_RESOURCEID(IDR_DEVICEWRAPPER)

	DECLARE_NOT_AGGREGATABLE(CDeviceWrapper)

	BEGIN_COM_MAP(CDeviceWrapper)
		COM_INTERFACE_ENTRY(IDeviceWrapper)
		COM_INTERFACE_ENTRY_IID(IID_IDirect3DDevice8, CDeviceWrapper)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

	// Internal Methods
	HRESULT SetDirect3D8(CDirect3D8Wrapper* pObject);
	HRESULT SetZBufferDiscarding(bool IsEnabled);

	CSwapChainWrapper* GetWrappedSwapChain(IDirect3DSwapChain9* pSwapChain9);
	CTextureWrapper* GetWrappedTexture(IDirect3DTexture9* pTexture9);
	CVolumeTextureWrapper* GetWrappedVolumeTexture(IDirect3DVolumeTexture9* pVolumeTexture9);
	CCubeTextureWrapper* GetWrappedCubeTexture(IDirect3DCubeTexture9* pCubeTexture9);
	CVertexBufferWrapper* GetWrappedVertexBuffer(IDirect3DVertexBuffer9* pVertexBuffer9);
	CIndexBufferWrapper* GetWrappedIndexBuffer(IDirect3DIndexBuffer9* pIndexBuffer);
	CSurfaceWrapper* GetWrappedSurface(IDirect3DSurface9* pSurface9);
	CVolumeWrapper* GetWrappedVolume(IDirect3DVolume9* pVolume9);

	void RemoveWrappedSwapChain(IDirect3DSwapChain9* pSwapChain9);
	void RemoveWrappedTexture(IDirect3DTexture9* pTexture9);
	void RemoveWrappedVolumeTexture(IDirect3DVolumeTexture9* pVolumeTexture9);
	void RemoveWrappedCubeTexture(IDirect3DCubeTexture9* pCubeTexture9);
	void RemoveWrappedVertexBuffer(IDirect3DVertexBuffer9* pVertexBuffer9);
	void RemoveWrappedIndexBuffer(IDirect3DIndexBuffer9* pIndexBuffer);
	void RemoveWrappedSurface(IDirect3DSurface9* pSurface9);
	void RemoveWrappedVolume(IDirect3DVolume9* pVolume9);

private:
	IDirect3DDevice9* pDevice9;

	IClassFactory* pSwapChainWrapperFactory;
	IClassFactory* pTextureWrapperFactory;
	IClassFactory* pVolumeTextureWrapperFactory;
	IClassFactory* pCubeTextureWrapperFactory;
	IClassFactory* pVertexBufferWrapperFactory;
	IClassFactory* pIndexBufferWrapperFactory;
	IClassFactory* pSurfaceWrapperFactory;

	IClassFactory* pVolumeWrapperFactory;

	std::map<IUnknown*, CSwapChainWrapper*> createdSwapChains;
	CRITICAL_SECTION createdSwapChainsMutex;

	std::map<IUnknown*, CTextureWrapper*> createdTextures;
	CRITICAL_SECTION createdTexturesMutex;

	std::map<IUnknown*, CVolumeTextureWrapper*> createdVolumeTextures;
	CRITICAL_SECTION createdVolumeTexturesMutex;

	std::map<IUnknown*, CCubeTextureWrapper*> createdCubeTextures;
	CRITICAL_SECTION createdCubeTexturesMutex;

	std::map<IUnknown*, CVertexBufferWrapper*> createdVertexBuffers;
	CRITICAL_SECTION createdVertexBuffersMutex;

	std::map<IUnknown*, CIndexBufferWrapper*> createdIndexBuffers;
	CRITICAL_SECTION createdIndexBuffersMutex;

	std::map<IUnknown*, CSurfaceWrapper*> createdSurfaces;
	CRITICAL_SECTION createdSurfacesMutex;

	std::map<IUnknown*, CVolumeWrapper*> createdVolumes;
	CRITICAL_SECTION createdVolumesMutex;

	CDirect3D8Wrapper* pDirect3D8Wrapper;

	D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;

	BOOL zBufferDiscardingEnabled;

	void* pCreateAdditionalSwapChainCallbackThis;
	void* pCreateAdditionalSwapChainCallbackFunction;

	void* pCreateTextureCallbackThis;
	void* pCreateTextureCallbackFunction;

	void* pCreateVolumeTextureCallbackThis;
	void* pCreateVolumeTextureCallbackFunction;

	void* pCreateCubeTextureCallbackThis;
	void* pCreateCubeTextureCallbackFunction;

	void* pCreateVertexBufferCallbackThis;
	void* pCreateVertexBufferCallbackFunction;

	void* pCreateIndexBufferCallbackThis;
	void* pCreateIndexBufferCallbackFunction;

	void* pCreateRenderTargetCallbackThis;
	void* pCreateRenderTargetCallbackFunction;

	void* pCreateDepthStencilSurfaceCallbackThis;
	void* pCreateDepthStencilSurfaceCallbackFunction;

	void* pCreateImageSurfaceCallbackThis;
	void* pCreateImageSurfaceCallbackFunction;

	void* pGetBackBufferCallbackThis;
	void* pGetBackBufferCallbackFunction;

public:
	// IDirect3DDevice8 Methods
	STDMETHOD(TestCooperativeLevel)(THIS);
	STDMETHOD_(UINT, GetAvailableTextureMem)(THIS);
	STDMETHOD(ResourceManagerDiscardBytes)(THIS_ DWORD Bytes);
	STDMETHOD(GetDirect3D)(THIS_ IDirect3D8** ppD3D8);
	STDMETHOD(GetDeviceCaps)(THIS_ D3DCAPS8* pCaps);
	STDMETHOD(GetDisplayMode)(THIS_ D3DDISPLAYMODE* pMode);
	STDMETHOD(GetCreationParameters)(THIS_ D3DDEVICE_CREATION_PARAMETERS *pParameters);
	STDMETHOD(SetCursorProperties)(THIS_ UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap);
	STDMETHOD_(void, SetCursorPosition)(THIS_ int X, int Y, DWORD Flags);
	STDMETHOD_(BOOL, ShowCursor)(THIS_ BOOL bShow);
	STDMETHOD(CreateAdditionalSwapChain)(THIS_ D3D8PRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** pSwapChain);
	STDMETHOD(Reset)(THIS_ D3D8PRESENT_PARAMETERS* pPresentationParameters);
	STDMETHOD(Present)(THIS_ CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	STDMETHOD(GetBackBuffer)(THIS_ UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer);
	STDMETHOD(GetRasterStatus)(THIS_ D3DRASTER_STATUS* pRasterStatus);
	STDMETHOD_(void, SetGammaRamp)(THIS_ DWORD Flags, CONST D3DGAMMARAMP* pRamp);
	STDMETHOD_(void, GetGammaRamp)(THIS_ D3DGAMMARAMP* pRamp);
	STDMETHOD(CreateTexture)(THIS_ UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture);
	STDMETHOD(CreateVolumeTexture)(THIS_ UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture);
	STDMETHOD(CreateCubeTexture)(THIS_ UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture);
	STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer);
	STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer);
	STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface);
	STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface);
	STDMETHOD(CreateImageSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface);
	STDMETHOD(CopyRects)(THIS_ IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray);
	STDMETHOD(UpdateTexture)(THIS_ IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture);
	STDMETHOD(GetFrontBuffer)(THIS_ IDirect3DSurface8* pDestSurface);
	STDMETHOD(SetRenderTarget)(THIS_ IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil);
	STDMETHOD(GetRenderTarget)(THIS_ IDirect3DSurface8** ppRenderTarget);
	STDMETHOD(GetDepthStencilSurface)(THIS_ IDirect3DSurface8** ppZStencilSurface);
	STDMETHOD(BeginScene)(THIS);
	STDMETHOD(EndScene)(THIS);
	STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
	STDMETHOD(SetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	STDMETHOD(GetTransform)(THIS_ D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
	STDMETHOD(MultiplyTransform)(THIS_ D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	STDMETHOD(SetViewport)(THIS_ CONST D3DVIEWPORT8* pViewport);
	STDMETHOD(GetViewport)(THIS_ D3DVIEWPORT8* pViewport);
	STDMETHOD(SetMaterial)(THIS_ CONST D3DMATERIAL8* pMaterial);
	STDMETHOD(GetMaterial)(THIS_ D3DMATERIAL8* pMaterial);
	STDMETHOD(SetLight)(THIS_ DWORD Index, CONST D3DLIGHT8* pLight);
	STDMETHOD(GetLight)(THIS_ DWORD Index, D3DLIGHT8* pLight);
	STDMETHOD(LightEnable)(THIS_ DWORD Index, BOOL Enable);
	STDMETHOD(GetLightEnable)(THIS_ DWORD Index, BOOL* pEnable);
	STDMETHOD(SetClipPlane)(THIS_ DWORD Index, CONST float* pPlane);
	STDMETHOD(GetClipPlane)(THIS_ DWORD Index, float* pPlane);
	STDMETHOD(SetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD Value);
	STDMETHOD(GetRenderState)(THIS_ D3DRENDERSTATETYPE State, DWORD* pValue);
	STDMETHOD(BeginStateBlock)(THIS);
	STDMETHOD(EndStateBlock)(THIS_ DWORD* pToken);
	STDMETHOD(ApplyStateBlock)(THIS_ DWORD Token);
	STDMETHOD(CaptureStateBlock)(THIS_ DWORD Token);
	STDMETHOD(DeleteStateBlock)(THIS_ DWORD Token);
	STDMETHOD(CreateStateBlock)(THIS_ D3DSTATEBLOCKTYPE Type, DWORD* pToken);
	STDMETHOD(SetClipStatus)(THIS_ CONST D3DCLIPSTATUS8* pClipStatus);
	STDMETHOD(GetClipStatus)(THIS_ D3DCLIPSTATUS8* pClipStatus);
	STDMETHOD(GetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture8** ppTexture);
	STDMETHOD(SetTexture)(THIS_ DWORD Stage, IDirect3DBaseTexture8* pTexture);
	STDMETHOD(GetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
	STDMETHOD(SetTextureStageState)(THIS_ DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
	STDMETHOD(ValidateDevice)(THIS_ DWORD* pNumPasses);
	STDMETHOD(GetInfo)(THIS_ DWORD DevInfoID, void* pDevInfoStruct, DWORD DevInfoStructSize);
	STDMETHOD(SetPaletteEntries)(THIS_ UINT PaletteNumber, CONST PALETTEENTRY* pEntries);
	STDMETHOD(GetPaletteEntries)(THIS_ UINT PaletteNumber, PALETTEENTRY* pEntries);
	STDMETHOD(SetCurrentTexturePalette)(THIS_ UINT PaletteNumber);
	STDMETHOD(GetCurrentTexturePalette)(THIS_ UINT *PaletteNumber);
	STDMETHOD(DrawPrimitive)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	STDMETHOD(DrawIndexedPrimitive)(THIS_ D3DPRIMITIVETYPE, UINT minIndex, UINT NumVertices, UINT startIndex, UINT primCount);
	STDMETHOD(DrawPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
	STDMETHOD(DrawIndexedPrimitiveUP)(THIS_ D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertexIndices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
	STDMETHOD(ProcessVertices)(THIS_ UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags);
	STDMETHOD(CreateVertexShader)(THIS_ CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage);
	STDMETHOD(SetVertexShader)(THIS_ DWORD Handle);
	STDMETHOD(GetVertexShader)(THIS_ DWORD* pHandle);
	STDMETHOD(DeleteVertexShader)(THIS_ DWORD Handle);
	STDMETHOD(SetVertexShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount);
	STDMETHOD(GetVertexShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount);
	STDMETHOD(GetVertexShaderDeclaration)(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData);
	STDMETHOD(GetVertexShaderFunction)(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData);
	STDMETHOD(SetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride);
	STDMETHOD(GetStreamSource)(THIS_ UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride);
	STDMETHOD(SetIndices)(THIS_ IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex);
	STDMETHOD(GetIndices)(THIS_ IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex);
	STDMETHOD(CreatePixelShader)(THIS_ CONST DWORD* pFunction, DWORD* pHandle);
	STDMETHOD(SetPixelShader)(THIS_ DWORD Handle);
	STDMETHOD(GetPixelShader)(THIS_ DWORD* pHandle);
	STDMETHOD(DeletePixelShader)(THIS_ DWORD Handle);
	STDMETHOD(SetPixelShaderConstant)(THIS_ DWORD Register, CONST void* pConstantData, DWORD ConstantCount);
	STDMETHOD(GetPixelShaderConstant)(THIS_ DWORD Register, void* pConstantData, DWORD ConstantCount);
	STDMETHOD(GetPixelShaderFunction)(THIS_ DWORD Handle, void* pData, DWORD* pSizeOfData);
	STDMETHOD(DrawRectPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	STDMETHOD(DrawTriPatch)(THIS_ UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	STDMETHOD(DeletePatch)(THIS_ UINT Handle);

	// IWrapper Methods
	STDMETHOD(GetWrappedInterface)(THIS_ void** ppvObject);
	STDMETHOD(SetWrappedInterface)(THIS_ void* pObject);

	// IDeviceWrapper Methods
	STDMETHOD(SetCreateAdditionalSwapChainCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateTextureCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateVolumeTextureCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateCubeTextureCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateVertexBufferCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateIndexBufferCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateRenderTargetCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateDepthStencilSurfaceCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetCreateImageSurfaceCallback)(THIS_ void* pThis, void* pCallbackFunction);
	STDMETHOD(SetGetBackBufferCallback)(THIS_ void* pThis, void* pCallbackFunction);
};

OBJECT_ENTRY_AUTO(__uuidof(DeviceWrapper),  CDeviceWrapper)
