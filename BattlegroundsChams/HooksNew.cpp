#include "stdafx.h"
#include "Hooks.h"
#include "Helpers.h"
#include "CCheat.h"
#include <strsafe.h>
#include <list>
#include ".\mainxxx.h"
//#include <d3dx11async.h>
#include <process.h>
#include <iomanip>
//#include <iosfwd>
#include <string>
//extern "C"
//{ 
//#include "..\Subliminal-Fortnite\FW1FontWrapper\FW1FontWrapper.h"
//};

#include<fstream>
using namespace std;
#include "xnamath.h"
#include "D3DX11tex.h"
#include <map>
#include <list>
#include <mutex>
//#include <iosfwd>
#include <sstream>  
#include "gloabls.h"
#include "FW1FontWrapper\FW1FontWrapper.h"
#include <algorithm>

#pragma comment(lib, "winmm.lib") //timeGetTime
#define INTVL  1

IFW1Factory *pFW1Factory = NULL;
IFW1FontWrapper *pFontWrapper = NULL;

ID3D11DepthStencilState *ppDepthStencilState__New = NULL;
ID3D11DepthStencilState *ppDepthStencilState__Old = NULL;
ID3D11PixelShader* pPixelShader__Old = NULL;
ID3D11DeviceContext* pMainContext = NULL;

UINT pStencilRef = 0;
extern HWND g_hWnd;
extern RECT g_lpRect;
extern HANDLE  g_Event_Shoot;
extern HANDLE  g_Event_CrossDraw;
extern bool bCrossDraw;
extern bool bHideFog;
bool bCheat = true;
bool bHideOne = false;
bool b2DShader = true;
bool bLog2Txt_F7 = false;
bool bLog2Txt_DOWN = false;
ofstream outfile;
string  g_NotRedListFName = "";

ID3D11Buffer* pHooksStageBuffer = NULL;
D3D11_MAPPED_SUBRESOURCE *pHooksMappedResource = NULL;
static map<string, list<byte*>> mapLogList;
static map<DWORD, void*> mapThreadList;
static map<void*, void*> mapMapBufList;
map<string, string> mapBuf;
mutex g_lock;

float fXYZ[3];
list <XMFLOAT3> g_lstPositions;
DWORD minX, minY, maxX, maxY = 0;
int g_iSelfIdx = -1;
DWORD dFrontColor = 0;

list <XMFLOAT3> g_lstPositions2;
DWORD minX2, minY2, maxX2, maxY2 = 0;
int g_iSelfIdx2 = -1;

UINT iFrames = 0;
UINT iName = 0;
stringstream g_ssCallsInFrame;
static ID3D11Texture2D* g_pCaptureTexture = NULL;

bool IsNotIn_ExcludeList(UINT Stride, UINT IndexCount);

template<class T>
struct DisableCompare :public std::binary_function<T, T, bool>
{
	bool operator()(T lhs, T rhs)  const
	{
		return true;
	}
};
static map<string, XMFLOAT4, DisableCompare<string>> mapTID_PTR_DATA;
//string sKey = "";

void Thread_ExitHook(PVOID param);
static HWND hOutWnd = NULL;
//static int iIndexCnt = 0;
std::list<std::string> sHideList;

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
tD3D11Map Hooks::oMap = NULL;
tD3D11UnMap Hooks::oUnMap = NULL;
tD3D11VSSetConstantBuffers Hooks::oVSSetConstantBuffers = NULL;
tD3D11PSSetShaderResources Hooks::oPSSetShaderResources = NULL;
tD3D11PSSetSamplers Hooks::oPSSetSamplers = NULL;
tD3D11UpdateSubresource Hooks::oUpdateSubresource = NULL;

tD3D11DrawInstanced Hooks::oDrawInstanced = NULL;
tD3D11DrawIndexedInstanced Hooks::oDrawIndexedInstanced = NULL;
tD3D11DrawInstancedIndirect Hooks::oDrawInstancedIndirect = NULL;
tD3D11DrawIndexedInstancedIndirect Hooks::oDrawIndexedInstancedIndirect = NULL;
int mPreviousPosX = 0;
int mPreviousPosY = 0;
std::vector<int> aaa;
std::vector<int> bbb;
int iiidx = 0;

std::vector<UINT64> lstAllStride00;

std::vector<UINT64> lstLogStrides;

std::vector<UINT64> lstAllStrides;
std::vector<UINT64> lstAvatar2412;
std::vector<UINT64> lstEqupm2412;
std::vector<UINT64> lstExcludeAll;
std::vector<UINT64> lstIncludeAll;
std::vector<UINT64> lstHideList;
std::vector<UINT64> lstCarOrBoat;
std::vector<UINT64> lstHeader;
//std::vector<int> lstRed24;
//std::vector<int> lstBase12;
//std::vector<int> lstRed12;
int iPos = 0;
int iPosHide = 0;

bool bInited = false;
bool bInvertEveryFrame = false;

//bool bTest2Draw = false;
bool bTest2Draw = true;
//==========================================================================================================================
bool bHideENV = false;
bool bHideGrass = false;
DWORD ppppp = 0;
int ipp = 3;
int gStride = 12;
int iMin = 601;
int iMax = 2990;
int itm = 0;

UINT64 iiiii = 0;
int g_iCurStride = 0;
int g_iCurIndexCount = 0;
int bRed = true;
int iRed = 0;
DWORD gggg = 0;
DWORD cover = 0;
bool bShoot = false;
bool bShow24 = false;
ID3D11ShaderResourceView *pTextureSRV = NULL;

ID3D11ShaderResourceView* createTex(ID3D11Device* device, string filename)
{

	// 如果纹理资源已经存在，则返回，否则创建
	if (pTextureSRV != NULL)
		return pTextureSRV;

	HRESULT result;
	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	loadInfo.Format = /*DXGI_FORMAT_BC3_UNORM;*/ DXGI_FORMAT_R8G8B8A8_UNORM;
	loadInfo.MipLevels = D3DX11_DEFAULT; //这时会产生最大的mipmaps层
	loadInfo.MipFilter = D3DX11_FILTER_LINEAR;

	//ID3D11ShaderResourceView* rv = 0;

	// 从一个文件创建纹理资源视图.
	result = D3DX11CreateShaderResourceViewFromFileA(device, filename.c_str(), &loadInfo, NULL, &pTextureSRV, NULL);
	if (FAILED(result))
	{
		return NULL;
	}

	return pTextureSRV;
}

//void tmppp(ID3D11DeviceContext* pContext)
//{
// D3DX11_IMAGE_LOAD_INFO loadSMInfo;
// loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

// ID3D11Texture2D* SMTexture = 0;
// D3DX11CreateTextureFromFile(CCheat::pDevice, L"skymap.dds",
//	 &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

// D3D11_TEXTURE2D_DESC SMTextureDesc;
// SMTexture->GetDesc(&SMTextureDesc);

// D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
// SMViewDesc.Format = SMTextureDesc.Format;
// SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
// SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
// SMViewDesc.TextureCube.MostDetailedMip = 0;

// CCheat::pDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &pTextureSRV);
// return;

// D3D11_TEXTURE2D_DESC textureDesc;
// ID3D11Texture2D *pTexture;

// ZeroMemory(&textureDesc, sizeof(textureDesc));
// textureDesc.Width = 1280;
// textureDesc.Height = 720;
// textureDesc.ArraySize = 1;
// textureDesc.Format = DXGI_FORMAT_R8_UNORM;
// textureDesc.SampleDesc.Count = 1;
// textureDesc.Usage = D3D11_USAGE_DEFAULT;
// textureDesc.MipLevels = 1;
// textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

// Helpers::LogFormat("Create glyph sheet texture 0");
// HRESULT hResult = CCheat::pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
// if (FAILED(hResult)) {
//	 //m_lastError = L"Failed to create glyph sheet texture";
//	 Helpers::LogFormat("Failed to create glyph sheet texture 1");
// }
// else {

//	 hResult = CCheat::pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureSRV);
//	 if (FAILED(hResult)) {
//		 //m_lastError = L"Failed to create shader resource view for glyph sheet texture";
//		 Helpers::LogFormat("Failed to create glyph sheet texture 2");
//	 }
//	 else {
//		 // Create coord buffer if enabled
//		 //if (m_hardwareCoordBuffer) {
//		 //	D3D11_BUFFER_DESC bufferDesc;
//		 //	ID3D11Buffer *pBuffer;

//		 //	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//		 //	bufferDesc.ByteWidth = m_maxGlyphCount * sizeof(FW1_GLYPHCOORDS);
//		 //	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
//		 //	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

//		 //	hResult = m_pDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
//		 //	if (FAILED(hResult)) {
//		 //		m_lastError = L"Failed to create glyph coord buffer";
//		 //	}
//		 //	else {
//		 //		D3D11_SHADER_RESOURCE_VIEW_DESC bufferSRVDesc;
//		 //		ID3D11ShaderResourceView *pBufferSRV;

//		 //		ZeroMemory(&bufferSRVDesc, sizeof(bufferSRVDesc));
//		 //		bufferSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//		 //		bufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//		 //		bufferSRVDesc.Buffer.ElementOffset = 0;
//		 //		bufferSRVDesc.Buffer.ElementWidth = m_maxGlyphCount * 2;// Two float4 per glyphcoords

//		 //		hResult = m_pDevice->CreateShaderResourceView(pBuffer, &bufferSRVDesc, &pBufferSRV);
//		 //		if (FAILED(hResult)) {
//		 //			m_lastError = L"Failed to create shader resource view for glyph coord buffer";
//		 //		}
//		 //		else {
//		 //			m_pCoordBuffer = pBuffer;
//		 //			m_pCoordBufferSRV = pBufferSRV;
//		 //		}

//		 //		if (FAILED(hResult))
//		 //			pBuffer->Release();
//		 //	}
//		 //}

//		 //if (SUCCEEDED(hResult)) {
//		 //	m_pTexture = pTexture;
//		 //	m_pTextureSRV = pTextureSRV;
//		 //}
//		 //else
//		 //	pTextureSRV->Release();
//	 }

//	 if (FAILED(hResult))
//	 {
//		 Helpers::LogFormat("Failed to create glyph sheet texture 3");
//		 pTexture->Release();
//	 }
//	 //Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, (ID3D11ShaderResourceView *const *)pTextureSRV);
// }
// Helpers::LogFormat("Done create glyph sheet texture 000000000000000000");
//}

void* FindByTID(DWORD tid)
{
	std::map<DWORD, void*>::iterator iter = mapThreadList.find(tid);

	if (mapThreadList.end() != iter)

		return iter->second;

	return NULL;
}

struct Vertex	//Overloaded Vertex Structure
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz),
		tangent(tx, ty, tz) {}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 biTangent;
};

ID3D11Texture2D *sharedTex11 = NULL;
ID3D11Buffer *d2dVertBuffer = NULL;
ID3D11Buffer *d2dIndexBuffer = NULL;
ID3D11ShaderResourceView *d2dTexture = NULL;

//void InitD2DScreenTexture()
//{
// HRESULT hResult;
// ////Create the vertex buffer
// //Vertex v[] =
// //{
//	// // Front Face
//	// Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
//	// Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
//	// Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
//	// Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
// //};

// //DWORD indices[] = {
//	// // Front Face
//	// 0,  1,  2,
//	// 0,  2,  3,
// //};

// //D3D11_BUFFER_DESC indexBufferDesc;
// //ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

// //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// //indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
// //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
// //indexBufferDesc.CPUAccessFlags = 0;
// //indexBufferDesc.MiscFlags = 0;

// //D3D11_SUBRESOURCE_DATA iinitData;

// //iinitData.pSysMem = indices;
// //hResult = CCheat::pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);
// //if (FAILED(hResult))
// //{
//	// Helpers::LogFormat("FAILED(hResult) 0");
// //}
// //D3D11_BUFFER_DESC vertexBufferDesc;
// //ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

// //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
// //vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
// //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// //vertexBufferDesc.CPUAccessFlags = 0;
// //vertexBufferDesc.MiscFlags = 0;

// //D3D11_SUBRESOURCE_DATA vertexBufferData;

// //ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
// //vertexBufferData.pSysMem = v;
// //hResult = CCheat::pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);
// //if (FAILED(hResult))
// //{
//	// Helpers::LogFormat("FAILED(hResult) 1");
// //}

// //D3D11_TEXTURE2D_DESC sharedTexDesc;

// //ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

// //sharedTexDesc.Width = 1280;
// //sharedTexDesc.Height = 720;
// //sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
// //sharedTexDesc.MipLevels = 1;
// //sharedTexDesc.ArraySize = 1;
// //sharedTexDesc.SampleDesc.Count = 1;
// //sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
// //sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
// //sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

// //hResult = CCheat::pDevice->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
// //if (FAILED(hResult))
// //{
//	// Helpers::LogFormat("FAILED(CreateTexture2D) .......");
// //}

// ////Create A shader resource view from the texture D2D will render to,
// ////So we can use it to texture a square which overlays our scene
// //hResult = CCheat::pDevice->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
// //if (FAILED(hResult))
// //{
//	// Helpers::LogFormat("FAILED(hResult) 222");
// //}


// ///Load Skymap's cube texture///
// D3DX11_IMAGE_LOAD_INFO loadSMInfo;
// loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

// ID3D11Texture2D* SMTexture = 0;
// hResult = D3DX11CreateTextureFromFile(CCheat::pDevice, L"..\\skymap.dds",
//	 &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

// D3D11_TEXTURE2D_DESC SMTextureDesc;
// SMTexture->GetDesc(&SMTextureDesc);

// D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
// SMViewDesc.Format = SMTextureDesc.Format;
// SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
// SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
// SMViewDesc.TextureCube.MostDetailedMip = 0;

// hResult = CCheat::pDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &d2dTexture);


// Helpers::LogFormat("SMTexture=[%x] d2dTexture=[[ %x ]] ", SMTexture, d2dTexture);
// system("pause");
//}

// //--------------------------------------------------------------------------------------
// // Helper function to capture a frame and dump it to disk 
// //--------------------------------------------------------------------------------------
//void CaptureFrame()
//{
//	// Retrieve RT resource
//	ID3D11Resource *pRTResource;
//	DXUTGetD3D11RenderTargetView()->GetResource(&pRTResource);
//
//	// Retrieve a Texture2D interface from resource
//	ID3D11Texture2D* RTTexture;
//	pRTResource->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&RTTexture);
//
//	// Check if RT is multisampled or not
//	D3D11_TEXTURE2D_DESC    TexDesc;
//	RTTexture->GetDesc(&TexDesc);
//	if (TexDesc.SampleDesc.Count>1)
//	{
//		// RT is multisampled, need resolving before dumping to disk
//
//		// Create single-sample RT of the same type and dimensions
//		DXGI_SAMPLE_DESC SingleSample = { 1, 0 };
//		TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//		TexDesc.MipLevels = 1;
//		TexDesc.Usage = D3D11_USAGE_DEFAULT;
//		TexDesc.CPUAccessFlags = 0;
//		TexDesc.BindFlags = 0;
//		TexDesc.SampleDesc = SingleSample;
//
//		ID3D11Texture2D *pSingleSampleTexture;
//		DXUTGetD3D11Device()->CreateTexture2D(&TexDesc, NULL, &pSingleSampleTexture);
//		DXUT_SetDebugName(pSingleSampleTexture, "Single Sample");
//
//		DXUTGetD3D11DeviceContext()->ResolveSubresource(pSingleSampleTexture, 0, RTTexture, 0, TexDesc.Format);
//
//		// Copy RT into STAGING texture
//		DXUTGetD3D11DeviceContext()->CopyResource(g_pCaptureTexture, pSingleSampleTexture);
//
//		D3DX11SaveTextureToFile(DXUTGetD3D11DeviceContext(), g_pCaptureTexture, D3DX11_IFF_BMP, g_CmdLineParams.strCaptureFilename);
//
//		SAFE_RELEASE(pSingleSampleTexture);
//
//	}
//	else
//	{
//		// Single sample case
//
//		// Copy RT into STAGING texture
//		DXUTGetD3D11DeviceContext()->CopyResource(g_pCaptureTexture, pRTResource);
//
//		D3DX11SaveTextureToFile(DXUTGetD3D11DeviceContext(), g_pCaptureTexture, D3DX11_IFF_BMP, g_CmdLineParams.strCaptureFilename);
//	}
//
//	SAFE_RELEASE(RTTexture);
//
//	SAFE_RELEASE(pRTResource);
//}
//

void Save_UnMapData_New(UINT Stride, UINT IndexCountPerInstance)
{
	return;
	g_lock.lock();
	if ((Stride == 24) && (IndexCountPerInstance == 3234))
	{
		ofstream outfile;
		outfile.open("..\\UnMap_New__.txt", ios::app);
		if (outfile)
		{
			outfile << ::GetCurrentThreadId() << "_DrawIndexed()===================== " << " Stride=" << Stride << " IdxCount=" << IndexCountPerInstance << std::endl << std::endl;

			map<string, XMFLOAT4, DisableCompare<string>>::iterator iter;//定义一个迭代指针iter
			for (iter = mapTID_PTR_DATA.begin(); iter != mapTID_PTR_DATA.end(); iter++)
			{
				if (iter->first.find(to_string(::GetCurrentThreadId()) + "_") != std::string::npos)
				{
					//if (*(UINT*)(&(iter->second.x)) == 0x88888888)
					//{
					//	continue;
					//}
					//UINT ppp = atoll(iter->first.substr(iter->first.find("_") + 1).c_str());
					//iter->second.x = *(FLOAT*)ppp;
					//iter->second.y = *(FLOAT*)(ppp + 4);
					//iter->second.z = *(FLOAT*)(ppp + 8);
					//iter->second.w = *(FLOAT*)(ppp + 12);
					//break;

					outfile << iter->first << ": ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&iter->second.x) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&iter->second.x) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&iter->second.y) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&iter->second.y) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&iter->second.z) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&iter->second.z) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&iter->second.w) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&iter->second.w) << "] " << std::endl;

				}
			}
			outfile << "mapSize=" << std::dec << mapTID_PTR_DATA.size() << std::endl;
			outfile << std::endl;
			outfile.close();
		}
	}
	mapTID_PTR_DATA.clear();
	g_lock.unlock();
}
void InitListFromFiles()
{
	std::cout << "lstAllStides.clear()..." << endl;
	lstAllStrides.clear();
	iiiii = 0;
	//从文件读取列表
	{
		//WinExec("cmd /K CD ", SW_SHOW);

		lstAvatar2412.clear();
		fstream fin("..\\avatarList.txt");  //打开文件
		string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstAvatar2412.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstAvatar2412 << ..\\avatarList.txt" << endl;

		lstEqupm2412.clear();
		fin.open("..\\equpmList.txt");  //打开文件
										//string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstEqupm2412.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstEqupm2412 << ..\\equpmList.txt" << endl;

		lstExcludeAll.clear();
		fin.open(g_NotRedListFName.c_str());  //打开文件
											  //string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstExcludeAll.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstNot2412 << " << g_NotRedListFName.c_str() << endl;

		lstIncludeAll.clear();
		fin.open("..\\V2_Include01.txt");  //打开文件
											  //string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstIncludeAll.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstIncludeAll << " << "..\\V2_Include01.txt" << endl;

		lstHideList.clear();
		fin.open("..\\HideList.txt");  //打开文件
									   //string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstHideList.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstHideList << ..\\HideList.txt" << endl;

		lstCarOrBoat.clear();
		fin.open("..\\CarOrBoatList.txt");  //打开文件
											//string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstCarOrBoat.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstCarOrBoat << ..\\CarOrBoatList.txt" << endl;

		lstHeader.clear();
		fin.open("..\\HeaderList.txt");  //打开文件
											//string ReadLine;
		while (getline(fin, ReadLine))  //逐行读取，直到结束
		{
			lstHeader.push_back(atoi(ReadLine.c_str()));
		}
		fin.close();
		std::cout << "逐行读取文件完成！ lstHeader << ..\\HeaderList.txt" << endl;
	}
}

