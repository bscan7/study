﻿
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>//generateshader
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")

#pragma comment(lib, "winmm.lib") //timeGetTime
#include "MinHook/include/MinHook.h" //detour x86&x64
#include "FW1FontWrapper/FW1FontWrapper.h" //font


typedef HRESULT(__stdcall *D3D11PresentHook) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__stdcall *D3D11DrawHook) (ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation);
typedef void(__stdcall *D3D11DrawIndexedHook) (ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
typedef void(__stdcall *D3D11DrawInstancedHook) (ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall *D3D11DrawIndexedInstancedHook) (ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);
typedef void(__stdcall *D3D11DrawInstancedIndirectHook) (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void(__stdcall *D3D11DrawIndexedInstancedIndirectHook) (ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs);
typedef void(__stdcall *D3D11PSSetShaderResourcesHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews);
typedef void(__stdcall *D3D11CreateQueryHook) (ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery);
typedef void(__stdcall *D3D11VSSetConstantBuffersHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
typedef void(__stdcall *D3D11PSSetSamplersHook) (ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers);


D3D11PresentHook phookD3D11Present = NULL;
D3D11DrawHook phookD3D11Draw = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11DrawInstancedHook phookD3D11DrawInstanced = NULL;
D3D11DrawIndexedInstancedHook phookD3D11DrawIndexedInstanced = NULL;
D3D11DrawInstancedIndirectHook phookD3D11DrawInstancedIndirect = NULL;
D3D11DrawIndexedInstancedIndirectHook phookD3D11DrawIndexedInstancedIndirect = NULL;
D3D11PSSetShaderResourcesHook phookD3D11PSSetShaderResources = NULL;
D3D11CreateQueryHook phookD3D11CreateQuery = NULL;
D3D11VSSetConstantBuffersHook phookD3D11VSSetConstantBuffers = NULL;
D3D11PSSetSamplersHook phookD3D11PSSetSamplers = NULL;

ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;

DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pContextVTable = NULL;
DWORD_PTR* pDeviceVTable = NULL;

IFW1Factory *pFW1Factory = NULL;
IFW1FontWrapper *pFontWrapper = NULL;

ID3D11DepthStencilState *ppDepthStencilStateNew = NULL;
ID3D11DepthStencilState *ppDepthStencilStateOld = NULL;
UINT pStencilRef = 0;
HWND g_hWnd = NULL;

#include "main.h"
//==========================================================================================================================
HWND _EnumChildWindows(HWND hParent, char* pCap)
{
	HWND hWndChild = GetWindow(hParent, GW_CHILD);
	HWND hWndTemp = GetWindow(hWndChild, GW_HWNDFIRST);
	do {
		char szWndInfo[512] = { 0 };
		char szWndTitle[256] = { 0 };
		char szClsName[64] = { 0 };

		GetWindowTextA(hWndTemp, szWndTitle, 256);
		GetClassNameA(hWndTemp, szClsName, 64);
		if (strstr(szWndTitle, pCap))
		{
			return hWndTemp;
		}
		//_stprintf(szWndInfo,
		//	_T("´°¿Ú%08X ")
		//	_T("\"%s\"")
		//	_T("%s ")
		//	_T("%s"),
		//	hWndTemp,
		//	szWndTitle,
		//	szClsName,
		//	IsWindowVisible(hWndTemp) ? _T("Visible") : _T("Invisible"));
		//SendMessage(hDisplay, LB_ADDSTRING, 0, (LPARAM)szWndInfo);
	} while (NULL != (hWndTemp = GetWindow(hWndTemp, GW_HWNDNEXT)));

	return NULL;
}
//==========================================================================================================================

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	OutputDebugStringA("hookD3D11Present+++++++------------------------------------------------------------------===");
	if (firstTime)
	{
		//get device and context
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		//create depthstencilstate
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
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::ENABLED)]);

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::DISABLED)]);

		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = false;
		stencilDesc.StencilReadMask = UINT8(0xFF);
		stencilDesc.StencilWriteMask = 0x0;
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

		stencilDesc.DepthEnable = true;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //
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
		pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilStates[static_cast<int>(eDepthState::READ_NO_WRITE)]);

		//wireframe
		D3D11_RASTERIZER_DESC rwDesc;
		pContext->RSGetState(&rwState); // retrieve the current state
		rwState->GetDesc(&rwDesc);    // get the desc of the state
		rwDesc.FillMode = D3D11_FILL_WIREFRAME;
		rwDesc.CullMode = D3D11_CULL_NONE;
		// create a whole new rasterizer state
		pDevice->CreateRasterizerState(&rwDesc, &rwState);

		//solid
		D3D11_RASTERIZER_DESC rsDesc;
		pContext->RSGetState(&rsState); // retrieve the current state
		rsState->GetDesc(&rsDesc);    // get the desc of the state
		rsDesc.FillMode = D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_NONE;
		// create a whole new rasterizer state
		pDevice->CreateRasterizerState(&rsDesc, &rsState);

		//create font
		HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
		hResult = pFW1Factory->CreateFontWrapper(pDevice, L"Tahoma", &pFontWrapper);
		pFW1Factory->Release();

		// use the back buffer address to create the render target
		//if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&RenderTargetTexture))))
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
		}

		firstTime = false;
	}

	//viewport
	pContext->RSGetViewports(&vps, &viewport);
	ScreenCenterX = viewport.Width / 2.0f;
	ScreenCenterY = viewport.Height / 2.0f;

	//if (pWorldViewCB != NULL && pProjCB != NULL)
	//{
		//TransformToScreenSpace(pContext);
	//}

	//shaders
	if (!psRed)
	GenerateShader(pDevice, &psRed, 1.0f, 0.0f, 0.0f);

	if (!psGreen)
	GenerateShader(pDevice, &psGreen, 0.0f, 1.0f, 0.0f);


	//call before you draw
	pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
	//draw
	if (pFontWrapper)
	pFontWrapper->DrawString(pContext, L"Who are youuuuuuuuuuuuuuu?", 14, 16.0f, 16.0f, 0xffff1612, FW1_RESTORESTATE);

	pFontWrapper->DrawString(pContext, L"Welcome Back Bscan*****============================" , 14.0f, 16.0f, 30.0f, 0xffffffff, FW1_RESTORESTATE);

	pFontWrapper->DrawString(pContext, L"[XXX]" , 14.0f, ScreenCenterX, ScreenCenterY, 0xff00ff00, FW1_RESTORESTATE);

	//draw esp
	if (AimEspInfo.size() != NULL)
	{
		for (unsigned int i = 0; i < AimEspInfo.size(); i++)
		{
			//text esp
			if (AimEspInfo[i].vOutX > 1 && AimEspInfo[i].vOutY > 1 && AimEspInfo[i].vOutX < viewport.Width && AimEspInfo[i].vOutY < viewport.Height)
			{
				pFontWrapper->DrawString(pContext, L"Enemy", 14, (int)AimEspInfo[i].vOutX, (int)AimEspInfo[i].vOutY, 0xFFFFFFFF, FW1_RESTORESTATE| FW1_NOGEOMETRYSHADER | FW1_CENTER | FW1_ALIASED);
			}
		}
	}
	
	//aimbot
	if (aimbot == 1 && AimEspInfo.size() != NULL && GetAsyncKeyState(Daimkey) & 0x8000)//warning: GetAsyncKeyState here will cause aimbot not to work for a few people
	//if (aimbot == 1 && AimEspInfo.size() != NULL)
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


		//if nearest target to crosshair
		if (BestTarget != -1)
		{
			double DistX = AimEspInfo[BestTarget].vOutX - ScreenCenterX;
			double DistY = AimEspInfo[BestTarget].vOutY - ScreenCenterY;

			DistX /= (1 + aimsens);
			DistY /= (1 + aimsens);

			//aim
			mouse_event(MOUSEEVENTF_MOVE, (float)DistX, (float)DistY, 0, NULL);

			//autoshoot on
			if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1))) //
			//if ((!GetAsyncKeyState(VK_LBUTTON) && (autoshoot == 1) && (GetAsyncKeyState(Daimkey) & 0x8000))) //
			{
				if (autoshoot == 1 && !IsPressed)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
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
			IsPressed = false;
			astime = timeGetTime();
		}
	}
	
	//logger
	/*if ((GetAsyncKeyState(VK_INSERT) & 1)) //ALT + CTRL + L toggles logger
		logger = !logger;
	if (logger && pFontWrapper) //&& countnum >= 0)
	{
		//bruteforce WorldViewCBnum
		if (GetAsyncKeyState('Z') & 1) //-
			WorldViewCBnum--;
		if (GetAsyncKeyState('U') & 1) //+
			WorldViewCBnum++;
		if (GetAsyncKeyState(0x37) & 1) //7 reset, set to 0
			WorldViewCBnum = 0;
		if (WorldViewCBnum < 0)
			WorldViewCBnum = 0;

		//bruteforce ProjCBnum
		if (GetAsyncKeyState('H') & 1) //-
			ProjCBnum--;
		if (GetAsyncKeyState('J') & 1) //+
			ProjCBnum++;
		if (GetAsyncKeyState(0x38) & 1) //8 reset, set to 0
			ProjCBnum = 0;
		if (ProjCBnum < 0)
			ProjCBnum = 0;

		//bruteforce matProjnum
		if (GetAsyncKeyState('N') & 1) //-
			matProjnum--;
		if (GetAsyncKeyState('M') & 1) //+
			matProjnum++;
		if (GetAsyncKeyState(0x39) & 1) //9 reset, set to 0
			matProjnum = 0;
		if (matProjnum < 0)
			matProjnum = 0;

		//call before you draw
		//pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);
		wchar_t reportValue[256];
		swprintf_s(reportValue, L"(Keys:-O P+ I=Log) countnum = %d", countnum);
		pFontWrapper->DrawString(pContext, reportValue, 20.0f, 220.0f, 100.0f, 0xffffffff, FW1_RESTORESTATE);

		wchar_t reportValueA[256];
		swprintf_s(reportValueA, L"(Keys:-Z U+) WorldViewCBnum = %d", WorldViewCBnum);
		pFontWrapper->DrawString(pContext, reportValueA, 20.0f, 220.0f, 120.0f, 0xffffffff, FW1_RESTORESTATE);

		wchar_t reportValueB[256];
		swprintf_s(reportValueB, L"(Keys:-H J+) ProjCBnum = %d", ProjCBnum);
		pFontWrapper->DrawString(pContext, reportValueB, 20.0f, 220.0f, 140.0f, 0xffffffff, FW1_RESTORESTATE);

		wchar_t reportValueC[256];
		swprintf_s(reportValueC, L"(Keys:-N M+) matProjnum = %d", matProjnum);
		pFontWrapper->DrawString(pContext, reportValueC, 20.0f, 220.0f, 160.0f, 0xffffffff, FW1_RESTORESTATE);

		//pFontWrapper->DrawString(pContext, L"hold P to +", 20.0f, 220.0f, 120.0f, 0xfff11111, FW1_RESTORESTATE);
		//pFontWrapper->DrawString(pContext, L"hold O to -", 20.0f, 220.0f, 140.0f, 0xfaf22222, FW1_RESTORESTATE);
		//pFontWrapper->DrawString(pContext, L"press I to log", 20.0f, 220.0f, 160.0f, 0xfff99999, FW1_RESTORESTATE);
		//pFontWrapper->DrawString(pContext, L"F9 to log drawfunc", 20.0f, 220.0f, 180.0f, 0xff00ff00, FW1_RESTORESTATE);
	}*/

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}

