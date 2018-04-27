#include "stdafx.h"
#include "Hooks.h"
#include "Helpers.h"
#include "CCheat.h"
#include <strsafe.h>
#include <list>
#include ".\mainxxx.h"
//#include <d3dx11async.h>
#include <process.h>

#pragma comment(lib, "winmm.lib") //timeGetTime
#define INTVL  1

ID3D11DepthStencilState *ppDepthStencilState__New = NULL;
ID3D11DepthStencilState *ppDepthStencilState__Old = NULL;
UINT pStencilRef = 0;
extern HWND g_hWnd;
extern RECT g_lpRect;
extern HANDLE  g_Event_Shoot;
void Thread_ExitHook(PVOID param);

////    对应Unicode的调试输出  
//inline void MyTraceW(LPCTSTR strFormat, ...)
//{
//	const int BUFFER_SIZE = 128;
//	va_list args = NULL;
//	va_start(args, strFormat);
//	TCHAR szBuffer[BUFFER_SIZE] = { 0 };
//	_vsnwprintf(szBuffer, BUFFER_SIZE, strFormat, args);//_vsnwprintf for Unicode  
//	va_end(args);
//	::OutputDebugString(szBuffer);
//}
////    对应ASCII的调试输出  
//inline void MyTraceA(const char* strFormat, ...)
//{
//	const int BUFFER_SIZE = 128;
//	char   szBuffer[BUFFER_SIZE] = { 0 };
//	va_list args = NULL;
//
//	// The va_start macro (defined in STDARG.H) is usually equivalent to:  
//	// pArgList = (char *) &szFormat + sizeof (szFormat) ;  
//	va_start(args, strFormat);
//
//	// The last argument to wvsprintf points to the arguments  
//	_vsnprintf(szBuffer, BUFFER_SIZE, strFormat, args);
//
//	// The va_end macro just zeroes out pArgList for no good reason  
//	va_end(args);
//	::OutputDebugStringA(szBuffer);
//}
//
tD3D11CreateQuery Hooks::oCreateQuery = NULL;
tD3D11Present Hooks::oPresent = NULL;
tD3D11DrawIndexed Hooks::oDrawIndexed = NULL;
tD3D11VSSetConstantBuffers Hooks::oVSSetConstantBuffers = NULL;

 tD3D11DrawInstanced Hooks::oDrawInstanced = NULL;
 tD3D11DrawIndexedInstanced Hooks::oDrawIndexedInstanced = NULL;
 tD3D11DrawInstancedIndirect Hooks::oDrawInstancedIndirect = NULL;
 tD3D11DrawIndexedInstancedIndirect Hooks::oDrawIndexedInstancedIndirect = NULL;
 int mPreviousPosX = 0;
 int mPreviousPosY = 0;
 std::vector<int> aaa;
 std::vector<int> bbb;
 int iiidx = 0;

 std::vector<int> lst24;
 std::vector<int> red24;
 int iPos = 0;

 bool bFlashIt = false;

 ID3D11PixelShader* psCrimson = NULL;
 ID3D11PixelShader* psYellow = NULL;
 ID3D11PixelShader* psGreen0 = NULL;
 ID3D11PixelShader* psd = NULL;
 ID3D11PixelShader* psBlue = NULL;
 ID3D11PixelShader* psRed0 = NULL;
 ID3D11PixelShader* psTmp = NULL;
 ID3D11ShaderResourceView* ShaderResourceView;


 //bool UpdateBuffers(ID3D11DeviceContext* d3dDeviceContext, int positionX, int positionY)
 //{
 //	//在顶点缓存最原始的数据被改变了,属于动态顶点缓存(以前教程那些原始顶点数据虽然后面诚意变换矩阵，但是未曾改变原始数据)  
 //
 //	//如何渲染图片的位置未曾改变，就退出函数，这样可以节省大量处理  
 //	if ((positionX == mPreviousPosX) && (positionY == mPreviousPosY))
 //	{
 //		return true;
 //	}
 //
 //	//如果改变渲染图片的位置改变了，就更新位置  
 //	mPreviousPosX = positionX;
 //	mPreviousPosY = positionY;
 //
 //	//求出win32坐标下图片的的left, right, top, bottom坐标,由WIN32坐标PosX和PosY变为D3D11坐标系  
 //	float left, right, top, bottom;
 //
 //	left = (float)((mScrrenWidth / 2) *-1) + (float)positionX;
 //	right = left + (float)mBitmapWidth;
 //	top = (float)(mScrrenHeight / 2) - (float)positionY;
 //	bottom = top - (float)mBitmapHeight;
 //
 //	//创建临时的顶点数组  
 //	Vertex *vertexs;
 //	vertexs = new Vertex[mVertexCount];
 //	if (!vertexs)
 //	{
 //		return false;
 //	}
 //
 //	//加载临时顶点数据,这些是DX11坐标,即屏幕中心为原点  
 //	vertexs[0].pos = XMFLOAT3(left, top, 0.0f);
 //	vertexs[0].color = XMFLOAT2(0.0f, 0.0f);
 //
 //	vertexs[1].pos = XMFLOAT3(right, bottom, 0.0f);
 //	vertexs[1].color = XMFLOAT2(1.0f, 1.0f);
 //
 //	vertexs[2].pos = XMFLOAT3(left, bottom, 0.0f);
 //	vertexs[2].color = XMFLOAT2(0.0f, 1.0f);
 //
 //	vertexs[3].pos = XMFLOAT3(left, top, 0.0f);
 //	vertexs[3].color = XMFLOAT2(0.0f, 0.0f);
 //
 //	vertexs[4].pos = XMFLOAT3(right, top, 0.0f);
 //	vertexs[4].color = XMFLOAT2(1.0f, 0.0f);
 //
 //	vertexs[5].pos = XMFLOAT3(right, bottom, 0.0f);
 //	vertexs[5].color = XMFLOAT2(1.0f, 1.0f);
 //
 //	//锁定顶点缓存为了可以进行写入（动态缓存不能用UpdateSubResources写入）  
 //	D3D11_MAPPED_SUBRESOURCE mappedResource;
 //	HR(d3dDeviceContext->Map(md3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
 //
 //	//获取指向顶点缓存的指针  
 //	Vertex* verticesPtr;
 //	verticesPtr = (Vertex*)mappedResource.pData;
 //
 //	//把数据复制进顶点缓存  
 //	memcpy(verticesPtr, (void*)vertexs, (sizeof(Vertex) * mVertexCount));
 //
 //	//解锁顶点缓存  
 //	d3dDeviceContext->Unmap(md3dVertexBuffer, 0);
 //
 //	//释放顶点数组  
 //	delete vertexs;
 //	vertexs = NULL;
 //	return true;
 //}
 /*
 Stride=12 IndexCount=639 草？？
 Stride=12 IndexCount=1431 大地
 Stride=12 IndexCount=1464 树
 Stride=12 IndexCount=2199 赛季木板
 Stride=12 IndexCount=2448 树干
 Stride=12 IndexCount=2838 胳臂
 Stride=12 IndexCount=2877 头发
 Stride=12 IndexCount=3132 上衣
 Stride=12 IndexCount=14136 汽车
 */

 /*
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=1791 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=1932 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=2556 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=3228 StartIndexLocation=0 BaseVertexLocation=-543
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=3234 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=1128 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=5124 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=975 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=5124 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=969 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=3234 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=1128 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=1932 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=2556 StartIndexLocation=0 BaseVertexLocation=0
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=3228 StartIndexLocation=0 BaseVertexLocation=-543
 [3404] hkD3D11DrawIndexed**************Stride=24 IndexCount=948 StartIndexLocation=0 BaseVertexLocation=0
 */
 int itm = 0;

 int iStride = 12;
 int iC24a = 1128;
 int iC24b = 1932;
 int iC24c = 2556;
 int iC24d = 3228;
 int iC24e = 3234;
 int bRed = true;
 int iRed = 0;
 DWORD gggg = 0;
 DWORD cover = 0;
 bool bShoot = false;

 void __stdcall CheatIt(ID3D11DeviceContext* pContext, UINT IndexCount, UINT IndexCountPerInstance, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
 {
	 if (ppDepthStencilState__New != NULL)
	 {
		 ppDepthStencilState__New->Release();
		 ppDepthStencilState__New = NULL;
	 }

	 if (GetAsyncKeyState(VK_RIGHT) & 1)
	 {
		 iRed = iRed++ % 3;;
	 }

	 if (GetAsyncKeyState('Q') & 1)
	 {
		 //bShoot = !bShoot;
	 }

	 if (iRed == 1)
	 {
		 bRed = false;
	 }
	 else if (iRed == 2)
	 {
		 if (gggg != timeGetTime() / INTVL)
		 {
			 bRed = !bRed;
			 gggg = timeGetTime() / INTVL;
		 }
	 }
	 else
		 bRed = true;

	 if (cover != timeGetTime() / INTVL)
	 {
		// bFlashIt = !bFlashIt;
		 cover = timeGetTime() / INTVL;
	 }

	 SYSTEMTIME st = { 0 };
	 GetLocalTime(&st);

	 UINT Stride;
	 ID3D11Buffer *veBuffer;
	 UINT veBufferOffset = 0;
	 pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	 MyTraceA("hkD3D11DrawIndexed**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);

	 if ((Stride == 12) && (IndexCount > 7000) && (IndexCount < 10000))
	 {

		 MyTraceA("hkD3D11DrawIndexedBIGBIG**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);
		 //		if (!bRed)
		 //			return;
	 }


	 //if (/*(Stride == iStride  && IndexCount == 2514)
	 //	|| (Stride == iStride  && IndexCount == 2682)
	 //	|| (Stride == iStride  && IndexCount == 2514)
	 //	|| (Stride == iStride  && IndexCount == 2838)
	 //	|| (Stride == iStride  && IndexCount == 2877)
	 //	|| (Stride == iStride  && IndexCount == 3102)
	 //	|| (Stride == iStride  && IndexCount == 3132)
	 //	|| (Stride == iStride  && IndexCount == 3162)
	 //	|| (Stride == iStride  && IndexCount == 3084)
	 //	|| (Stride == iStride  && IndexCount == 14136)
	 //	//|| (Stride == iStride && (IndexCount > 2500) && (IndexCount < 3163))
	 //	|| */(Stride == 24))
	 //if ((Stride != 8) && (Stride != 12))
	 //if (
	 //	((Stride == 24) && (IndexCount == iC24a) ) ||
	 //	((Stride == 24) && (IndexCount == iC24b) ) ||
	 //	((Stride == 24) && (IndexCount == iC24c) ) ||
	 //	((Stride == 24) && (IndexCount == iC24d) ) ||
	 //	((Stride == 24) && (IndexCount == iC24e) ) 
	 //	)
	 //if (Stride == iStride)
	 //	return;
	 /*	if (aaa.size() == 0)
	 {
	 return;
	 }

	 for (int i=0; i<aaa.size(); i++)
	 {
	 if (!(Stride == aaa[i] && IndexCount == bbb[i]))
	 {
	 aaa.push_back(Stride);
	 bbb.push_back(IndexCount);
	 break;
	 }
	 }
	 if (Stride == aaa[iiidx] && IndexCount == bbb[iiidx])
	 {
	 return;
	 */
	 if ((Stride == 24))
	 {
		 if (find(lst24.begin(), lst24.end(), IndexCount) != lst24.end()) {
			 //找到
		 }
		 else {
			 //没找到
			 lst24.push_back(IndexCount);
		 }
	 }

	 if (
		 (
		 ((Stride == 24) /*|| (Stride == 12)*/)
			 && bRed
			 //&& bUp

			 && (IndexCount > 200)
			 && (IndexCount != 69)
			 && (IndexCount != 96)
			 && (IndexCount != 192)
			 && (IndexCount != 876)

			 && (IndexCount != 1128)
			 && (IndexCount != 1728)
			 && (IndexCount != 1842)
			 && (IndexCount != 1932)
			 && (IndexCount != 2556)
			 && (IndexCount != 3228)
			 && (IndexCount != 3234)
			 && (IndexCount != 5124)
			 )
		 || (Stride == 12 && IndexCount == 2637) // 三级盔 近处
		 || (Stride == 12 && IndexCount == 1116) // 2级盔 近处
		 || (Stride == 12 && IndexCount == 816) // ？盔 远处
												//|| (Stride == 12 && IndexCount == 192) // ？盔 远处
												//|| (Stride == 12 && IndexCount == 156) // ？盔 远处
												//|| (Stride == 12 && IndexCount == 180) // ？盔 远处
												//|| (Stride == 12 && IndexCount == 276) // ？盔 远处
												//|| (Stride == 12 && IndexCount == 294) // ？盔 远处
												//(
												////(Stride == 12 && IndexCount > 3800) ||
												//	(Stride == 12 && IndexCount == 2838) ||// 胳臂
												//	(Stride == 12 && IndexCount == 2877) ||// 头发
												//	(Stride == 12 && IndexCount == 3132) ||// 上衣
												//	(Stride == 12 && IndexCount == 14136) // 汽车
												//	)
		 )
	 {
		 //if (
		 //	IndexCount == lst24[iPos]
		 //	//&& (IndexCount != iC24a)
		 //	//&& (IndexCount != iC24b)
		 //	//&& (IndexCount != iC24c)
		 //	//&& (IndexCount != iC24d)
		 //	//&& (IndexCount != iC24e)
		 //	//&& (IndexCount != 5124)
		 //	//&& (IndexCount != 1791)
		 //	//&& (IndexCount != 975)
		 //	//&& (IndexCount != 969)
		 //	//&& (IndexCount != 948)
		 //	//&& (IndexCount < 1000)
		 //	//&& (IndexCount < 2000)
		 //	|| (find(red24.begin(), red24.end(), IndexCount) != red24.end())
		 //	)
		 //{
			 MyTraceA("hkD3D11 DrawIndexed**********透明了****iPos=%d  Stride=%d  IndexCount=%d red24.size=%d", iPos, Stride, IndexCount, red24.size());

			 //return;
			 //if (ppDepthStencilState__Old == NULL)
			 {
				 pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
			 }

			 if (bFlashIt)
			 {
				 //AutoShootIfCenter();
				 //SetEvent(g_Event_Shoot);

				 //pContext->PSSetShader(psYellow, NULL, NULL);
				 //ppDepthStencilState->GetDesc(&depthStencilDesc);

				 // Create the depth stencil state.
				 //if (ppDepthStencilStateNew == NULL)
				 {
					 D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
					 ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
					 
					  //depthStencilDesc.DepthEnable = TRUE;
					 //depthStencilDesc.DepthEnable = FALSE;
					 //depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
					 depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
					 //depthStencilDesc.StencilEnable = FALSE;
					 ID3D11Device *ppDevice;
					 pContext->GetDevice(&ppDevice);
					 ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
					 pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
					 pContext->PSSetShader(psTmp, NULL, NULL);
				 }
				 //// Set the depth stencil state.
				 //pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
				 //pContext->PSSetShader(psBlue, NULL, NULL);
				 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
				 //Hooks::oDrawIndexedInstanced(pContext, IndexCount, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);

				 //{
					// D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
					// ppDepthStencilState__New->GetDesc(&depthStencilDesc);

					// //depthStencilDesc.DepthEnable = TRUE;
					// //depthStencilDesc.DepthEnable = FALSE;
					// //depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
					// depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
					// //depthStencilDesc.StencilEnable = FALSE;
					// ID3D11Device *ppDevice;
					// pContext->GetDevice(&ppDevice);
					// ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
					// pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
					// pContext->PSSetShader(psRed, NULL, NULL);
				 //}
				 //// Set the depth stencil state.
				 //pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
				 //pContext->PSSetShader(psBlue, NULL, NULL);
				 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
				 //Hooks::oDrawIndexedInstanced(pContext, IndexCount, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			 }
			 else
				 pContext->PSSetShader(psRed, NULL, NULL);
			 //{
			 //	{
			 //		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			 //		ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
			 //		//depthStencilDesc.DepthEnable = 0;
			 //		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			 //		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			 //		ID3D11Device *ppDevice;
			 //		pContext->GetDevice(&ppDevice);
			 //		ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
			 //	}
			 //	//// Set the depth stencil state.
			 //	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
			 //}
			 //if (Stride == 12)
				// pContext->PSSetShader(psBlue, NULL, NULL);
			 //if (Stride == 24)
				// pContext->PSSetShader(psRed, NULL, NULL);
			 //if (
			 //	(IndexCount == 1128)
			 //	|| (IndexCount == 1728)
			 //	|| (IndexCount == 1842)
			 //	|| (IndexCount == 1932)
			 //	|| (IndexCount == 2556)
			 //	|| (IndexCount == 3228)
			 //	|| (IndexCount == 3234)
			 //	|| (IndexCount == 5124)
			 //	)
			 //{
			 //	pContext->PSSetShader(psc, NULL, NULL);
			 //}
			 //else
			 //{
				// //pContext->PSSetShader(psG, NULL, NULL);
			 //}
			 //&& (IndexCount != 1128)
			 //&& (IndexCount != 1728)
			 //&& (IndexCount != 1842)
			 //&& (IndexCount != 1932)
			 //&& (IndexCount != 2556)
			 //&& (IndexCount != 3228)
			 //&& (IndexCount != 3234)
			 //&& (IndexCount != 5124)

			 //pContext->OMGetDepthStencilState(&ppDepthStencilState, &pStencilRef);
			 //ppDepthStencilState->GetDesc(&depthStencilDesc);

			 //if (IndexCount == iC24a)
			 //{
			 //	//pContext->PSSetShader(psCrimson, NULL, NULL);
			 //}
			 //else if (IndexCount == iC24b)
			 //{
			 //	//pContext->PSSetShader(psYellow, NULL, NULL);
			 //}
			 //else if (IndexCount == iC24c)
			 //{
			 //	//pContext->PSSetShader(psc, NULL, NULL);
			 //}
			 //else if (IndexCount == iC24d)
			 //{
			 //	//pContext->PSSetShader(psd, NULL, NULL);
			 //}
			 //else if (IndexCount == iC24e)
			 //{
			 //	//pContext->PSSetShader(pse, NULL, NULL); //MY HEAD
			 //}
			 //else
			 //{
				// //pContext->PSSetShader(psG, NULL, NULL); //ENEMY
			 //}
			 //pContext->PSSetShaderResources(0, 1, &ShaderResourceView);

			 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
			 //pContext->OMSetDepthStencilState(myDepthStencilStates[ENABLED], 1);
			 // Set the depth stencil state.
			 //pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
			 //ppDepthStencilStateNew->Release();
			 //AddModel(pContext);//w2s

			 return;

			 //SetDepthStencilState(ENABLED);
			 //UINT numViewports = 1; 
			 //D3D11_VIEWPORT mViewport;
			 //pContext->RSGetViewports(&numViewports, &mViewport);
			 //mViewport.MinDepth = 0.0f;
			 //mViewport.MaxDepth = 0.1f;
			 //pContext->RSSetViewports(1, &mViewport);
			 // Create the viewport.
			 //m_deviceContext->RSSetViewports(1, &viewport);
		 //}
	 }
	 else if ((Stride == 12) && IndexCount<15000)
	 {
		 //{
			// pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
		 //}

		 ////if (bFlashIt)
		 //{
			// //AutoShootIfCenter();
			// //SetEvent(g_Event_Shoot);

			// //pContext->PSSetShader(psYellow, NULL, NULL);
			// //ppDepthStencilState->GetDesc(&depthStencilDesc);

			// // Create the depth stencil state.
			// //if (ppDepthStencilStateNew == NULL)
			// {
			//	 D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			//	 ppDepthStencilState__Old->GetDesc(&depthStencilDesc);

			//	 depthStencilDesc.DepthEnable = TRUE;
			//	 depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			//	 depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;

			//	 //depthStencilDesc.StencilEnable = TRUE;

			//	 ID3D11Device *ppDevice;
			//	 pContext->GetDevice(&ppDevice);
			//	 ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
			// }
			// //// Set the depth stencil state.
			// pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);

		 //}
		 //else
		 //{
		 //	{
		 //		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		 //		ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
		 //		//depthStencilDesc.DepthEnable = 0;
		 //		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		 //		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		 //		ID3D11Device *ppDevice;
		 //		pContext->GetDevice(&ppDevice);
		 //		ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
		 //	}
		 //	//// Set the depth stencil state.
		 //	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
		 //}
		 //if (Stride == 12)
			// pContext->PSSetShader(psBlue, NULL, NULL);
		 //if (Stride == 24)
			// pContext->PSSetShader(psRed, NULL, NULL);
	 }
 }
 void InitForHook(IDXGISwapChain* pSwapChain)
 {
	 Helpers::Log("DLL InitForHook。。。");

	 if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&CCheat::pDevice)))
	 {
		 pSwapChain->GetDevice(__uuidof(CCheat::pDevice), (void**)&CCheat::pDevice);
		 CCheat::pDevice->GetImmediateContext(&CCheat::pContext);
	 }

	 D3D11_DEPTH_STENCIL_DESC  stencilDesc;
	 stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	 stencilDesc.StencilEnable = true;
	 stencilDesc.StencilReadMask = 0xFF;
	 stencilDesc.StencilWriteMask = 0xFF;
	 stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	 stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	 stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	 stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	 stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	 stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	 stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	 stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	 stencilDesc.DepthEnable = true;
	 stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	 CCheat::pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);

	 stencilDesc.DepthEnable = false;
	 stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	 CCheat::pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);

	 stencilDesc.DepthEnable = false;
	 stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	 stencilDesc.StencilEnable = false;
	 stencilDesc.StencilReadMask = UINT8(0xFF);
	 stencilDesc.StencilWriteMask = 0x0;
	 CCheat::pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

	 stencilDesc.DepthEnable = true;
	 stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	 stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	 stencilDesc.StencilEnable = false;
	 stencilDesc.StencilReadMask = UINT8(0xFF);
	 stencilDesc.StencilWriteMask = 0x0;

	 stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	 stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	 stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	 stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	 stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	 stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	 stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	 stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
	 CCheat::pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);

	 D3D11_RASTERIZER_DESC rwDesc;
	 CCheat::pContext->RSGetState(&rwState);
	 rwState->GetDesc(&rwDesc);
	 rwDesc.FillMode = D3D11_FILL_WIREFRAME;
	 rwDesc.CullMode = D3D11_CULL_NONE;
	 CCheat::pDevice->CreateRasterizerState(&rwDesc, &rwState);

	 D3D11_RASTERIZER_DESC rsDesc;
	 CCheat::pContext->RSGetState(&rsState);
	 rsState->GetDesc(&rsDesc);
	 rsDesc.FillMode = D3D11_FILL_SOLID;
	 rsDesc.CullMode = D3D11_CULL_NONE;
	 CCheat::pDevice->CreateRasterizerState(&rsDesc, &rsState);
 }

HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
{
	char szCast[] = "struct VS_OUT"
		"{"
		" float4 Position : SV_Position;"
		" float4 Color : COLOR;"
		"};"

		"float4 ColorPixelShader( VS_OUT input ) : SV_Target"
		"{"
		" float4 fake;"
		" fake.a = 1.0f;"
		" fake.r = %f;"
		" fake.g = %f;"
		" fake.b = %f;"
		" return fake;"
		"}";
	ID3D10Blob* pBlob = NULL;
	char szPixelShader[1000];

	sprintf_s(szPixelShader, szCast, r, g, b);

	ID3DBlob* d3dErrorMsgBlob = NULL;

	HRESULT	hr = D3DCompile(szPixelShader,
			sizeof(szPixelShader),
			"shader",
			NULL, NULL,
			"ColorPixelShader",
			"ps_5_0",
			NULL, NULL,
			&pBlob, &d3dErrorMsgBlob);

	if (FAILED(hr))
	{
		Helpers::LogAddress("D3DCompile hr = ", hr);
		MyTraceA("D3DCompile hr = %x d3dErrorMsgBlob = %x", hr, d3dErrorMsgBlob);
		// 如果ps编译失败，输出错误信息.
		if (d3dErrorMsgBlob)
		{
			Helpers::Log("D3DCompile error 。。。");
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			Helpers::Log("Missing Shader File?????????????????????????????");
		}

		MyTraceA("D3DCompile error 111");

		//_beginthread(Thread_ExitHook, 0, NULL);
		//Sleep(100);
		return hr;
	}
	/*
	ID3D10Blob* errorMessage;
	//ID3D10Blob* vertexShaderBuffer;
	//ID3D10Blob* pixelShaderBuffer;
	// 编译ps.
	HRESULT hr = D3DX11CompileFromFileA("Color.ps", 
		NULL, NULL, 
		"ColorPixelShader", 
		"ps_5_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&pBlob, &errorMessage, NULL);
	if (FAILED(hr))
	{
			MessageBoxA(NULL, "Color errorMessage", "errorMessage", MB_OK);
		WinExec("cmd /K CD ", SW_SHOW);
		// 如果ps编译失败，输出错误信息.
		if (errorMessage)
		{
		}
		// 如果没有任何错误消息，可能是shader文件丢失.
		else
		{
			MessageBoxA(NULL, "Color.ps", "Missing Shader File", MB_OK);
		}

		return false;
	}
	*/

	hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);

	if (FAILED(hr))
	{
		Helpers::Log("D3DCompile error CreatePixelShader error 222。。。");
		MyTraceA("D3DCompile CreatePixelShader error 222");
		return hr;
	}

	Helpers::Log("Done GenerateShader。。。");
	return S_OK;
}
/*
D3D11_ERROR_FILE_NOT_FOUND	The file was not found.
D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS	There are too many unique instances of a particular type of state object.
D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS	There are too many unique instances of a particular type of view object.
D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD	The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.
D3DERR_INVALIDCALL(replaced with DXGI_ERROR_INVALID_CALL)	The method call is invalid.For example, a method's parameter may not be a valid pointer.
D3DERR_WASSTILLDRAWING(replaced with DXGI_ERROR_WAS_STILL_DRAWING)	The previous blit operation that is transferring information to or from this surface is incomplete.
E_FAIL	Attempted to create a device with the debug layer enabled and the layer is not installed.
E_INVALIDARG	An invalid parameter was passed to the returning function.
E_OUTOFMEMORY	Direct3D could not allocate sufficient memory to complete the call.
E_NOTIMPL	The method call isn't implemented with the passed parameter combination.
S_FALSE	Alternate success value, indicating a successful but nonstandard completion(the precise meaning depends on context).
S_OK*/
//ID3D11RasterizerState * rwState;
//ID3D11RasterizerState * rsState;
//
//enum eDepthState
//{
//	ENABLED,
//	DISABLED,
//	READ_NO_WRITE,
//	NO_READ_NO_WRITE,
//	_DEPTH_COUNT
//};