std::wstring StringToWString(const std::string& str)
{
	setlocale(LC_ALL, "chs");
	const char* point_to_source = str.c_str();
	size_t new_size = str.size() + 1;
	wchar_t *point_to_destination = new wchar_t[new_size];
	wmemset(point_to_destination, 0, new_size);
	mbstowcs(point_to_destination, point_to_source, new_size);
	std::wstring result = point_to_destination;
	delete[]point_to_destination;
	setlocale(LC_ALL, "C");
	return result;
}

void Send2Hwnd(UINT IndexCountPerInstance, UINT Stride)
{
	if (hOutWnd) {
		std::string sendData = std::to_string(IndexCountPerInstance);
		while (sendData.length() < 5)
		{
			sendData = "0" + sendData;
		}
		sendData = std::to_string(Stride) + "_" + sendData;

		/*if (type == USER_ID) {
		sendData = CUserOperate::Instance().getUserId();
		}
		else if (type == NETBAR_ID) {
		sendData = ConfigMan::getInstance()->getNetbarId();
		}
		else if (type == AGENT_ID) {
		sendData = ConfigMan::getInstance()->getAgentId();
		}*/
		COPYDATASTRUCT copyData = { 0 };
		copyData.lpData = (void *)sendData.c_str();
		copyData.cbData = sendData.length() + 1;
		copyData.dwData = Stride/*type*/;
		::SendMessage(hOutWnd, WM_COPYDATA, NULL, (LPARAM)&copyData);
	}
}
ID3D11PixelShader* psCrimson = NULL;
ID3D11PixelShader* psYellow = NULL;
ID3D11PixelShader* psGreen0 = NULL;
ID3D11PixelShader* psFront = NULL;
ID3D11PixelShader* psBlue = NULL;
ID3D11PixelShader* psRed0 = NULL;
ID3D11PixelShader* psBack = NULL;
ID3D11ShaderResourceView* ShaderResourceView;

void Thread_fileWatcher(PVOID param)
{
	DWORD cbBytes;
	char file_name[MAX_PATH]; //设置文件名
	char file_name2[MAX_PATH]; //设置文件重命名后的名字
	char notify[1024];
	int count = 0; //文件数量。可能同时拷贝、删除多个文件，可以进行更友好的提示。
	char *dir = /*_T*/("..\\");

	HANDLE dirHandle = CreateFileA(dir, GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (dirHandle == INVALID_HANDLE_VALUE) //若网络重定向或目标文件系统不支持该操作，函数失败，同时调用GetLastError()返回ERROR_INVALID_FUNCTION
	{
		cout << "error" + GetLastError() << endl;
	}


	memset(notify, 0, strlen(notify));

	FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)notify;
	cout << "start...." << endl;
	while (true)
	{

		if (ReadDirectoryChangesW(dirHandle, &notify, 1024, true,
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME
			//| FILE_NOTIFY_CHANGE_CREATION
			//| FILE_NOTIFY_CHANGE_LAST_WRITE
			| FILE_NOTIFY_CHANGE_SIZE,
			&cbBytes, NULL, NULL))
		{
			//转换文件名为多字节字符串
			if (pnotify->FileName)
			{
				memset(file_name, 0, strlen(file_name));

				WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, file_name, 99, NULL, NULL);
			}

			//获取重命名的文件名
			if (pnotify->NextEntryOffset != 0 && (pnotify->FileNameLength > 0 && pnotify->FileNameLength < MAX_PATH))
			{
				PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pnotify + pnotify->NextEntryOffset);
				memset(file_name2, 0, sizeof(file_name2));
				WideCharToMultiByte(CP_ACP, 0, p->FileName, p->FileNameLength / 2, file_name2, 99, NULL, NULL);
			}

			//设置类型过滤器,监听文件创建、更改、删除、重命名等
			switch (pnotify->Action)
			{
			case FILE_ACTION_ADDED:
				count++;
				cout << count << setw(5) << "file add:" << setw(5) << file_name << endl;
				break;
			case FILE_ACTION_MODIFIED:
				cout << "file modified:" << setw(5) << file_name << endl;
				break;
			case FILE_ACTION_REMOVED:
				count++;
				cout << count << setw(5) << "file removed:" << setw(5) << file_name << endl;
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
				cout << "file renamed:" << setw(5) << file_name << "->" << file_name2 << endl;
				if (strstr(file_name2, "12.") == file_name2)
				{
					string sHide12 = strstr(file_name2, "12.") + 3;
				}
				break;

			default:
				cout << "unknow command!" << endl;

			}

		}

	}
	CloseHandle(dirHandle);
}

extern bool bStoped;


void SaveMapToFile()
{
	//save map to file
	//del old files 
	g_lock.lock();
	//loop map
	map<string, list<byte*>>::iterator it;
	for (it = mapLogList.begin(); it != mapLogList.end(); ++it)
	{
		//UINT xxx = *(UINT*)((UINT)it->second.front());
		if ((it->second.size() > 1))
		{
			//save to file
			ofstream outfile;
			outfile.open("..\\" + it->first + to_string(timeGetTime()) + ".txt", ios::app);
			if (outfile)
			{
				outfile << std::endl;
				list<byte*>::iterator itor;
				for (itor = it->second.begin(); itor != it->second.end(); itor++)
				{
					byte* pBuf = *itor;
					UINT xxx = *(UINT*)((UINT)pBuf);

					if ((xxx == 0xb896d4bc) ||
						(xxx / 0x100 == 0xb9a30c) ||
						(xxx == 0xb9a30cae)
						)
					{
						continue;
					}
					//outfile.open("..\\" + to_string(timeGetTime()), ios::app);
					//D3D11_BUFFER_DESC desc;
					//((ID3D11Buffer*)pBuf)->GetDesc(&desc);

					//outfile << " BindFlags=" << desc.BindFlags << " ByteWidth=" << desc.ByteWidth << " MiscFlags=" << desc.MiscFlags << " StructureByteStride=" << desc.StructureByteStride << " Usage=" << desc.Usage << std::hex << " CPUAccessFlags=0x" << desc.CPUAccessFlags << std::endl;
					//outfile << std::hex << " pBuf=0x" << pBuf << std::endl;
					for (int i = 0; i < 16; i += sizeof(int))
					{
						if (i > 0 && i % 16 == 0)
						{
							outfile << std::endl;
						}
						int xxx = *(int*)((int)pBuf + i);
						outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)((int)pBuf + i) << "[" << std::setw(10) << std::setfill(' ') << *(float*)((int)pBuf + i) << "] ";
					}
					outfile << std::endl;
				}
				outfile.close();
			}
		}
		//Clear byte[16]
		if (it->second.size() > 0)
		{
			list<byte*>::iterator itor;
			for (itor = it->second.begin(); itor != it->second.end(); itor++)
			{
				delete[] * itor;
			}
			//Clear list
			it->second.clear();
		}
	}
	//Clear map
	mapLogList.clear();
	mapThreadList.clear();
	g_lock.unlock();
}


void Append2Txt(string sTxtLine)
{
	ofstream outfile;
	outfile.open(("..\\" + to_string(iFrames)+"_Frame.log").c_str(), ios::app);
	if (!outfile)
	{
		std::cout << "打开文件失败！" << (to_string(iFrames) + ".txt").c_str() << endl;
	}
	else if (sTxtLine.length() > 0)
	{
		outfile << std::dec << sTxtLine.c_str() << std::endl;
		outfile.close();
		std::cout << std::dec << sTxtLine.c_str() << " 写入文件完成！" << (to_string(iFrames) + ".txt").c_str() << endl;
	}
}

void Append2ExcludeLst()
{
	ofstream outfile;
	outfile.open(g_NotRedListFName.c_str(), ios::app);
	if (!outfile)
	{
		std::cout << "打开文件失败！" << g_NotRedListFName.c_str() << endl;
	}
	else if (iiiii > 0)
	{
		outfile << std::dec << iiiii << std::endl;
		outfile.close();
		std::cout << std::dec << iiiii << " 写入文件完成！" << g_NotRedListFName.c_str() << endl;

		InitListFromFiles();
	}
}
void Append2CarOrBoatLst()
{
	ofstream outfile;
	outfile.open("..\\CarOrBoatList.txt", ios::app);
	if (!outfile)
	{
		std::cout << "打开文件失败！" << "..\\CarOrBoatList.txt" << endl;
	}
	else if (iiiii > 0)
	{
		outfile << std::dec << iiiii << std::endl;
		outfile.close();
		std::cout << std::dec << iiiii << " 写入文件完成！" << "..\\CarOrBoatList.txt" << endl;

		InitListFromFiles();
	}
}

void Append2IncludeLst()
{
	ofstream outfile;
	outfile.open("..\\V2_Include01.txt", ios::app);
	if (!outfile)
	{
		std::cout << "打开文件失败！" << "..\\V2_Include01.txt" << endl;
	}
	else if (iiiii > 0)
	{
		outfile << std::dec << iiiii << std::endl;
		outfile.close();
		std::cout << std::dec << iiiii << " 写入文件完成！" << "..\\V2_Include01.txt" << endl;

		InitListFromFiles();
	}
}

void Append2HideLst()
{
	ofstream outfile;
	outfile.open("..\\HideList.txt", ios::app);
	if (!outfile)
	{
		std::cout << "打开文件失败！" << "..\\HideList.txt" << endl;
	}
	else if (iiiii > 0)
	{
		outfile << std::dec << iiiii << std::endl;
		outfile.close();
		std::cout << std::dec << iiiii << " 写入文件完成！" << "..\\HideList.txt" << endl;

		InitListFromFiles();
	}
}

void W_SHIFT_KeyUp()
{
	Helpers::LogFormat("2A--------bGo = %d bCheat = %d ", bGo, bCheat);
	keybd_event(87, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	Helpers::LogFormat("2B--------bGo = %d bCheat = %d ", bGo, bCheat);
}


void SHIFT_W_KeyDown()
{
	Helpers::LogFormat("1a--------bGo = %d bCheat = %d ", bGo, bCheat);
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(87, 0, 0, 0);
	Helpers::LogFormat("1b--------bGo = %d bCheat = %d ", bGo, bCheat);
}


void Dbl_Key_()
{
	keybd_event(192, 0, 0, 0);
	keybd_event(192, 0, KEYEVENTF_KEYUP, 0);
	Sleep(50);
	keybd_event(192, 0, 0, 0);
	keybd_event(192, 0, KEYEVENTF_KEYUP, 0);
}

/*
当前视图：
看到的
1. 加入排除列表，不要变色
2. 加入隐藏列表，不要显示
3. 加入车船列表，变绿色

VK_HOME		Un/Load DLL
VK_SCROLL	单模型高亮查找
VK_ADD		bShoot = !bShoot;
VK_NUMPAD0	多模式切换
VK_NUMPAD1	sHideList.clear();
VK_NUMPAD2	bHideFog = !bHideFog;烟雾弹
VK_NUMPAD3
VK_F12
VK_F10
VK_F9		bCrossDraw = !bCrossDraw;
VK_F8		bCheat = !bCheat;
VK_F7		bLog2Txt_F7 = !bLog2Txt_F7;
VK_F6		bHideOne = !bHideOne;
VK_F5		pssrStartSlot++;
VK_F3		b2DShader = !b2DShader;
VK_DELETE
VK_PAUSE	InitListFromFiles();
VK_RIGHT
VK_UP		红色回退
VK_DOWN		bVideo4Rec_PAUSE = !bVideo4Rec_PAUSE;
VK_RETURN	
VK_PRIOR	[PageUp]:Append2ExcludeLst();加入排除列表，不要变色
VK_NEXT		[PageDown]:Append2CarOrBoatLst();加入车船列表，变绿色
*/

void Thread_KeysSwitch(PVOID param)
{
	Helpers::LogFormat("--------Thread_KeysSwitch---------Started ");
	int iBW_Pos = 0;
	while (!bStoped)
	{
		if (GetAsyncKeyState(VK_HOME) & 1)
		{
			CCheat::Release();
		}

		if (GetAsyncKeyState(VK_SCROLL) & 1)
		{
			bVideo4Rec_PAUSE = false;
			bVideo4Rec_SCROL = !bVideo4Rec_SCROL;
		}
		if (GetAsyncKeyState(VK_ADD) & 1)
		{
			bShoot = !bShoot;
			//red24.push_back(lst24[iPos]);
			if (bShoot)
			{
				WinExec("cmd /C del /Q ..\\tmp\\*_Center_.*", SW_HIDE);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			ipp = ipp++ % 4;
		}
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			sHideList.clear();
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bHideFog = !bHideFog;
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			//bCrossDraw = !bCrossDraw;
		}
		if (GetAsyncKeyState(VK_F12) & 1)
		{
			//if (iBW_Pos >= lstAllStride00.size())
			//{
			//	iBW_Pos = 0;
			//}
			//if (lstAllStride00.size() > 0)
			//{
			//	pssrStride = lstAllStride00.at(iBW_Pos++);
			//	Helpers::LogFormat("pssrStride  == [[ %d ]], lstAllByteWidth.size()=[%d]", pssrStride, lstAllStride00.size());
			//}
		}
		if (GetAsyncKeyState(VK_F10) & 1)
		{
			//bShow24 = !bShow24;
		}
		if (GetAsyncKeyState(VK_F9) & 1)
		{
			//bLogTxt = !bLogTxt;
			bCrossDraw = !bCrossDraw;
		}
		if (GetAsyncKeyState(VK_F8) & 1)
		{
			bCheat = !bCheat;
		}
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			bLog2Txt_F7 = !bLog2Txt_F7;
			if (!bLog2Txt_F7)
			{
				SaveMapToFile();
			}
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			bHideOne = !bHideOne;
		}
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			lstAllStride00.clear();
			iBW_Pos = 0;

			pssrStartSlot++;
			if (pssrStartSlot > 5)
			{
				pssrStartSlot = 0;
			}
			Helpers::LogFormat("pssrStartSlot= < %d >, ", pssrStartSlot);

		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			b2DShader = !b2DShader;
		}

		//if (GetAsyncKeyState(VK_DELETE) & 1)
		{
			//ppDepthStencilState__Old = NULL;
			//ppDepthStencilState__New = NULL;
		}

		if (GetAsyncKeyState(VK_PAUSE) & 1)
		{
			//lstAllStides.clear();
			InitListFromFiles();
		}
		if (GetAsyncKeyState(VK_RIGHT) & 1)
		{
			//iRed = iRed++ % 3;
		}
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			bVideo4Rec_SCROL = true;
			bVideo4Rec_PAUSE = true;
			//红色回退
			if (lstAllStrides.size() > 0)
			{
			nnnn:
				iPos--;
				if (iPos < 0)
				{
					iPos = lstAllStrides.size() - 1;
				}
				else if (iPos >=lstAllStrides.size())
				{
					iPos = lstAllStrides.size() - 1;
				}


				iiiii = lstAllStrides.at(iPos);
				g_iCurStride = iiiii % 100;
				g_iCurIndexCount = iiiii / 100;;
				//Helpers::LogFormat("%d %d-%d %d %ld", iPos, iStride, iIndexCount, lstAll2412.size(), lstAll2412.at(iPos));
				//if (!IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount))
				//{
				//	goto nnnn;
				//}
			}

		}
		if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			bVideo4Rec_SCROL = true;
			bVideo4Rec_PAUSE = true;
			//红色回退
			if (lstAllStrides.size() > 0)
			{
			nnnn2:
				iPos++;
				if (iPos < 0)
				{
					iPos = 0;
				}
				else if (iPos >= lstAllStrides.size())
				{
					iPos = 0;
				}


				iiiii = lstAllStrides.at(iPos);
				g_iCurStride = iiiii % 100;
				g_iCurIndexCount = iiiii / 100;;
				//Helpers::LogFormat("%d %d-%d %d %ld", iPos, iStride, iIndexCount, lstAll2412.size(), lstAll2412.at(iPos));
				//if (!IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount))
				//{
				//	goto nnnn2;
				//}
			}
		}
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			Append2IncludeLst();
			bVideo4Rec_SCROL = false;
			bVideo4Rec_PAUSE = false;
		}
		if (GetAsyncKeyState(VK_END) & 1)
		{
			Append2HideLst();
			bVideo4Rec_SCROL = false;
			bVideo4Rec_PAUSE = false;
		}
		if (GetAsyncKeyState(VK_PRIOR) & 1)
		{
			Append2ExcludeLst();
			bVideo4Rec_SCROL = false;
			bVideo4Rec_PAUSE = false;
		}
		if (GetAsyncKeyState(VK_NEXT) & 1)
		{
			Append2CarOrBoatLst(); 
			bVideo4Rec_SCROL = false;
			bVideo4Rec_PAUSE = false;
		}
		if (GetAsyncKeyState(83) & 1) //'S' KEY
		{
			bGo = false;
			W_SHIFT_KeyUp();
			bCheat = true;
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 1)
		{
			Dbl_Key_();
		}
		if (GetAsyncKeyState(70) & 1) //'F' KEY
		{
			Dbl_Key_();
		}

		if (GetAsyncKeyState(VK_DELETE) & 1)
		{
			bGo = !bGo;
			if (bGo)
			{
				bCheat = false;
				SHIFT_W_KeyDown();
			}
			else
			{
				W_SHIFT_KeyUp();
				bCheat = true;
			}

		}

		//if (GetAsyncKeyState(VK_RETURN) & 1)
		//{
		//	//lstRed24.clear();
		//}

		//if (GetAsyncKeyState(VK_RIGHT) & 1)
		//{
		//	ofstream output(".\\1111.txt", ios::binary);
		//	for (int i=0;i<red24.size();i++)
		//	{
		//		output.write((char*)(&(red24[i])), sizeof((red24[i]))); //写入
		//		output.write("\r\n", 2); //写入
		//	}
		//	//int i2 = 0;
		//	//ifstream input("./1.txt", ios::binary);
		//	//input.read((char*)(&i2), sizeof(i2)); //读取
		//}

		if (ipp == 1)
		{
			bHideENV = true;
			bHideGrass = false;
		}
		else if (ipp == 2)
		{
			if (ppppp != timeGetTime() / INTVL)
			{
				bHideENV = !bHideENV;
				ppppp = timeGetTime() / INTVL;
			}
		}
		else if (ipp == 3)
		{
			bHideGrass = !bHideGrass;
			bHideGrass = true;
			bHideENV = false;
		}
		else
		{
			bHideGrass = false;
			bHideENV = false;
		}


		HWND hOutWnd000 = NULL;
		hOutWnd000 = ::FindWindowExA(NULL, NULL, "#32770", "懵圈宝宝 哈哈哈");
		if (hOutWnd000)
		{
			hOutWnd = hOutWnd000;
		}

		char szWndTitle[256] = { 0 };
		if (hOutWnd)
		{
			GetWindowTextA(hOutWnd, szWndTitle, 256);
			//iIndexCnt = atoi(szWndTitle);

			//std::list::iterator iter;
			list<string>::iterator iter;
			iter = std::find(sHideList.begin(), sHideList.end(), szWndTitle);

			if (iter != sHideList.end())
			{
				//lst中存在 szWndTitle
				sHideList.erase(iter);
			}
			else
			{
				//没找到
				sHideList.push_back(szWndTitle);
			}

		}

		Sleep(100);
	}
}

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