//==========================================================================================================================

void __stdcall hookD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	OutputDebugStringA("hookD3D11CreateQuery+++++++------------------------------------------------------------------===");
	//if (GetAsyncKeyState(VK_F9) & 1)
		//Log("DrawIndexed called");
	
	//get stride & vdesc.ByteWidth
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if (veBuffer)
		veBuffer->GetDesc(&vedesc);
	if (veBuffer != NULL){ veBuffer->Release(); veBuffer = NULL; }

	//get indesc.ByteWidth
	pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
	if (inBuffer)
		inBuffer->GetDesc(&indesc);
	if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }
	
	//wallhack/chams
	if (Stride == 0)
	//if(Stride == 40||Stride == 44)//ut4 models (incomplete)
	//if(Stride == 8 && IndexCount == 600)// && indesc.ByteWidth == 24912 && vedesc.ByteWidth == 24912)//quake yellow ball in tutorial (hybrid engine made up of id tech and saber tech)
	//if (Stride == 32 && Descr.Format == 49)//outlast (ut4 engine)
	//if (Stride == 32 && IndexCount != 6)//rising storm 2 (ut3 engine)
	{
		SetDepthStencilState(DISABLED);
		pContext->PSSetShader(psRed, NULL, NULL);
		phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		pContext->PSSetShader(psGreen, NULL, NULL);
		SetDepthStencilState(READ_NO_WRITE);
	}

	//w2s (Warning, the worse model rec, the more fps loss)
	if (Stride == 24)
	//if ((Stride == 40 || Stride == 44)&&(psStartSlot > 1))//ut4 models (incomplete)
	//if(Stride == 8 && IndexCount == 600)// && indesc.ByteWidth == 24912 && vedesc.ByteWidth == 24912)//quake yellow ball in tutorial (hybrid engine made up of id tech and saber tech)
	//if (Stride == 32 && Descr.Format == 49)//outlast (ut4 engine)
	//if (Stride == 32 && IndexCount != 6)//rising storm 2 (ut3 engine)
	{
		if (ppDepthStencilStateOld == NULL)
		{
			pContext->OMGetDepthStencilState(&ppDepthStencilStateOld, &pStencilRef);
		}

		//pContext->PSSetShader(psYellow, NULL, NULL);
		//ppDepthStencilState->GetDesc(&depthStencilDesc);

		// Create the depth stencil state.
		if (ppDepthStencilStateNew == NULL)
		{
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
			ppDepthStencilStateOld->GetDesc(&depthStencilDesc);
			//depthStencilDesc.DepthEnable = 0;
			//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			ID3D11Device *ppDevice;
			pContext->GetDevice(&ppDevice);
			ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilStateNew);
		}
		// Set the depth stencil state.
		pContext->OMSetDepthStencilState(ppDepthStencilStateNew, pStencilRef);

		//pContext->OMGetDepthStencilState(&ppDepthStencilState, &pStencilRef);
		//ppDepthStencilState->GetDesc(&depthStencilDesc);

		pContext->PSSetShader(psRed, NULL, NULL);
		//pContext->PSSetShaderResources(0, 1, &ShaderResourceView);

		phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		//pContext->OMSetDepthStencilState(myDepthStencilStates[ENABLED], 1);

		// Set the depth stencil state.
		pContext->OMSetDepthStencilState(ppDepthStencilStateOld, pStencilRef);

		//AddModel(pContext);//w2s
	}
	
	//small bruteforce logger
	//ALT + CTRL + L toggles logger
	if (logger)
	{
		if ((Stride == 40 || Stride == 44) && (GetAsyncKeyState(VK_F10) & 1))
			Log("Stride == %d && IndexCount == %d && indesc.ByteWidth == %d && vedesc.ByteWidth == %d && Descr.Format == %d && pscdesc.ByteWidth == %d", Stride, IndexCount, indesc.ByteWidth, vedesc.ByteWidth, Descr.Format, pscdesc.ByteWidth);

		//hold down P key until a texture is wallhacked, press I to log values of those textures
		if (GetAsyncKeyState('O') & 1) //-
			countnum--;
		if (GetAsyncKeyState('P') & 1) //+
			countnum++;
		if ((GetAsyncKeyState(VK_MENU)) && (GetAsyncKeyState('9') & 1)) //reset, set to -1
			countnum = -1;
		if (countnum == IndexCount / 100)
			if (GetAsyncKeyState('I') & 1)
				Log("Stride == %d && IndexCount == %d && indesc.ByteWidth == %d && vedesc.ByteWidth == %d && Descr.Format == %d && pscdesc.ByteWidth == %d", Stride, IndexCount, indesc.ByteWidth, vedesc.ByteWidth, Descr.Format, pscdesc.ByteWidth);//vsConstant_StartSlot
			
		if (countnum == IndexCount / 100)
		{
			SetDepthStencilState(DISABLED);
			//pContext->RSSetState(rwState);    //wireframe
			pContext->PSSetShader(psRed, NULL, NULL);
			phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
			SetDepthStencilState(READ_NO_WRITE);
			//pContext->RSSetState(rsState);    //solid
			pContext->PSSetShader(psGreen, NULL, NULL);
		}
	}
	
    return phookD3D11DrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
}

