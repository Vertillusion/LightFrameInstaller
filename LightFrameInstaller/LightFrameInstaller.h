#pragma once

#ifdef _WIN64
#error Build for WIN64 is not available yet.
#endif

#include "resource.h"

#include <json/json.h>
#include <VertexUI/VertexUI.min.h>
#include <VertexUI/VertexUI.Window.hpp>
#include <curl/curl.h>
#include <WinDNS.h>
#include <string>

#pragma comment(lib,"lib/libcurl.lib")
#pragma comment(lib,"Dnsapi.lib")
#ifdef _DEBUG
#pragma comment(lib,"../Debug/jsoncpp.lib")
#else
#pragma comment(lib,"../Release/jsoncpp.lib"
#endif

#define PANELS				3
#define MAX_NODE			8
#define USERAGENT			"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1 LightframeInstaller/2.0.0 libcurl/8.0.0"

HWND GlobalhWnd;
VertexUIPanel MainPanel[PANELS];
int TargetPanel;
int NodeNum;

//
//  结构：NODE_INFO
//
//  功能：存储从DNS TXT记录读取到的节点信息
//
//  注释：
//    File204：generate_204api地址，用于测速
//    ManagerAPIURL：服务器包管理器php地址
//
struct NODE_INFO {
	std::string File204;
	std::string ManagerAPIURL;
}Node[MAX_NODE];

//
//  函数：curl_default_callback(const char*, size_t, size_t, std::string*)
//
//  功能：curl默认callback
//
size_t curl_default_callback(const char* ptr, size_t size, size_t nmemb, std::string* stream) {
	assert(stream != NULL);
	size_t len = size * nmemb;
	stream->append(ptr, len);
	return len;
}

//
//  函数：curl_writefile_callback(const char*, size_t, size_t, FILE*)
//
//  功能：curl下载文件callback
//
size_t curl_writefile_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

//
//  函数：curl_showprogress_callback(void* , double, double, double, double)
//
//  功能：curl显示下载进度callback
//
size_t curl_showprogress_callback(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded) {
	CURL* mCurl = static_cast<CURL*>(ptr);
	double fractiondownloaded, speed;
	int pointer;
	char szBuffer[128];
	std::string unit = "B";

	if (TotalToDownload == 0.0)
		fractiondownloaded = 0;
	else
		fractiondownloaded = NowDownloaded / TotalToDownload;
	pointer = round(fractiondownloaded * 100);

	curl_easy_getinfo(mCurl, CURLINFO_SPEED_DOWNLOAD, &speed);

	double leftTime = (TotalToDownload - NowDownloaded) / speed;
	int minutes = leftTime / 60;
	int seconds = leftTime - minutes * 60;

	sprintf_s(szBuffer, "%02d:%02d", minutes, seconds);

	if (speed >= 1024)
	{
		unit = "kB";
		speed /= 1024;
	}
	if (speed >= 1024)
	{
		unit = "MB";
		speed /= 1024;
	}
	if (speed >= 1024)
	{
		unit = "GB";
		speed /= 1024;
	}

	sprintf_s(szBuffer, "%.2f%s/s", speed, unit.c_str());

	return 0;
}

//
//  函数：UnicodeStringToSTLString(LPSTR strUTF)
//
//  功能：宽字节转STL
//
std::string UnicodeStringToSTLString(std::wstring strUTF) {
	char* m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, strUTF.c_str(), strUTF.size(), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, strUTF.c_str(), strUTF.size(), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

//
//  函数：FreeResFile(DWORD, std::wstring, std::wstring)
//
//  功能：释放资源文件到磁盘
//
BOOL FreeResFile(DWORD dwResName, std::wstring lpResType, std::wstring lpFilePathName)
{
	HMODULE hInstance = ::GetModuleHandle(NULL);//得到自身实例句柄  

	HRSRC hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResName), lpResType.c_str());//查找资源  
	HGLOBAL hRes = ::LoadResource(hInstance, hResID);//加载资源  
	LPVOID pRes = ::LockResource(hRes);//锁定资源  

	if (pRes == NULL)//锁定失败  
	{
		return FALSE;
	}
	DWORD dwResSize = ::SizeofResource(hInstance, hResID);//得到待释放资源文件大小  
	HANDLE hResFile = CreateFile(lpFilePathName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//创建文件  

	if (INVALID_HANDLE_VALUE == hResFile)
	{
		//TRACE("创建文件失败！");  
		return FALSE;
	}

	DWORD dwWritten = 0;//写入文件的大小     
	WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL);//写入文件  
	CloseHandle(hResFile);//关闭文件句柄  

	return (dwResSize == dwWritten);//若写入大小等于文件大小，返回成功，否则失败  
}

//
//  函数：OnTestNode()
//  
//  功能：测试节点
//
//  返回值：最快的节点索引
// 
//  注释：
//    被OnUpdateMain调用，非线程，根据cpu核心数量创建线程进行204api测速
//    查询失败时返回-1
//
int OnTestNode();

//
//  函数：OnUpdateMain(LPVOID)
//
//  功能：更新主线程
//
DWORD WINAPI OnUpdateMain(LPVOID lpParam);