bool IsNotIn_ExcludeList(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstExcludeAll.begin(), lstExcludeAll.end(), IndexCountStride) != lstExcludeAll.end()) {
		//找到
		return false;
	}
	else {
		//没找到
		//if (!bVideo4Rec_SCROL && (/*(Stride == 24) &&*/ (IndexCount <= 200)))
		//{
		// return false;
		//}
		//else
		{
			return true;
		}
	}

	//return ((Stride == 24) // ...
	// && (IndexCount > 200)
	// && (IndexCount != 69)
	// && (IndexCount != 96)
	// && (IndexCount != 192)
	// && (IndexCount != 876)

	// && (IndexCount != 1128)
	// && (IndexCount != 1728)
	// && (IndexCount != 1842)
	// && (IndexCount != 1932)
	// && (IndexCount != 2556)
	// && (IndexCount != 3228)
	// && (IndexCount != 3234)
	// && (IndexCount != 5124)
	// );
}

bool IsIn_IncludeList(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstIncludeAll.begin(), lstIncludeAll.end(), IndexCountStride) != lstIncludeAll.end()) {
		//找到
			return true;
	}
	else {
		//没找到
		return false;
	}
}

bool Is_CarOrBoat(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstCarOrBoat.begin(), lstCarOrBoat.end(), IndexCountStride) != lstCarOrBoat.end()) {
		//找到
		return true;
	}
	else {
		return false;
	}
}

bool Is_Header(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstHeader.begin(), lstHeader.end(), IndexCountStride) != lstHeader.end()) {
		//找到
		return true;
	}
	else {
		return false;
	}
}

bool IsIn_HideList(UINT Stride, UINT IndexCount, UINT BaseVertexLocation)
{
	//全息：	<0x06399FD0> ID3D11DeviceContext::DrawIndexed(6, 0, -1495)	154012179757					

	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstHideList.begin(), lstHideList.end(), IndexCountStride) != lstHideList.end()) {
		//找到
		if (BaseVertexLocation != -1495) //不是全息中心点
		{
			return true;
		}
	}
	else {
		return false;
	}
	return false;
}

bool IsEquipment(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstEqupm2412.begin(), lstEqupm2412.end(), IndexCountStride) != lstEqupm2412.end()) {
		//找到
		return true;
	}
	else {
		//没找到
		return false;
	}
	//return (Stride == 24 && IndexCount == 969) // 车
	// || (Stride == 24 && IndexCount == 8238) // 车
	// || (Stride == 24 && IndexCount == 8118) // 车
	// || (Stride == 24 && IndexCount == 6807) // 车
	// || (Stride == 24 && IndexCount == 6357) // 车

	// || (Stride == 12 && IndexCount == 1002) // 
	// || (Stride == 12 && IndexCount == 1008) // 
	// || (Stride == 12 && IndexCount == 1344) // 
	// || (Stride == 12 && IndexCount == 1362) // 
	// || (Stride == 12 && IndexCount == 1386) // 
	// || (Stride == 12 && IndexCount == 1512) // 

	// || (Stride == 12 && IndexCount == 627) // 
	// || (Stride == 12 && IndexCount == 672) // 
	// ;
}

bool IsAvatar(UINT Stride, UINT IndexCount)
{
	UINT64 IndexCountStride = IndexCount * 100 + Stride;
	if (find(lstAvatar2412.begin(), lstAvatar2412.end(), IndexCountStride) != lstAvatar2412.end()) {
		//找到
		return true;
	}
	else {
		//没找到
		return false;
	}

	//return (Stride == 12 && IndexCount == 192) // ？盔 远处
	// || (Stride == 12 && IndexCount == 156) // ？盔 远处
	// || (Stride == 12 && IndexCount == 180) // ？盔 远处
	// || (Stride == 12 && IndexCount == 276) // ？盔 远处
	// || (Stride == 12 && IndexCount == 294) // ？盔 远处
	// || (Stride == 12 && IndexCount == 627) // 头;
	// || (Stride == 12 && IndexCount == 637) // 头;
	// || (Stride == 12 && IndexCount == 672) // 头;
	// || (Stride == 12 && IndexCount == 672) // 头;
	// || (Stride == 12 && IndexCount == 816) // ？盔 远处;
	// || (Stride == 12 && IndexCount == 1002) // 身体
	// || (Stride == 12 && IndexCount == 1008) // 身体
	// || (Stride == 12 && IndexCount == 1116) // 2级盔 近处
	// || (Stride == 12 && IndexCount == 1344) // 身体
	// || (Stride == 12 && IndexCount == 1362) // 身体
	// || (Stride == 12 && IndexCount == 1386) // 身体
	// || (Stride == 12 && IndexCount == 1512) // 身体
	// || (Stride == 12 && IndexCount == 2637) // 三级盔 近处
	// || (Stride == 12 && IndexCount == 2838) // 胳臂
	// || (Stride == 12 && IndexCount == 2868) // 头发
	// || (Stride == 12 && IndexCount == 2877) // 头发
	// || (Stride == 12 && IndexCount == 3132) // 上衣
	// || (Stride == 12 && IndexCount == 3162) // 上衣
	// || (Stride == 12 && IndexCount == 3252) // 吉利服

	// || (Stride == 24 && IndexCount == 498) // 衣服
	// || (Stride == 24 && IndexCount == 552) // 衣服
	// || (Stride == 24 && IndexCount == 1080) // 衣服
	// || (Stride == 24 && IndexCount == 1728) // 衣服
	// || (Stride == 24 && IndexCount == 2070) // 头
	// || (Stride == 24 && IndexCount == 2472) // 衣服
	// || (Stride == 24 && IndexCount == 2478) // 衣服;
	// || (Stride == 24 && IndexCount == 2538) // 衣服
	// || (Stride == 24 && IndexCount == 3234) // 头
	// || (Stride == 24 && IndexCount == 6546) // 胳臂
	// ;
}
//(
////(Stride == 12 && IndexCount > 3800) ||
//	(Stride == 12 && IndexCount == 14136) // 汽车
//	)
//void CheatItNew(ID3D11DeviceContext* pContext, ID3D11PixelShader* psSSS)
//{
//	if (ppDepthStencilState__New != NULL)
//	{
//		ppDepthStencilState__New->Release();
//		ppDepthStencilState__New = NULL;
//	}
//
//	if (iRed == 1)
//	{
//		bRed = false;
//	}
//	else if (iRed == 2)
//	{
//		if (gggg != timeGetTime() / INTVL)
//		{
//			bRed = !bRed;
//			gggg = timeGetTime() / INTVL;
//		}
//	}
//	else
//		bRed = true;
//
//	//if (cover != timeGetTime() / INTVL)
//	//{
//	// // bFlashIt = !bFlashIt;
//	// cover = timeGetTime() / INTVL;
//	//}
//
//	SYSTEMTIME st = { 0 };
//	GetLocalTime(&st);
//
//
//	//AutoShootIfCenter();
//	//SetEvent(g_Event_Shoot);
//
//	//pContext->PSSetShader(psYellow, NULL, NULL);
//	//ppDepthStencilState->GetDesc(&depthStencilDesc);
//
//	//效果不好，还影响性能，注释了先。还有一处
//	//if (1)
//	if (bTest2Draw || bInvertEveryFrame)
//	{
//		{
//			// Create the depth stencil state.
//			D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//			pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
//			ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
//
//			//depthStencilDesc.DepthEnable = TRUE;
//			//depthStencilDesc.DepthEnable = FALSE;
//			//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//			depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
//			//depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
//			//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//			//depthStencilDesc.StencilEnable = FALSE;
//			ID3D11Device *ppDevice;
//			pContext->GetDevice(&ppDevice);
//
//			pContext->PSGetShader(&pPixelShader__Old, NULL, NULL);
//			ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//			pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//			psSSS = psBack;
//			pContext->PSSetShader(psSSS, NULL, NULL);
//		}
//		//if (ppDepthStencilStateNew == NULL)
//	}
//	if ((bRed) && b2DShader)
//	{
//		//if ((Stride == 24 && IndexCount == 2070) // 头
//		// || (Stride == 24 && IndexCount == 3234) // 头
//		// || (Stride == 12 && IndexCount == 2877) // 头发
//		// || (Stride == 12 && IndexCount == 2868) // 头发
//		// || (Stride == 12 && IndexCount == 2637) // 三级盔 近处
//		// || (Stride == 12 && IndexCount == 1116) // 2级盔 近处
//		// || (Stride == 12 && IndexCount == 816) // ？盔 远处
//		// || (Stride == 12 && IndexCount == 192) // ？盔 远处
//		// || (Stride == 12 && IndexCount == 156) // ？盔 远处
//		// || (Stride == 12 && IndexCount == 180) // ？盔 远处
//		// || (Stride == 12 && IndexCount == 276) // ？盔 远处
//		// || (Stride == 12 && IndexCount == 294) // ？盔 远处
//		// )
//		//{
//		// pContext->PSSetShader(psBlue, NULL, NULL);
//		//}
//		//else 
//		//if (IsAvatar(Stride, IndexCount))
//		// pContext->PSSetShader(psSSS, NULL, NULL);
//		//else if (IsEquipment(Stride, IndexCount))
//		//{
//		// pContext->PSSetShader(psGreen, NULL, NULL);
//		//}
//		//else
//		// pContext->PSSetShader(psd, NULL, NULL);
//
//		//pContext->PSSetShader(psSSS, NULL, NULL);
//	}
//
//}
// void __stdcall CheatIt(ID3D11DeviceContext* pContext, UINT IndexCount, UINT IndexCountPerInstance, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
//{

// if (ppDepthStencilState__New != NULL)
// {
//	 ppDepthStencilState__New->Release();
//	 ppDepthStencilState__New = NULL;
// }

// if (iRed == 1)
// {
//	 bRed = false;
// }
// else if (iRed == 2)
// {
//	 if (gggg != timeGetTime() / INTVL)
//	 {
//		 bRed = !bRed;
//		 gggg = timeGetTime() / INTVL;
//	 }
// }
// else
//	 bRed = true;

// if (cover != timeGetTime() / INTVL)
// {
//	// bFlashIt = !bFlashIt;
//	 cover = timeGetTime() / INTVL;
// }

// SYSTEMTIME st = { 0 };
// GetLocalTime(&st);

// UINT Stride;
// ID3D11Buffer *veBuffer;
// UINT veBufferOffset = 0;
// pContext->IAGetVertexBuffers(0/*StartSlot*/, 1, &veBuffer, &Stride, &veBufferOffset);

// 
// MyTraceA("CheatIt**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);

// Send2Hwnd(IndexCount, Stride);

// //if ((Stride == 12) && (IndexCount > 7000) && (IndexCount < 10000))
// //{

//	// MyTraceA("CheatItBIGBIG**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);
//	// //		if (!bRed)
//	// //			return;
// //}


// //if (/*(Stride == iStride  && IndexCount == 2514)
// //	|| (Stride == iStride  && IndexCount == 2682)
// //	|| (Stride == iStride  && IndexCount == 2514)
// //	|| (Stride == iStride  && IndexCount == 2838)
// //	|| (Stride == iStride  && IndexCount == 2877)
// //	|| (Stride == iStride  && IndexCount == 3102)
// //	|| (Stride == iStride  && IndexCount == 3132)
// //	|| (Stride == iStride  && IndexCount == 3162)
// //	|| (Stride == iStride  && IndexCount == 3084)
// //	|| (Stride == iStride  && IndexCount == 14136)
// //	//|| (Stride == iStride && (IndexCount > 2500) && (IndexCount < 3163))
// //	|| */(Stride == 24))
// //if ((Stride != 8) && (Stride != 12))
// //if (
// //	((Stride == 24) && (IndexCount == iC24a) ) ||
// //	((Stride == 24) && (IndexCount == iC24b) ) ||
// //	((Stride == 24) && (IndexCount == iC24c) ) ||
// //	((Stride == 24) && (IndexCount == iC24d) ) ||
// //	((Stride == 24) && (IndexCount == iC24e) ) 
// //	)
// //if (Stride == iStride)
// //	return;
// /*	if (aaa.size() == 0)
// {
// return;
// }

// for (int i=0; i<aaa.size(); i++)
// {
// if (!(Stride == aaa[i] && IndexCount == bbb[i]))
// {
// aaa.push_back(Stride);
// bbb.push_back(IndexCount);
// break;
// }
// }
// if (Stride == aaa[iiidx] && IndexCount == bbb[iiidx])
// {
// return;
// */
// //if ((Stride == 24))
// {
//	 UINT IndexCountStride = IndexCount * 100 + Stride;
//	 if (find(lstAllStides.begin(), lstAllStides.end(), IndexCountStride) != lstAllStides.end()) {
//		 //找到
//	 }
//	 else {
//		 //没找到
//		 lstAllStides.push_back(IndexCountStride);
//	 }
// }

// if (
//	 IsNotIn_ExcludeList(Stride, IndexCount) &&
//	 (IsAvatar(Stride, IndexCount) || IsEquipment(Stride, IndexCount))
//	)									
// //if(iIndexCnt == IndexCount)
// {
//	 //if (
//	 //	IndexCount == lst24[iPos]
//	 //	//&& (IndexCount != iC24a)
//	 //	//&& (IndexCount != iC24b)
//	 //	//&& (IndexCount != iC24c)
//	 //	//&& (IndexCount != iC24d)
//	 //	//&& (IndexCount != iC24e)
//	 //	//&& (IndexCount != 5124)
//	 //	//&& (IndexCount != 1791)
//	 //	//&& (IndexCount != 975)
//	 //	//&& (IndexCount != 969)
//	 //	//&& (IndexCount != 948)
//	 //	//&& (IndexCount < 1000)
//	 //	//&& (IndexCount < 2000)
//	 //	|| (find(red24.begin(), red24.end(), IndexCount) != red24.end())
//	 //	)
//	 //{
//		 //MyTraceA("hkD3D11 CheatIt**********透明了****iPos=%d  Stride=%d  IndexCount=%d red24.size=%d", iPos, Stride, IndexCount, lstRed24.size());

//		 //return;
//		 //if (ppDepthStencilState__Old == NULL)
//		 {
//			 pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
//		 }

//		 ID3D11PixelShader* psSSS = psRed;
//		 if (bInvertEveryFrame)
//		 {
//			 //AutoShootIfCenter();
//			 //SetEvent(g_Event_Shoot);

//			 //pContext->PSSetShader(psYellow, NULL, NULL);
//			 //ppDepthStencilState->GetDesc(&depthStencilDesc);

//			 // Create the depth stencil state.
//			 //if (ppDepthStencilStateNew == NULL)
//			 {
//				 D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//				 ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
//				 
//				  //depthStencilDesc.DepthEnable = TRUE;
//				 //depthStencilDesc.DepthEnable = FALSE;
//				 //depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//				 depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
//				 //depthStencilDesc.StencilEnable = FALSE;
//				 ID3D11Device *ppDevice;
//				 pContext->GetDevice(&ppDevice);
//				 ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//				 pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//				 psSSS = psObscured;
//			 }
//			 //// Set the depth stencil state.
//			 //pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//			 //pContext->PSSetShader(psBlue, NULL, NULL);
//			 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
//			 //Hooks::oDrawIndexedInstanced(pContext, IndexCount, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);

//			 //{
//				// D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//				// ppDepthStencilState__New->GetDesc(&depthStencilDesc);

//				// //depthStencilDesc.DepthEnable = TRUE;
//				// //depthStencilDesc.DepthEnable = FALSE;
//				// //depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//				// depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
//				// //depthStencilDesc.StencilEnable = FALSE;
//				// ID3D11Device *ppDevice;
//				// pContext->GetDevice(&ppDevice);
//				// ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//				// pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//				// pContext->PSSetShader(psRed, NULL, NULL);
//			 //}
//			 //// Set the depth stencil state.
//			 //pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//			 //pContext->PSSetShader(psBlue, NULL, NULL);
//			 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
//			 //Hooks::oDrawIndexedInstanced(pContext, IndexCount, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
//		 }

//		 if ( (bRed))
//			 {
//				 if ((Stride == 24 && IndexCount == 2070) // 头
//					 || (Stride == 24 && IndexCount == 3234) // 头
//					 || (Stride == 12 && IndexCount == 2877) // 头发
//					 || (Stride == 12 && IndexCount == 2868) // 头发
//					 || (Stride == 12 && IndexCount == 2637) // 三级盔 近处
//					 || (Stride == 12 && IndexCount == 1116) // 2级盔 近处
//					 || (Stride == 12 && IndexCount == 816) // ？盔 远处
//					 || (Stride == 12 && IndexCount == 192) // ？盔 远处
//					 || (Stride == 12 && IndexCount == 156) // ？盔 远处
//					 || (Stride == 12 && IndexCount == 180) // ？盔 远处
//					 || (Stride == 12 && IndexCount == 276) // ？盔 远处
//					 || (Stride == 12 && IndexCount == 294) // ？盔 远处
//					 )
//				 {
//					 pContext->PSSetShader(psBlue, NULL, NULL);
//				 }
//				 else if (IsAvatar(Stride, IndexCount))
//					 pContext->PSSetShader(psSSS, NULL, NULL);
//				 else if (IsEquipment(Stride, IndexCount))
//				 {
//					 pContext->PSSetShader(psGreen, NULL, NULL);
//				 }
//				 else
//					 pContext->PSSetShader(psd, NULL, NULL);
//			 }
//		 //{
//		 //	{
//		 //		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//		 //		ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
//		 //		//depthStencilDesc.DepthEnable = 0;
//		 //		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//		 //		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
//		 //		ID3D11Device *ppDevice;
//		 //		pContext->GetDevice(&ppDevice);
//		 //		ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//		 //	}
//		 //	//// Set the depth stencil state.
//		 //	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//		 //}
//		 //if (Stride == 12)
//			// pContext->PSSetShader(psBlue, NULL, NULL);
//		 //if (Stride == 24)
//			// pContext->PSSetShader(psRed, NULL, NULL);
//		 //if (
//		 //	(IndexCount == 1128)
//		 //	|| (IndexCount == 1728)
//		 //	|| (IndexCount == 1842)
//		 //	|| (IndexCount == 1932)
//		 //	|| (IndexCount == 2556)
//		 //	|| (IndexCount == 3228)
//		 //	|| (IndexCount == 3234)
//		 //	|| (IndexCount == 5124)
//		 //	)
//		 //{
//		 //	pContext->PSSetShader(psc, NULL, NULL);
//		 //}
//		 //else
//		 //{
//			// //pContext->PSSetShader(psG, NULL, NULL);
//		 //}
//		 //&& (IndexCount != 1128)
//		 //&& (IndexCount != 1728)
//		 //&& (IndexCount != 1842)
//		 //&& (IndexCount != 1932)
//		 //&& (IndexCount != 2556)
//		 //&& (IndexCount != 3228)
//		 //&& (IndexCount != 3234)
//		 //&& (IndexCount != 5124)

//		 //pContext->OMGetDepthStencilState(&ppDepthStencilState, &pStencilRef);
//		 //ppDepthStencilState->GetDesc(&depthStencilDesc);

