#pragma once
#include "stdafx.h"

// DEFINITIONS

typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *tD3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *tD3D11CreateQuery)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *tD3D11EndScene)(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *tD3D11VSSetConstantBuffers) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);

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
	extern tD3D11VSSetConstantBuffers oVSSetConstantBuffers;

	extern tD3D11DrawInstanced oDrawInstanced;
	extern tD3D11DrawIndexedInstanced oDrawIndexedInstanced;
	extern tD3D11DrawInstancedIndirect oDrawInstancedIndirect;
	extern tD3D11DrawIndexedInstancedIndirect oDrawIndexedInstancedIndirect;
	//HOOK FUNCTIONS

	HRESULT __stdcall hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void __stdcall hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void __stdcall hkD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
	void __stdcall hkD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);

	void __stdcall hkD3D11DrawInstanced(ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
	void __stdcall hkD3D11DrawIndexedInstanced (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
	void __stdcall hkD3D11DrawInstancedIndirect (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
	void __stdcall hkD3D11DrawIndexedInstancedIndirect (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);

}

