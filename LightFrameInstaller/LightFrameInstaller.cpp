// LightFrameInstaller.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "LightFrameInstaller.h"

#define MAX_LOADSTRING 100

VertexUIInit;

// 全局变量:
HINSTANCE hInst;								// 当前实例
WCHAR szTitle[MAX_LOADSTRING];				  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR	lpCmdLine,
					 _In_ int	   nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_LIGHTFRAMEINSTALLER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	VuiColorSystemInit();
	TargetPanel = 0;
	NodeNum = 0;

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LIGHTFRAMEINSTALLER));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style		  = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra	 = 0;
	wcex.cbWndExtra	 = 0;
	wcex.hInstance	  = hInstance;
	wcex.hIcon		  = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LIGHTFRAMEINSTALLER));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//		在此函数中，我们在全局变量中保存实例句柄并
//		创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, 600,350, nullptr, nullptr, hInstance, nullptr);

   SendMessage(hWnd, WM_CREATE, 0, 0);
   CenterWindow(hWnd);
   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
   Style = Style & ~WS_CAPTION & ~WS_SYSMENU & ~WS_SIZEBOX;
   ::SetWindowLongPtr(hWnd, GWL_STYLE, Style);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   VertexUI::Window::OuterShadow::DropShadow Shadow;
   Shadow.Initialize(0);
   Shadow.SetSharpness(60);
   Shadow.SetSize(25);
   Shadow.SetDarkness(160);
   Shadow.SetPosition(0, 0);
   Shadow.Create(hWnd);

   if (!hWnd)
   {
	  return FALSE;
   }

   FreeResFile(IDR_CERT1, L"Cert", L"cacert.pem");
   GlobalhWnd = hWnd;
   CreateThread(
	   NULL,
	   0,
	   OnUpdateMain,
	   NULL,
	   0,
	   NULL
   );

   return TRUE;
}

int OnGetNodes() {
	DNS_STATUS status;
	PDNS_RECORD pDnsRecord;
	std::string QueryResult;
	Json::Reader jReader;
	Json::Value jRoot;

//	解析节点
//	参考：https://learn.microsoft.com/zh-cn/troubleshoot/windows/win32/use-dnsquery-resolve-host-names
	status = DnsQuery(
#ifdef _DEBUG
		L"testnodes.dl.vertillusion.xyz",
#else
		L"nodes-v2.dl.vertillusion.xyz",
#endif
		DNS_TYPE_TEXT,
		DNS_QUERY_BYPASS_CACHE | DNS_QUERY_NO_LOCAL_NAME | DNS_QUERY_NO_HOSTS_FILE | DNS_QUERY_ACCEPT_TRUNCATED_RESPONSE,
		NULL,
		&pDnsRecord,
		NULL
	);
	if (status || pDnsRecord == NULL)return -1;

	QueryResult = UnicodeStringToSTLString(*(pDnsRecord->Data.TXT.pStringArray));
	
	if (!jReader.parse(QueryResult, jRoot))return -1;

	CURL* mCurl = curl_easy_init();
	CURLcode mCode;
	std::string mUA = USERAGENT;
	std::string szbuffer;
	std::string szheader_buffer;
	int num = jRoot.size() > MAX_NODE ? MAX_NODE : jRoot.size();
	double val;

	curl_global_init(CURL_GLOBAL_ALL);
	curl_easy_setopt(mCurl, CURLOPT_USERAGENT, mUA.c_str());	//设置UserAgent
	curl_easy_setopt(mCurl, CURLOPT_SSL_VERIFYHOST, 0L);		//设置SSL验证级别（0-2，宽松-严格）
	curl_easy_setopt(mCurl, CURLOPT_CAINFO, "cacert.pem");		//根证书信息
	curl_easy_setopt(mCurl, CURLOPT_MAXREDIRS, 5);				//设置最大重定向次数
	curl_easy_setopt(mCurl, CURLOPT_FOLLOWLOCATION, 1);			//设置301、302跳转跟随location
	curl_easy_setopt(mCurl, CURLOPT_TIMEOUT, MAXLATENCY / 1000);//超时设置
	curl_easy_setopt(mCurl, CURLOPT_CONNECTTIMEOUT, 5L);		//连接超时设置
	curl_easy_setopt(mCurl, CURLOPT_FAILONERROR, 1);			//服务端返回40x代码时返回错误而不是下载错误页
	//抓取内容后，回调函数  
	curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, curl_default_callback);
	curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, &szbuffer);
	//抓取头信息，回调函数  
	curl_easy_setopt(mCurl, CURLOPT_HEADERFUNCTION, curl_default_callback);
	curl_easy_setopt(mCurl, CURLOPT_HEADERDATA, &szheader_buffer);

	for (int i = 0; i < num; i++) {
		szbuffer = "";
		szheader_buffer = "";

		Node[i].Config = jRoot[i].asString();
		Node[i].Reachable = false;

		curl_easy_setopt(mCurl, CURLOPT_URL, Node[i].Config.c_str());

		mCode = curl_easy_perform(mCurl);
		if (mCode != CURLE_OK)
			continue;

		mCode = curl_easy_getinfo(mCurl, CURLINFO_TOTAL_TIME, &val);
		if ((CURLE_OK == mCode) && (val > 0))
			Node[i].Latency = val * 1000;

		Json::Value jNode;
		if (!jReader.parse(szbuffer, jNode))
			continue;

		Node[i].Name = jNode["name"].asString();
		Node[i].Lightframe = jNode["lightframe"].asString();
		Node[i].Buildver = jNode["buildver"].asString();

		Node[i].Reachable = true;
	}

	curl_easy_cleanup(mCurl);
	curl_global_cleanup();

	int pos = -1,
		min = MAXLATENCY;

	for (int i = 0; i < num; i++)
		if (Node[i].Reachable && Node[i].Latency < min)
			pos = i, min = Node[i].Latency;

	return pos;
}