//		 //if (IndexCount == iC24a)
//		 //{
//		 //	//pContext->PSSetShader(psCrimson, NULL, NULL);
//		 //}
//		 //else if (IndexCount == iC24b)
//		 //{
//		 //	//pContext->PSSetShader(psYellow, NULL, NULL);
//		 //}
//		 //else if (IndexCount == iC24c)
//		 //{
//		 //	//pContext->PSSetShader(psc, NULL, NULL);
//		 //}
//		 //else if (IndexCount == iC24d)
//		 //{
//		 //	//pContext->PSSetShader(psd, NULL, NULL);
//		 //}
//		 //else if (IndexCount == iC24e)
//		 //{
//		 //	//pContext->PSSetShader(pse, NULL, NULL); //MY HEAD
//		 //}
//		 //else
//		 //{
//			// //pContext->PSSetShader(psG, NULL, NULL); //ENEMY
//		 //}
//		 //pContext->PSSetShaderResources(0, 1, &ShaderResourceView);

//		 //Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
//		 //pContext->OMSetDepthStencilState(myDepthStencilStates[ENABLED], 1);
//		 // Set the depth stencil state.
//		 //pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
//		 //ppDepthStencilStateNew->Release();
//		 //AddModel(pContext);//w2s

//		 return;

//		 //SetDepthStencilState(ENABLED);
//		 //UINT numViewports = 1; 
//		 //D3D11_VIEWPORT mViewport;
//		 //pContext->RSGetViewports(&numViewports, &mViewport);
//		 //mViewport.MinDepth = 0.0f;
//		 //mViewport.MaxDepth = 0.1f;
//		 //pContext->RSSetViewports(1, &mViewport);
//		 // Create the viewport.
//		 //m_deviceContext->RSSetViewports(1, &viewport);
//	 //}
// }
//// else if ((Stride == 12) && IndexCount<15000)
// {
//	 //{
//		// pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
//	 //}

//	 ////if (bFlashIt)
//	 //{
//		// //AutoShootIfCenter();
//		// //SetEvent(g_Event_Shoot);

//		// //pContext->PSSetShader(psYellow, NULL, NULL);
//		// //ppDepthStencilState->GetDesc(&depthStencilDesc);

//		// // Create the depth stencil state.
//		// //if (ppDepthStencilStateNew == NULL)
//		// {
//		//	 D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//		//	 ppDepthStencilState__Old->GetDesc(&depthStencilDesc);

//		//	 depthStencilDesc.DepthEnable = TRUE;
//		//	 depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//		//	 depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;

//		//	 //depthStencilDesc.StencilEnable = TRUE;

//		//	 ID3D11Device *ppDevice;
//		//	 pContext->GetDevice(&ppDevice);
//		//	 ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//		// }
//		// //// Set the depth stencil state.
//		// pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);

//	 //}
//	 //else
//	 //{
//	 //	{
//	 //		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
//	 //		ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
//	 //		//depthStencilDesc.DepthEnable = 0;
//	 //		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//	 //		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
//	 //		ID3D11Device *ppDevice;
//	 //		pContext->GetDevice(&ppDevice);
//	 //		ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
//	 //	}
//	 //	//// Set the depth stencil state.
//	 //	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
//	 //}
//	 //if (Stride == 12)
//		// pContext->PSSetShader(psBlue, NULL, NULL);
//	 //if (Stride == 24)
//		// pContext->PSSetShader(psRed, NULL, NULL);
// }
//}

void InitForHook(IDXGISwapChain* pSwapChain)
{
	Helpers::Log("DLL InitForHook。。。");
	hOutWnd = ::FindWindowExA(NULL, NULL, "#32770", "懵圈宝宝 哈哈哈");

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

	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.FrontCounterClockwise = true; //逆时针绕线方向 
	CCheat::pDevice->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	CCheat::pDevice->CreateRasterizerState(&cmdesc, &CWcullMode);

	cmdesc.CullMode = D3D11_CULL_NONE;
	//cmdesc.FillMode = D3D11_FILL_WIREFRAME;
	CCheat::pDevice->CreateRasterizerState(&cmdesc, &RSCullNone);
	//
	//cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FillMode = D3D11_FILL_WIREFRAME;
	CCheat::pDevice->CreateRasterizerState(&cmdesc, &RSCullWireFrame);

	cmdesc.CullMode = D3D11_CULL_NONE;
	cmdesc.FillMode = D3D11_FILL_SOLID;
	CCheat::pDevice->CreateRasterizerState(&cmdesc, &RSCullSolid);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CCheat::pDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;
	CCheat::pDevice->CreateDepthStencilState(&dssDesc, &DSLess);

	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	CCheat::pDevice->CreateDepthStencilState(&dssDesc, &DSGreatEqual);

	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	CCheat::pDevice->CreateDepthStencilState(&dssDesc, &DSGreat);

	//create font
	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hResult = pFW1Factory->CreateFontWrapper(CCheat::pDevice, L"Tahoma", &pFontWrapper);
	pFW1Factory->Release();

	// use the back buffer address to create the render target
	//if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&RenderTargetTexture))))

	//if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
	//{
	//	CCheat::pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
	//	RenderTargetTexture->Release();
	//}

	InitListFromFiles();
	//tmppp(CCheat::pContext);
	createTex(CCheat::pDevice, string("A01.dds"));
	//InitD2DScreenTexture();
	//_beginthread(Thread_fileWatcher, 0, NULL);
	_beginthread(Thread_KeysSwitch, 0, NULL);
}

