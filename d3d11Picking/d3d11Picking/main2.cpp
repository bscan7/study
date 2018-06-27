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

//全局描述符
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

//索引缓冲
ID3D11Buffer* squareIndexBuffer;
ID3D11Buffer* squareVertBuffer;

//深度值-20170927
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;

//着色器
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

const int Width = 800; //设置宽
const int Height = 800; // 设置高

						///四个空间以及相机属性
XMMATRIX WVP;
XMMATRIX World;
XMMATRIX camView;
XMMATRIX camProjection;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

//////函数声明
bool InitializeDirect3d11App(HINSTANCE hInstance);
//void ReleaseObjects();
void CleanUp();
bool InitScene();
void UpdateScene();
void DrawScene();

// 初始化窗口
bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed);

//初始化消息循环函数
int messageloop();
//初始化窗口回调过程。Windows API是事件驱动型的编程模型。在该函数中捕获Windows消息，比如一个按键按下（也叫事件）以及程序操作流程。

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

//创建效果常量缓冲的结构体
struct cbPerObject
{
	XMMATRIX WVP;
};

cbPerObject cbPerObj;

//顶点结构体以及顶点布局（输入布局）
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

//主函数，传入应用程序句柄hInstance,前一个应用程序句柄hPrevInstance，传给函数处理的命令行lpCmdLine以及窗口显示方式的nShowCmd
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	//创建并注册窗口
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBoxA(0, "Window Initilization - Failed", "Error", MB_OK);
		return 0;
	}

	/////new
	if (!InitializeDirect3d11App(hInstance)) // 初始化D3D
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
// windowed 若为true则为窗口模式显示，若为false则为全屏模式显示
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
	wc.cbSize = sizeof(WNDCLASSEX); //window类的大小
									/********windows类风格
									*CS_CLASSDC 一个使用该类创建的在所有窗口间共享的设备上下文
									*CS_DBLCLKS 在窗口上使能双击功能
									*CS_HREDRAW 若窗口的宽度有改变或者窗口水平地移动，窗口将会刷新
									*CS_NOCLOSE 窗口菜单上禁止关闭选项
									*CS_OWNDC   为每个窗口创建自己的设备上下文。正好与CS_CLASSDC相反
									*CS_PARENTDC 这会设置创建的子窗口的剪裁四边形到父窗口，这允许子窗口能够在父窗口上绘画
									*CS_VERDRAW 若在窗口的高度或窗口在垂直方向有移动窗口会重绘
									**/
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//lpfnWndProc是一个指向处理窗口消息函数的指针，设置窗口处理函数的函数名WndProc
	wc.lpfnWndProc = WndProc;
	//cbClsExtra是WNDCLASSEX之后额外申请的字节数
	wc.cbClsExtra = NULL;
	//cbWndExtra指定窗口实例之后所申请的字节数
	wc.cbWndExtra = NULL;
	//当前窗口应用程序的句柄，通过给函数GetModuleHandle()函数第一个参数传入NULL可获取当前窗口应用程序。
	wc.hInstance = hInstance;

	//hIcon用来指定窗口标题栏左上角的图标。以下是一些标准图标：
	/*
	*IDI_APPLICATION 默认应用程序图标
	*IDI_HAND 手形状的图标
	*IDI_EXCLAMATION 感叹号图标
	*IDI_INFORMATION 星号图标
	*IDI_QUESTION 问号图标
	*IDI_WINLOGO 若使用的是XP则是默认应用程序图标，否则是窗口logo
	*/
	wc.hIcon = LoadIcon(NULL, (LPCTSTR)IDI_HAND);

	/*定义光标图标
	*IDC_APPSTARTING 标准箭头以及小型沙漏光标
	*IDC_ARROW 标准箭头光标
	*IDC_CROSS 十字线光标
	*IDC_HAND 手型光标
	*IDC_NO 斜线圈光标
	*IDC_WAIT 沙漏光标
	*/
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//hbrBackground是一个刷子的句柄，可使得背景黑色。
	wc.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW + 2);
	//附加到窗口的菜单名字，不需要的话设置为NULL
	wc.lpszMenuName = NULL;
	//对类进行命名
	wc.lpszClassName = WndClassName;
	//指定任务栏的图标，使用上面的IDI＿图标
	wc.hIconSm = LoadIcon(NULL, (LPCTSTR)IDI_HAND);
	//注册类。若失败则会获得一个错误，若成功，则继续创建窗口
	if (!RegisterClassEx(&wc))
	{
		MessageBoxA(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//创建窗口
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
	UpdateWindow(hwnd);// 发送WM_PAINT消息到窗口过程，若窗口客户区没有任何东西要显示，则不发送消息。返回true，继续运行到mainloop中去。

	return true;
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	//声明缓冲
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//声明交换链
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


	//创建交换链
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//创建后缓冲
	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//创建渲染目标
	d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//创建深度模板缓冲
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; //绑定到OM
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//创建深度模板视图
	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	//设置渲染目标
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
	//释放创建的COM对象
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
}

bool InitScene()
{
	//编译着色器
	hr = D3DX11CompileFromFileA("Effects2.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFileA("Effects2.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);

	//创建着色器对象
	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	//设置顶点和像素着色器
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	//创建顶点缓冲
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

	//设置顶点缓冲
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

	//创建输入布局
	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//设置输入布局
	d3d11DevCon->IASetInputLayout(vertLayout);

	//设置图元拓扑
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//创建视口
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;

	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	//设置视口
	d3d11DevCon->RSSetViewports(1, &viewport);

	//创建缓冲用来发送到效果文件的cbuffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	//相机信息
	camPosition = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//设置视图矩阵
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//设置投影矩阵
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)Width / Height, 1.0f, 1000.0f);




	return true;
}