//ID3D11DepthStencilState* myDepthStencilStates[static_cast<int>(eDepthState::_DEPTH_COUNT)];

//void SetDepthStencilState(eDepthState aState)
//{
//	CCheat::pContext->OMSetDepthStencilState(myDepthStencilStates[aState], 1);
//}

#define SHOOT_AREA  5
static BOOL bDoneOnShoot = false;

//HBITMAP AutoShootIfCenter(BOOL bSave = false)
void AutoShootIfCenter(PVOID param)
//lpRect 代表选定区域
{
	//return;
	::GetWindowRect(g_hWnd, &g_lpRect);

	RECT lpRect;
	int iW = g_lpRect.right - g_lpRect.left;
	int iH = g_lpRect.bottom - g_lpRect.top;
	int iCenterX = iW / 2 + g_lpRect.left;
	int iCenterY = iH /2 + g_lpRect.top;

	lpRect.top = iCenterY - SHOOT_AREA;
	lpRect.bottom = iCenterY + SHOOT_AREA;
	lpRect.left = iCenterX - SHOOT_AREA;
	lpRect.right = iCenterX + SHOOT_AREA;

	HDC       hScrDC, hMemDC;
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;
	// 位图句柄
	int       nX, nY, nX2, nY2;
	// 选定区域坐标
	int       nWidth, nHeight;

	// 确保选定区域不为空矩形
	if (IsRectEmpty(&lpRect))
		return /*NULL*/;
	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);


	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect.left;
	nY = lpRect.top;
	nX2 = lpRect.right;
	nY2 = lpRect.bottom;


	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	//if (nX2 > m_xScreen)
	//	nX2 = m_xScreen;
	//if (nY2 > m_yScreen)
	//	nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	//hBitmap = CreateCompatibleBitmap
	//(hScrDC, nWidth, nHeight);

	// 初始化BITMAPINFO信息，以便使用CreateDIBSection
	BITMAPINFO RGB32BitsBITMAPINFO;
	ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
	RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	RGB32BitsBITMAPINFO.bmiHeader.biWidth = nWidth;
	RGB32BitsBITMAPINFO.bmiHeader.biHeight = nHeight;
	RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
	RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;
	UINT * ptPixels;

	HBITMAP DirectBitmap = CreateDIBSection(hMemDC,
		(BITMAPINFO *)&RGB32BitsBITMAPINFO,
		DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);


	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, DirectBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	//if (bSave)
	//{
	//	CDC dcCompatible;
	//	dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
	//	dcCompatible.SelectObject(m_pBitmap);

	//	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
	//		dcCompatible, nX, nY, SRCCOPY);
	//}
	//else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}
	
	// 转换 COLORREF 为 RGB  
	//cOldColor = COLORREF2RGB(cOldColor);
	//cNewColor = COLORREF2RGB(cNewColor);
	// 替换颜色  
	for (int i = ((nWidth * nHeight) - 1); i >= 0; i--)
	{
		if (!ptPixels)
		{
			break;
		}
		//ptPixels[i]; //0xff 29 27 21 红绿蓝
		//if (ptPixels[i] == 0xff800000)

		if( /*(ptPixels[i] == 0xff000080) 
			||*/(ptPixels[i] == 0xff800000)
			)
		{
			//MyTraceA("+-+-+-+-%x 射击射击射击", ptPixels[i]);
			//::OutputDebugStringA("+-+-+-+-瞄准瞄准瞄准瞄准");
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(10);
			//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			Sleep(10);
			//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(10);
			bDoneOnShoot = false;
			break;
		}
		//	ptPixels[i] = cNewColor;
		bDoneOnShoot = true;
	}

	if (bDoneOnShoot)
	{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	}
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	if (0/*bSave*/)
	{

		if (OpenClipboard(NULL))
		{
			//清空剪贴板
			EmptyClipboard();
			//把屏幕内容粘贴到剪贴板上,
			//hBitmap 为刚才的屏幕位图句柄
			SetClipboardData(CF_BITMAP, hBitmap);
			//关闭剪贴板
			CloseClipboard();
		}
	}
	return /*hBitmap*/;
}
HRESULT __stdcall Hooks::hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	//Helpers::LogAddress("\r\n hkD3D11Present++++++++++++++++++++*===");
	bFlashIt = !bFlashIt;
	MyTraceA("hkD3D11Present+++++++----------------------------------------------------------------- bUp = %d", bFlashIt);
	if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		iPos++;
		if (iPos >= lst24.size())
		{
			iPos = 0;
		}		
	}

	if (GetAsyncKeyState(VK_ADD) & 1)
	{
		bShoot = !bShoot;
		//red24.push_back(lst24[iPos]);
	}

	if (bShoot)
	{
		SetEvent(g_Event_Shoot);
	}

	if (GetAsyncKeyState(VK_DELETE) & 1)
	{
		ppDepthStencilState__Old = NULL;
		ppDepthStencilState__New = NULL;
	}

	if (GetAsyncKeyState(VK_RETURN) & 1)
	{
		red24.clear();
	}

	if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		ofstream output(".\\1111.txt", ios::binary);
		for (int i=0;i<red24.size();i++)
		{
			output.write((char*)(&(red24[i])), sizeof((red24[i]))); //写入
			output.write("\r\n", 2); //写入
		}
		//int i2 = 0;
		//ifstream input("./1.txt", ios::binary);
		//input.read((char*)(&i2), sizeof(i2)); //读取
	}

	//RECT lpRect;
	//int iW = g_lpRect.right - g_lpRect.left;
	//int iH = g_lpRect.bottom - g_lpRect.top;
	//int iCenterX = iW / 2 + g_lpRect.left;
	//int iCenterY = iH /2 + g_lpRect.top;

	//lpRect.top = iCenterY - 20;
	//lpRect.bottom = iCenterY + 20;
	//lpRect.left = iCenterX - 20;
	//lpRect.right = iCenterX + 20;
	static bool bOnce = false;
	//MyTraceA("hkD3D11Present+++++++------------------------------------------------------------------= key=0X%x", ((GetAsyncKeyState(VK_RETURN) & 1)));

	//if (GetAsyncKeyState(VK_RETURN) & 1)
	//{
	//	CopyScreenToBitmap(&lpRect, true);
	//	
	//		if (iiidx == aaa.size()-1)
	//		{
	//			iiidx = 0;
	//		}
	//		iiidx++;
	//}

	if (!bOnce)
	{
		InitForHook(pSwapChain);

		aaa.push_back(12);
		bbb.push_back(702);
		aaa.push_back(12);
		bbb.push_back(2826);
		aaa.push_back(12);
		bbb.push_back(285);
		aaa.push_back(12);
		bbb.push_back(2001);
		aaa.push_back(12);
		bbb.push_back(1926);
		aaa.push_back(12);
		bbb.push_back(2022);
		aaa.push_back(12);
		bbb.push_back(816);
		aaa.push_back(12);
		bbb.push_back(372);
		aaa.push_back(12);
		bbb.push_back(348);
		aaa.push_back(12);
		bbb.push_back(474);
		aaa.push_back(12);
		bbb.push_back(3306);
		aaa.push_back(12);
		bbb.push_back(4551);
		aaa.push_back(12);
		bbb.push_back(1242);
		aaa.push_back(12);
		bbb.push_back(1521);
		aaa.push_back(12);
		bbb.push_back(3510);
		aaa.push_back(12);
		bbb.push_back(3270);
		aaa.push_back(12);
		bbb.push_back(2751);
		aaa.push_back(12);
		bbb.push_back(2250);
		aaa.push_back(12);
		bbb.push_back(174);
		aaa.push_back(12);
		bbb.push_back(246);
		aaa.push_back(12);
		bbb.push_back(654);
		aaa.push_back(12);
		bbb.push_back(192);
		aaa.push_back(12);
		bbb.push_back(1377);
		aaa.push_back(12);
		bbb.push_back(318);
		aaa.push_back(12);
		bbb.push_back(57);
		aaa.push_back(12);
		bbb.push_back(36);
		aaa.push_back(12);
		bbb.push_back(33);
		aaa.push_back(12);
		bbb.push_back(42);
		aaa.push_back(12);
		bbb.push_back(1176);
		aaa.push_back(12);
		bbb.push_back(2160);
		aaa.push_back(12);
		bbb.push_back(95256);
		aaa.push_back(12);
		bbb.push_back(23814);
		aaa.push_back(12);
		bbb.push_back(5766);
		aaa.push_back(12);
		bbb.push_back(1350);
		aaa.push_back(12);
		bbb.push_back(294);
		aaa.push_back(12);
		bbb.push_back(60);
		aaa.push_back(12);
		bbb.push_back(1200);
		aaa.push_back(12);
		bbb.push_back(600);
		aaa.push_back(12);
		bbb.push_back(13593);
		aaa.push_back(12);
		bbb.push_back(6);
		aaa.push_back(16);
		bbb.push_back(6);
		aaa.push_back(16);
		bbb.push_back(108);
		aaa.push_back(16);
		bbb.push_back(54);
		aaa.push_back(16);
		bbb.push_back(12);
		aaa.push_back(16);
		bbb.push_back(18);
		aaa.push_back(16);
		bbb.push_back(30);
		aaa.push_back(16);
		bbb.push_back(156);
		aaa.push_back(16);
		bbb.push_back(200);
		aaa.push_back(16);
		bbb.push_back(66);
		aaa.push_back(24);
		bbb.push_back(5124);
		aaa.push_back(24);
		bbb.push_back(1842);
		aaa.push_back(24);
		bbb.push_back(3234);
		aaa.push_back(24);
		bbb.push_back(1128);
		aaa.push_back(24);
		bbb.push_back(1932);
		aaa.push_back(24);
		bbb.push_back(2556);
		aaa.push_back(24);
		bbb.push_back(3228);
		aaa.push_back(32);
		bbb.push_back(52560);
		aaa.push_back(40);
		bbb.push_back(12);
		aaa.push_back(40);
		bbb.push_back(6);
		aaa.push_back(40);
		bbb.push_back(18);
		aaa.push_back(40);
		bbb.push_back(24);
		aaa.push_back(8);
		bbb.push_back(6);

		Helpers::Log("D3D11Present initialised");


		bOnce = true;

	}

	//viewport
	CCheat::pContext->RSGetViewports(&vps, &viewport);
	ScreenCenterX = viewport.Width / 2.0f;
	ScreenCenterY = viewport.Height / 2.0f;
	if (!psCrimson)
		GenerateShader(CCheat::pDevice, &psCrimson, 1.f, 0.2f, 0.2f);

	if (!psYellow)
		GenerateShader(CCheat::pDevice, &psYellow, 1.f, 0.6f, 0);

	if (!psRed)
		GenerateShader(CCheat::pDevice, &psRed, 0.5f, 0.0f, 0.0f);
	if (!psGreen)
		GenerateShader(CCheat::pDevice, &psGreen, 0.0f, 0.5f, 0.0f);
	if (!psBlue)
		GenerateShader(CCheat::pDevice, &psBlue, 0.0f, 0.0f, 0.5f);
	if (!psTmp)
		GenerateShader(CCheat::pDevice, &psTmp, 0.0f, 0.3f, 0.3f);

	if (!psd)
		GenerateShader(CCheat::pDevice, &psd, 0.6f, 0.6f, 0);


	//call before you draw
	CCheat::pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	//draw
	//if (pFontWrapper)
	//pFontWrapper->DrawString(CCheat::pContext, L"Subliminal Fortnite Cheat", 14, 16.0f, 16.0f, 0xffff1612, FW1_RESTORESTATE);

	//pFontWrapper->DrawString(CCheat::pContext, L"Welcome Back Keidrich" , 14.0f, 16.0f, 30.0f, 0xffffffff, FW1_RESTORESTATE);


	//draw esp
	//if (AimEspInfo.size() != NULL)
	//{
	//	for (unsigned int i = 0; i < AimEspInfo.size(); i++)
	//	{
	//		//text esp
	//		if (AimEspInfo[i].vOutX > 1 && AimEspInfo[i].vOutY > 1 && AimEspInfo[i].vOutX < viewport.Width && AimEspInfo[i].vOutY < viewport.Height)
	//		{
	//			pFontWrapper->DrawString(CCheat::pContext, L"Enemy", 14, (int)AimEspInfo[i].vOutX, (int)AimEspInfo[i].vOutY, 0xFFFFFFFF, FW1_RESTORESTATE| FW1_NOGEOMETRYSHADER | FW1_CENTER | FW1_ALIASED);
	//		}
	//	}
	//}
	
	//aimbot
	//if (aimbot == 1 && AimEspInfo.size() != NULL && GetAsyncKeyState(Daimkey) & 0x8000)//warning: GetAsyncKeyState here will cause aimbot not to work for a few people
	if (aimbot == 1 && AimEspInfo.size() != NULL)
	{
		UINT BestTarget = -1;
		DOUBLE fClosestPos = 99999;

		for (unsigned int i = 0; i < AimEspInfo.size(); i++)
		{
			//aimfov
			float radiusx = (aimfov*10.0f) * (ScreenCenterX / 100.0f);
			float radiusy = (aimfov*10.0f) * (ScreenCenterY / 100.0f);

			if (aimfov == 0)
			{
				radiusx = 5.0f * (ScreenCenterX / 100.0f);
				radiusy = 5.0f * (ScreenCenterY / 100.0f);
			}

			//get crosshairdistance
			AimEspInfo[i].CrosshairDst = GetmDst(AimEspInfo[i].vOutX, AimEspInfo[i].vOutY, ScreenCenterX, ScreenCenterY);

			//aim at team 1 or 2 (not needed)
			//if (aimbot == AimEspInfo[i].iTeam)

			//if in fov
			if (AimEspInfo[i].vOutX >= ScreenCenterX - radiusx && AimEspInfo[i].vOutX <= ScreenCenterX + radiusx && AimEspInfo[i].vOutY >= ScreenCenterY - radiusy && AimEspInfo[i].vOutY <= ScreenCenterY + radiusy)

				//get closest/nearest target to crosshair
				if (AimEspInfo[i].CrosshairDst < fClosestPos)
				{
					fClosestPos = AimEspInfo[i].CrosshairDst;
					BestTarget = i;
				}
		}

		OutputDebugStringA("hkD3D11Present+++++++-BestTarget???");
		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			double DistX = AimEspInfo[BestTarget].vOutX - ScreenCenterX;
			double DistY = AimEspInfo[BestTarget].vOutY - ScreenCenterY;

			DistX /= (1 + aimsens);
			DistY /= (1 + aimsens);

			//aim
			mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);
			OutputDebugStringA("hkD3D11Present+++++++-BestTargetmouse_event MOUSEEVENTF_MOVE");

			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1))) //
			//if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			{
				if (autoshoot == 1 && !IsPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					OutputDebugStringA("hkD3D11Present+++++++-BestTargetmouse_event2 MOUSEEVENTF_LEFTDOWN");
					//LLeftClickDown();
					IsPressed = true;
				}
			}
		}
	}
	AimEspInfo.clear();

	//autoshoot off
	if (autoshoot == 1 && IsPressed)
	{
		if (timeGetTime() - astime >= asdelay)
		{
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			OutputDebugStringA("hkD3D11Present+++++++-BestTargetmouse_event2 MOUSEEVENTF_LEFTUP");
			IsPressed = false;
			astime = timeGetTime();
		}
	}
	return Hooks::oPresent(pSwapChain, SyncInterval, Flags);
}

