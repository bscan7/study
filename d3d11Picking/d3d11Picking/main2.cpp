//#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
//#include "Resource.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

//ȫ��������
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

//��������
ID3D11Buffer* squareIndexBuffer;
ID3D11Buffer* squareVertBuffer;

//���ֵ-20170927
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;

//��ɫ��
ID3D11Buffer* triangleVertBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;

///
ID3D11Buffer* cbPerObjectBuffer;
///


#if 0
float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colormodr = 1;
int colormodg = 1;
int colormodb = 1;
#endif
/////
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;
HRESULT hr;

const int Width = 800; //���ÿ�
const int Height = 800; // ���ø�

						///�ĸ��ռ��Լ��������
XMMATRIX WVP;
XMMATRIX World;
XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

//////��������
bool InitializeDirect3d11App(HINSTANCE hInstance);
//void ReleaseObjects();
void CleanUp();
bool InitScene();
void UpdateScene();
void DrawScene();

// ��ʼ������
bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed);

//��ʼ����Ϣѭ������
int messageloop();
//��ʼ�����ڻص����̡�Windows API���¼������͵ı��ģ�͡��ڸú����в���Windows��Ϣ������һ���������£�Ҳ���¼����Լ�����������̡�

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

//����Ч����������Ľṹ��
struct cbPerObject
{
	XMMATRIX WVP;
};

cbPerObject cbPerObj;

//����ṹ���Լ����㲼�֣����벼�֣�
struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z,
		float cr, float cg, float cb, float ca)
		:pos(x, y, z), color(cr, cg, cb, ca) {}
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

D3D11_INPUT_ELEMENT_DESC layout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
UINT numElements = ARRAYSIZE(layout);