HRESULT GenerateShader(ID3D11Device* pD3DDevice, ID3D11PixelShader** pShader, float r, float g, float b)
{
	char szCast[] = "struct VS_OUT\r\n"
		"{\r\n"
		" float4 Position : SV_Position;\r\n"
		" float4 Color : COLOR;\r\n"
		"};\r\n"

		"float4 ColorPixelShader( VS_OUT input ) : SV_Target\r\n"
		"{\r\n"
		" float4 fake;\r\n"
		" fake.a = 1.0f;\r\n"
		" fake.r = %.2f;\r\n"
		" fake.g = %.2f;\r\n"
		" fake.b = %.2f;\r\n"

		//" fake.a = input.Color.a;"
		//" fake.r = input.Color.r;"
		//" fake.g = input.Color.g;"
		//" fake.b = input.Color.b;"
		" return fake;\r\n"
		//"return input.Color;"
		"};\r\n";
	ID3D10Blob* pBlob = NULL;
	char szPixelShader[2000];

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
		//Helpers::LogFormat("\r\n%s", szPixelShader);
		//Helpers::LogFormat("D3DCompile hr = %x d3dErrorMsgBlob = %s", hr, *d3dErrorMsgBlob);
		//MyTraceA("D3DCompile hr = %x d3dErrorMsgBlob = %s", hr, *d3dErrorMsgBlob);
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

#define SHOOT_AREA_RADII  7

BOOL SaveDcToBMP(BYTE *pBmpBuffer,
	HBITMAP hbitmapSave,
	BITMAPINFO srcdibbmap,
	string sBmpPath)
{
	BOOL        bReturn = TRUE;
	HANDLE    hFile = NULL;  //handle of bitmap file which will be saved  
	DWORD     dwWritten = 0;     //written count  
	DWORD     dwOffsetSize = 0;
	DWORD     dwBufferSize = 0;
	WORD      wBitCount = 32;   //位图中每个像素所占字节数    
								//HBITMAP   hbitmapSave = NULL;
								//HBITMAP   hbitmapOld = NULL;
								//HDC       hDcDev = NULL;
	HDC       hDcMem = NULL;
	//BYTE      *pBmpBuffer = NULL;
	int       iBits = 0;
	BITMAP           stSrcbmp;
	//;
	BITMAPFILEHEADER bmFileHeader; //位图文件头结构  
								   //SecureZeroMemory(&stSrcbmp, sizeof(BITMAP));
								   ////SecureZeroMemory(&srcdibbmap, sizeof(BITMAPINFO));
	SecureZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	//// Fill bitmap information constructor  
	////srcdibbmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	////srcdibbmap.bmiHeader.biWidth = WVGA_WIDTH;
	////srcdibbmap.bmiHeader.biHeight = WVGA_HIGHT;
	////srcdibbmap.bmiHeader.biPlanes = 1;
	////srcdibbmap.bmiHeader.biBitCount = wBitCount;
	////srcdibbmap.bmiHeader.biCompression = BI_RGB;
	//hDcMem = CreateCompatibleDC(NULL);
	//if (NULL == hDcMem)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	//hbitmapSave = CreateDIBSection(hDcMem, &srcdibbmap, DIB_RGB_COLORS, (void**)&pBmpBuffer, NULL, 0);
	//if (NULL == hbitmapSave)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	//hbitmapOld = (HBITMAP)SelectObject(hDcMem, hbitmapSave);
	//if (NULL == hbitmapOld)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	////将传进来的DC画到定义的内存DC上去  
	//if (!StretchBlt(hDcMem,
	//	0,
	//	0,
	//	WVGA_WIDTH,
	//	WVGA_HIGHT,
	//	m_hdcMem,
	//	0,
	//	0,
	//	m_iScaleWidth,
	//	m_iScaleHight,
	//	SRCCOPY))
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}

	if (0 == GetObject(hbitmapSave, sizeof(stSrcbmp), &stSrcbmp))
	{
		bReturn = FALSE;
		goto Exit;
	}
	dwBufferSize = stSrcbmp.bmWidth * stSrcbmp.bmHeight * wBitCount / 8;
	dwOffsetSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	// Fill bitmap header constructor  
	bmFileHeader.bfType = 0x4D42;
	bmFileHeader.bfSize = dwOffsetSize + dwBufferSize;
	bmFileHeader.bfReserved1 = 0;
	bmFileHeader.bfReserved2 = 0;
	bmFileHeader.bfOffBits = dwOffsetSize;

	hFile = CreateFileA(sBmpPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, &srcdibbmap, sizeof(BITMAPINFO), &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, pBmpBuffer, dwBufferSize, &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	bReturn = TRUE;
Exit:
	if (INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	return bReturn;
}

static BOOL bDoneOnShoot = false;
UINT iBmpNamePreFix = 0;

bool IsCenterRed_Old()//DC截屏，效率差
					  //lpRect 代表选定区域
{
	bool bOK = false;
	//return;
	//Helpers::Log("==============AutoShootIfCenter");
	::GetWindowRect(g_hWnd, &g_lpRect);
	//Helpers::Log("IsCenterRed==============...0");

	RECT lpRect;
	int iW = g_lpRect.right - g_lpRect.left;
	int iH = g_lpRect.bottom - g_lpRect.top;
	int iCenterX = iW / 2 + g_lpRect.left;
	int iCenterY = iH / 2 + g_lpRect.top;

	lpRect.top = iCenterY - SHOOT_AREA_RADII;
	lpRect.bottom = iCenterY + SHOOT_AREA_RADII;
	lpRect.left = iCenterX - SHOOT_AREA_RADII;
	lpRect.right = iCenterX + SHOOT_AREA_RADII;

	HDC       hScrDC, hMemDC;
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;
	// 位图句柄
	int       nX, nY, nX2, nY2;
	// 选定区域坐标
	int       nWidth, nHeight;

	//Helpers::Log("IsCenterRed==============...1");
	// 确保选定区域不为空矩形
	if (IsRectEmpty(&lpRect))
		return false;
	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	//Helpers::Log("IsCenterRed==============...2");

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

	//Helpers::Log("IsCenterRed==============...3");

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

	//Helpers::Log("IsCenterRed==============...4");
	// 转换 COLORREF 为 RGB  
	//cOldColor = COLORREF2RGB(cOldColor);
	//cNewColor = COLORREF2RGB(cNewColor);
	// 替换颜色 
	string sFName = "..\\tmp\\" + to_string(iBmpNamePreFix++) + "_" + to_string(timeGetTime()) + "_Center_";
	if (ptPixels)
		SaveDcToBMP((BYTE *)ptPixels, DirectBitmap, RGB32BitsBITMAPINFO, sFName + ".bmp");

	/*Helpers::LogFormat("ptPixels[0] = %08x ", ptPixels[0]);*/
	ofstream outfile;
	outfile.open((sFName + ".raw").c_str(), ios::app);

	for (int i = ((nWidth * nHeight) - 1); i >= 0; i--)
	{
		if (!ptPixels)
		{
			//Helpers::Log("IsCenterRed==============...5555555555555555555");
			break;
		}
		if (outfile)
		{
			outfile << std::hex << std::setw(8) << std::setfill('0') << ptPixels[i] << " ";
			if (i % nWidth == 0)
			{
				outfile << std::endl;
			}
		}
		//ptPixels[i]; //0xff 29 27 21 红绿蓝
		//if (ptPixels[i] == 0xff800000)

		//Helpers::LogFormat("%08x ", ptPixels[i]);
		if ( /*(ptPixels[i] == 0xff000080)
			 ||*/(ptPixels[i] % 0x1000000 == 0x800000)
			|| (ptPixels[i] % 0x1000000 == 0x790000)
			|| (ptPixels[i] % 0x1000000 == 0x810000)
			)
		{
			//MyTraceA("+-+-+-+-%x 射击射击射击", ptPixels[i]);
			//::OutputDebugStringA("+-+-+-+-瞄准瞄准瞄准瞄准");
			Helpers::Log("==============+-+-+-+- 射击射击射击");
			bOK = true;

			break;
		}
		//	ptPixels[i] = cNewColor;
	}
	if (outfile)
	{
		outfile.close();
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	return bOK;
}

//--------------------------------------------------------------------------------------
// Helper function to capture a frame and dump it to disk 
//--------------------------------------------------------------------------------------
BYTE* CaptureFrame(int iRadii = 0, bool bToFile = false)
{
	//::GetWindowRect(g_hWnd, &g_lpRect);
	//RECT lpRect;
	//int iW = g_lpRect.right - g_lpRect.left;
	//int iH = g_lpRect.bottom - g_lpRect.top;
	//int iCenterX = iW / 2 + g_lpRect.left;
	//int iCenterY = iH / 2 + g_lpRect.top;

	//lpRect.top = iCenterY - SHOOT_AREA_RADII;
	//lpRect.bottom = iCenterY + SHOOT_AREA_RADII;
	//lpRect.left = iCenterX - SHOOT_AREA_RADII;
	//lpRect.right = iCenterX + SHOOT_AREA_RADII;

	BYTE* outBuffer = NULL;
	std::wstring wsBMP_Filename = L"strCaptureFilename_";
	wsBMP_Filename += std::to_wstring(iFrames);
	wsBMP_Filename += L".bmp";

	std::string wsRAW_Filename = "strCaptureFilename_";
	wsRAW_Filename += std::to_string(iFrames);
	wsRAW_Filename += ".raw.bmp";

	HRESULT hr = 0;
	ID3D11Device *d3d11Device;
	pMainContext->GetDevice(&d3d11Device);

	ID3D11RenderTargetView *ppRenderTargetViews11;
	pMainContext->OMGetRenderTargets(1, &ppRenderTargetViews11, 0);

	// Retrieve RT resource
	ID3D11Resource *pRTResource;
	ppRenderTargetViews11->GetResource(&pRTResource);

	// Retrieve a Texture2D interface from resource
	ID3D11Texture2D* RTTexture;
	pRTResource->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&RTTexture);

	// Check if RT is multisampled or not
	D3D11_TEXTURE2D_DESC    TexDesc;
	RTTexture->GetDesc(&TexDesc);

	////////////////////////////////////////////////////
	if (g_pCaptureTexture == NULL)
	{
		// We need a screen-sized STAGING resource for frame capturing
		D3D11_TEXTURE2D_DESC TexDesc2;
		DXGI_SAMPLE_DESC SingleSample = { 1, 0 };
		TexDesc2.Width = TexDesc.Width;
		TexDesc2.Height = TexDesc.Height;
		//TexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		TexDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		TexDesc2.SampleDesc = SingleSample;
		TexDesc2.MipLevels = 1;
		TexDesc2.Usage = D3D11_USAGE_STAGING;
		TexDesc2.MiscFlags = 0;
		TexDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		TexDesc2.BindFlags = 0;
		TexDesc2.ArraySize = 1;
		(d3d11Device->CreateTexture2D(&TexDesc2, NULL, &g_pCaptureTexture));
		//			DXUT_SetDebugName(g_pCaptureTexture, "Capture");
	}

	if (TexDesc.SampleDesc.Count > 1)// RT is multisampled, need resolving before dumping to disk
	{
		// Create single-sample RT of the same type and dimensions
		DXGI_SAMPLE_DESC SingleSample = { 1, 0 };//？？？？？？
		TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		TexDesc.MipLevels = 1;
		TexDesc.Usage = D3D11_USAGE_DEFAULT;
		TexDesc.CPUAccessFlags = 0;
		TexDesc.BindFlags = 0;
		TexDesc.SampleDesc = SingleSample;

		ID3D11Texture2D *pSingleSampleTexture;
		d3d11Device->CreateTexture2D(&TexDesc, NULL, &pSingleSampleTexture);
		//DXUT_SetDebugName(pSingleSampleTexture, "Single Sample");

		pMainContext->ResolveSubresource(pSingleSampleTexture, 0, RTTexture, 0, TexDesc.Format);

		////////////////////////////////////////////////////
		// Copy RT into STAGING texture
		pMainContext->CopyResource(g_pCaptureTexture, pSingleSampleTexture);
		SAFE_RELEASE(pSingleSampleTexture);
		//hr = D3DX11SaveTextureToFile(pMainContext, g_pCaptureTexture, D3DX11_IFF_BMP, www.c_str());
	}
	else// Single sample case
	{
		// Copy RT into STAGING texture
		pMainContext->CopyResource(g_pCaptureTexture, pRTResource);
	}


	{
		////////////////////////////////
		// Copy image into GDI drawing texture
		//lImmediateContext->CopyResource(g_pCaptureTexture, lAcquiredDesktopImage);
		//lAcquiredDesktopImage.Release();
		//lDeskDupl->ReleaseFrame();

		// Copy GPU Resource to CPU
		D3D11_TEXTURE2D_DESC desc;
		g_pCaptureTexture->GetDesc(&desc);
		D3D11_MAPPED_SUBRESOURCE resource;
		UINT subresource = D3D11CalcSubresource(0, 0, 0);
		pMainContext->Map(g_pCaptureTexture, subresource, D3D11_MAP_READ, 0, &resource);

		UINT lBmpRowPitch = TexDesc.Width * 4;
		//std::unique_ptr<BYTE> pBuf(new BYTE[lBmpRowPitch*desc.Height]);
		BYTE* pBuf = new BYTE[lBmpRowPitch*desc.Height];

		BYTE* srcData = reinterpret_cast<BYTE*>(resource.pData);
		BYTE* dstData = pBuf + lBmpRowPitch*(desc.Height - 1);
		UINT minRowPitch = std::min<UINT>(lBmpRowPitch, resource.RowPitch);

		for (size_t h = 0; h < TexDesc.Height; ++h)
		{//GPU 16字节对齐的原因，实际图像需要按行复制，且只复制前面需要的数据长度
			memcpy_s(dstData, lBmpRowPitch, srcData, minRowPitch);
			srcData += resource.RowPitch;
			dstData -= lBmpRowPitch;
		}

		pMainContext->Unmap(g_pCaptureTexture, subresource);
		long g_captureSize = minRowPitch*desc.Height;

		DWORD dOutBufLen = iRadii * 2 * iRadii * 2 * 4;
		int nW = iRadii * 2;
		int hLeft = TexDesc.Width / 2 - iRadii;

		int hTop = TexDesc.Height / 2 - iRadii;
		int hBottom = TexDesc.Height / 2 + iRadii;

		if (dOutBufLen == 0)
		{
			dOutBufLen = g_captureSize;
			hTop = 0;
			hBottom = TexDesc.Height;
			hLeft = 0;
			nW = TexDesc.Width;
		}
		outBuffer = new BYTE[dOutBufLen];
		memset(outBuffer, 0, dOutBufLen);
		//Copying to BYTE buffer 
		//memcpy(outBuffer, (void*)pBuf, g_captureSize);
		//Helpers::LogFormat("-----------------iFrames = %d < %d >,< %d >, ", iFrames, hTop, hBottom);
		for (size_t h = hTop; h < hBottom; ++h)
		{
			BYTE* aa = outBuffer + ((h - hTop)*nW * 4);
			BYTE* bb = (BYTE*)(pBuf + (h*minRowPitch) + (hLeft * 4));
			//Helpers::LogFormat("memcpy (0x%08x ,0x%08x, %d), %d, %d, %d, %d, %d, %d, %d, %d", aa, bb, nW * 4,
			//	*(BYTE*)(bb),
			//	*(BYTE*)(bb + 1),
			//	*(BYTE*)(bb + 2),
			//	*(BYTE*)(bb + 3),
			//	*(BYTE*)(bb + 4),
			//	*(BYTE*)(bb + 5),
			//	*(BYTE*)(bb + 6),
			//	*(BYTE*)(bb + 7)
			//);
			memcpy(aa, bb, nW * 4);
			/*Helpers::LogFormat("memcpy2(0x%08x ,0x%08x, %d), %d, %d, %d, %d, %d, %d, %d, %d", aa, bb, nW * 4,
			*(BYTE*)(aa),
			*(BYTE*)(aa + 1),
			*(BYTE*)(aa + 2),
			*(BYTE*)(aa + 3),
			*(BYTE*)(aa + 4),
			*(BYTE*)(aa + 5),
			*(BYTE*)(aa + 6),
			*(BYTE*)(aa + 7)
			);*/
		}
		//ofstream fout( ( wsRAW_Filename + "00.bmp").c_str(), ios::out | ios::binary);
		//fout.write((char*)outBuffer, (dOutBufLen));
		//fout.close();

		//BGRA 格式, 且图像是倒的

		if (bToFile)
		{
			ofstream fout(wsRAW_Filename.c_str(), ios::out | ios::binary);
			fout.write((char*)pBuf, g_captureSize);
			fout.close();

			hr = D3DX11SaveTextureToFile(pMainContext, g_pCaptureTexture, D3DX11_IFF_BMP, wsBMP_Filename.c_str());
		}

		////////////////////////////////

		delete[](BYTE*) pBuf;
		//delete[] (BYTE*) outBuffer;
	}
	DWORD dwErr = GetLastError();
	SAFE_RELEASE(RTTexture);

	SAFE_RELEASE(pRTResource);
	SAFE_RELEASE(g_pCaptureTexture);
	return outBuffer;
}

bool IsCenterRed()
{//lpRect 代表选定区域
	bool bOK = false;
	UINT * ptPixels = nullptr;

	//捕获屏幕选定区域
	ptPixels = (UINT *)CaptureFrame(SHOOT_AREA_RADII, false);

	//对内存进行颜色比对
	//// 选定区域坐标
	int       nWidth = SHOOT_AREA_RADII * 2;
	int       nHeight = SHOOT_AREA_RADII * 2;
	Helpers::LogFormat(" ptPixels = 0x%08x  nWidth %d, nHeight %d", ptPixels, nWidth, nHeight);

	//return;
	//Helpers::Log("==============AutoShootIfCenter");
	//Helpers::Log("IsCenterRed==============...0");

	//HDC       hScrDC, hMemDC;
	//// 屏幕和内存设备描述表
	//HBITMAP    hBitmap, hOldBitmap;
	//// 位图句柄
	//int       nX, nY, nX2, nY2;

	////Helpers::Log("IsCenterRed==============...1");
	//// 确保选定区域不为空矩形
	//if (IsRectEmpty(&lpRect))
	//	return false;
	////为屏幕创建设备描述表
	//hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	////Helpers::Log("IsCenterRed==============...2");

	////为屏幕设备描述表创建兼容的内存设备描述表
	//hMemDC = CreateCompatibleDC(hScrDC);
	//// 获得选定区域坐标
	//nX = lpRect.left;
	//nY = lpRect.top;
	//nX2 = lpRect.right;
	//nY2 = lpRect.bottom;


	////确保选定区域是可见的
	//if (nX < 0)
	//	nX = 0;
	//if (nY < 0)
	//	nY = 0;
	////if (nX2 > m_xScreen)
	////	nX2 = m_xScreen;
	////if (nY2 > m_yScreen)
	////	nY2 = m_yScreen;
	//nWidth = nX2 - nX;
	//nHeight = nY2 - nY;
	//// 创建一个与屏幕设备描述表兼容的位图
	////hBitmap = CreateCompatibleBitmap
	////(hScrDC, nWidth, nHeight);

	//// 初始化BITMAPINFO信息，以便使用CreateDIBSection
	//BITMAPINFO RGB32BitsBITMAPINFO;
	//ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
	//RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//RGB32BitsBITMAPINFO.bmiHeader.biWidth = nWidth;
	//RGB32BitsBITMAPINFO.bmiHeader.biHeight = nHeight;
	//RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
	//RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

	//HBITMAP DirectBitmap = CreateDIBSection(hMemDC,
	//	(BITMAPINFO *)&RGB32BitsBITMAPINFO,
	//	DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);

	////Helpers::Log("IsCenterRed==============...3");

	//// 把新位图选到内存设备描述表中
	//hOldBitmap = (HBITMAP)SelectObject(hMemDC, DirectBitmap);
	//// 把屏幕设备描述表拷贝到内存设备描述表中
	////if (bSave)
	////{
	////	CDC dcCompatible;
	////	dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
	////	dcCompatible.SelectObject(m_pBitmap);

	////	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
	////		dcCompatible, nX, nY, SRCCOPY);
	////}
	////else
	//{
	//	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
	//		hScrDC, nX, nY, SRCCOPY);
	//}

	////Helpers::Log("IsCenterRed==============...4");
	//// 转换 COLORREF 为 RGB  
	////cOldColor = COLORREF2RGB(cOldColor);
	////cNewColor = COLORREF2RGB(cNewColor);
	//// 替换颜色 
	//string sFName = "..\\tmp\\" + to_string(iBmpNamePreFix++) + "_" + to_string(timeGetTime()) + "_Center_";
	//if (ptPixels)
	//	SaveDcToBMP((BYTE *)ptPixels, DirectBitmap, RGB32BitsBITMAPINFO, sFName + ".bmp");

	///*Helpers::LogFormat("ptPixels[0] = %08x ", ptPixels[0]);*/
	//ofstream outfile;
	//outfile.open((sFName + ".raw").c_str(), ios::app);

	for (int i = ((nWidth * nHeight) - 1); i >= 0; i--)
	{
		if (!ptPixels)
		{
			//Helpers::Log("IsCenterRed==============...5555555555555555555");
			break;
		}
		//Helpers::LogFormat(" 0x%08x", ptPixels[i]);
		//if (outfile)
		//{
		//	outfile << std::hex << std::setw(8) << std::setfill('0') << ptPixels[i] << " ";
		//	if (i % nWidth == 0)
		//	{
		//		outfile << std::endl;
		//	}
		//}
		//ptPixels[i]; //0xff 29 27 21 红绿蓝
		//if (ptPixels[i] == 0xff800000)

		//Helpers::LogFormat("ptPixels[i]=%08x ", ptPixels[i]);
		if ( /*(ptPixels[i] == 0xff000080)
			 ||*/
			(ptPixels[i] % 0x1000000 == 0x80)
			|| (ptPixels[i] % 0x1000000 == 0x79)
			|| (ptPixels[i] % 0x1000000 == 0x81)
			|| (ptPixels[i] % 0x1000000 == 0x800000)
			|| (ptPixels[i] % 0x1000000 == 0x790000)
			|| (ptPixels[i] % 0x1000000 == 0x810000)
			)
		{
			//MyTraceA("+-+-+-+-%x 射击射击射击", ptPixels[i]);
			//::OutputDebugStringA("+-+-+-+-瞄准瞄准瞄准瞄准");
			Helpers::LogFormat("==============+-+-+-+- 射击射击射击 0x%08x", ptPixels[i]);
			bOK = true;

			break;
		}
		//	ptPixels[i] = cNewColor;
	}
	//if (outfile)
	//{
	//	outfile.close();
	//}

	//hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	////得到屏幕位图的句柄
	////清除 
	//DeleteDC(hScrDC);
	//DeleteDC(hMemDC);

	if (ptPixels)
	{
		delete[](BYTE*)ptPixels;
	}
	return bOK;
}

UINT64 lastCount = 0;
UINT64 lastWIDTH = 0;
HRESULT __stdcall Hooks::hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	iFrames++;

	if (lstLogStrides.size() > 0)
	{
		UINT64 uCount = 0;
		for (int i = 0; i < lstLogStrides.size(); i++)
		{
			uCount += lstLogStrides.at(i) ;
		}
		if (uCount != lastCount)
		{
			sort(lstLogStrides.begin(), lstLogStrides.end());
			ofstream outfile;
			outfile.open(("..\\" + to_string(iFrames) + "_Frame.log").c_str(), ios::app);
			for (int i = 0; i < lstLogStrides.size(); i++)
			{
				//if ((lstAllStides.at(i) % 100) == 12)
				{
					if (!outfile)
					{
						std::cout << "打开文件失败！" << endl;
					}
					else
					{
						outfile << lstLogStrides.at(i) << std::endl;
					}
				}
			}
			if (outfile)		outfile.close();
			lastCount = uCount;
		}

		lstLogStrides.clear();
	}

	if (bShoot)
	{
		std::cout << "hkD3D11Present =======>> PulseEvent(g_Event_Shoot)" << std::endl;
		PulseEvent(g_Event_Shoot);
	}
	//SaveMapToFile();

	if (bLog2Txt_DOWN)
	{
		//outfile.open("..\\Log2Txt.txt", ios::app);
		ofstream outFile;
		//outFile.open("..\\3632_" + to_string(iFrames) + ".txt", ios::app);
		outFile.open("..\\FrameOf_" + to_string(iFrames) + ".txt", ios::app);
		if (!outFile)
		{
			std::cout << "打开Frame***文件失败！" << endl;
		}
		else
		{
			//std::map<string, string>::iterator it;// = mapIconIdx.find(sKey.c_str());
			//for (it = mapBuf.begin(); it != mapBuf.end(); ++it)
			//{
			//	outFile << (*it).first << std::endl;
			//	outFile << (*it).second << std::endl;
			//}
			//string text = g_ssCallsInFrame.str();
			outFile << g_ssCallsInFrame.str().c_str() << std::endl;
			outfile.close();
		}
	}
	bLog2Txt_DOWN = false;
	mapBuf.clear();
	g_ssCallsInFrame.str("");
	g_ssCallsInFrame.clear();

	//if (minX2 == 0 && minY2 == 0 && maxX2 == 0 && maxY2 == 0)
	//{
	//	minX2 = minX;
	//	minY2 = minY;
	//	maxX2 = maxX;
	//	maxY2 = maxY;
	//	g_lstPositions2 = g_lstPositions;
	//}
	//g_lstPositions.clear();
	//minX = 0;
	//minY = 0;
	//maxX = 0;
	//maxY = 0;

	if (bLog2Txt_F7)
	{
		//;
		//if (outfile.is_open())
		//{
		//	outfile.close();
		//}
		//
		//outfile.open("..\\xFrame_" + to_string(iName++), ios::app);
		//if (!outfile)
		//{
		//	std::cout << "打开Log2Txt.txt文件失败！" << endl;
		//}
		//else
		//{
		//}
	}
	//Helpers::LogFormat("hkD3D11Present+++++++-------------------------------- bUp = %d", bFlashIt);
	DWORD bgtime = timeGetTime();
	if (!bCheat)
	{
		return Hooks::oPresent(pSwapChain, SyncInterval, Flags);
	}

	//Helpers::LogAddress("\r\n hkD3D11Present++++++++++++++++++++*===");
	//bInvertEveryFrame = !bInvertEveryFrame;

	//RECT lpRect;
	//int iW = g_lpRect.right - g_lpRect.left;
	//int iH = g_lpRect.bottom - g_lpRect.top;
	//int iCenterX = iW / 2 + g_lpRect.left;
	//int iCenterY = iH /2 + g_lpRect.top;

	//lpRect.top = iCenterY - 20;
	//lpRect.bottom = iCenterY + 20;
	//lpRect.left = iCenterX - 20;
	//lpRect.right = iCenterX + 20;
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

	if (!bInited)
	{
		InitForHook(pSwapChain);

		//Helpers::Log("D3D11Present initialised");
		bInited = true;
	}

	//viewport
	CCheat::pContext->RSGetViewports(&vps, &viewport);
	ScreenCenterX = viewport.Width / 2.0f;
	ScreenCenterY = viewport.Height / 2.0f;
	HRESULT hr = S_OK;
	if (!psCrimson)
		hr = GenerateShader(CCheat::pDevice, &psCrimson, 1.f, 0.2f, 0.2f);
	if (!psYellow)
		hr = GenerateShader(CCheat::pDevice, &psYellow, 1.f, 0.6f, 0);
	if (!psRed)
		hr = GenerateShader(CCheat::pDevice, &psRed, 0.5f, 0.0f, 0.0f);
	if (!psGreen)
		hr = GenerateShader(CCheat::pDevice, &psGreen, 0.0f, 0.5f, 0.0f);
	if (!psBlue)
		hr = GenerateShader(CCheat::pDevice, &psBlue, 0.0f, 0.0f, 0.5f);
	if (!psBack)
		hr = GenerateShader(CCheat::pDevice, &psBack, 0.6f, 0.6f, 0.5f);

	RGB3 ccc;
	ccc.r = 0.94f;
	ccc.g = 0.78f;
	ccc.b = 0.01f;

	//dFrontColor = round(ccc.r*255)*0x10000 +
	//	round(ccc.g * 255) * 0x100 +
	//	round(ccc.b * 255) ;
	if (!psFront)
		hr = GenerateShader(CCheat::pDevice, &psFront, ccc.r, ccc.g, ccc.b);
	//hr = GenerateShader(CCheat::pDevice, &psFront, 0.5f, 0.0f, 0.0f);

	if (S_OK == hr)
	{
		pssrStartSlot = 1;
	}
	//Helpers::Log2Txt("hkD3D11Present++++++++++++++++++++*=== 1 usedTime = ", timeGetTime() - bgtime);

	if (lastWIDTH != (int)viewport.Width)
	{
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			SAFE_RELEASE(RenderTargetView);
			CCheat::pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
		}

		lastWIDTH = (int)viewport.Width;
	}
	//call before you draw
	CCheat::pContext->OMSetRenderTargets(/*1*/vps, &RenderTargetView, NULL); //?????? 1 
																			 //draw
																			 //if (pFontWrapper)
																			 //{
																			 //pFontWrapper->DrawString(CCheat::pContext, L"Who are youuuuuuuuuuuuuuu?", 14, 16.0f, 16.0f, 0xffff1612, FW1_RESTORESTATE);
																			 //pFontWrapper->DrawString(CCheat::pContext, L"Welcome Back Bscan*****============================", 14.0f, 16.0f, 30.0f, 0xffffffff, FW1_RESTORESTATE);

																			 //std::string sData = std::to_string(iStride);
																			 //sData += "_";
																			 //sData += std::to_string(iIndexCount);
																			 //
																			 //pFontWrapper->DrawString(CCheat::pContext, StringToWString(sData).c_str(), 18.0f, ScreenCenterX, ScreenCenterY, 0xff00ff00, FW1_RESTORESTATE);
																			 //Helpers::Log("D3D11Present pFontWrapper->DrawString \"Who are youuuuuuuuuuuuuuu ? \"");
																			 //}

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
																			 /*	if (aimbot == 1 && AimEspInfo.size() != NULL)
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

																			 //OutputDebugStringA("hkD3D11Present+++++++-BestTarget???");
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
																			 */
																			 //ID3D11Texture2D *BackBuffer110;
																			 //hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer110);

																			 //std::wstring wwwW = L"strCaptureFilename_";
																			 //wwwW += std::to_wstring(iFrames);
																			 //wwwW += L".PNG";

																			 //if (pMainContext)
																			 //{
																			 //	hr = D3DX11SaveTextureToFile(pMainContext, BackBuffer110, D3DX11_IFF_PNG, wwwW.c_str());
																			 //}

																			 //CaptureFrame();
																			 //if (IsCenterRed())
																			 //	Helpers::LogFormat("hkD3D11Present +++++++++++++++红+++++色了+++++ ");

	if (bCrossDraw)
	{
		PulseEvent(g_Event_CrossDraw);
	}

	//if (bVideo4Rec_SCROL)
	{
		wstring www = L"Stride=";
		www += std::to_wstring(g_iCurStride);
		www += L" IndexCount=";
		www += std::to_wstring(g_iCurIndexCount);
		www += L" Pos=";
		www += std::to_wstring(iPos);
		www += L"/";
		www += std::to_wstring(lstAllStrides.size());
		if (pFontWrapper)
		{
			pFontWrapper->DrawString(CCheat::pContext, www.c_str(), 40, 16.0f, 36.0f, 0xffff1612, FW1_RESTORESTATE);
			if (bVideo4Rec_PAUSE)
			{
				pFontWrapper->DrawString(CCheat::pContext, L"如果中心红，或者红区是要排除的对象，按'PageUp[原色]/Down[绿色]'保存，否则'上下键'继续找", 30, 16.0f, 86.0f, 0xff1612ff, FW1_RESTORESTATE);
			}
		}
	}

	if (bVideo4Rec_SCROL && (lstAllStrides.size()>0) && !bVideo4Rec_PAUSE)
	{
		Helpers::LogFormat("hkD3D11Present 一帧查红色同步+++++++++++++");
		D3D11_QUERY_DESC pQueryDesc;
		pQueryDesc.Query = D3D11_QUERY_EVENT;
		pQueryDesc.MiscFlags = 0;
		ID3D11Query *pEventQuery;
		CCheat::pDevice->CreateQuery(&pQueryDesc, &pEventQuery);

		CCheat::pContext->End(pEventQuery); // 在 pushbuffer 中插入一个篱笆
		while (CCheat::pContext->GetData(pEventQuery, NULL, 0, 0) == S_FALSE) {} // 自旋等待事件结束

		Sleep(100);//等渲染的延迟
		Helpers::LogFormat("hkD3D11Present 一帧查红色开始+++++++++++++");
		if (!IsCenterRed() || (iiiii == 0))
		{//当前帧，中心不是红色
			if (lstAllStrides.size() > 0)
			{
			nnnn:
				if (iPos >= lstAllStrides.size())
				{
					iPos = 0;
				}

				iiiii = lstAllStrides.at(iPos);
				g_iCurStride = iiiii % 100;
				g_iCurIndexCount = iiiii / 100;;
				//Helpers::LogFormat("%d %d-%d %d %ld", iPos, iStride, iIndexCount, lstAll2412.size(), lstAll2412.at(iPos));
				iPos++;
				if (!IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount))
				{
					goto nnnn;
				}
			}
		}
		else if (IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount)) //IsCenterRed()
		{//当前帧，中心是红色，且不在排除列表内

			bVideo4Rec_PAUSE = true;
			//{
			//	wstring www = L"Stride=";
			//	www += std::to_wstring(g_iCurStride);
			//	www += L" IndexCount=";
			//	www += std::to_wstring(g_iCurIndexCount);
			//	if (pFontWrapper)
			//	{
			//		//pFontWrapper->DrawString(CCheat::pContext, www.c_str(), 40, 16.0f, 36.0f, 0xffff1612, FW1_RESTORESTATE);
			//		pFontWrapper->DrawString(CCheat::pContext, L"如果中心红，或者红区是要排除的对象，按'ENTER'保存，否则'上下键'继续找", 30, 16.0f, 86.0f, 0xff1612ff, FW1_RESTORESTATE);
			//	}
			//}

			Helpers::LogFormat("hkD3D11Present 红色了+++++ iStride=%d iIndexCount=%d i=%d l=%d ==%ld", g_iCurStride, g_iCurIndexCount, iPos, lstAllStrides.size(), iiiii);
			//Sleep(5000);
			//if (IsCenterRed())
			//{
			//	Helpers::LogFormat("Sleep(5000);hkD3D11Present 红色了+++++ iStride=%d iIndexCount=%d i=%d l=%d ==%ld", g_iCurStride, g_iCurIndexCount, iPos, lstAllStrides.size(), iiiii);
			//	Append2ExcludeLst();
			//	bVideo4Rec_SCROL = false;
			//	bVideo4Rec_PAUSE = false;
			//}
			//keybd_event(VK_SNAPSHOT, 0, 0, 0);

			//ofstream outfile;
			//outfile.open(g_NotRedListFName.c_str(), ios::app);
			//if (!outfile)
			//{
			//	std::cout << "打开文件失败！" << g_NotRedListFName.c_str() << endl;
			//}
			//else if (iiiii > 0)
			//{
			//	outfile << std::dec << iiiii << std::endl;
			//	outfile.close();
			//	std::cout << std::dec << iiiii << " 写入文件完成！" << g_NotRedListFName.c_str() << endl;

			//	InitListFromFiles();
			//}
		}
		else
		{
			bVideo4Rec_PAUSE = true;
			//InitListFromFiles();
			Helpers::LogFormat("hkD3D11Present !!!!!!错误 iStride=%d iIndexCount=%d ", g_iCurStride, g_iCurIndexCount);
		}

		pEventQuery->Release();

		Helpers::LogFormat("hkD3D11Present 一帧查红色结束+++++++++++++ iStride=%d iIndexCount=%d (%d/%d)", g_iCurStride, g_iCurIndexCount, iPos, lstAllStrides.size());
	}

	//(UINT *)CaptureFrame(SHOOT_AREA_RADII, true);
	Hooks::oPresent(pSwapChain, SyncInterval, Flags);

	//iFrames++;
	//(UINT *)CaptureFrame(SHOOT_AREA_RADII, true);

	//iFrames += 100;

	if (minX2 == 0 && minY2 == 0 && maxX2 == 0 && maxY2 == 0)
	{
		minX2 = minX;
		minY2 = minY;
		maxX2 = maxX;
		maxY2 = maxY;
		g_lstPositions2 = g_lstPositions;
		g_iSelfIdx2 = g_iSelfIdx;
	}
	g_lstPositions.clear();
	minX = 0;
	minY = 0;
	maxX = 0;
	maxY = 0;
	g_iSelfIdx = -1;

	if (bHideOne)
	{
		bHideOne = !bHideOne;
		ofstream outfile;

		outfile.open("..\\AllStride.txt", ios::app);
		for (int i = 0; i<lstAllStrides.size(); i++)
		{
			//if ((lstAllStides.at(i) % 100) == 12)
			{
				if (!outfile)
				{
					std::cout << "打开文件失败！" << endl;
				}
				else
				{
					outfile << lstAllStrides.at(i) << std::endl;
				}
			}
		}
		if (outfile)		outfile.close();


		if (lstAllStrides.size() > 0)
		{
			if (iPosHide >= lstAllStrides.size())
			{
				iPosHide = 0;
			}

			iiiii = lstAllStrides.at(iPosHide);
			g_iCurStride = iiiii % 100;
			g_iCurIndexCount = iiiii / 100;;

			Helpers::LogFormat("hkD3D11Present bHideOne i=%d/%d (%d-%d) %ld", iPosHide, lstAllStrides.size(), g_iCurStride, g_iCurIndexCount, lstAllStrides.at(iPosHide));
			iPosHide++;
		}
	}

	//Helpers::Log2Txt("hkD3D11Present++++++++++++++++++++*=== 2 usedTime = ", timeGetTime() - bgtime);

	return S_OK;
}

