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
#include <iosfwd>
#include <string>
extern "C"
{ 
#include "..\Subliminal-Fortnite\FW1FontWrapper\FW1FontWrapper.h"
};

#include<fstream>
using namespace std;
#include "xnamath.h"
#include "D3DX11tex.h"

#pragma comment(lib, "winmm.lib") //timeGetTime
#define INTVL  1

IFW1Factory *pFW1Factory = NULL;
IFW1FontWrapper *pFontWrapper = NULL;

ID3D11DepthStencilState *ppDepthStencilState__New = NULL;
ID3D11DepthStencilState *ppDepthStencilState__Old = NULL;
UINT pStencilRef = 0;
extern HWND g_hWnd;
extern RECT g_lpRect;
extern HANDLE  g_Event_Shoot;
extern bool bCrossDraw;
bool bCheat = true;
bool bLog2Txt = false;
ofstream outfile;

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

 std::vector<UINT64> lstAll2412;
 std::vector<UINT64> lstAvatar2412;
 std::vector<UINT64> lstEqupm2412;
 std::vector<UINT64> lstNot2412;
 //std::vector<int> lstRed24;
 //std::vector<int> lstBase12;
 //std::vector<int> lstRed12;
 int iPos = 0;

 bool bInited = false;
 bool bFlashIt = false;
 //==========================================================================================================================
 bool bHideTrees = false;
 bool bHideGrass = false;
 DWORD ppppp = 0;
 int ipp = 0;
 int gStride = 12;
 int iMin = 601;
 int iMax = 2990;
 int itm = 0;

 UINT64 iiiii = 0;
 int iStride = 0;
 int iIndexCount = 0;
 int bRed = true;
 int iRed = 0;
 DWORD gggg = 0;
 DWORD cover = 0;
 bool bShoot = false;
 bool bShow24 = false;
