#pragma once
/*
* VertexUI ClickArea
*
* By EnderMo,VertexStudio 2021
*
*/
#include "framework.h"
#include "VertexUI.Panel.h"
#include <stdio.h>


typedef int RUNFUN;

namespace VertexUI
{
	namespace Click
	{
		using namespace::VertexUI::Panel;
		int hState = 0;
		typedef struct VERTEXUIMOUSEEVENT
		{
			UINT hindex = 0;
			UINT phindex = 0;
		}VUIMOUSEEVENT, * PVUIMOUSEEVENT;
		VUIMOUSEEVENT vev;
		int ClickMsg = 0;
		int KeepDownMsg = 0;
		int RClickMsg = 0;
		int RDClickMsg = 0;
		int LDClickMsg = 0;
		template<class rf>
		int ClickArea(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, rf function)
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				function();
				return -1;
			}
			return -1;
		}
		int CursorClickArea(HWND hWnd, int x, int y, int sizex, int sizey, RUNFUN function())
		{
			POINT pt;
			//wchar_t txt[80];
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT rect2;
			GetWindowRect(hWnd, &rect2);
			pt.x += rect2.left;
			pt.y += rect2.top;
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				function();
				return -1;
			}
			return -1;
		}
		int GetPtInfo(HWND hWnd, int x, int y, int sizex, int sizey, RECT& rcx)
		{
			POINT pt;
			//wchar_t txt[80];
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			ScreenToClient(hWnd, &pt);
			//ClientToScreen(hWnd, &pt);
			RECT  rc = { x,y,x + sizex,y + sizey };
			rcx = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}
		int GetPtInfo(HWND hWnd, int x, int y, int sizex, int sizey)
		{
			RECT rc;
			return GetPtInfo(hWnd, x, y, sizex, sizey, rc);
		}
		int GetPtInfo(HWND hWnd, VertexUIPos vp, RECT& rcx)
		{
			return GetPtInfo(hWnd, vp.x, vp.y, vp.cx, vp.cy, rcx);
		}
		int GetScreenPtInfo(int x, int y, int sizex, int sizey, RECT& rcx)
		{
			POINT pt;
			//wchar_t txt[80];
			GetCursorPos(&pt);
			//wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
		   // OutputDebugString(txt);
			RECT  rc = { x,y,x + sizex,y + sizey };
			rcx = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}
		int _ClickArea(HWND hWnd, RECT rc, LPARAM lParam, RUNFUN function())
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			if (PtInRect(&rc, pt))
			{
				function();
				return -1;
			}
			return -1;
		}

		int ClickAreaEx(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), int msg)
		{
			POINT pt;
			wchar_t txt[80];
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			wsprintf(txt, L"x=%d;y=%d\n", LOWORD(lParam), HIWORD(lParam));
			OutputDebugString(txt);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				if (msg == 1)
				{
					function();
					return 0;
				}
				if (msg == 0)
				{
					return -1;
				}
				else
				{
					return -1;
				}
				return -1;
			}
			return -1;
		}

		int GetAreaPtInfo(HWND hWnd, int x, int y, int sizex, int sizey, RECT& rc2, LPARAM lParam)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			rc2 = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				return 1;
			}
			else
			{
				return 0;
			}
			return -1;
		}

		int ClickAreaPtInfo(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, int& value)
		{
			POINT pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			RECT rect;
			GetClientRect(hWnd, &rect);
			RECT  rc = { x,y,x + sizex,y + sizey };
			if (PtInRect(&rc, pt))
			{
				value = 1;
				return -1;
			}
			else
			{
				value = 0;
				return -1;
			}
			return -1;
		}

		int _CreateClickEvent(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), int msg)
		{
			ClickAreaEx(hWnd, x, y, sizex, sizey, lParam, function, msg);
			return 0;
		}

		int CreateClickEvent(HWND hWnd, int x, int y, int sizex, int sizey, LPARAM lParam, RUNFUN function(), const wchar_t* Panelid)
		{
			int msg = 0;
			if (Panelid == PanelID)
			{
				msg = 1;
			}
			if (Panelid == PrevPanelID)
			{
				msg = 1;
			}

			else
			{
				msg = 0;
			}
			ClickAreaEx(hWnd, x, y, sizex, sizey, lParam, function, msg);
			return 0;
		}

		int _NewValue(int& ov, int v)
		{
			ov = v;
			return ov;
		}


		void SnedClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			ClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			ClickMsg = 0;
			KeepDownMsg = 0;
		}
		void SendClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			ClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			ClickMsg = 0;
			KeepDownMsg = 0;
		}
		void SendKeepDownEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			KeepDownMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
		}
		void SendRClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			RClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			RClickMsg = 0;
		}
		void SendRDClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			RDClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			RDClickMsg = 0;
		}
		void SendDClickEvent(HWND hWnd, WPARAM w, LPARAM l)
		{
			LDClickMsg = 1;
			SendMessage(hWnd, WM_MOUSEMOVE, w, l);
			LDClickMsg = 0;
		}


	}

}

