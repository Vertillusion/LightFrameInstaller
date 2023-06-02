// LightFrameInstaller.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "LightFrameInstaller.h"

#include <Urlmon.h>
#include <shellapi.h>
#pragma comment(lib,"Urlmon.lib")

#define MSG_BEGIN				L"按确定开始后台更新\r\n更新完成后将自动重启LightFrame"
#define MSG_CANCEL				L"用户取消"
#define MSG_INVALIDPARAGRAM		L"无效参数！"
#define MSG_UNNECESSARY			L"LightFrame已是最新"
#define MSG_NETWORKERR			L"更新失败：网络错误"
#define URL_LIGHTFRAME			L"https://dl.vertillusion.xyz/lightframe/release/LightFrame.zip"
#define URL_BUILDVER			L"https://dl.vertillusion.xyz/lightframe/release/buildver"

int GetRemoteVer() {
	IStream* stream;
	HRESULT ret = URLOpenBlockingStreamW(
		nullptr,
		URL_BUILDVER,
		&stream,
		0,
		nullptr
	);
	if (ret != S_OK) {
		return -1;
	}

	char buffer[1024] = { 0 };
	DWORD readBytes = 0;

	do {
		stream->Read(
			buffer,
			1024,
			&readBytes
		);
	} while (readBytes > 0);

	stream->Release();

	return atoi(buffer);
}

bool DownloadZipFile() {
	HRESULT ret = URLDownloadToFileW(
		nullptr,
		URL_LIGHTFRAME,
		L"LightFrame.zip",
		0,
		nullptr
	);

	return !(ret == S_OK);
}

void CloseLightFrame() {
	HWND hWndLF = FindWindow(L"LIGHTFRAME", L"LightFrame");
	::PostMessage(hWndLF, 0xff3, 0, 1);
}

void UnzipNewLightFrame(LPCWSTR lpLightFrameName) {
	HZIP hz = OpenZip(L"LightFrame.zip", 0);
	ZIPENTRY ze; GetZipItem(hz, -1, &ze);
	GetZipItem(hz, 0, &ze);
	UnzipItem(hz, 0, lpLightFrameName);
	CloseZip(hz);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR	lpCmdLine,
					 _In_ int	   nCmdShow)
{
	int RemoteVer, LocalVer;
	LPCWSTR lpFileName;

	lpFileName = L"LightFrame.exe";
	LocalVer = -1;

	for (int i = 1; i < __argc; i++) {
		if (wcsstr(__wargv[i], L"--CurrentVer"))
			LocalVer = _wtoi(__wargv[++i]);
		if (wcsstr(__wargv[i], L"--FileName"))
			lpFileName = __wargv[++i];
	}

	if (LocalVer == -1) {
		MessageBox(NULL, MSG_INVALIDPARAGRAM, L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	
	if (MessageBox(NULL, MSG_BEGIN, L"LightFrameInstaller", MB_YESNO) == IDNO) {
		MessageBox(NULL, MSG_CANCEL, L"Info", MB_OK);
		return 0;
	}

	RemoteVer = GetRemoteVer();
	if (RemoteVer == -1) {
		MessageBox(NULL, MSG_NETWORKERR, L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	if(RemoteVer<=LocalVer){
		MessageBox(NULL, MSG_UNNECESSARY, L"Info", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	if(DownloadZipFile()) {
		MessageBox(NULL, MSG_NETWORKERR, L"Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	CloseLightFrame();
	DeleteFile(lpFileName);

	UnzipNewLightFrame(lpFileName);

	DeleteFile(L"LightFrame.zip");

	return 0;
}