void UpdateScene()
{
	// 更新场景颜色
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
	//将更新的颜色填充后缓冲
	//	D3DXCOLOR bgColor(red, green, blue, 1.0f);
	float bgColor[4] = { (0.0f, 0.0f, 0.0f, 0.0f) };
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	//刷新深度模板视图
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//设置世界/视图/投影矩阵，随后发送到效果文件的常量缓冲中
	World = XMMatrixIdentity();

	WVP = World * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	//绘制三角形
	d3d11DevCon->DrawIndexed(6, 0, 0);
	//画三角形
	//d3d11DevCon->Draw(3, 0);

	//将后缓冲呈现到屏幕
	SwapChain->Present(0, 0);
}

int messageloop() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));//清除结构体被设为NULL。

	while (true) {
		//使用PeekMessage()检查是否有消息传进来
		/*LPMSG lpMsg 消息结构体的指针
		*HWND hWnd 发送消息的窗口句柄。若设为NULL，那么它会从当前程序中接收来自任何一个窗口的消息
		*UINT wMsgFilterMin 指定消息范围内第一个要检查的消息的值。若wMsgFilterMin和wMsgFilterMax都设为0，那么PeekMessage将会检查素有的消息
		*UINT wMsgFilterMax 指定消息范围内最后一个要检测的消息的值
		*UINT wRemoveMsg 指定消息的处理方式。若设置为PM_REMOVE，则在读取之后会被删除
		*/
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			//若消息为窗口消息，则解析并分发它。TranslateMessage()将会让窗口做一些解析，类似键盘的虚拟键值转换到字符形式。
			//而DispatchMessage()则发送消息到窗口过程WndProc。
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //若没有窗口消息，则运行游戏
		{
			// run game code
			UpdateScene();
			DrawScene();
		}
	}
	return msg.wParam;
}

//窗口消息处理函数
//HWND hwnd 获取消息的窗口句柄
//UINT msg 消息的内容
/*
*WM_ACTIVE 当窗口激活时发送的消息
*WM_CLOSE 当窗口关闭时发送的消息
*WM_CREATE 当窗口创建时发送的消息
*WM_DESTROY 当窗口销毁时发送的消息
*/
//wParam和lParam时消息的额外信息。使用wParam来检测键盘输入消息
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	// 这是事件检测消息的地方，若escape键被按下，会显示一个消息框，询问是否真的退出。若点击yes，则程序关闭。若不点击，则消息框关闭。若消息包含WM_DESTROY
	// 则意味着窗口正在被销毁，返回0并且程序关闭
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

	//调用默认窗口过程函数
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}