//==========================================================================================================================

//void __stdcall hookD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
//{
//	//works ok in ut4 alpha only
//	//if (Stride == 40 || Stride == 44) //ut4 models
//	//{
//	//AddModel(pContext);//w2s
//	//}
//
//	return phookD3D11VSSetConstantBuffers(pContext, StartSlot, NumBuffers, ppConstantBuffers);
//}

//==========================================================================================================================

void __stdcall hookD3D11PSSetSamplers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
{
	OutputDebugStringA("hookD3D11CreateQuery+++++++------------------------------------------------------------------===");
	if(ppSamplers!=NULL)
	psStartSlot = StartSlot;

	//if (GetAsyncKeyState(VK_F10) & 1)
	//Log("ppSamplers == %d && StartSlot == %x && NumSamplers == %d", ppSamplers, StartSlot, NumSamplers);
	//ppSamplers == 533852720 && StartSlot == 1,2,3,5,7 && NumSamplers == 1

	return phookD3D11PSSetSamplers(pContext, StartSlot, NumSamplers, ppSamplers);
}

//==========================================================================================================================

void __stdcall hookD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
	OutputDebugStringA("hookD3D11CreateQuery+++++++------------------------------------------------------------------===");
	pssrStartSlot = StartSlot;

	for (UINT j = 0; j < NumViews; j++)
	{
		//Resources loop
		ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[j];
		if (pShaderResView)
		{
			pShaderResView->GetDesc(&Descr);
			//Descr.Buffer.NumElements;
			//Descr.Format;

			if (
				(Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFER)
				||
				(Descr.ViewDimension == D3D11_SRV_DIMENSION_BUFFEREX)
				)
			{
				continue;//Skip buffer resources
			}

		}// if( pShaderResView )
	}// for

	 //get pscdesc.ByteWidth
	pContext->PSGetConstantBuffers(pscStartSlot, 1, &pcsBuffer);
	if (pcsBuffer)
		pcsBuffer->GetDesc(&pscdesc);
	if (pcsBuffer != NULL) { pcsBuffer->Release(); pcsBuffer = NULL; }

	/*
	//alternative wallhack example for f'up games
	if (pssrStartSlot == 1) //if black screen, find correct pssrStartSlot
		SetDepthStencilState(READ_NO_WRITE);
	if (pscdesc.ByteWidth == 224 && Descr.Format == 71) //models in Tom Clancys Rainbow Six Siege old version
	{
		SetDepthStencilState(DISABLED);
		//pContext->PSSetShader(psRed, NULL, NULL);
	}
	else if (pssrStartSlot == 1) //if black screen, find correct pssrStartSlot
	SetDepthStencilState(READ_NO_WRITE);
	*/
	return phookD3D11PSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);
}