void __stdcall Hooks::hkD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer */*const*/ *ppConstantBuffers)
{
	//Helpers::LogAddress("\r\n hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	//OutputDebugStringA("hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	//works ok in ut4 alpha only
	//if (Stride == 40 || Stride == 44) //ut4 models
	//{
	//AddModel(pContext);//w2s
	//}
	//MyTraceA("hkD3D11VSSetConstantBuffers**************pContext=%x StartSlot=%d NumBuffers=%d ppConstantBuffers=%x ", pContext, StartSlot, NumBuffers, ppConstantBuffers);

	//if ((NumBuffers == 1) && ppConstantBuffers)
	//{
	//	D3D11_BUFFER_DESC pDesc;
	//	if ((*ppConstantBuffers))
	//	{
	//		(*ppConstantBuffers)->GetDesc(&pDesc);
	//		if (pDesc.ByteWidth == 3632)
	//			//Helpers::Log("VSSetConstantBuffers 3632 --------------->>>>>>>>>>>>>>> ");
	//			Helpers::LogAddress("VSSetConstantBuffers 3632 ---------------> ", reinterpret_cast<int64_t>(*ppConstantBuffers));
	//	}
	//}



	return Hooks::oVSSetConstantBuffers(pContext, StartSlot, NumBuffers, ppConstantBuffers);
}
ID3D11ShaderResourceView *tmpTextureSRV = NULL;
void __stdcall Hooks::hkD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
	g_StartSlot = StartSlot;
	//	//MyTraceA("hkD3D11PSSetShaderResources==> pContext=%08x, StartSlot=%d, NumViews=%d, ppShaderResourceViews=%08x", pContext, StartSlot, NumViews, ppShaderResourceViews);
	//	//pssrStartSlot = StartSlot;
	//	ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[0];
	//	if (pShaderResView)
	//	{
	//		pShaderResView->GetDesc(&Descr);
	//	}
	//	//get pscdesc.ByteWidth
	//	pContext->PSGetConstantBuffers(StartSlot, 1, &pcsBuffer);
	//	if (pcsBuffer)
	//		pcsBuffer->GetDesc(&pscdesc);
	//	if (pcsBuffer != NULL) { pcsBuffer->Release(); pcsBuffer = NULL; }
	//
	//	//(pscdesc.ByteWidth == 224 && Descr.Format == 71)
	//	UINT Stride = 0;
	//	ID3D11Buffer *veBuffer;
	//	UINT veBufferOffset = 0;
	//	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	//	if ((pssrStartSlot == StartSlot))
	//	{
	//
	//		//Helpers::LogFormat("hkD3D11PSSetShaderResources==> NumViews=%d, ppSRViews=%08x pscdesc.BW=%d, Descr.F=%d, Descr.V=%d, StartSlot=%d", NumViews, ppShaderResourceViews, pscdesc.ByteWidth, Descr.Format, Descr.ViewDimension, StartSlot);
	//		//Helpers::LogFormat("pssrStartSlot=[%d] pssrStride=[[ %d ]] curStride=[%d] size()=[%d]", StartSlot, pssrStride, Stride, lstAllStride00.size());
	//
	//		if (find(lstAllStride00.begin(), lstAllStride00.end(), Stride) != lstAllStride00.end()) {
	//			//找到
	//		}
	//		else {
	//			//没找到
	//
	//			lstAllStride00.push_back(Stride);
	//		}
	//	}
	//
	//	if (((1 == StartSlot) || (2 == StartSlot) || (pssrStartSlot == StartSlot))
	//		&&
	//		((Stride == 12) ||(Stride == 24))
	//		&&
	//		(Descr.Format==28 && Descr.ViewDimension==4)//28,4 10,9
	//		)//不显示它的贴图
	//	//if ((pssrStartSlot == StartSlot)) 
	//	{
	//		//Helpers::LogFormat("不显示它的贴图..........curStartSlot=[%d] curStride=[%d] pssrStride=[[ %d ]] size()=[%d]", StartSlot, Stride, pssrStride, lstAllStride00.size());
	//		//Helpers::LogFormat("不显示它的贴图==> NVs=%d, SRVs=%08x BW=%d, F=%d, V=%d, StartSlot=%d", NumViews, ppShaderResourceViews, pscdesc.ByteWidth, Descr.Format, Descr.ViewDimension, StartSlot);
	//		//tmppp(pContext, StartSlot, NumViews);
	//		//if (d2dTexture)
	//		//{
	//		//	//Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, (ID3D11ShaderResourceView *const *)d2dTexture);
	//		//}
	//		//pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
	//
	//		////if (ppDepthStencilStateNew == NULL)
	//		//{
	//		//	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//		//	ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
	//
	//		//	//depthStencilDesc.DepthEnable = TRUE;
	//		//	//depthStencilDesc.DepthEnable = FALSE;
	//		//	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//		//	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	//		//	//depthStencilDesc.StencilEnable = FALSE;
	//		//	ID3D11Device *ppDevice;
	//		//	pContext->GetDevice(&ppDevice);
	//		//	ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
	//		//	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
	//		//}
	//
	//		//pContext->PSSetShader(psRed, NULL, NULL);
	//	}
	//	else
	//Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);

	if (StartSlot == 0 || (!bCheat) || (b2DShader))
	{
		tmpTextureSRV = *ppShaderResourceViews;
		Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);
	}
	else
	{
		//createTex(CCheat::pDevice, string("A01.dds"));
		//Helpers::LogFormat("pTextureSRV (=%d) ", pTextureSRV);

		if (tmpTextureSRV)
		{
			Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, &tmpTextureSRV);
		}
	}

	return;
}