//������������Ӧ�ó�����hInstance,ǰһ��Ӧ�ó�����hPrevInstance���������������������lpCmdLine�Լ�������ʾ��ʽ��nShowCmd
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	//������ע�ᴰ��
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBoxA(0, "Window Initilization - Failed", "Error", MB_OK);
		return 0;
	}

	/////new
	if (!InitializeDirect3d11App(hInstance)) // ��ʼ��D3D
	{
		MessageBoxA(0, "Direct3D Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	if (!InitScene())
	{
		MessageBoxA(0, "Scene Initialization - Failed", "Error", MB_OK);
		return 0;
	}

	messageloop();
	CleanUp();
	//ReleaseObjects();

	return 0;
}
// windowed ��Ϊtrue��Ϊ����ģʽ��ʾ����Ϊfalse��Ϊȫ��ģʽ��ʾ
bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed)
{
	/*typedef struct _WNDCLASS{
	UINT cbSize;
	UINT style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HANDLE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCTSTR lpszMenuName;
	LPCTSTR lpszClassName;
	}WNDCLASS;
	*/
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); //window��Ĵ�С
									/********windows����
									*CS_CLASSDC һ��ʹ�ø��ഴ���������д��ڼ乲����豸������
									*CS_DBLCLKS �ڴ�����ʹ��˫������
									*CS_HREDRAW �����ڵĿ���иı���ߴ���ˮƽ���ƶ������ڽ���ˢ��
									*CS_NOCLOSE ���ڲ˵��Ͻ�ֹ�ر�ѡ��
									*CS_OWNDC   Ϊÿ�����ڴ����Լ����豸�����ġ�������CS_CLASSDC�෴
									*CS_PARENTDC ������ô������Ӵ��ڵļ����ı��ε������ڣ��������Ӵ����ܹ��ڸ������ϻ滭
									*CS_VERDRAW ���ڴ��ڵĸ߶Ȼ򴰿��ڴ�ֱ�������ƶ����ڻ��ػ�
									**/
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//lpfnWndProc��һ��ָ��������Ϣ������ָ�룬���ô��ڴ������ĺ�����WndProc
	wc.lpfnWndProc = WndProc;
	//cbClsExtra��WNDCLASSEX֮�����������ֽ���
	wc.cbClsExtra = NULL;
	//cbWndExtraָ������ʵ��֮����������ֽ���
	wc.cbWndExtra = NULL;
	//��ǰ����Ӧ�ó���ľ����ͨ��������GetModuleHandle()������һ����������NULL�ɻ�ȡ��ǰ����Ӧ�ó���
	wc.hInstance = hInstance;

	//hIcon����ָ�����ڱ��������Ͻǵ�ͼ�ꡣ������һЩ��׼ͼ�꣺
	/*
	*IDI_APPLICATION Ĭ��Ӧ�ó���ͼ��
	*IDI_HAND ����״��ͼ��
	*IDI_EXCLAMATION ��̾��ͼ��
	*IDI_INFORMATION �Ǻ�ͼ��
	*IDI_QUESTION �ʺ�ͼ��
	*IDI_WINLOGO ��ʹ�õ���XP����Ĭ��Ӧ�ó���ͼ�꣬�����Ǵ���logo
	*/
	wc.hIcon = LoadIcon(NULL, (LPCTSTR)IDI_HAND);

	/*������ͼ��
	*IDC_APPSTARTING ��׼��ͷ�Լ�С��ɳ©���
	*IDC_ARROW ��׼��ͷ���
	*IDC_CROSS ʮ���߹��
	*IDC_HAND ���͹��
	*IDC_NO б��Ȧ���
	*IDC_WAIT ɳ©���
	*/
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//hbrBackground��һ��ˢ�ӵľ������ʹ�ñ�����ɫ��
	wc.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW + 2);
	//���ӵ����ڵĲ˵����֣�����Ҫ�Ļ�����ΪNULL
	wc.lpszMenuName = NULL;
	//�����������
	wc.lpszClassName = WndClassName;
	//ָ����������ͼ�꣬ʹ�������IDI��ͼ��
	wc.hIconSm = LoadIcon(NULL, (LPCTSTR)IDI_HAND);
	//ע���ࡣ��ʧ�������һ���������ɹ����������������
	if (!RegisterClassEx(&wc))
	{
		MessageBoxA(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//��������
	hwnd = CreateWindowEx(NULL, WndClassName, L"Drawing Square", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width,
		height, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBoxA(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//BOOL ShowWindow(HWND hWnd, int nCmdShow);
	//BOOL UpdateWindow(HWND hWnd);

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);// ����WM_PAINT��Ϣ�����ڹ��̣������ڿͻ���û���κζ���Ҫ��ʾ���򲻷�����Ϣ������true���������е�mainloop��ȥ��

	return true;
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	//��������
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//����������
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//����������
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//�����󻺳�
	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//������ȾĿ��
	d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//�������ģ�建��
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; //�󶨵�OM
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//�������ģ����ͼ
	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	//������ȾĿ��
	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return true;
}

void CleanUp()
{
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
	renderTargetView->Release();

	squareVertBuffer->Release();
	squareIndexBuffer->Release();

	//triangleVertBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();

	depthStencilView->Release();
	depthStencilBuffer->Release();

	//
	cbPerObjectBuffer->Release();
}

void ReleaseObjects()
{
	//�ͷŴ�����COM����
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}

bool InitScene()
{
	//������ɫ��
	hr = D3DX11CompileFromFileA("Effects2.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFileA("Effects2.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);

	//������ɫ������
	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//���ö����������ɫ��
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	//�������㻺��
	Vertex v[] = {
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = {
		0,1,2,
		0,2,3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);
	d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

	//���ö��㻺��
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

	//�������벼��
	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//�������벼��
	d3d11DevCon->IASetInputLayout(vertLayout);

	//����ͼԪ����
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�����ӿ�
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;

	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	//�����ӿ�
	d3d11DevCon->RSSetViewports(1, &viewport);

	//���������������͵�Ч���ļ���cbuffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	//�����Ϣ
	camPosition = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//������ͼ����
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//����ͶӰ����
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)Width / Height, 1.0f, 1000.0f);




	return true;
}

void UpdateScene()
{
	// ���³�����ɫ
#if 0
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;
#endif
}

void DrawScene()
{
	//�����µ���ɫ���󻺳�
	//	D3DXCOLOR bgColor(red, green, blue, 1.0f);
	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	//ˢ�����ģ����ͼ
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//��������/��ͼ/ͶӰ��������͵�Ч���ļ��ĳ���������
	World = XMMatrixIdentity();

	WVP = World * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	//����������
	d3d11DevCon->DrawIndexed(6, 0, 0);
	//��������
	//d3d11DevCon->Draw(3, 0);

	//���󻺳���ֵ���Ļ
	SwapChain->Present(0, 0);
}

int messageloop() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));//����ṹ�屻��ΪNULL��

	while (true) {
		//ʹ��PeekMessage()����Ƿ�����Ϣ������
		/*LPMSG lpMsg ��Ϣ�ṹ���ָ��
		*HWND hWnd ������Ϣ�Ĵ��ھ��������ΪNULL����ô����ӵ�ǰ�����н��������κ�һ�����ڵ���Ϣ
		*UINT wMsgFilterMin ָ����Ϣ��Χ�ڵ�һ��Ҫ������Ϣ��ֵ����wMsgFilterMin��wMsgFilterMax����Ϊ0����ôPeekMessage���������е���Ϣ
		*UINT wMsgFilterMax ָ����Ϣ��Χ�����һ��Ҫ������Ϣ��ֵ
		*UINT wRemoveMsg ָ����Ϣ�Ĵ���ʽ��������ΪPM_REMOVE�����ڶ�ȡ֮��ᱻɾ��
		*/
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			//����ϢΪ������Ϣ����������ַ�����TranslateMessage()�����ô�����һЩ���������Ƽ��̵������ֵת�����ַ���ʽ��
			//��DispatchMessage()������Ϣ�����ڹ���WndProc��
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //��û�д�����Ϣ����������Ϸ
		{
			// run game code
			UpdateScene();
			DrawScene();
		}
	}
	return msg.wParam;
}

//������Ϣ������
//HWND hwnd ��ȡ��Ϣ�Ĵ��ھ��
//UINT msg ��Ϣ������
/*
*WM_ACTIVE �����ڼ���ʱ���͵���Ϣ
*WM_CLOSE �����ڹر�ʱ���͵���Ϣ
*WM_CREATE �����ڴ���ʱ���͵���Ϣ
*WM_DESTROY ����������ʱ���͵���Ϣ
*/
//wParam��lParamʱ��Ϣ�Ķ�����Ϣ��ʹ��wParam��������������Ϣ
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	// �����¼������Ϣ�ĵط�����escape�������£�����ʾһ����Ϣ��ѯ���Ƿ�����˳��������yes�������رա��������������Ϣ��رա�����Ϣ����WM_DESTROY
	// ����ζ�Ŵ������ڱ����٣�����0���ҳ���ر�
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBoxA(0, "Are you sure you want to exit?",
				"Really?", MB_YESNO | MB_ICONASTERISK) == IDYES)
			{
				DestroyWindow(hwnd);
			}
			return 0;

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	//����Ĭ�ϴ��ڹ��̺���
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}