void __stdcall Hooks::hkD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
{
	//Helpers::LogAddress("\r\n hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	OutputDebugStringA("hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	//works ok in ut4 alpha only
	//if (Stride == 40 || Stride == 44) //ut4 models
	//{
	//AddModel(pContext);//w2s
	//}
	//MyTraceA("hkD3D11VSSetConstantBuffers**************pContext=%x StartSlot=%d NumBuffers=%d ppConstantBuffers=%x ", pContext, StartSlot, NumBuffers, ppConstantBuffers);

	return Hooks::oVSSetConstantBuffers(pContext, StartSlot, NumBuffers, ppConstantBuffers);
}

void __stdcall Hooks::hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexed++++++++++++++++++++*===");
	CheatIt(pContext, IndexCount, 0, StartIndexLocation, BaseVertexLocation, 0);

	Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

	pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	return;

}
void __stdcall Hooks::hkD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery)
{
	//Helpers::LogAddress("\r\n hkD3D11CreateQuery++++++++++++++++++++*===");

	OutputDebugStringA("hkD3D11CreateQuery++++++++++++++++++++*===");
	// YOU GOT ANY MORE OF THAT OCCLUSION???+
	//if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
	//{
	//	D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
	//	(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
	//	(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;

	//	return Hooks::oCreateQuery(pDevice, &oqueryDesc, ppQuery);
	//}

	return Hooks::oCreateQuery(pDevice, pQueryDesc, ppQuery);
}