RUNFUN OnlyTest()
{
	MessageBox(0, L"MSG", L"Tip~qwq", 0);
	return 0;
}
bool CtlControllerForceStop = false;
int VertexUI::VertexUIControl::AddEvent()
{
	if (CtlControllerForceStop == true)return -1;
	using namespace::VertexUI::Click;
	auto type = this->CtlName;
	auto x = this->x;
	auto y = this->y;
	auto wid = this->sizex;
	auto hei = this->sizey;
	auto Text = this->Text;
	auto pInfo = this->pInfo;
	RECT rc;
	if ((GetPtInfo(hWnd, x, y, wid, hei, rc)) == 1)
	{
		if (ClickMsg == 1)
		{
			ClickMsg = 0;
			//WaitForSingleObject(this->aHandle, INFINITE);
			this->runfun();
			CtlControllerForceStop = false;
			KeepDownMsg = 0;
		}
		if (KeepDownMsg == 1)
		{
			this->PlayAnimation();
		}
		if (hState == 0)
		{
			/*
			HDC hdc = GetDC(hWnd);
			CreateRect(hWnd, hdc, rc.left, rc.bottom - 5, (rc.right - rc.left), 5,pInfo.HoverColor);
			DeleteObject(hdc);
			ReleaseDC(hWnd, hdc);
			*/
			this->PlayAnimation();
			hState = 1;
			return 0;
		}
		return 0;
	}
	return -1;
}

DWORD WINAPI  VertexUI::VertexUIControl::AnimationTest(LPVOID pCtl)
{
	if (CtlControllerForceStop == true)return true;
	VertexUIControl* p = (VertexUIControl*)pCtl;
	//wchar_t i[20];
	//_itow(p->x, i, 10);
	HWND hWnd = p->hWnd;
	HDC hdc = GetDC(hWnd);
	using namespace::VertexUI;
	if (p->CtlName == std::wstring(L"Button2"))
	{
		static bool s = false;
		VertexUIColorInfo _ = p->pInfo;
		_.BkColor = p->pInfo.HoverColor;
		if (VertexUI::Click::KeepDownMsg == 1 && s == false)
		{
			s = true;
			vcdb.RoundButton(hWnd, hdc, { p->x - 1,p->y - 1,p->sizex + 2,p->sizey + 2 }, p->pInfo.Corner, p->Text, _);
		}
		if (VertexUI::Click::KeepDownMsg == 0)
		{
			s = false;
			vcdb.RoundButton(hWnd, hdc, { p->x,p->y,p->sizex,p->sizey}, p->pInfo.Corner, p->Text, _);

		}
	}
	else
	{
		for (int i = 0; i <= 20; i++)
		{
			if (PanelID != PrevPanelID)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				InvalidateRect(hWnd, &rc, 0);
				DeleteObject(hdc);
				ReleaseDC(hWnd, hdc);
				//CtlControllerForceStop = false;
				return true;
			}
			float play = CalcEaseOutCurve(i, 0, 10, 20);
			RECT irc;
			if (VertexUI::Click::GetPtInfo(p->hWnd, p->x, p->y, p->sizex, p->sizey, irc) == 0)
			{
				InvalidateRect(p->hWnd, &irc, 0);
				OutputDebugString(L"qwq!\n");
				DeleteObject(hdc);
				ReleaseDC(hWnd, hdc);
				return true;
			}

			if (VertexUI::Click::KeepDownMsg == 1)VertexUI::Panel::CreateRect(hWnd, hdc, p->x - (((0.1 * play) * p->sizex) / 2) + (p->sizex) / 2, p->y + p->sizey - 5, (0.1 * play) * p->sizex, 5, RGB(GetMaxValue(GetRValue(p->pInfo.HoverColor) + 40, 255), GetMaxValue(GetGValue(p->pInfo.HoverColor) + 40, 255), GetMaxValue(GetBValue(p->pInfo.HoverColor) + 40, 255)));
			else VertexUI::Panel::CreateRect(hWnd, hdc, p->x - (((0.1 * play) * p->sizex) / 2) + (p->sizex) / 2, p->y + p->sizey - 5, (0.1 * play) * p->sizex, 5, p->pInfo.HoverColor);
			if (CtlControllerForceStop == true)break;
			Sleep(10);

		}
	}