void __stdcall Hooks::hkD3D11UpdateSubresource(ID3D11DeviceContext* pContext, ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	if (!bCheat)
	{
		return Hooks::oUpdateSubresource(pContext, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
	}
	g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId() << std::hex << " 0x" << pContext << std::dec << " UpdateSubresource(" << std::hex << "0x" << pDstResource << std::dec << "," << DstSubresource << ","
		<< std::hex << "0x" << pDstBox << std::dec << ","
		<< std::hex << "0x" << pSrcData << std::dec << ","
		<< SrcRowPitch << "," << SrcDepthPitch << ")" << std::endl;;

	//system("cls");
	//int i = 0;
	//Helpers::LogFormat("====\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f", 
	//	*(float*)((int)pSrcData + 0), *(float*)((int)pSrcData + 4), *(float*)((int)pSrcData + 8), *(float*)((int)pSrcData + 12),
	//	*(float*)((int)pSrcData + 16), *(float*)((int)pSrcData + 16 + 4), *(float*)((int)pSrcData + 16 + 8), *(float*)((int)pSrcData + 16 + 12),
	//	*(float*)((int)pSrcData + 32), *(float*)((int)pSrcData + 32 + 4), *(float*)((int)pSrcData + 32 + 8), *(float*)((int)pSrcData + 32 + 12),
	//	*(float*)((int)pSrcData + 48), *(float*)((int)pSrcData + 48 + 4), *(float*)((int)pSrcData + 48 + 8), *(float*)((int)pSrcData + 48 + 12)
	//);
	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);

	//int i = 0;

	if ((Stride == 24) || (Stride == 56))
	{
		Helpers::LogBuf2Txt("UpdateSubresource_", pSrcData, 0xa0);
		//system("cls");
		//std::cout << *(float*)((int)pSrcData + 0) << " " << *(float*)((int)pSrcData + 4) << " " << *(float*)((int)pSrcData + 8) << " " << *(float*)((int)pSrcData + 12) << std::endl;
		//std::cout << *(float*)((int)pSrcData + 16) << " " << *(float*)((int)pSrcData + 16 + 4) << " " << *(float*)((int)pSrcData + 16 + 8) << " " << *(float*)((int)pSrcData + 16 + 12) << std::endl;
		//std::cout << *(float*)((int)pSrcData + 32) << " " << *(float*)((int)pSrcData + 32 + 4) << " " << *(float*)((int)pSrcData + 32 + 8) << " " << *(float*)((int)pSrcData + 32 + 12) << std::endl;
		//std::cout << *(float*)((int)pSrcData + 48) << " " << *(float*)((int)pSrcData + 48 + 4) << " " << *(float*)((int)pSrcData + 48 + 8) << " " << *(float*)((int)pSrcData + 48 + 12) << std::endl;

		//for (int i = 0; i < 60; i++)
		//{
		//	std::cout << std::hex << std::setw(9) << std::setfill('0') << *(int*)((int)(ppConstantBuffers)+(i * 16) + 0) << " "
		//		<< std::hex << std::setw(9) << std::setfill('0') << *(int*)((int)(ppConstantBuffers)+(i * 16) + 4) << " "
		//		<< std::hex << std::setw(9) << std::setfill('0') << *(int*)((int)(ppConstantBuffers)+(i * 16) + 8) << " "
		//		<< std::hex << std::setw(9) << std::setfill('0') << *(int*)((int)(ppConstantBuffers)+(i * 16) + 12) << std::endl;

		//	//if ((*(float*)((int)(ppConstantBuffers)+(i * 16) + 0) != 0.00000000) || 
		//	//	(*(float*)((int)(ppConstantBuffers)+(i * 16) + 4) != 0.00000000) ||
		//	//	(*(float*)((int)(ppConstantBuffers)+(i * 16) + 8) != 0.00000000) ||
		//	//	(*(float*)((int)(ppConstantBuffers)+(i * 16) + 12) != 0.00000000) )
		//	//{
		//	//	Helpers::LogFormat("i=%d ===%03.8f %03.8f %03.8f %03.8f", i, /*18.123456789,*/
		//	//		*(float*)((int)(ppConstantBuffers)+(i*16) + 0),
		//	//		*(float*)((int)(ppConstantBuffers)+(i * 16) + 4),
		//	//		*(float*)((int)(ppConstantBuffers)+(i * 16) + 8),
		//	//		*(float*)((int)(ppConstantBuffers)+(i * 16) + 12)
		//	//	);
		//	//}
		//}
	}
	return Hooks::oUpdateSubresource(pContext, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void __stdcall Hooks::hkD3D11PSSetSamplers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers)
{
	//g_StartSlot = StartSlot;
	//MyTraceA("hkD3D11PSSetSamplers==> pContext=%08x, StartSlot=%d, NumSamplers=%d, ppSamplers=%08x", pContext, StartSlot, NumSamplers, ppSamplers);
	return Hooks::oPSSetSamplers(pContext, StartSlot, NumSamplers, ppSamplers);
}


void GoDrawCall(UINT InstanceCount, UINT StartInstanceLocation, ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT StartIndexLocation, INT BaseVertexLocation, UINT Stride = 0)
{
	if (Stride > 0)
	{
		UINT IndexCountStride = IndexCountPerInstance * 100 + Stride;
		if (find(lstAllStrides.begin(), lstAllStrides.end(), IndexCountStride) != lstAllStrides.end()) {
			//找到
		}
		else {
			//没找到
			lstAllStrides.push_back(IndexCountStride);
			//Helpers::LogFormat("hkD3D11DrawIndexedInstanced lstAll2412.push_back ++++++++ size=%d (%d) ", lstAllStides.size(), IndexCountStride);
		}

		if (bVideo4Rec_SCROL)
		{
			if ((Stride == g_iCurStride) && (IndexCountPerInstance == g_iCurIndexCount)
				//&&	IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount)
				)
			{
				//if ((Stride == 24) || (Stride == 12))
				{
					Helpers::LogFormat("PSSetShader(psRed, NULL, NULL) iStride=[%d] iIndexCount=[[ %d ]] BaseVertexLocation=[%d]", g_iCurStride, g_iCurIndexCount, BaseVertexLocation);
					pContext->PSSetShader(psRed, NULL, NULL);
				}
			}
		}
	}

	if ((InstanceCount == 9999) && (StartInstanceLocation == 9999))
	{
		Hooks::oDrawIndexed(pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	}
	else
	{
		Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	}
}

void __stdcall DrawIdxed_Or_Instanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	//cout << " ===================================DrawIdxed_Or_Instanced:" << "\n";
	//DWORD bgtime = timeGetTime();

	pMainContext = pContext;
	if (!bCheat)
	{
		GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
		return;
	}

	//int idx = 0;
	//clock_t start0 = clock();
	//clock_t start1 = clock();

	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(/*g_StartSlot*/0, 1, &veBuffer, &Stride, &veBufferOffset);


	//clock_t finish1 = clock();
	//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//start1 = clock();
	//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);

	if ((IndexCountPerInstance == 3234) ||
		(IndexCountPerInstance == 2898) ||
		(IndexCountPerInstance == 1878) ||
		(IndexCountPerInstance == 1002) ||
		(IndexCountPerInstance == 762) ||
		(IndexCountPerInstance == 570) ||
		(IndexCountPerInstance == 498) ||
		(IndexCountPerInstance == 450) ||
		(IndexCountPerInstance == 552)//整个小人
		)
	{

		if (!(abs(fXYZ[0]) <= 1e-6))
		{
			g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId();
			g_ssCallsInFrame << "  x=" << std::dec << fXYZ[0];
			g_ssCallsInFrame << "  y=" << std::dec << fXYZ[1];
			g_ssCallsInFrame << "  z=" << std::dec << fXYZ[2];
			g_ssCallsInFrame << std::endl;;

			if ((minX > fXYZ[0]) || (minX == 0))
			{
				minX = fXYZ[0];
			}
			if ((maxX < fXYZ[0]) || (maxX == 0))
			{
				maxX = fXYZ[0];
			}
			if ((minY > fXYZ[1]) || (minY == 0))
			{
				minY = fXYZ[1];
			}
			if ((maxY < fXYZ[1]) || (maxY == 0))
			{
				maxY = fXYZ[1];
			}
			g_lstPositions.push_back(XMFLOAT3(fXYZ[0], fXYZ[1], fXYZ[2]));
			if ((IndexCountPerInstance == 3234))
				g_iSelfIdx = g_lstPositions.size();
		}
	}
	//finish1 = clock();
	//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//start1 = clock();
	//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);


	fXYZ[0] = 0.0;
	fXYZ[1] = 0.0;
	fXYZ[2] = 0.0;
	//////////////////////////////////////////////////////
	////测试代码
	////D3D11_BUFFER_DESC pDesc;
	////(cbPerObjectBuffer)->GetDesc(&pDesc);
	////if (pDesc.ByteWidth == 3632);

	//ID3D11Buffer* pConstBuf = nullptr;
	//ID3D11Buffer* m_pCurConstBuf = nullptr;
	//pContext->VSGetConstantBuffers(0, 1, &pConstBuf);//

	//if (pConstBuf != NULL)
	//	m_pCurConstBuf = CopyBufferToCpu(pConstBuf);
	//SAFE_RELEASE(pConstBuf);
	//UINT bByteWidth = 0;
	//float matWorldView[4][4];
	//{
	//	float* WorldViewCB;
	//	MapBuffer(m_pCurConstBuf, (void**)&WorldViewCB, &bByteWidth);
	//	memcpy(matWorldView, &WorldViewCB[0], sizeof(matWorldView));
	//	//matWorldView[3][2] = matWorldView[3][2] + (aimheight*20);	//aimheight is usually done here for body parts

	//	DWORD* xxx = (DWORD*)WorldViewCB;
	//	if (bByteWidth == 3632)
	//	{
	//		//Helpers::Log("VSSetConstantBuffers 3632 --------------->>>>>>>>>>>>>>> ");
	//		Helpers::LogFormat("3632-帧-%5d-0X%x [0X%08x]=[%.3f,%.3f,%.3f] %d, %d, %d, %d", iFrames, pConstBuf, *(xxx + 0), /**(xxx + 1), *(xxx + 2),*/ (*(float*)(xxx + 36)), (*(float*)(xxx + 37)), (*(float*)(xxx + 38)), IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation);

	//		//string sTmp = "";
	//		//for (int i = 0; i < 908; i++)
	//		//{
	//		//	sTmp += to_string((*(float*)(xxx + i)));
	//		//	if (((i + 1) % 8) == 0)
	//		//	{
	//		//		sTmp += "\r\n";
	//		//	}
	//		//	else
	//		//	{
	//		//		sTmp += " ";
	//		//	}
	//		//}
	//		//mapBuf[to_string(Stride) + "_" + to_string(IndexCountPerInstance)] = sTmp;
	//	}

	//	UnmapBuffer(m_pCurConstBuf);
	//	SAFE_RELEASE(m_pCurConstBuf);
	//}
	////测试代码 Done
	////////////////////////////////////////


	Save_UnMapData_New(Stride, IndexCountPerInstance);
	//finish1 = clock();
	//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//start1 = clock();
	//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);

	//ofstream outfile;
	//outfile.open("..\\UnMap_Map__.txt", ios::app);
	//if (outfile)
	//{
	//	outfile << ::GetCurrentThreadId() << "_hkD3D11DrawIndexedInstanced()===================== " << " Stride=" << Stride << " IdxCount=" << IndexCountPerInstance << std::endl << std::endl;
	//	outfile.close();
	//}

	//Helpers::LogFormat("hkD3D11DrawIndexedInstanced (i=%d) ", lstAll2412.size());
	//{
	//	UINT IndexCountStride = IndexCountPerInstance * 100 + Stride;
	//	if (find(lstAllStrides.begin(), lstAllStrides.end(), IndexCountStride) != lstAllStrides.end()) {
	//		//找到
	//	}
	//	else {
	//		//没找到
	//		lstAllStrides.push_back(IndexCountStride);
	//		//Helpers::LogFormat("hkD3D11DrawIndexedInstanced lstAll2412.push_back ++++++++ size=%d (%d) ", lstAllStides.size(), IndexCountStride);
	//	}
	//}
	//finish1 = clock();
	//cout << idx++ << " ++++++take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//start1 = clock();
	//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);


	//if (bVideo4Rec_SCROL)
	//{
	//	if ((Stride == g_iCurStride) && (IndexCountPerInstance == g_iCurIndexCount) 
	//		//&&	IsNotIn_ExcludeList(g_iCurStride, g_iCurIndexCount)
	//		)
	//	{
	//		//if ((Stride == 24) || (Stride == 12))
	//		{
	//			Helpers::LogFormat("PSSetShader(psRed, NULL, NULL) iStride=[%d] iIndexCount=[[ %d ]]", g_iCurStride, g_iCurIndexCount);
	//			pContext->PSSetShader(psRed, NULL, NULL);
	//		}
	//	}

	//	GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	//	return;
	//}

	if (bHideOne)
	{
		if ((Stride == g_iCurStride) && (IndexCountPerInstance == g_iCurIndexCount))
		{
			//if ((Stride == 24) || (Stride == 12))
			{
				Helpers::LogFormat("bHideOne==> iStride=[%d] iIndexCount=[[ %d ]]", g_iCurStride, g_iCurIndexCount);
				//pContext->PSSetShader(psRed, NULL, NULL);
			}
		}

		//Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		return;
	}
	//if (IsIn_HideList(Stride, IndexCountPerInstance))
	//{
	//	return;
	//}

	if (bHideFog &&
		((Stride == 16) && (IndexCountPerInstance == 6))
		)
	{
		return;
	}

	//New...
	//if ((Stride != gStride) && IsNotIn_ExcludeList(Stride, IndexCountPerInstance))
	if (((Stride == 24) && (psFront) && IsNotIn_ExcludeList(Stride, IndexCountPerInstance))
		|| IsIn_IncludeList(Stride, IndexCountPerInstance)
		)
	{
		//pContext->PSGetShader(&pPixelShader__Old, NULL, NULL);
		//	ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
		//	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);

		//	pContext->PSSetShader(psObscured, NULL, NULL); //设为灰色
		//	GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
		//}
		//BBB//////////////////////////////////////////////
		pContext->RSSetState(CWcullMode);
		//pContext->RSSetState(RSCullSolid);
		pContext->PSSetShader(psBack, NULL, NULL); //设为灰色
		pContext->OMSetDepthStencilState(DSLess, 0);
		GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);

			//pContext->RSSetState(RSCullWireFrame);
			if (b2DShader &&psFront)
			{
				
				if (Is_CarOrBoat(Stride, IndexCountPerInstance))
				{
					pContext->PSSetShader(psGreen, NULL, NULL); //设为绿色
				} 
				else if (Is_Header(Stride, IndexCountPerInstance))
				{
					pContext->PSSetShader(psBlue, NULL, NULL); //设为蓝色
				} 
				else
				{
					pContext->PSSetShader(psFront, NULL, NULL); //设为明亮色
					//Helpers::LogFormat("PSSetShader(明亮色, NULL, NULL) iStride=[%d] iIndexCount=[[ %d ]]", Stride, IndexCountPerInstance);
				}
			}
			else
				pContext->PSSetShader(pPixelShader__Old, NULL, NULL);//设色

			pContext->OMSetDepthStencilState(DSGreat, 0);

			//pContext->RSSetState(RSCullWireFrame);
			GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, Stride);

			//{
			//	UINT IndexCountStride = IndexCountPerInstance * 100 + Stride;
			//	if (find(lstAllStrides.begin(), lstAllStrides.end(), IndexCountStride) != lstAllStrides.end()) {
			//		//找到
			//	}
			//	else {
			//		//没找到
			//		lstAllStrides.push_back(IndexCountStride);
			//		//Helpers::LogFormat("hkD3D11DrawIndexedInstanced lstAll2412.push_back ++++++++ size=%d (%d) ", lstAllStides.size(), IndexCountStride);
			//	}
			//}

			return;
	}
	else if ((Stride == gStride)  && bHideENV)
		return;
	else if (bHideGrass && IsIn_HideList(Stride, IndexCountPerInstance, BaseVertexLocation))
	{
		return;
	}
	else
		GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation, Stride);

	return;


	//if ((((Stride == gStride) /*|| IsIn_HideList(Stride, IndexCountPerInstance)*/) && bHideENV
	//	/*&&(
	//	(IndexCountPerInstance <= iMin) ||
	//	(IndexCountPerInstance >= iMax))*/
	//	))
	//{
	//	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 55 usedTime = ", timeGetTime() - bgtime);
	//}
	////else if (bHideENV && (Stride >12))
	////{
	////	//if (IsNotIn_ExcludeList(Stride, IndexCountPerInstance))
	////	{
	////		pContext->PSSetShader(psFront, NULL, NULL); //设为明亮色
	////		GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	////	}
	////}
	//else
	//{
	//	if (bHideENV)
	//	{
	//		UINT64 sss = IndexCountPerInstance * 100 + Stride;
	//		if (find(lstLogStrides.begin(), lstLogStrides.end(), sss) == lstLogStrides.end()) {
	//			//没找到
	//			lstLogStrides.push_back(sss);
	//		}
	//	}
	//	//finish1 = clock();
	//	//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//	//start1 = clock();
	//	//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);

	//	if (bHideGrass &&
	//		IsIn_HideList(Stride, IndexCountPerInstance, BaseVertexLocation)
	//		)
	//	{
	//		return;
	//	}
	//	else
	//		//if (/*(!bInList) &&*/ !((Stride == 24) && (IndexCountPerInstance == 54)) //6X
	//		//&& !((Stride == 24) && (IndexCountPerInstance == 75)) //3X
	//		//&& !((Stride == 24) && (IndexCountPerInstance == 72)) //
	//		//)
	//	{

	//		//finish1 = clock();
	//		//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//		//start1 = clock();
	//		//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);

	//		if ((1 == g_StartSlot) || (2 == g_StartSlot))
	//		{
	//			g_StartSlot = 0;
	//			if (((12 == Stride) || (24 == Stride)) &&
	//				IsNotIn_ExcludeList(Stride, IndexCountPerInstance))
	//			{
	//				//if (Is_CarOrBoat(Stride, IndexCountPerInstance))
	//				//{
	//				//	CheatItNew(pContext, psGreen);
	//				//}
	//				//else
	//				{
	//					if ((NULL != pHooksMappedResource) && (NULL != pHooksStageBuffer))
	//					{
	//						//获取指向顶点缓存的指针  
	//						//Vertex* verticesPtr;
	//						//verticesPtr = (Vertex*)pResource;

	//						////把数据复制进顶点缓存  
	//						//memcpy(verticesPtr, (void*)vertexs, (sizeof(Vertex) * mVertexCount));

	//						////解锁顶点缓存  
	//						//d3dDeviceContext->Unmap(md3dVertexBuffer, 0);
	//						Helpers::LogBuf2Txt("DrawIdxIns_" + std::to_string((UINT)::GetCurrentThreadId()) + "_" + std::to_string((UINT)Stride) + "_" + std::to_string((UINT)IndexCountPerInstance) + "_" + std::to_string((UINT)pHooksStageBuffer) + "_" + std::to_string((UINT)pHooksMappedResource->pData) + "_", pHooksMappedResource->pData, 0x40);
	//					}
	//					pHooksMappedResource = NULL;
	//					pHooksStageBuffer = NULL;

	//					D3D11_COMPARISON_FUNC DepthFunc000;
	//					//CheatItNew(pContext, psd);

	//					//AAA//////////////////////////////////////////////
	//					//if (b2DShader)
	//					//{
	//					//	pContext->PSSetShader(psd, NULL, NULL); //设为色
	//					//}
	//					//GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);

	//					//{
	//					//	// Create the depth stencil state.
	//					//	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//					//	pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
	//					//	ppDepthStencilState__Old->GetDesc(&depthStencilDesc);

	//					//	//depthStencilDesc.DepthEnable = TRUE;
	//					//	//depthStencilDesc.DepthEnable = FALSE;
	//					//	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//					//	//DepthFunc000 = depthStencilDesc.DepthFunc;
	//					//	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//					//	//depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	//					//	//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//					//	//depthStencilDesc.StencilEnable = FALSE;
	//					//	ID3D11Device *ppDevice;
	//					//	pContext->GetDevice(&ppDevice);

	//					pContext->PSGetShader(&pPixelShader__Old, NULL, NULL);
	//					//	ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
	//					//	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);

	//					//	pContext->PSSetShader(psObscured, NULL, NULL); //设为灰色
	//					//	GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	//					//}
	//					//BBB//////////////////////////////////////////////
	//					pContext->RSSetState(CWcullMode);
	//					//pContext->RSSetState(RSCullSolid);
	//					pContext->PSSetShader(psBack, NULL, NULL); //设为灰色
	//					pContext->OMSetDepthStencilState(DSLess, 0);
	//					GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);

	//					{
	//						//pContext->RSSetState(RSCullWireFrame);
	//						if (b2DShader &&psFront)
	//						{
	//							{
	//								pContext->PSSetShader(psFront, NULL, NULL); //设为明亮色
	//							}
	//						}
	//						else
	//							pContext->PSSetShader(pPixelShader__Old, NULL, NULL);//设色

	//						pContext->OMSetDepthStencilState(DSGreat, 0);

	//						//pContext->RSSetState(RSCullWireFrame);
	//						GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	//						//d3d11DevCon->OMSetDepthStencilState(NULL, 0);
	//					}

	//					{
	//						UINT IndexCountStride = IndexCountPerInstance * 100 + Stride;
	//						if (find(lstAllStrides.begin(), lstAllStrides.end(), IndexCountStride) != lstAllStrides.end()) {
	//							//找到
	//						}
	//						else {
	//							//没找到
	//							lstAllStrides.push_back(IndexCountStride);
	//							//Helpers::LogFormat("hkD3D11DrawIndexedInstanced lstAll2412.push_back ++++++++ size=%d (%d) ", lstAllStides.size(), IndexCountStride);
	//						}
	//					}

	//					//CCC//////////////////////////////////////////////
	//					//if (0)
	//					//{
	//					//
	//					//						//效果不好，还影响性能，注释了先。还有一处
	//					//						if (bTest2Draw)
	//					//						//if (0)
	//					//						{
	//					//							if (pPixelShader__Old != NULL)
	//					//							{
	//					//								D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//					//								//pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
	//					//								ppDepthStencilState__Old->GetDesc(&depthStencilDesc);
	//					//
	//					//								//depthStencilDesc.DepthEnable = TRUE;
	//					//								//depthStencilDesc.DepthEnable = FALSE;
	//					//								//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//					//								depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	//					//								//depthStencilDesc.DepthFunc = DepthFunc000;
	//					//								//depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//					//								//depthStencilDesc.StencilEnable = FALSE;
	//					//								ID3D11Device *ppDevice;
	//					//								pContext->GetDevice(&ppDevice);
	//					//
	//					//								ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__Old);
	//					//								pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	//					//
	//					//								if (b2DShader)
	//					//								{
	//					//									pContext->PSSetShader(psd, NULL, NULL); //设为色
	//					//								}
	//					//								else
	//					//									pContext->PSSetShader(pPixelShader__Old, NULL, NULL);//设色
	//					//							GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	//					//								pPixelShader__Old = NULL;
	//					//							}
	//					//						}
	//					//}

	//					if (ppDepthStencilState__Old)
	//					{
	//						//pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	//						ppDepthStencilState__Old = NULL;
	//					}


	//				}
	//			}
	//			else
	//				GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);
	//		}
	//		else
	//			GoDrawCall(InstanceCount, StartInstanceLocation, pContext, IndexCountPerInstance, StartIndexLocation, BaseVertexLocation);

	//		//finish1 = clock();
	//		//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//		//start1 = clock();
	//		//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d", idx, timeGetTime() - bgtime);


	//		//createTex(CCheat::pDevice, string("A01.dds"));
	//		//Helpers::LogFormat("pTextureSRV (=%d) ", pTextureSRV);

	//		//finish1 = clock();
	//		//cout << idx++ << " take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//		//start1 = clock();
	//		//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d ", idx, timeGetTime() - bgtime);

	//		if (/*(IndexCountPerInstance == 3234) && */pTextureSRV)
	//		{
	//			//Hooks::oPSSetShaderResources(pContext, 1, 1, &pTextureSRV);
	//		}

	//		//finish1 = clock();
	//		//cout << idx++ << " GoDrawCall take time(s):" << (double)(finish1 - start1) / 1.00f << "\n";
	//		//start1 = clock();
	//		//Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d ", idx, timeGetTime() - bgtime);


	//	}
	//}
	////finish1 = clock();
	////cout << "return..." << (double)(finish1 - start0) / 1.00f << "\n";
	////start1 = clock();
	////Helpers::LogFormat("DrawIdxed_Or_Instanced++++++++++++++++++++*=== %d usedTime = %d End", idx, timeGetTime() - bgtime);

	//return;
}

void __stdcall Hooks::hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	//g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId() <<std::hex << " 0x" << pContext << std::dec << "==DrawIndexed(" << IndexCount << "," << StartIndexLocation << "," << BaseVertexLocation << ")================" << std::endl;;

	//Helpers::LogAddress("\r\n hkD3D11DrawIndexed++++++++++++++++++++*===");
	//CheatIt(pContext, IndexCount, 0, StartIndexLocation, BaseVertexLocation, 0);
	//Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

	return DrawIdxed_Or_Instanced(pContext, IndexCount, 9999, StartIndexLocation, BaseVertexLocation, 9999);
	/////////////////////////////////////////////////////////
	//MyTraceA("hkD3D11DrawIndexedInstanced**************Stride=%d IndexCountPerInstance=%d InstanceCount=%d StartIndexLocation=%d BaseVertexLocation=%d StartInstanceLocation=%d \r\n", Stride, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);


	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 1 usedTime = ", timeGetTime() - bgtime);
	//Send2Hwnd(IndexCountPerInstance, Stride);

	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 2 usedTime = ", timeGetTime() - bgtime);
	//if (bCrossDraw)
	//{
	//	CheatIt(pContext, IndexCountPerInstance, InstanceCount/**/, StartIndexLocation, BaseVertexLocation, StartInstanceLocation/**/);
	//}

	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 3 usedTime = ", timeGetTime() - bgtime);

	//std::string szCurIdx = std::to_string(IndexCountPerInstance);
	//while (szCurIdx.length() < 5)
	//{
	//	szCurIdx = "0" + szCurIdx;
	//}
	//szCurIdx = std::to_string(Stride) + "_" + szCurIdx;

	//list<string>::iterator iter;
	//iter = std::find(sHideList.begin(), sHideList.end(), szCurIdx);

	//bool bInList = false;
	//if (iter != sHideList.end())
	//{
	//	//lst中存在 
	//	bInList = true;
	//}
	//else
	//{
	//	//没找到
	//	bInList = false;
	//}
	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 4 usedTime = ", timeGetTime() - bgtime);

	///*if (!(bHideTrees && (Stride == 12) && (
	//IndexCountPerInstance < abc ||

	////IndexCountPerInstance == iIndexCnt ||
	//IndexCountPerInstance == 6 ||
	//IndexCountPerInstance == 18 ||
	//IndexCountPerInstance == 27 ||
	//IndexCountPerInstance == 45 ||
	//IndexCountPerInstance == 2991 || //2991 42	3456
	//IndexCountPerInstance == 600 || //2991 42	3456
	//IndexCountPerInstance == 42 || //2991 42	3456
	//IndexCountPerInstance == 3456  //2991 42	3456
	//)))*/
	////if (! (bHideTrees && (Stride == 12 ) && (IndexCountPerInstance <abc)))
	////if ((IndexCountPerInstance>= abc))
	//{
	//	//if (!bInList)
	//	if (((Stride == gStride) && bHideTrees
	//		/*&&(
	//		(IndexCountPerInstance <= iMin) ||
	//		(IndexCountPerInstance >= iMax))*/
	//		))
	//	{
	//		//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 55 usedTime = ", timeGetTime() - bgtime);
	//	}
	//	else
	//	{
	//		if (bHideGrass &&
	//			(Stride == 12) &&
	//			(
	//			(IndexCountPerInstance == 6) ||
	//				(IndexCountPerInstance == 15) ||
	//				(IndexCountPerInstance == 18) ||
	//				(IndexCountPerInstance == 21) ||
	//				(IndexCountPerInstance == 27) ||
	//				(IndexCountPerInstance == 45)
	//				)
	//			)
	//		{
	//		}
	//		else if ((!bInList) && !((Stride == 24) && (IndexCountPerInstance == 54)) //6X
	//			&& !((Stride == 24) && (IndexCountPerInstance == 75)) //3X
	//			&& !((Stride == 24) && (IndexCountPerInstance == 72)) //
	//			)
	//		{

	//			//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 5 usedTime = ", timeGetTime() - bgtime);
	//			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	//		}
	//	}
	//}
	return;

}