ID3D11ShaderResourceView *pTextureSRV = NULL;

 void tmppp(ID3D11DeviceContext* pContext)
 {
	 D3D11_TEXTURE2D_DESC textureDesc;
	 ID3D11Texture2D *pTexture;

	 ZeroMemory(&textureDesc, sizeof(textureDesc));
	 textureDesc.Width = 1280;
	 textureDesc.Height = 720;
	 textureDesc.ArraySize = 1;
	 textureDesc.Format = DXGI_FORMAT_R8_UNORM;
	 textureDesc.SampleDesc.Count = 1;
	 textureDesc.Usage = D3D11_USAGE_DEFAULT;
	 textureDesc.MipLevels = 1;
	 textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	 Helpers::LogFormat("Create glyph sheet texture 0");
	 HRESULT hResult = CCheat::pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture);
	 if (FAILED(hResult)) {
		 //m_lastError = L"Failed to create glyph sheet texture";
		 Helpers::LogFormat("Failed to create glyph sheet texture 1");
	 }
	 else {

		 hResult = CCheat::pDevice->CreateShaderResourceView(pTexture, NULL, &pTextureSRV);
		 if (FAILED(hResult)) {
			 //m_lastError = L"Failed to create shader resource view for glyph sheet texture";
			 Helpers::LogFormat("Failed to create glyph sheet texture 2");
		 }
		 else {
			 // Create coord buffer if enabled
			 //if (m_hardwareCoordBuffer) {
			 //	D3D11_BUFFER_DESC bufferDesc;
			 //	ID3D11Buffer *pBuffer;

			 //	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			 //	bufferDesc.ByteWidth = m_maxGlyphCount * sizeof(FW1_GLYPHCOORDS);
			 //	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			 //	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			 //	hResult = m_pDevice->CreateBuffer(&bufferDesc, NULL, &pBuffer);
			 //	if (FAILED(hResult)) {
			 //		m_lastError = L"Failed to create glyph coord buffer";
			 //	}
			 //	else {
			 //		D3D11_SHADER_RESOURCE_VIEW_DESC bufferSRVDesc;
			 //		ID3D11ShaderResourceView *pBufferSRV;

			 //		ZeroMemory(&bufferSRVDesc, sizeof(bufferSRVDesc));
			 //		bufferSRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			 //		bufferSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			 //		bufferSRVDesc.Buffer.ElementOffset = 0;
			 //		bufferSRVDesc.Buffer.ElementWidth = m_maxGlyphCount * 2;// Two float4 per glyphcoords

			 //		hResult = m_pDevice->CreateShaderResourceView(pBuffer, &bufferSRVDesc, &pBufferSRV);
			 //		if (FAILED(hResult)) {
			 //			m_lastError = L"Failed to create shader resource view for glyph coord buffer";
			 //		}
			 //		else {
			 //			m_pCoordBuffer = pBuffer;
			 //			m_pCoordBufferSRV = pBufferSRV;
			 //		}

			 //		if (FAILED(hResult))
			 //			pBuffer->Release();
			 //	}
			 //}

			 //if (SUCCEEDED(hResult)) {
			 //	m_pTexture = pTexture;
			 //	m_pTextureSRV = pTextureSRV;
			 //}
			 //else
			 //	pTextureSRV->Release();
		 }

		 if (FAILED(hResult))
		 {
			 Helpers::LogFormat("Failed to create glyph sheet texture 3");
			 pTexture->Release();
		 }
		 //Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, (ID3D11ShaderResourceView *const *)pTextureSRV);
	 }
	 Helpers::LogFormat("Done create glyph sheet texture 000000000000000000");
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

 void InitD2DScreenTexture()
 {
	 HRESULT hResult;
	 ////Create the vertex buffer
	 //Vertex v[] =
	 //{
		// // Front Face
		// Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		// Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		// Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		// Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
	 //};

	 //DWORD indices[] = {
		// // Front Face
		// 0,  1,  2,
		// 0,  2,  3,
	 //};

	 //D3D11_BUFFER_DESC indexBufferDesc;
	 //ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	 //indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	 //indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	 //indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	 //indexBufferDesc.CPUAccessFlags = 0;
	 //indexBufferDesc.MiscFlags = 0;

	 //D3D11_SUBRESOURCE_DATA iinitData;

	 //iinitData.pSysMem = indices;
	 //hResult = CCheat::pDevice->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);
	 //if (FAILED(hResult))
	 //{
		// Helpers::LogFormat("FAILED(hResult) 0");
	 //}
	 //D3D11_BUFFER_DESC vertexBufferDesc;
	 //ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	 //vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	 //vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	 //vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	 //vertexBufferDesc.CPUAccessFlags = 0;
	 //vertexBufferDesc.MiscFlags = 0;

	 //D3D11_SUBRESOURCE_DATA vertexBufferData;

	 //ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	 //vertexBufferData.pSysMem = v;
	 //hResult = CCheat::pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);
	 //if (FAILED(hResult))
	 //{
		// Helpers::LogFormat("FAILED(hResult) 1");
	 //}

	 //D3D11_TEXTURE2D_DESC sharedTexDesc;

	 //ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	 //sharedTexDesc.Width = 1280;
	 //sharedTexDesc.Height = 720;
	 //sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	 //sharedTexDesc.MipLevels = 1;
	 //sharedTexDesc.ArraySize = 1;
	 //sharedTexDesc.SampleDesc.Count = 1;
	 //sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	 //sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	 //sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	 //hResult = CCheat::pDevice->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
	 //if (FAILED(hResult))
	 //{
		// Helpers::LogFormat("FAILED(CreateTexture2D) .......");
	 //}

	 ////Create A shader resource view from the texture D2D will render to,
	 ////So we can use it to texture a square which overlays our scene
	 //hResult = CCheat::pDevice->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
	 //if (FAILED(hResult))
	 //{
		// Helpers::LogFormat("FAILED(hResult) 222");
	 //}


	 ///Load Skymap's cube texture///
	 D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	 loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	 ID3D11Texture2D* SMTexture = 0;
	 hResult = D3DX11CreateTextureFromFile(CCheat::pDevice, L"..\\skymap.dds",
		 &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	 D3D11_TEXTURE2D_DESC SMTextureDesc;
	 SMTexture->GetDesc(&SMTextureDesc);

	 D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	 SMViewDesc.Format = SMTextureDesc.Format;
	 SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	 SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	 SMViewDesc.TextureCube.MostDetailedMip = 0;

	 hResult = CCheat::pDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &d2dTexture);


	 Helpers::LogFormat("SMTexture=[%x] d2dTexture=[[ %x ]] ", SMTexture, d2dTexture);
	 system("pause");
 }

 void InitListFromFiles()
 {
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

		 lstEqupm2412.clear();
		 fin.open("..\\equpmList.txt");  //打开文件
										 //string ReadLine;
		 while (getline(fin, ReadLine))  //逐行读取，直到结束
		 {
			 lstEqupm2412.push_back(atoi(ReadLine.c_str()));
		 }
		 fin.close();

		 lstNot2412.clear();
		 fin.open("..\\notList.txt");  //打开文件
									   //string ReadLine;
		 while (getline(fin, ReadLine))  //逐行读取，直到结束
		 {
			 lstNot2412.push_back(atoi(ReadLine.c_str()));
		 }
		 fin.close();
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
 ID3D11PixelShader* psd = NULL;
 ID3D11PixelShader* psBlue = NULL;
 ID3D11PixelShader* psRed0 = NULL;
 ID3D11PixelShader* psTmp = NULL;
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

 void Thread_KeysSwitch(PVOID param)
 {
	 int iBW_Pos = 0;
	 while (true)
	 {
		 if (GetAsyncKeyState(VK_SCROLL) & 1)
		 {
			 bVideo4Rec_SCROL = !bVideo4Rec_SCROL;
		 }
		 if (GetAsyncKeyState(VK_ADD) & 1)
		 {
			 bShoot = !bShoot;
			 //red24.push_back(lst24[iPos]);
		 }

		 if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		 {
			 ipp = ipp++ % 4;
		 }
		 if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		 {
			 sHideList.clear();
		 }
		 if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		 {
			 bCrossDraw = !bCrossDraw;
		 }
		 if (GetAsyncKeyState(VK_F12) & 1)
		 {
			 if (iBW_Pos >= lstAllStride00.size())
			 {
				 iBW_Pos = 0;
			 } 
			 if (lstAllStride00.size() > 0)
			 {
				 pssrStride = lstAllStride00.at(iBW_Pos++);
				 Helpers::LogFormat("pssrStride  == [[ %d ]], lstAllByteWidth.size()=[%d]", pssrStride, lstAllStride00.size());
			 }
		 }
		 if (GetAsyncKeyState(VK_F10) & 1)
		 {
			 bShow24 = !bShow24;
		 }
		 if (GetAsyncKeyState(VK_F9) & 1)
		 {
			 //bLogTxt = !bLogTxt;
		 }
		 if (GetAsyncKeyState(VK_F8) & 1)
		 {
			 bCheat = !bCheat;
		 }
		 if (GetAsyncKeyState(VK_F7) & 1)
		 {
			 bLog2Txt = !bLog2Txt;
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

		 if (GetAsyncKeyState(VK_DELETE) & 1)
		 {
			 ppDepthStencilState__Old = NULL;
			 ppDepthStencilState__New = NULL;
		 }

		 if (GetAsyncKeyState(VK_PAUSE) & 1)
		 {
			 InitListFromFiles();
		 }
		 if (GetAsyncKeyState(VK_RIGHT) & 1)
		 {
			 iRed = iRed++ % 3;;
		 }

		 if (GetAsyncKeyState('Q') & 1)
		 {
			 //bShoot = !bShoot;
		 }

		 if (bShoot)
		 {
			 SetEvent(g_Event_Shoot);
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
			 bHideTrees = true;
			 bHideGrass = false;
		 }
		 else if (ipp == 2)
		 {
			 if (ppppp != timeGetTime() / INTVL)
			 {
				 bHideTrees = !bHideTrees;
				 ppppp = timeGetTime() / INTVL;
			 }
		 }
		 else if (ipp == 3)
		 {
			 bHideGrass = !bHideGrass;
			 bHideGrass = true;
			 bHideTrees = false;
		 }
		 else
		 {
			 bHideGrass = false;
			 bHideTrees = false;
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

		 Sleep(300);
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

 bool IsNotWhat(UINT Stride, UINT IndexCount)
 {
	 UINT64 IndexCountStride = IndexCount * 100 + Stride;
	 if (find(lstNot2412.begin(), lstNot2412.end(), IndexCountStride) != lstNot2412.end()) {
		 //找到
		 return false;
	 }
	 else {
		 //没找到
		 if (/*(Stride == 24) &&*/ (IndexCount <= 200))
		 {
			 return false;
		 }
		 else
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

  void __stdcall CheatIt(ID3D11DeviceContext* pContext, UINT IndexCount, UINT IndexCountPerInstance, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
 {

	 if (ppDepthStencilState__New != NULL)
	 {
		 ppDepthStencilState__New->Release();
		 ppDepthStencilState__New = NULL;
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
	 pContext->IAGetVertexBuffers(0/*StartSlot*/, 1, &veBuffer, &Stride, &veBufferOffset);

	 
	 MyTraceA("CheatIt**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);

	 Send2Hwnd(IndexCount, Stride);

	 //if ((Stride == 12) && (IndexCount > 7000) && (IndexCount < 10000))
	 //{

		// MyTraceA("CheatItBIGBIG**************Stride=%d IndexCount=%d StartIndexLocation=%d BaseVertexLocation=%d \r\n", Stride, IndexCount, StartIndexLocation, BaseVertexLocation);
		// //		if (!bRed)
		// //			return;
	 //}


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
	 //if ((Stride == 24))
	 {
		 UINT IndexCountStride = IndexCount * 100 + Stride;
		 if (find(lstAll2412.begin(), lstAll2412.end(), IndexCountStride) != lstAll2412.end()) {
			 //找到
		 }
		 else {
			 //没找到
			 lstAll2412.push_back(IndexCountStride);
		 }
	 }

	 if (
		 IsNotWhat(Stride, IndexCount) &&
		 (IsAvatar(Stride, IndexCount) || IsEquipment(Stride, IndexCount))
		)									
	 //if(iIndexCnt == IndexCount)
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
			 //MyTraceA("hkD3D11 CheatIt**********透明了****iPos=%d  Stride=%d  IndexCount=%d red24.size=%d", iPos, Stride, IndexCount, lstRed24.size());

			 //return;
			 //if (ppDepthStencilState__Old == NULL)
			 {
				 pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);
			 }

			 ID3D11PixelShader* psSSS = psRed;
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
					 psSSS = psTmp;
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

			 if ( (bRed))
				 {
					 if ((Stride == 24 && IndexCount == 2070) // 头
						 || (Stride == 24 && IndexCount == 3234) // 头
						 || (Stride == 12 && IndexCount == 2877) // 头发
						 || (Stride == 12 && IndexCount == 2868) // 头发
						 || (Stride == 12 && IndexCount == 2637) // 三级盔 近处
						 || (Stride == 12 && IndexCount == 1116) // 2级盔 近处
						 || (Stride == 12 && IndexCount == 816) // ？盔 远处
						 || (Stride == 12 && IndexCount == 192) // ？盔 远处
						 || (Stride == 12 && IndexCount == 156) // ？盔 远处
						 || (Stride == 12 && IndexCount == 180) // ？盔 远处
						 || (Stride == 12 && IndexCount == 276) // ？盔 远处
						 || (Stride == 12 && IndexCount == 294) // ？盔 远处
						 )
					 {
						 pContext->PSSetShader(psBlue, NULL, NULL);
					 }
					 else if (IsAvatar(Stride, IndexCount))
						 pContext->PSSetShader(psSSS, NULL, NULL);
					 else if (IsEquipment(Stride, IndexCount))
					 {
						 pContext->PSSetShader(psGreen, NULL, NULL);
					 }
					 else
						 pContext->PSSetShader(psd, NULL, NULL);
				 }
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
	// else if ((Stride == 12) && IndexCount<15000)
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

	 //create font
	 HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	 hResult = pFW1Factory->CreateFontWrapper(CCheat::pDevice, L"Tahoma", &pFontWrapper);
	 pFW1Factory->Release();

	 // use the back buffer address to create the render target
	 //if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&RenderTargetTexture))))
	 if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
	 {
		 CCheat::pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
		 RenderTargetTexture->Release();
	 }

	 InitListFromFiles();
	 //tmppp(CCheat::pContext);

	 //InitD2DScreenTexture();
	 //_beginthread(Thread_fileWatcher, 0, NULL);
	 _beginthread(Thread_KeysSwitch, 0, NULL);
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

bool IsCenterRed()
//lpRect 代表选定区域
{
	bool bOK = false;
	//return;
	//Helpers::Log("==============AutoShootIfCenter");
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
		return false;
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
			Helpers::Log("==============+-+-+-+- 射击射击射击");
			bOK = true;

			break;
		}
		//	ptPixels[i] = cNewColor;
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	return bOK;
}

UINT iName = 0;
HRESULT __stdcall Hooks::hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (bLog2Txt)
	{
		;
		if (outfile.is_open())
		{
			outfile.close();
		}
		
		outfile.open("..\\xFrame_" + to_string(iName++), ios::app);
		if (!outfile)
		{
			std::cout << "打开Log2Txt.txt文件失败！" << endl;
		}
		else
		{
		}
	}
	//Helpers::LogFormat("hkD3D11Present+++++++-------------------------------- bUp = %d", bFlashIt);
	DWORD bgtime = timeGetTime();
	if (!bCheat)
	{
		return Hooks::oPresent(pSwapChain, SyncInterval, Flags);
	}

	//Helpers::LogAddress("\r\n hkD3D11Present++++++++++++++++++++*===");
	bFlashIt = !bFlashIt;

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
	HRESULT hr;
	if (!psCrimson)
		hr=GenerateShader(CCheat::pDevice, &psCrimson, 1.f, 0.2f, 0.2f);
	if (!psYellow)
		hr = GenerateShader(CCheat::pDevice, &psYellow, 1.f, 0.6f, 0);
	if (!psRed)
		hr = GenerateShader(CCheat::pDevice, &psRed, 0.5f, 0.0f, 0.0f);
	if (!psGreen)
		hr = GenerateShader(CCheat::pDevice, &psGreen, 0.0f, 0.5f, 0.0f);
	if (!psBlue)
		hr = GenerateShader(CCheat::pDevice, &psBlue, 0.0f, 0.0f, 0.5f);
	if (!psTmp)
		hr = GenerateShader(CCheat::pDevice, &psTmp, 0.45f, 0.3f, 0.1f);
	if (!psd)
		hr = GenerateShader(CCheat::pDevice, &psd, 0.6f, 0.6f, 0);

	if (S_OK == hr)
	{
		pssrStartSlot = 1;
	}
	//Helpers::Log2Txt("hkD3D11Present++++++++++++++++++++*=== 1 usedTime = ", timeGetTime() - bgtime);
	//call before you draw
	CCheat::pContext->OMSetRenderTargets(/*1*/vps, &RenderTargetView, NULL); //?????? 1 
	//draw
	if (pFontWrapper)
	{
		//pFontWrapper->DrawString(CCheat::pContext, L"Who are youuuuuuuuuuuuuuu?", 14, 16.0f, 16.0f, 0xffff1612, FW1_RESTORESTATE);
		//pFontWrapper->DrawString(CCheat::pContext, L"Welcome Back Bscan*****============================", 14.0f, 16.0f, 30.0f, 0xffffffff, FW1_RESTORESTATE);

		//std::string sData = std::to_string(iStride);
		//sData += "_";
		//sData += std::to_string(iIndexCount);
		//
		//pFontWrapper->DrawString(CCheat::pContext, StringToWString(sData).c_str(), 18.0f, ScreenCenterX, ScreenCenterY, 0xff00ff00, FW1_RESTORESTATE);
		//Helpers::Log("D3D11Present pFontWrapper->DrawString \"Who are youuuuuuuuuuuuuuu ? \"");
	}


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
	if (bVideo4Rec_SCROL && !IsCenterRed())
	{
		if (lstAll2412.size() > 0)
		{
			if (iPos >= lstAll2412.size())
			{
				iPos = 0;
			}

			iiiii = lstAll2412.at(iPos);
			iStride = iiiii % 100;
			iIndexCount = iiiii / 100;;

			//Helpers::LogFormat("%d %d-%d %d %ld", iPos, iStride, iIndexCount, lstAll2412.size(), lstAll2412.at(iPos));
			iPos++;
		}
	}
	else if (bVideo4Rec_SCROL) //IsCenterRed()
	{
		bVideo4Rec_SCROL = !bVideo4Rec_SCROL;
		Helpers::LogFormat("hkD3D11Present 红色了+++++++++++++ iStride=%d iIndexCount=%d ==%ld", iStride, iIndexCount, iiiii);
	}
	if (bVideo4Rec_SCROL) //IsCenterRed()
	{
		//bVideo4Rec = !bVideo4Rec;
		Helpers::LogFormat("hkD3D11Present 一帧+++++++++++++ iStride=%d iIndexCount=%d", iStride, iIndexCount);
	}
	//Helpers::Log2Txt("hkD3D11Present++++++++++++++++++++*=== 2 usedTime = ", timeGetTime() - bgtime);

	return Hooks::oPresent(pSwapChain, SyncInterval, Flags);
}

void __stdcall Hooks::hkD3D11VSSetConstantBuffers(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
{
	g_StartSlot = StartSlot;
	//Helpers::LogAddress("\r\n hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	//OutputDebugStringA("hkD3D11VSSetConstantBuffers++++++++++++++++++++*===");
	//works ok in ut4 alpha only
	//if (Stride == 40 || Stride == 44) //ut4 models
	//{
	//AddModel(pContext);//w2s
	//}
	//MyTraceA("hkD3D11VSSetConstantBuffers**************pContext=%x StartSlot=%d NumBuffers=%d ppConstantBuffers=%x ", pContext, StartSlot, NumBuffers, ppConstantBuffers);



	return Hooks::oVSSetConstantBuffers(pContext, StartSlot, NumBuffers, ppConstantBuffers);
}

void __stdcall Hooks::hkD3D11PSSetShaderResources(ID3D11DeviceContext* pContext, UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews)
{
	g_StartSlot = StartSlot;
	//MyTraceA("hkD3D11PSSetShaderResources==> pContext=%08x, StartSlot=%d, NumViews=%d, ppShaderResourceViews=%08x", pContext, StartSlot, NumViews, ppShaderResourceViews);
	//pssrStartSlot = StartSlot;
	ID3D11ShaderResourceView* pShaderResView = ppShaderResourceViews[0];
	if (pShaderResView)
	{
		pShaderResView->GetDesc(&Descr);
	}
	//get pscdesc.ByteWidth
	pContext->PSGetConstantBuffers(StartSlot, 1, &pcsBuffer);
	if (pcsBuffer)
		pcsBuffer->GetDesc(&pscdesc);
	if (pcsBuffer != NULL) { pcsBuffer->Release(); pcsBuffer = NULL; }

	//(pscdesc.ByteWidth == 224 && Descr.Format == 71)
	UINT Stride = 0;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
	if ((pssrStartSlot == StartSlot))
	{

		//Helpers::LogFormat("hkD3D11PSSetShaderResources==> NumViews=%d, ppSRViews=%08x pscdesc.BW=%d, Descr.F=%d, Descr.V=%d, StartSlot=%d", NumViews, ppShaderResourceViews, pscdesc.ByteWidth, Descr.Format, Descr.ViewDimension, StartSlot);
		//Helpers::LogFormat("pssrStartSlot=[%d] pssrStride=[[ %d ]] curStride=[%d] size()=[%d]", StartSlot, pssrStride, Stride, lstAllStride00.size());

		if (find(lstAllStride00.begin(), lstAllStride00.end(), Stride) != lstAllStride00.end()) {
			//找到
		}
		else {
			//没找到

			lstAllStride00.push_back(Stride);
		}
	}

	if (((1 == StartSlot) || (2 == StartSlot) || (pssrStartSlot == StartSlot))
		&&
		((Stride == 12) ||(Stride == 24))
		&&
		(Descr.Format==28 && Descr.ViewDimension==4)//28,4 10,9
		)//不显示它的贴图
	//if ((pssrStartSlot == StartSlot)) 
	{
		//Helpers::LogFormat("不显示它的贴图..........curStartSlot=[%d] curStride=[%d] pssrStride=[[ %d ]] size()=[%d]", StartSlot, Stride, pssrStride, lstAllStride00.size());
		//Helpers::LogFormat("不显示它的贴图==> NVs=%d, SRVs=%08x BW=%d, F=%d, V=%d, StartSlot=%d", NumViews, ppShaderResourceViews, pscdesc.ByteWidth, Descr.Format, Descr.ViewDimension, StartSlot);
		//tmppp(pContext, StartSlot, NumViews);
		//if (d2dTexture)
		//{
		//	//Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, (ID3D11ShaderResourceView *const *)d2dTexture);
		//}
		//pContext->OMGetDepthStencilState(&ppDepthStencilState__Old, &pStencilRef);

		////if (ppDepthStencilStateNew == NULL)
		//{
		//	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		//	ppDepthStencilState__Old->GetDesc(&depthStencilDesc);

		//	//depthStencilDesc.DepthEnable = TRUE;
		//	//depthStencilDesc.DepthEnable = FALSE;
		//	//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		//	//depthStencilDesc.StencilEnable = FALSE;
		//	ID3D11Device *ppDevice;
		//	pContext->GetDevice(&ppDevice);
		//	ppDevice->CreateDepthStencilState(&depthStencilDesc, &ppDepthStencilState__New);
		//	pContext->OMSetDepthStencilState(ppDepthStencilState__New, pStencilRef);
		//}

		//pContext->PSSetShader(psRed, NULL, NULL);
	}
	else
		Hooks::oPSSetShaderResources(pContext, StartSlot, NumViews, ppShaderResourceViews);

	return;
}

void __stdcall Hooks::hkD3D11UpdateSubresource(ID3D11DeviceContext* pContext, ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
	//system("cls");
	//int i = 0;
	//Helpers::LogFormat("====\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f\r\n%03.8f %03.8f %03.8f %03.8f", 
	//	*(float*)((int)pSrcData + 0), *(float*)((int)pSrcData + 4), *(float*)((int)pSrcData + 8), *(float*)((int)pSrcData + 12),
	//	*(float*)((int)pSrcData + 16), *(float*)((int)pSrcData + 16 + 4), *(float*)((int)pSrcData + 16 + 8), *(float*)((int)pSrcData + 16 + 12),
	//	*(float*)((int)pSrcData + 32), *(float*)((int)pSrcData + 32 + 4), *(float*)((int)pSrcData + 32 + 8), *(float*)((int)pSrcData + 32 + 12),
	//	*(float*)((int)pSrcData + 48), *(float*)((int)pSrcData + 48 + 4), *(float*)((int)pSrcData + 48 + 8), *(float*)((int)pSrcData + 48 + 12)
	//);
	//UINT Stride;
	//ID3D11Buffer *veBuffer;
	//UINT veBufferOffset = 0;
	//pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);

	//int i = 0;

	//if (Stride == 24)
	{
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
	g_StartSlot = StartSlot;
	//MyTraceA("hkD3D11PSSetSamplers==> pContext=%08x, StartSlot=%d, NumSamplers=%d, ppSamplers=%08x", pContext, StartSlot, NumSamplers, ppSamplers);
	return Hooks::oPSSetSamplers(pContext, StartSlot, NumSamplers, ppSamplers);
}

void __stdcall Hooks::hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexed++++++++++++++++++++*===");
	//CheatIt(pContext, IndexCount, 0, StartIndexLocation, BaseVertexLocation, 0);

	Hooks::oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);

	//pContext->OMSetDepthStencilState(ppDepthStencilState__Old, pStencilRef);
	return;

}

void __stdcall Hooks::hkD3D11Map(ID3D11DeviceContext* pContext, _In_ ID3D11Resource *pResource, _In_ UINT Subresource, _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE *pMappedResource)
{
	//锁定顶点缓存为了可以进行写入（动态缓存不能用UpdateSubResources写入）  
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//(pContext->Map(pResource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);

	Hooks::oMap(pContext, pResource, Subresource, MapType, MapFlags, pMappedResource);

	//m_immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 得到const buffer指针.  
	//if (D3D11_MAP_WRITE_DISCARD == MapType)
	if ((g_StartSlot == 1) && (Stride == 24))
	{
		UINT* p = (UINT*)pMappedResource->pData;
		//Helpers::LogFormat("%08x %08x %08x %08x %08x %08x %08x %08x Slot=%d Stride=%d", *p, *(p + 4), *(p + 8), *(p + 12), *(p + 16), *(p + 20), *(p + 24), *(p + 28), g_StartSlot, Stride);
	}

	//if (Stride == 24)
	//{
	//	//获取指向顶点缓存的指针  
	//	Vertex* verticesPtr;
	//	verticesPtr = (Vertex*)pMappedResource->pData;

	//	////把数据复制进顶点缓存  
	//	//memcpy(verticesPtr, (void*)vertexs, (sizeof(Vertex) * mVertexCount));

	//	////解锁顶点缓存  
	//	//d3dDeviceContext->Unmap(md3dVertexBuffer, 0);
	//	Helpers::LogFormat("hkD3D11Map...(Stride:24) %03.8f %03.8f %03.8f %03.8f", *(float*)((int)verticesPtr + 28), *(float*)((int)verticesPtr + 32), *(float*)((int)verticesPtr + 36), *(float*)((int)verticesPtr + 40));
	//}
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

void __stdcall Hooks::hkD3D11DrawIndexedInstanced(ID3D11DeviceContext* pContext, UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
	//Helpers::LogAddress("\r\n hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	//	OutputDebugStringA("hkD3D11DrawIndexedInstanced++++++++++++++++++++*===");
	DWORD bgtime = timeGetTime();
	if (!bCheat)
	{
		Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		return;
	}
	UINT Stride;
	ID3D11Buffer *veBuffer;
	UINT veBufferOffset = 0;
	pContext->IAGetVertexBuffers(0/*StartSlot*/, 1, &veBuffer, &Stride, &veBufferOffset);
	//MyTraceA("hkD3D11DrawIndexedInstanced**************Stride=%d IndexCountPerInstance=%d InstanceCount=%d StartIndexLocation=%d BaseVertexLocation=%d StartInstanceLocation=%d \r\n", Stride, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);

	{
		UINT IndexCountStride = IndexCountPerInstance * 100 + Stride;
		if (find(lstAll2412.begin(), lstAll2412.end(), IndexCountStride) != lstAll2412.end()) {
			//找到
		}
		else {
			//没找到
			lstAll2412.push_back(IndexCountStride);
			//Helpers::LogFormat("lstAll2412.push_back(%d) ", IndexCountStride);
		}
	}

	if (bVideo4Rec_SCROL)
	{
		if ((Stride == iStride) && (IndexCountPerInstance == iIndexCount))
		{
			//if ((Stride == 24) || (Stride == 12))
			{
				Helpers::LogFormat("PSSetShader(psRed, NULL, NULL) iStride=[%d] iIndexCount=[[ %d ]]", iStride, iIndexCount);
				pContext->PSSetShader(psRed, NULL, NULL);
			}
		}

		Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		return;
	}

	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 1 usedTime = ", timeGetTime() - bgtime);
	Send2Hwnd(IndexCountPerInstance, Stride);

	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 2 usedTime = ", timeGetTime() - bgtime);
	if (bCrossDraw)
	{
		CheatIt(pContext, IndexCountPerInstance, InstanceCount/**/, StartIndexLocation, BaseVertexLocation, StartInstanceLocation/**/);
	}

	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 3 usedTime = ", timeGetTime() - bgtime);

	std::string szCurIdx = std::to_string(IndexCountPerInstance);
	while (szCurIdx.length() < 5)
	{
		szCurIdx = "0" + szCurIdx;
	}
	szCurIdx = std::to_string(Stride) + "_" + szCurIdx;

	list<string>::iterator iter;
	iter = std::find(sHideList.begin(), sHideList.end(), szCurIdx);

	bool bInList = false;
	if (iter != sHideList.end())
	{
		//lst中存在 
		bInList = true;
	}
	else
	{
		//没找到
		bInList = false;
	}
	//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 4 usedTime = ", timeGetTime() - bgtime);

	/*if (!(bHideTrees && (Stride == 12) && (
		IndexCountPerInstance < abc ||

		//IndexCountPerInstance == iIndexCnt ||
		IndexCountPerInstance == 6 ||
		IndexCountPerInstance == 18 ||
		IndexCountPerInstance == 27 ||
		IndexCountPerInstance == 45 ||
		IndexCountPerInstance == 2991 || //2991 42	3456
		IndexCountPerInstance == 600 || //2991 42	3456
		IndexCountPerInstance == 42 || //2991 42	3456
		IndexCountPerInstance == 3456  //2991 42	3456
		)))*/
		//if (! (bHideTrees && (Stride == 12 ) && (IndexCountPerInstance <abc)))
		//if ((IndexCountPerInstance>= abc))
	{
		//if (!bInList)
		if (((Stride == gStride) && bHideTrees 
			 /*&&(
									(IndexCountPerInstance <= iMin) ||
									(IndexCountPerInstance >= iMax))*/
			))
		{
			//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 55 usedTime = ", timeGetTime() - bgtime);
		}
		else
		{
			if (bHideGrass && 
				(Stride == 12) &&
				(
				(IndexCountPerInstance == 6) ||
					(IndexCountPerInstance == 15) ||
					(IndexCountPerInstance == 18) ||
					(IndexCountPerInstance == 21) ||
					(IndexCountPerInstance == 27) ||
					(IndexCountPerInstance == 45)
					)
				)
			{
			}
			else if ((!bInList) && !((Stride == 24) && (IndexCountPerInstance == 54)) //6X
				&& !((Stride == 24) && (IndexCountPerInstance == 75)) //3X
				&& !((Stride == 24) && (IndexCountPerInstance == 72)) //
				)
			{

				//Helpers::Log2Txt("hkD3D11DrawIndexedInstanced++++++++++++++++++++*=== 5 usedTime = ", timeGetTime() - bgtime);
				Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			}
		}
	}

	//if (bShow24)
	//{
	//	if ((Stride != 12))
	//	{
	//			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	//	}
	//}
	//else
	//{
	//	if ((Stride != 24))
	//	{
	//			Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	//	}

	//}
				//Hooks::oDrawIndexedInstanced(pContext, IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
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