Final:
	DeleteObject(hdc);
	ReleaseDC(hWnd, hdc);
	return true;
}
#define add_eventPtr(ctl) if(ctl->AddEvent()==0)return 0
#define add_event(ctl) if(ctl.AddEvent()==0)return 0
void ClickAreaBoxHoverAnimation(HWND hWnd, HDC hdc, RECT rc)
{
	CreateRect(hWnd, hdc, rc.left, rc.bottom - 1, (rc.right - rc.left), 1, VERTEXUICOLOR_LAVENDER);
	Sleep(8);
	CreateRect(hWnd, hdc, rc.left, rc.bottom - 2, (rc.right - rc.left), 2, VERTEXUICOLOR_LAVENDER);
	Sleep(8);
	CreateRect(hWnd, hdc, rc.left, rc.bottom - 4, (rc.right - rc.left), 4, VERTEXUICOLOR_LAVENDER);
	Sleep(8);
	CreateRect(hWnd, hdc, rc.left, rc.bottom - 5, (rc.right - rc.left), 5, VERTEXUICOLOR_LAVENDER);
}
#define AddBoxClickArea(hWnd,lParam,x,y,cx,cy,runfun) if ((GetPtInfo(hWnd, x, y, cx, cy, rc)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
		}\
		if (hState == 0)\
		{\
			HDC hdc = GetDC(hWnd);\
			CreateRect(hWnd, hdc, rc.left, rc.bottom - 5, (rc.right - rc.left), 5, VERTEXUICOLOR_LAVENDER);\
			DeleteObject(hdc);\
			ReleaseDC(hWnd, hdc);\
			DeleteDC(hdc);\
			hState = 1;\
		}\
		return 0;\
		}
#define AddBoxClickAreaDDraw(hWnd,lParam,x,y,cx,cy,runfun,ia) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
		}\
		if (hState == 0)\
		{\
			ia();\
			hState = 1;\
		}\
		return 0;\
		}
#define AddBoxClickArea_Animation(hWnd,lParam,x,y,cx,cy,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
		}\
		if (hState == 0)\
		{\
			HDC hdc = GetDC(hWnd);\
			ClickAreaBoxHoverAnimation(hWnd,hdc,rc);\
			DeleteObject(hdc);\
			ReleaseDC(hWnd, hdc);\
			DeleteDC(hdc);\
			hState = 1;\
		}\
		return 0;\
		}

//窗口,鼠标传参,x,y,大小,大小,x偏移量,大小偏移量,高度偏移量,颜色,运行


#define AddBoxClickAreaEx(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
		}\
		if (hState == 0)\
		{\
			HDC hdc = GetDC(hWnd);\
			CreateRect(hWnd, hdc, rc.left + x1, rc.bottom - hsize, rc.right - rc.left - wsize, hsize,color);\
			DeleteObject(hdc);\
			ReleaseDC(hWnd, hdc);\
			DeleteDC(hdc);\
			hState = 1;\
		}\
		return 0;\
		}

#define AddBoxClickAreaExExit(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
			DestroyWindow(hWnd);\
		}\
		if (hState == 0)\
		{\
			HDC hdc = GetDC(hWnd);\
			CreateRect(hWnd, hdc, rc.left + x1, rc.bottom - hsize, rc.right - rc.left - wsize, hsize,color);\
			DeleteObject(hdc);\
			ReleaseDC(hWnd, hdc);\
			DeleteDC(hdc);\
			hState = 1;\
		}\
		return 0;\
		}

#define _AddBoxClickAreaEx(hWnd,lParam,x,y,cx,cy,x1,wsize,hsize,color,runfun,dwf) if ((GetAreaPtInfo(hWnd, x, y, cx, cy, rc, lParam)) == 1)\
		{\
		if (ClickMsg == 1)\
		{\
			ClickMsg = 0;\
			runfun();\
		}\
		if (hState == 0)\
		{\
			dwf\
			hState = 1;\
		}\
		return 0;\
		}
#define AddCtlEvent(hWnd,lParam,ctl) AddBoxClickArea(hWnd,lParam,ctl.x,ctl.y,ctl.sizex,ctl.sizey,ctl.runfun)