//==========================================================================================================================

void __stdcall hookD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery)
{
	OutputDebugStringA("hookD3D11CreateQuery+++++++------------------------------------------------------------------===");
	//Disable Occlusion which prevents rendering player models through certain objects (used by wallhack to see models through walls at all distances, REDUCES FPS)
	if (pQueryDesc->Query == D3D11_QUERY_OCCLUSION)
	{
		D3D11_QUERY_DESC oqueryDesc = CD3D11_QUERY_DESC();
		(&oqueryDesc)->MiscFlags = pQueryDesc->MiscFlags;
		(&oqueryDesc)->Query = D3D11_QUERY_TIMESTAMP;

		return phookD3D11CreateQuery(pDevice, &oqueryDesc, ppQuery);
	}

	return phookD3D11CreateQuery(pDevice, pQueryDesc, ppQuery);
}

////==========================================================================================================================
//
//void __stdcall hookD3D11Draw(ID3D11DeviceContext* pContext, UINT VertexCount, UINT StartVertexLocation)
//{
//	if (GetAsyncKeyState(VK_F9) & 1)
//		Log("Draw called");
//
//	return phookD3D11Draw(pContext, VertexCount, StartVertexLocation);
//}
//
////==========================================================================================================================
//
//void __stdcall hookD3D11DrawInstanced(ID3D11DeviceContext* pContext, UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
//{
//	if (GetAsyncKeyState(VK_F9) & 1)
//		Log("DrawInstanced called");
//
//	return phookD3D11DrawInstanced(pContext, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
//}
//
////==========================================================================================================================
//
//void __stdcall hookD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
//{
//	if (GetAsyncKeyState(VK_F9) & 1)
//		Log("DrawIndexedInstanced called");
//
//	return phookD3D11DrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
//}
//
////==========================================================================================================================
//
//void __stdcall hookD3D11DrawInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
//{
//	if (GetAsyncKeyState(VK_F9) & 1)
//		Log("DrawInstancedIndirect called");
//
//	return phookD3D11DrawInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
//}
//
////==========================================================================================================================
//
//void __stdcall hookD3D11DrawIndexedInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
//{
//	if (GetAsyncKeyState(VK_F9) & 1)
//		Log("DrawIndexedInstancedIndirect called");
//	
//	return phookD3D11DrawIndexedInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
//}