DWORD WINAPI OnUpdateMain(LPVOID lpParam) {
	OnGetNodes();
	CURL* mCurl = curl_easy_init();
	CURLcode mCode;
	std::string mUA = USERAGENT;
	std::string szbuffer;
	std::string szheader_buffer;
	return 0;
}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int val = 0;
		ClickAreaPtInfo(hWnd, 0, 0, rc.right - 35, 40, lParam, val);
		if (val == 1)
		{
			SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
		else
		{
			SendKeepDownEvent(hWnd, wParam, lParam);
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		SendClickEvent(hWnd, wParam, lParam);
		break;
	}
	case WM_MOUSEMOVE: {
		if (GetPtInfo(hWnd, rc.right - 40, 6, 35, 35))
		{
			if (ClickMsg == 1)
			{

				ClickMsg = 0;
				PostQuitMessage(0);
				return 0;
			}
		}
		add_event(MainPanel[TargetPanel]);
		if (hState == 1) {
			hState = 0;
			InvalidateRect(hWnd, &rc, 0);
			return 0;
		}
		break;
	}
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			CreatePanel(hWnd, hdc, [](HWND hWnd, HDC hdc)->void {
				RECT rc;

				GetClientRect(hWnd, &rc);
				CreateFillArea(hWnd, hdc, VERTEXUICOLOR_MIDNIGHT);


				MainPanel[TargetPanel].Set(hWnd, hdc);

				PanelDrawCloseBtn(hWnd, hdc, rc.right - 34, 6, 25, 25, 4, RGB(187, 192, 201));

				VertexUIControl Cancel;
				Cancel.CreateCtl(L"Button2", rc.right - 100, rc.bottom - 70, 80, 45,
					[&, hWnd] {
						PostQuitMessage(0);
						return 0; 
					}
				, L"取消", { VERTEXUICOLOR_LAVENDER });
				MainPanel[TargetPanel].Add(Cancel);


				});
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_SIZE: {
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