//void tmpCode(ID3D11DeviceContext* d3dDeviceContext, ID3D11Resource *md3dVertexBuffer)
//{
//	UINT mVertexCount=88;
//
//	//填充(顶点)缓存形容结构体和子资源数据结构体,并创建顶点缓存(这里用的是动态缓存)
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	//这里此时每帧进行该缓存资源的更新应该用MAP和UMAP进行资源更新
//	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mVertexCount;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.MiscFlags = 0;
//	vertexBufferDesc.StructureByteStride = 0;
//
//
//
//	//锁定顶点缓存为了可以进行写入（动态缓存不能用UpdateSubResources写入）
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	d3dDeviceContext->Map(md3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//
//	//获取指向顶点缓存的指针
//	Vertex* verticesPtr;
//	verticesPtr = (Vertex*)mappedResource.pData;
//
//	//把数据复制进顶点缓存
//	//memcpy(verticesPtr, (void*)vertexs, (sizeof(Vertex) * mVertexCount));
//
//	//解锁顶点缓存
//	d3dDeviceContext->Unmap(md3dVertexBuffer, 0);
//}

//void *g_pMappedResourcepData = NULL;

void __stdcall Hooks::hkD3D11Map(ID3D11DeviceContext* pContext, _In_ ID3D11Buffer *pResource, _In_ UINT Subresource, _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE *pMappedResource)
{
	Hooks::oMap(pContext, pResource, Subresource, MapType, MapFlags, pMappedResource);
	//if (!bCheat)
	{

		return;
	}
	if (pMappedResource->DepthPitch == 3840 && pMappedResource->RowPitch == 3840)
	{
		mapMapBufList[pResource] = pMappedResource->pData;
	}

	g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId() << std::hex << " 0x" << pContext << std::dec << " Map(" << std::hex << "0x" << pResource << std::dec << "," << Subresource << "," << MapType << "," << MapFlags << ","
		<< std::hex << "0x" << pMappedResource << std::dec
		<< ") pData=" << pMappedResource->pData
		<< " DepthPitch=" << pMappedResource->DepthPitch
		<< " RowPitch=" << pMappedResource->RowPitch

		<< std::endl;;


	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);

	return;

	//if ((Stride == 24))
	{
		g_lock.lock();

		*(UINT*)(&(mapTID_PTR_DATA[to_string(::GetCurrentThreadId()) + "_" + to_string((UINT)(pMappedResource->pData))].x)) = 0x88888888;
		g_lock.unlock();
	}
	if (0)
	{
		ofstream outfile;
		outfile.open("..\\UnMap_Map__.txt", ios::app);
		if (outfile)
		{
			outfile << ::GetCurrentThreadId() << "_" << (UINT)(pMappedResource->pData) << " MapType=" << MapType << " MapFlags=" << MapFlags << " Stride=" << Stride << std::endl;
			outfile.close();
		}
	}
	//58784_126877696  = 5  = 0 Stride = 24
	//if ((Stride == 12) || (Stride == 24) || (Stride == 56))
	if ((Stride == 24) && (MapType == 4) && (MapFlags == 0))
	{
		if ((UINT)pMappedResource->pData > 0xf)
		{
			mapThreadList[::GetCurrentThreadId()] = pMappedResource->pData;
		}
		pHooksMappedResource = pMappedResource;
	}
	else
		pHooksMappedResource = NULL;
}

int SehFilter(DWORD dwExceptionCode)
{
	switch (dwExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return EXCEPTION_EXECUTE_HANDLER;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
void FillData()
{
	void * ptr = FindByTID(::GetCurrentThreadId());
	if (NULL == ptr)
		return;

	if (0)
	{
		string sKey = "";
		sKey = "UnMap_" + std::to_string((UINT)::GetCurrentThreadId()) /*+ "_" + std::to_string((UINT)pHooksStageBuffer)*/
			+ "_" + std::to_string((UINT)ptr) + "___";
		if (mapLogList[sKey].size() > 0)
		{
			bool bDiff = false;
			byte* pLastBuf = mapLogList[sKey].back();
			for (int i = 0; i < 16; i++)
			{
				if (((byte*)(ptr))[i] != pLastBuf[i])
				{//Keep it
					bDiff = true;
					break;
				}
			}
			if (bDiff)
			{
				byte* pBuf = new byte[16];
				memcpy(pBuf, (byte*)ptr, 16);
				mapLogList[sKey].push_back(pBuf);
			}
		}
		else
		{
			byte* pBuf = new byte[16];
			memcpy(pBuf, (byte*)ptr, 16);
			mapLogList[sKey].push_back(pBuf);
		}
	}
}

void CloneData()
{
	__try
	{
		FillData();
	}
	__except (SehFilter(GetExceptionCode()))
	{
		printf("EXCEPTION_ACCESS_VIOLATION");
	}
}

void __stdcall Hooks::hkD3D11UnMap(ID3D11DeviceContext* pContext, __in ID3D11Buffer* pStageBuffer, __in UINT Subresource)
{
	//if (!bCheat)
	{
		Hooks::oUnMap(pContext, pStageBuffer, Subresource);
		return;
	}
	void * g_pMappedResourcepData = mapMapBufList[pStageBuffer];
	g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId() << std::hex << " 0x" << pContext << std::dec << " UnMap(" << std::hex << "0x" << pStageBuffer << std::dec << "," << Subresource << ")";


	if (g_pMappedResourcepData != nullptr)
	{
		D3D11_BUFFER_DESC desc;
		pStageBuffer->GetDesc(&desc);
		g_ssCallsInFrame << " byteWid=" << desc.ByteWidth;
		g_ssCallsInFrame << std::endl;

		if (desc.ByteWidth == 3632)
		{
			//for (int i=0;i<4;i++)
			//{
			//	g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId();

			//	for (int b = 0; b < 16; b++)
			//	{
			//		g_ssCallsInFrame << "  " << std::dec << (*(float*)(((DWORD*)g_pMappedResourcepData +(i*16)+ b)));
			//	}
			//	g_ssCallsInFrame << std::endl;;
			//}
			fXYZ[0] = (*(float*)(((DWORD*)g_pMappedResourcepData + 36)));
			fXYZ[1] = (*(float*)(((DWORD*)g_pMappedResourcepData + 37)));
			fXYZ[2] = (*(float*)(((DWORD*)g_pMappedResourcepData + 38)));

			//g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId();
			//g_ssCallsInFrame << "  x=" << std::dec << (*(float*)(((DWORD*)g_pMappedResourcepData + 36)));
			//g_ssCallsInFrame << "  y=" << std::dec << (*(float*)(((DWORD*)g_pMappedResourcepData + 37)));
			//g_ssCallsInFrame << "  z=" << std::dec << (*(float*)(((DWORD*)g_pMappedResourcepData + 38)));
			//g_ssCallsInFrame << std::endl;;

			//Helpers::LogFormat("3632-帧-%5d- =[%.3f,%.3f,%.3f]", iFrames, /*pConstBuf, *(xxx + 0), *(xxx + 1), *(xxx + 2),*/ 
			//	(*(float*)(((DWORD*)g_pMappedResourcepData + 36))),
			//	(*(float*)(((DWORD*)g_pMappedResourcepData + 37))),
			//	(*(float*)(((DWORD*)g_pMappedResourcepData + 38)))
			//);

		}
		g_ssCallsInFrame << std::endl;;
	}
	mapMapBufList.erase(pStageBuffer);

	g_ssCallsInFrame << std::endl;

	//DWORD* xxx = (DWORD*)WorldViewCB;
	//	if (bByteWidth == 3632)
	//	{
	//		//Helpers::Log("VSSetConstantBuffers 3632 --------------->>>>>>>>>>>>>>> ");
	//		Helpers::LogFormat("3632-帧-%5d-0X%x [0X%08x]=[%.3f,%.3f,%.3f] %d, %d, %d, %d", iFrames, pConstBuf, *(xxx + 0), /**(xxx + 1), *(xxx + 2),*/ (*(float*)(xxx + 36))
	if (0)
	{
		ofstream outfile;
		outfile.open("..\\UnMap_Map__.txt", ios::app);
		if (outfile)
		{
			outfile << ::GetCurrentThreadId() << "_UnMap() ";
			g_lock.lock();
			map<string, XMFLOAT4, DisableCompare<string>>::reverse_iterator iter;//定义一个迭代指针iter
			for (iter = mapTID_PTR_DATA.rbegin(); iter != mapTID_PTR_DATA.rend(); iter++)
			{
				if (iter->first.find(to_string(::GetCurrentThreadId()) + "_") != std::string::npos)
				{
					if (*(UINT*)(&(iter->second.x)) != 0x88888888)
					{
						continue;
					}
					UINT ppp = atoll(iter->first.substr(iter->first.find("_") + 1).c_str());
					iter->second.x = *(FLOAT*)ppp;
					iter->second.y = *(FLOAT*)(ppp + 4);
					iter->second.z = *(FLOAT*)(ppp + 8);
					iter->second.w = *(FLOAT*)(ppp + 12);

					outfile << iter->first << ": ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&*(FLOAT*)ppp) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&*(FLOAT*)ppp) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&*(FLOAT*)(ppp + 4)) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&*(FLOAT*)(ppp + 4)) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&*(FLOAT*)(ppp + 8)) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&*(FLOAT*)(ppp + 8)) << "] ";
					outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)(&*(FLOAT*)(ppp + 12)) << "[" << std::setw(10) << std::setfill(' ') << *(float*)(&*(FLOAT*)(ppp + 12)) << "] " << std::endl;

					break;
				}
			}
			g_lock.unlock();

			//outfile << std::endl;
			outfile.close();
		}
	}
	//锁定顶点缓存为了可以进行写入（动态缓存不能用UpdateSubResources写入）  
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//(pContext->Map(pResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));


	//m_immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 得到const buffer指针.  
	//if (D3D11_MAP_WRITE_DISCARD == MapType)
	//if ((g_StartSlot == 1) && (Stride == 24))
	//{
	//	UINT* p = (UINT*)pMappedResource->pData;
	//	//Helpers::LogFormat("%08x %08x %08x %08x %08x %08x %08x %08x Slot=%d Stride=%d", *p, *(p + 4), *(p + 8), *(p + 12), *(p + 16), *(p + 20), *(p + 24), *(p + 28), g_StartSlot, Stride);
	//}

	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);

	//if ((Stride == 24))
	//{
	//	g_lock.lock();
	//	map<string, XMFLOAT4, DisableCompare<string>>::reverse_iterator iter;//定义一个迭代指针iter
	//	for (iter = mapTID_PTR_DATA.rbegin(); iter != mapTID_PTR_DATA.rend(); iter++)
	//	{
	//		if (iter->first.find(to_string(::GetCurrentThreadId()) + "_") != std::string::npos)
	//		{
	//			if (*(UINT*)(&(iter->second.x)) != 0x88888888)
	//			{
	//				continue;
	//			}
	//			UINT ppp = atoll(iter->first.substr(iter->first.find("_") + 1).c_str());
	//			iter->second.x = *(FLOAT*)ppp;
	//			iter->second.y = *(FLOAT*)(ppp + 4);
	//			iter->second.z = *(FLOAT*)(ppp + 8);
	//			iter->second.w = *(FLOAT*)(ppp + 12);
	//			break;
	//		}
	//	}
	//	g_lock.unlock();
	//}

	pHooksStageBuffer = pStageBuffer;

	if (0)
	{
		if ((NULL != pHooksMappedResource) && (NULL != pHooksStageBuffer))
		{
			//Helpers::LogBuf2Txt("UnMap_" + std::to_string((UINT)::GetCurrentThreadId()) + "_" /*+ std::to_string((UINT)Stride) + "_" + std::to_string((UINT)IndexCountPerInstance)*/ + "_" + std::to_string((UINT)pHooksStageBuffer) + "_" + std::to_string((UINT)pHooksMappedResource->pData) + "_", pHooksMappedResource->pData, 0x40);
			g_lock.lock();

			CloneData();
			//catch (...)
			//{
			//	printf("UnMap_ Error\n");
			//}
			g_lock.unlock();
		}
	}

	Hooks::oUnMap(pContext, pStageBuffer, Subresource);
	return;
}

void __stdcall Hooks::hkD3D11CreateQuery(ID3D11Device* pDevice, const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery)
{
	//Helpers::LogAddress("\r\n hkD3D11CreateQuery++++++++++++++++++++*===");

	//OutputDebugStringA("hkD3D11CreateQuery++++++++++++++++++++*===");
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
	//OutputDebugStringA("hkD3D11DrawInstanced++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawInstanced called");
	//UINT Stride;
	//ID3D11Buffer *veBuffer;
	//UINT veBufferOffset = 0;
	//pContext->IAGetVertexBuffers(/*g_StartSlot*/0, 1, &veBuffer, &Stride, &veBufferOffset);

	//ofstream outfile;
	//outfile.open("..\\UnMap_Map__.txt", ios::app);
	//if (outfile)
	//{
	//	outfile << ::GetCurrentThreadId() << "hkD3D11DrawIndexedInstancedIndirect()===================== " << " Stride=" << Stride << " IdxCount=" << VertexCountPerInstance << std::endl << std::endl;
	//	outfile.close();
	//}


	return oDrawInstanced(pContext, VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

void __stdcall Hooks::hkD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	//g_ssCallsInFrame << std::hex << "0x" << ::GetCurrentThreadId() <<std::hex << " 0x" << pContext << std::dec << "==DrawIndexedInstanced(" << IndexCountPerInstance << "," << InstanceCount << "," << StartIndexLocation << "," << BaseVertexLocation << "," << StartInstanceLocation << ")================" << std::endl;;

	//Helpers::LogAddress("\r\n hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	//	OutputDebugStringA("hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	return DrawIdxed_Or_Instanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	/////////////////////////////////////////////////////////
	//MyTraceA("hkD3D11DrawIndexedInstanced**************Stride=%d IndexCountPerInstance=%d InstanceCount=%d StartIndexLocation=%d BaseVertexLocation=%d StartInstanceLocation=%d \r\n", Stride, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);


	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 1 usedTime = ", timeGetTime() - bgtime);
	//Send2Hwnd(IndexCountPerInstance, Stride);

	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 2 usedTime = ", timeGetTime() - bgtime);
	//if (bCrossDraw)
	//{
	//	CheatIt(pContext, IndexCountPerInstance, InstanceCount/**/, StartIndexLocation, BaseVertexLocation, StartInstanceLocation/**/);
	//}

	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 3 usedTime = ", timeGetTime() - bgtime);

	//std::string szCurIdx = std::to_string(IndexCountPerInstance);
	//while (szCurIdx.length() < 5)
	//{
	//	szCurIdx = "0" + szCurIdx;
	//}
	//szCurIdx = std::to_string(Stride) + "_" + szCurIdx;

	//list<string>::iterator iter;
	//iter = std::find(sHideList.begin(), sHideList.end(), szCurIdx);

	//bool bInList = false;
	//if (iter != sHideList.end())
	//{
	//	//lst中存在 
	//	bInList = true;
	//}
	//else
	//{
	//	//没找到
	//	bInList = false;
	//}
	////Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 4 usedTime = ", timeGetTime() - bgtime);

	///*if (!(bHideTrees && (Stride == 12) && (
	//	IndexCountPerInstance < abc ||

	//	//IndexCountPerInstance == iIndexCnt ||
	//	IndexCountPerInstance == 6 ||
	//	IndexCountPerInstance == 18 ||
	//	IndexCountPerInstance == 27 ||
	//	IndexCountPerInstance == 45 ||
	//	IndexCountPerInstance == 2991 || //2991 42	3456
	//	IndexCountPerInstance == 600 || //2991 42	3456
	//	IndexCountPerInstance == 42 || //2991 42	3456
	//	IndexCountPerInstance == 3456  //2991 42	3456
	//	)))*/
	//	//if (! (bHideTrees && (Stride == 12 ) && (IndexCountPerInstance <abc)))
	//	//if ((IndexCountPerInstance>= abc))
	//{
	//	//if (!bInList)
	//	if (((Stride == gStride) && bHideTrees 
	//		 /*&&(
	//								(IndexCountPerInstance <= iMin) ||
	//								(IndexCountPerInstance >= iMax))*/
	//		))
	//	{
	//		//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 55 usedTime = ", timeGetTime() - bgtime);
	//	}
	//	else
	//	{
	//		if (bHideGrass && 
	//			(Stride == 12) &&
	//			(
	//			(IndexCountPerInstance == 6) ||
	//				(IndexCountPerInstance == 15) ||
	//				(IndexCountPerInstance == 18) ||
	//				(IndexCountPerInstance == 21) ||
	//				(IndexCountPerInstance == 27) ||
	//				(IndexCountPerInstance == 45)
	//				)
	//			)
	//		{
	//		}
	//		else if ((!bInList) && !((Stride == 24) && (IndexCountPerInstance == 54)) //6X
	//			&& !((Stride == 24) && (IndexCountPerInstance == 75)) //3X
	//			&& !((Stride == 24) && (IndexCountPerInstance == 72)) //
	//			)
	//		{

	//			//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 5 usedTime = ", timeGetTime() - bgtime);
	//			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	//		}
	//	}
	//}

	////if (bShow24)
	////{
	////	if ((Stride != 12))
	////	{
	////			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	////	}
	////}
	////else
	////{
	////	if ((Stride != 24))
	////	{
	////			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	////	}

	////}
	//			//Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	////pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	//return;
}

//==========================================================================================================================

void __stdcall Hooks::hkD3D11DrawInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawInstancedIndirect++++++++++++++++++++*===");
	//OutputDebugStringA("hkD3D11DrawInstancedIndirect++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawInstancedIndirect called");

	//if (GetAsyncKeyState(VK_RETURN) & 1)
	//	bHideTrees = bHideTrees ? false : true;

	//UINT Stride;
	//ID3D11Buffer *veBuffer;
	//UINT veBufferOffset = 0;
	//pContext->IAGetVertexBuffers(/*g_StartSlot*/0, 1, &veBuffer, &Stride, &veBufferOffset);

	//ofstream outfile;
	//outfile.open("..\\UnMap_Map__.txt", ios::app);
	//if (outfile)
	//{
	//	outfile << ::GetCurrentThreadId() << "hkD3D11DrawInstancedIndirect()===================== " << " Stride=" << Stride  << std::endl << std::endl;
	//	outfile.close();
	//}

	//if (!bHideTrees)
	{
		return oDrawInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
	}
}

//==========================================================================================================================

void __stdcall Hooks::hkD3D11DrawIndexedInstancedIndirect(ID3D11DeviceContext* pContext, ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexedInstancedIndirect++++++++++++++++++++*===");
	//OutputDebugStringA("hkD3D11DrawIndexedInstancedIndirect++++++++++++++++++++*===");
	//if (GetAsyncKeyState(VK_F9) & 1)
	//	Log("DrawIndexedInstancedIndirect called");
	//UINT Stride;
	//ID3D11Buffer *veBuffer;
	//UINT veBufferOffset = 0;
	//pContext->IAGetVertexBuffers(/*g_StartSlot*/0, 1, &veBuffer, &Stride, &veBufferOffset);

	//ofstream outfile;
	//outfile.open("..\\UnMap_Map__.txt", ios::app);
	//if (outfile)
	//{
	//	outfile << ::GetCurrentThreadId() << "hkD3D11DrawIndexedInstancedIndirect()===================== " << " Stride=" << Stride <<std::endl << std::endl;
	//	outfile.close();
	//}


	return oDrawIndexedInstancedIndirect(pContext, pBufferForArgs, AlignedByteOffsetForArgs);
}