//==========================================================================================================================

const int MultisampleCount = 1; // Set to 1 to disable multisampling
LRESULT CALLBACK DXGIMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){ return DefWindowProc(hwnd, uMsg, wParam, lParam); }
DWORD __stdcall InitializeHook(LPVOID)
{
	//Sleep(5000);

	HMODULE hDXGIDLL = 0;
	do
	{
		hDXGIDLL = GetModuleHandle("dxgi.dll");
		Sleep(200);
	} while (!hDXGIDLL);
	Sleep(100);
	MessageBoxA(NULL, "InitializeHook...!", "uBoos?", MB_ICONINFORMATION);

	//HMODULE hD3D11DLL = 0;
	//do
	//{
		//hD3D11DLL = GetModuleHandle("d3d11.dll");
		//Sleep(100);
	//} while (!hD3D11DLL);
	//Sleep(1000);

    IDXGISwapChain* pSwapChain;

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DXGIMsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	//HWND hWnd = CreateWindowA("DXbscan", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);


	//HWND hWnd = FindWindowA(NULL, "夜神模拟器");
	//if (!hWnd)
	//{
	//	MessageBoxA(NULL, "Not found HWND!", "uBoos?", MB_ICONINFORMATION);
	//	return NULL;
	//}
	//hWnd = _EnumChildWindows(hWnd, "ScreenBoardClassWindow");

	//HWND hWnd = (HWND)0x0031162C;
	g_hWnd = FindWindowA(NULL, "BlueStacks App Player");
	if (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "夜神模拟器");
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found HWND 夜神模拟器!", "uBoos?", MB_ICONINFORMATION);
			//return;
		}
		else
		{
			g_hWnd = _EnumChildWindows(g_hWnd, "ScreenBoardClassWindow");
		}
		//HWND hWnd = (HWND)0x0031162C;
	}
	if (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "雷电模拟器");
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found HWND 雷电模拟器!", "uBoos?", MB_ICONINFORMATION);
			//return;
		}
		else
		{
			g_hWnd = _EnumChildWindows(g_hWnd, "TheRender");
		}
	}
	if (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "绝地求生 刺激战场 - MuMu模拟器");
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
			//return;
		}
		else
		{
			g_hWnd = _EnumChildWindows(g_hWnd, "");
		}
	}
	if (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "绝地求生 全军出击 - MuMu模拟器");
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
			//return;
		}
		else
		{
			g_hWnd = _EnumChildWindows(g_hWnd, "");
		}
	}

	if (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "腾讯手游助手【极速傲引擎】");
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
			//return;
		}
		else
		{
			g_hWnd = _EnumChildWindows(g_hWnd, "AEngineRenderWindow");
		}
	}
	if (!g_hWnd)
	{
		MessageBoxA(NULL, "Not found child HWND!", "uBoos?", MB_ICONINFORMATION);
		return NULL;
	}

	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = g_hWnd;
	scd.SampleDesc.Count = MultisampleCount;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(g_hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	// LibOVR 0.4.3 requires that the width and height for the backbuffer is set even if
	// you use windowed mode, despite being optional according to the D3D11 documentation.
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;

	UINT createFlags = 0;
#ifdef _DEBUG
	// This flag gives you some quite wonderful debug text. Not wonderful for performance, though!
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	IDXGISwapChain* d3dSwapChain = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		requestedLevels,
		sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		&obtainedLevel,
		&pContext)))
	{
		MessageBox(g_hWnd, "Failed to create directX device and swapchain!", "Error", MB_ICONERROR);
		return NULL;
	}


    pSwapChainVtable = (DWORD_PTR*)pSwapChain;
    pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

    pContextVTable = (DWORD_PTR*)pContext;
    pContextVTable = (DWORD_PTR*)pContextVTable[0];

	pDeviceVTable = (DWORD_PTR*)pDevice;
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];

	if (MH_Initialize() != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[8], hookD3D11Present, reinterpret_cast<void**>(&phookD3D11Present)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) { return 1; }
	if (MH_CreateHook((DWORD_PTR*)pContextVTable[12], hookD3D11DrawIndexed, reinterpret_cast<void**>(&phookD3D11DrawIndexed)) != MH_OK) { return 1; }
	if (MH_EnableHook((DWORD_PTR*)pContextVTable[12]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pDeviceVTable[24], hookD3D11CreateQuery, reinterpret_cast<void**>(&phookD3D11CreateQuery)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pDeviceVTable[24]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[39], hookD3D11DrawIndexedInstancedIndirect, reinterpret_cast<void**>(&phookD3D11DrawIndexedInstancedIndirect)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[39]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[20], hookD3D11DrawIndexedInstanced, reinterpret_cast<void**>(&phookD3D11DrawIndexedInstanced)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[20]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[21], hookD3D11DrawInstanced, reinterpret_cast<void**>(&phookD3D11DrawInstanced)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[21]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[40], hookD3D11DrawInstancedIndirect, reinterpret_cast<void**>(&phookD3D11DrawInstancedIndirect)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[40]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[13], hookD3D11Draw, reinterpret_cast<void**>(&phookD3D11Draw)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[13]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[8], hookD3D11PSSetShaderResources, reinterpret_cast<void**>(&phookD3D11PSSetShaderResources)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[8]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[7], hookD3D11VSSetConstantBuffers, reinterpret_cast<void**>(&phookD3D11VSSetConstantBuffers)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[7]) != MH_OK) { return 1; }
	//if (MH_CreateHook((DWORD_PTR*)pContextVTable[10], hookD3D11PSSetSamplers, reinterpret_cast<void**>(&phookD3D11PSSetSamplers)) != MH_OK) { return 1; }
	//if (MH_EnableHook((DWORD_PTR*)pContextVTable[10]) != MH_OK) { return 1; }

    DWORD dwOld;
    VirtualProtect(phookD3D11Present, 2, PAGE_EXECUTE_READWRITE, &dwOld);

    DWORD dwOld2;
    VirtualProtect(phookD3D11DrawIndexed, 2, PAGE_EXECUTE_READWRITE, &dwOld2);
	while (true) {
		Sleep(10);
	}

	pDevice->Release();
	pContext->Release();
	pSwapChain->Release();

    return NULL;
}

