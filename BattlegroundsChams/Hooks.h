#pragma once
#include "stdafx.h"

// DEFINITIONS

typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *tD3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *tD3D11UnMap)(ID3D11DeviceContext* pContext, _In_ ID3D11Buffer *pResource, _In_ UINT Subresource/*, _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE *pMappedResource*/);
typedef void(__stdcall *tD3D11CreateQuery)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *tD3D11EndScene)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *tD3D11VSSetConstantBuffers) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
typedef void(__stdcall *tD3D11PSSetShaderResources) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
typedef void(__stdcall *tD3D11PSSetSamplers) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
typedef void(__stdcall *tD3D11UpdateSubresource) (ID3D11DeviceContext* pContext, ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);

typedef void(__stdcall *tD3D11DrawInstanced) (ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall *tD3D11DrawIndexedInstanced) (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall *tD3D11DrawInstancedIndirect) (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void(__stdcall *tD3D11DrawIndexedInstancedIndirect) (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);

namespace Hooks
{
	//ORIGINAL FUNCTIONS

	extern tD3D11CreateQuery oCreateQuery;
	extern tD3D11Present oPresent;
	extern tD3D11DrawIndexed oDrawIndexed;
	extern tD3D11UnMap oUnMap;
	extern tD3D11VSSetConstantBuffers oVSSetConstantBuffers;
	extern tD3D11PSSetShaderResources oPSSetShaderResources;
	extern tD3D11PSSetSamplers oPSSetSamplers;
	extern tD3D11UpdateSubresource oUpdateSubresource;

	extern tD3D11DrawInstanced oDrawInstanced;
	extern tD3D11DrawIndexedInstanced oDrawIndexedInstanced;
	extern tD3D11DrawInstancedIndirect oDrawInstancedIndirect;
	extern tD3D11DrawIndexedInstancedIndirect oDrawIndexedInstancedIndirect;
	//HOOK FUNCTIONS

	HRESULT __stdcall hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void __stdcall hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	//void __stdcall hkD3D11UnMap(ID3D11DeviceContext* pContext, _In_ ID3D11Resource *pResource, _In_ UINT Subresource, _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE *pMappedResource);
	void __stdcall hkD3D11UnMap(ID3D11DeviceContext* pContext, __in ID3D11Buffer *pResource, __in UINT Subresource);
	void __stdcall hkD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
	void __stdcall hkD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);

	void __stdcall hkD3D11PSSetShaderResources (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
	void __stdcall hkD3D11PSSetSamplers (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);
	void __stdcall hkD3D11UpdateSubresource(ID3D11DeviceContext* pContext, ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);

	void __stdcall hkD3D11DrawInstanced(ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void __stdcall hkD3D11DrawIndexedInstanced (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	void __stdcall hkD3D11DrawInstancedIndirect (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void __stdcall hkD3D11DrawIndexedInstancedIndirect (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
}

