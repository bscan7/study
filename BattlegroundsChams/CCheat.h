#pragma once
#include "stdafx.h"
#define SHOOT_AREA_RADII  6
#define SEARCH_AREA  100


namespace CCheat
{
	//Variables
	extern ID3D11Device *pDevice;
	extern ID3D11DeviceContext *pContext;
	extern IDXGISwapChain* pSwapChain;

	//Functions
	void Initialise();
	void Release();
}