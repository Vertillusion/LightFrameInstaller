#pragma once

#include "resource.h"
#include <string>

#include <VertexUI/VertexUI.min.h>
#include <VertexUI/VertexUI.Window.hpp>
#include <WinDNS.h>
#include <base64/base64.h>

#pragma comment(lib,"Dnsapi.lib")

#define PANELS				3

HWND GlobalhWnd;
VertexUIPanel MainPanel[PANELS];
int TargetPanel;
int NodeNum;

//
//  ������OnUpdateMain(LPVOID)
//
//  ���ܣ��������߳�
//
DWORD WINAPI OnUpdateMain(LPVOID lpParam);