//==========================================================================================================================

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{ 
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH: // A process is loading the DLL.
		DisableThreadLibraryCalls(hModule);
		GetModuleFileName(hModule, dlldir, 512);
		for (size_t i = strlen(dlldir); i > 0; i--) { if (dlldir[i] == '\\') { dlldir[i + 1] = 0; break; } }
		CreateThread(NULL, 0, InitializeHook, NULL, 0, NULL);
		break;

	case DLL_PROCESS_DETACH: // A process unloads the DLL.
		if (MH_Uninitialize() != MH_OK) { return 1; }
		if (MH_DisableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) { return 1; }
		if (MH_DisableHook((DWORD_PTR*)pContextVTable[12]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pDeviceVTable[24]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[39]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[20]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[21]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[40]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[13]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[8]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[7]) != MH_OK) { return 1; }
		//if (MH_DisableHook((DWORD_PTR*)pContextVTable[10]) != MH_OK) { return 1; }
		break;
	}
	return TRUE;
}

/*
dx11 vtable CONTEXT index
Index: 0 | QueryInterface
Index: 1 | Addref
Index: 2 | Release
Index: 3 | GetDevice
Index: 4 | GetPrivateData
Index: 5 | SetPrivateData
Index: 6 | SetPrivateDataInterface
Index: 7 | VSSetConstantBuffers
Index: 8 | PSSetShaderResources
Index: 9 | PSSetShader
Index: 10 | SetSamplers
Index: 11 | VSSetShader
Index: 12 | DrawIndexed
Index: 13 | Draw
Index: 14 | Map
Index: 15 | Unmap
Index: 16 | PSSetConstantBuffer
Index: 17 | IASetInputLayout
Index: 18 | IASetVertexBuffers
Index: 19 | IASetIndexBuffer
Index: 20 | DrawIndexedInstanced
Index: 21 | DrawInstanced
Index: 22 | GSSetConstantBuffers
Index: 23 | GSSetShader
Index: 24 | IASetPrimitiveTopology
Index: 25 | VSSetShaderResources
Index: 26 | VSSetSamplers
Index: 27 | Begin
Index: 28 | End
Index: 29 | GetData
Index: 30 | GSSetPredication
Index: 31 | GSSetShaderResources
Index: 32 | GSSetSamplers
Index: 33 | OMSetRenderTargets
Index: 34 | OMSetRenderTargetsAndUnorderedAccessViews
Index: 35 | OMSetBlendState
Index: 36 | OMSetDepthStencilState
Index: 37 | SOSetTargets
Index: 38 | DrawAuto
Index: 39 | DrawIndexedInstancedIndirect
Index: 40 | DrawInstancedIndirect
Index: 41 | Dispatch
Index: 42 | DispatchIndirect
Index: 43 | RSSetState
Index: 44 | RSSetViewports
Index: 45 | RSSetScissorRects
Index: 46 | CopySubresourceRegion
Index: 47 | CopyResource
Index: 48 | UpdateSubresource
Index: 49 | CopyStructureCount
Index: 50 | ClearRenderTargetView
Index: 51 | ClearUnorderedAccessViewUint
Index: 52 | ClearUnorderedAccessViewFloat
Index: 53 | ClearDepthStencilView
Index: 54 | GenerateMips
Index: 55 | SetResourceMinLOD
Index: 56 | GetResourceMinLOD
Index: 57 | ResolveSubresource
Index: 58 | ExecuteCommandList
Index: 59 | HSSetShaderResources
Index: 60 | HSSetShader
Index: 61 | HSSetSamplers
Index: 62 | HSSetConstantBuffers
Index: 63 | DSSetShaderResources
Index: 64 | DSSetShader
Index: 65 | DSSetSamplers
Index: 66 | DSSetConstantBuffers
Index: 67 | DSSetShaderResources
Index: 68 | CSSetUnorderedAccessViews
Index: 69 | CSSetShader
Index: 70 | CSSetSamplers
Index: 71 | CSSetConstantBuffers
Index: 72 | VSGetConstantBuffers
Index: 73 | PSGetShaderResources
Index: 74 | PSGetShader
Index: 75 | PSGetSamplers
Index: 76 | VSGetShader
Index: 77 | PSGetConstantBuffers
Index: 78 | IAGetInputLayout
Index: 79 | IAGetVertexBuffers
Index: 80 | IAGetIndexBuffer
Index: 81 | GSGetConstantBuffers
Index: 82 | GSGetShader
Index: 83 | IAGetPrimitiveTopology
Index: 84 | VSGetShaderResources
Index: 85 | VSGetSamplers
Index: 86 | GetPredication
Index: 87 | GSGetShaderResources
Index: 88 | GSGetSamplers
Index: 89 | OMGetRenderTargets
Index: 90 | OMGetRenderTargetsAndUnorderedAccessViews
Index: 91 | OMGetBlendState
Index: 92 | OMGetDepthStencilState
Index: 93 | SOGetTargets
Index: 94 | RSGetState
Index: 95 | RSGetViewports
Index: 96 | RSGetScissorRects
Index: 97 | HSGetShaderResources
Index: 98 | HSGetShader
Index: 99 | HSGetSamplers
Index: 100 | HSGetConstantBuffers
Index: 101 | DSGetShaderResources
Index: 102 | DSGetShader
Index: 103 | DSGetSamplers
Index: 104 | DSGetConstantBuffers
Index: 105 | CSGetShaderResources
Index: 106 | CSGetUnorderedAccessViews
Index: 107 | CSGetShader
Index: 108 | CSGetSamplers
Index: 109 | CSGetConstantBuffers
Index: 110 | ClearState
Index: 111 | Flush
Index: 112 | GetType
Index: 113 | GetContextFlags
Index: 114 | FinishCommandList


// ID3D11 DEVICE virtuals
*QueryInterface 						0
*AddRef 								1
*Release 								2
*CreateBuffer 							3
*CreateTexture1D 						4
*CreateTexture2D 						5
*CreateTexture3D 						6
*CreateShaderResourceView				7
*CreateUnorderedAccessView 				8
*CreateRenderTargetView 				9
*CreateDepthStencilView 				10
*CreateInputLayout 						11
*CreateVertexShader 					12
*CreateGeometryShader 					13
*CreateGeometryShaderWithStreamOutput	14
*CreatePixelShader 						15
*CreateHullShader 						16
*CreateDomainShader						17
*CreateComputeShader					18
*CreateClassLinkage 					19
*CreateBlendState 						20
*CreateDepthStencilState 				21
*CreateRasterizerState 					22
*CreateSamplerState						23
*CreateQuery 							24
*CreatePredicate 						25
*CreateCounter 							26
*CreateDeferredContext 					27
*OpenSharedResource 					28
*CheckFormatSupport 					29
*CheckMultisampleQualityLevels 			30
*CheckCounterInfo 						31
*CheckCounter 							32
*CheckFeatureSupport 					33
*GetPrivateData 						34
*SetPrivateData 						35
*SetPrivateDataInterface 				36
*GetFeatureLevel 						37
*GetCreationFlags 						38
*GetDeviceRemovedReason 				39
*GetImmediateContext 					40
*SetExceptionMode 						41
*GetExceptionMode 						42


// IDXGI SWAPCHAIN virtuals
[0]    7405CADA    (CMTUseCountedObject<CDXGISwapChain>::QueryInterface)
[1]    7405C9A7    (CMTUseCountedObject<CDXGISwapChain>::AddRef)
[2]    7405C9D8    (CMTUseCountedObject<CDXGISwapChain>::Release)
[3]    7405D6BF    (CDXGISwapChain::SetPrivateData)
[4]    7405F6FC    (CDXGISwapChain::SetPrivateDataInterface)
[5]    7405D6AF    (CDXGISwapChain::GetPrivateData)
[6]    7406106A    (CDXGISwapChain::GetParent)
[7]    7405EFDE    (CDXGISwapChain::GetDevice)
[8]    74061BD1    (CDXGISwapChain::Present)
[9]    740617A7    (CDXGISwapChain::GetBuffer)
[10]    74065CD6    (CDXGISwapChain::SetFullscreenState)
[11]    740662DC    (CDXGISwapChain::GetFullscreenState)
[12]    74061146    (CDXGISwapChain::GetDesc)
[13]    740655ED    (CDXGISwapChain::ResizeBuffers)
[14]    74065B8D    (CDXGISwapChain::ResizeTarget)
[15]    7406197B    (CDXGISwapChain::GetContainingOutput)
[16]    74066524    (CDXGISwapChain::GetFrameStatistics)
[17]    74066A58    (CDXGISwapChain::GetLastPresentCount)
[18]    740612C6    (CDXGISwapChain::GetDesc1)
[19]    740613E0    (CDXGISwapChain::GetFullscreenDesc)
[20]    740614F9    (CDXGISwapChain::GetHwnd)
[21]    7406156D    (CDXGISwapChain::GetCoreWindow)
[22]    74061D0D    (CDXGISwapChain[::IDXGISwapChain1]::Present1)
[23]    74062069    (CDXGISwapChain::IsTemporaryMonoSupported)
[24]    740615BB    (CDXGISwapChain::GetRestrictToOutput)
[25]    740615FB    (CDXGISwapChain::SetBackgroundColor)
[26]    740616F1    (CDXGISwapChain::GetBackgroundColor)
[27]    7406173F    (CDXGISwapChain::SetRotation)
[28]    74061770    (CDXGISwapChain::GetRotation)
[29]    7405CC1A    (CMTUseCountedObject<CDXGISwapChain>::`vector deleting destructor')
[30]    7405181E    (CMTUseCountedObject<CDXGISwapChain>::LUCCompleteLayerConstruction)
[31]    7405CBA5    (DXGID3D10ETWRundown)


// DXGI VTable:
[0]	6ED3F979	(CMTUseCountedObject<CDXGISwapChain>::QueryInterface)
[1]	6ED3F84D	(CMTUseCountedObject<CDXGISwapChain>::AddRef)
[2]	6ED3F77D	(CMTUseCountedObject<CDXGISwapChain>::Release)
[3]	6ED6A6D7	(CDXGISwapChain::SetPrivateData)
[4]	6ED6A904	(CDXGISwapChain::SetPrivateDataInterface)
[5]	6ED72BC9	(CDXGISwapChain::GetPrivateData)
[6]	6ED6DCDD	(CDXGISwapChain::GetParent)
[7]	6ED69BF4	(CDXGISwapChain::GetDevice)
[8]	6ED3FAAD	(CDXGISwapChain::Present)
[9]	6ED40209	(CDXGISwapChain::GetBuffer)
[10]	6ED47C1C	(CDXGISwapChain::SetFullscreenState)
[11]	6ED48CD9	(CDXGISwapChain::GetFullscreenState)
[12]	6ED40CB1	(CDXGISwapChain::GetDesc)
[13]	6ED48A3B	(CDXGISwapChain::ResizeBuffers)
[14]	6ED6F153	(CDXGISwapChain::ResizeTarget)
[15]	6ED47BA5	(CDXGISwapChain::GetContainingOutput)
[16]	6ED6D9B5	(CDXGISwapChain::GetFrameStatistics)
[17]	6ED327B5	(CDXGISwapChain::GetLastPresentCount)
[18]	6ED43400	(CDXGISwapChain::GetDesc1)
[19]	6ED6D9D0	(CDXGISwapChain::GetFullscreenDesc)
[20]	6ED6DA90	(CDXGISwapChain::GetHwnd)
[21]	6ED6D79F	(CDXGISwapChain::GetCoreWindow)
[22]	6ED6E352	(?Present1@?QIDXGISwapChain2@@CDXGISwapChain@@UAGJIIPBUDXGI_PRESENT_PARAMETERS@@@Z)
[23]	6ED6E240	(CDXGISwapChain::IsTemporaryMonoSupported)
[24]	6ED44146	(CDXGISwapChain::GetRestrictToOutput)
[25]	6ED6F766	(CDXGISwapChain::SetBackgroundColor)
[26]	6ED6D6B9	(CDXGISwapChain::GetBackgroundColor)
[27]	6ED4417B	(CDXGISwapChain::SetRotation)
[28]	6ED6DDE3	(CDXGISwapChain::GetRotation)
[29]	6ED6FF85	(CDXGISwapChain::SetSourceSize)
[30]	6ED6DF4F	(CDXGISwapChain::GetSourceSize)
[31]	6ED6FCBD	(CDXGISwapChain::SetMaximumFrameLatency)
[32]	6ED6DBE5	(CDXGISwapChain::GetMaximumFrameLatency)
[33]	6ED6D8CD	(CDXGISwapChain::GetFrameLatencyWaitableObject)
[34]	6ED6FB45	(CDXGISwapChain::SetMatrixTransform)
[35]	6ED6DAD0	(CDXGISwapChain::GetMatrixTransform)
[36]	6ED6C155	(CDXGISwapChain::CheckMultiplaneOverlaySupportInternal)
[37]	6ED6E82D	(CDXGISwapChain::PresentMultiplaneOverlayInternal)
[38]	6ED4397A	(CMTUseCountedObject<CDXGISwapChain>::`vector deleting destructor')
[39]	6ED4EAE0	(CSwapBuffer::AddRef)
[40]	6ED46C81	(CMTUseCountedObject<CDXGISwapChain>::LUCBeginLayerDestruction)
*/
