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

HWND GlobalhWnd;
VertexUIPanel MainPanel[PANELS];
int TargetPanel;
int NodeNum;

//
//  �ṹ��NODE_INFO
//
//  ���ܣ��洢��DNS TXT��¼��ȡ���Ľڵ���Ϣ
//
//  ע�ͣ�
//    File204��generate_204api��ַ�����ڲ���
//    ManagerAPIURL����������������php��ַ
//
struct NODE_INFO {
	std::string File204;
	std::string ManagerAPIURL;
}Node[MAX_NODE];

//
//  ������curl_default_callback(const char*, size_t, size_t, std::string*)
//
//  ���ܣ�curlĬ��callback
//
size_t curl_default_callback(const char* ptr, size_t size, size_t nmemb, std::string* stream) {
	assert(stream != NULL);
	size_t len = size * nmemb;
	stream->append(ptr, len);
	return len;
}

//
//  ������curl_writefile_callback(const char*, size_t, size_t, FILE*)
//
//  ���ܣ�curl�����ļ�callback
//
size_t curl_writefile_callback(void* ptr, size_t size, size_t nmemb, FILE* stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

//
//  ������curl_showprogress_callback(void* , double, double, double, double)
//
//  ���ܣ�curl��ʾ���ؽ���callback
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
//  ������UnicodeStringToSTLString(LPSTR strUTF)
//
//  ���ܣ����ֽ�תSTL
//
char* UnicodeStringToSTLString(const wchar_t* strUTF) {
	char* m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, strUTF, wcslen(strUTF), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, strUTF, wcslen(strUTF), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}