//==========================================================================================================================

void __stdcall Hooks::hkD3D11DrawInstanced(ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawInstanced++++++++++++++++++++*===");
	OutputDebugStringA("hkD3D11DrawInstanced++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawInstanced called");

	return oDrawInstanced(pContext, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

//==========================================================================================================================
bool bHideTrees = false;
DWORD ppppp = 0;
int ipp = 0;
void __stdcall Hooks::hkD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	//	OutputDebugStringA("hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	MyTraceA("hkD3D11DrawIndexedInstanced**************Stride=%d IndexCountPerInstance=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation);


	if (GetAsyncKeyState(VK_NUMPAD0) & 1)
	{
		ipp = ipp++ % 3;
	}
	//	Log("DrawIndexedInstanced called");

	//if (GetAsyncKeyState(VK_RETURN) & 1)

	if (ipp == 1)
	{
		bHideTrees = true;
	} 
	else if (ipp == 2)
	{
		if (ppppp != timeGetTime() / INTVL)
		{
			bHideTrees = !bHideTrees;
			ppppp = timeGetTime() / INTVL;
		}
	}
	else
		bHideTrees = false;

	CheatIt(pContext, IndexCountPerInstance, InstanceCount/**/, StartIndexLocation, BaseVertexLocation, StartInstanceLocation/**/);

	if (! (bHideTrees && Stride==12 && IndexCountPerInstance<15000))
		Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);

	//pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	return;
}

//==========================================================================================================================

void __stdcall Hooks::hkD3D11DrawInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawInstancedIndirect++++++++++++++++++++*===");
	OutputDebugStringA("hkD3D11DrawInstancedIndirect++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawInstancedIndirect called");

	//if (GetAsyncKeyState(VK_RETURN) & 1)
	//	bHideTrees = bHideTrees ? false : true;

	//if (!bHideTrees)
	{
		return oDrawInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
	}
}

//==========================================================================================================================

void __stdcall Hooks::hkD3D11DrawIndexedInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexedInstancedIndirect++++++++++++++++++++*===");
	OutputDebugStringA("hkD3D11DrawIndexedInstancedIndirect++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawIndexedInstancedIndirect called");
	
	return oDrawIndexedInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
}
