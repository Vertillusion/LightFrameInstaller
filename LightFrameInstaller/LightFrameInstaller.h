#pragma once

#ifdef _WIN64
#error Build for WIN64 is not available yet.
#endif

#include "resource.h"

#include <json/json.h>
#include <VertexUI/VertexUI.min.h>
#include <curl/curl.h>
#include <WinDNS.h>

#pragma comment(lib,"lib/libcurl.lib")
#pragma comment(lib,"Dnsapi.lib")
#ifdef _DEBUG
#pragma comment(lib,"../Debug/jsoncpp.lib")
#else
#pragma comment(lib,"../Release/jsoncpp.lib"
#endif