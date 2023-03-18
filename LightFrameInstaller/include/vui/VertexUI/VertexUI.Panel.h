#pragma once
/*
*
* VertexUI PanelDrawingInterface
*
* Vertillusion Studio 2021
*
*/
#pragma comment(lib,"Msimg32.lib")
#include "framework.h"
#include "VertexUI.Colors.h"
#include <Shlwapi.h>
#include <ShlObj_core.h>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <vector>
#include <functional>
#ifdef _GDIPLUS
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib") 
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;
#endif
#pragma warning(disable:4996)
#define FullDesign

#ifdef FullDesign
#include "../LightFrame.AA.h"
#include "VertexUI.ClickArea.h"
#endif
#include <shellapi.h>

#define VERTEXUI_FLAGS_ANIMATION 0

#define VERTEXUI_FLAGS_NOANIMATION 1

#define VERTEXUI_FLAGS_MEMDRAW 2

#define VERTEXUI_FLAGS_CACHE 1

#define VERTEXUI_DEVMODE

#define RCWID (rc.right -rc.left)

#define InitCtl(ctlname,x,y,cx,cy,fun,txt) {ctlname,x,y,cx,cy,fun,txt}
namespace VertexUI
{
	typedef void (DRAWPANEL)(HWND, HDC);
	typedef void (SDRAWPANEL)(HDC);
	typedef class VERTEXUICTL {
	public:
		const wchar_t* CtlName;
		int x;
		int y;
		int sizex;
		int sizey;
		int (*runfun)() = 0;
		const wchar_t* Text;
	}VUICTL;
	enum Layout
	{
		Free,
		Left,
		Pushing,
		Flex
	};
	class VertexUIColorGradientInfo
	{
	public:
		COLORREF From = 0;
		COLORREF To = 0;
		int Degree = 0;
	};
	class VertexUIColorInfo
	{
	public:
		COLORREF BkColor = VERTEXUICOLOR_SEA;
		COLORREF TxtColor = VERTEXUICOLOR_WHITE;
		COLORREF HoverColor = VERTEXUICOLOR_DEEPSEA;
		int BtnHoverMode = 0; //0:底部颜色 | 1:整体颜色 | 2:整体渐变
		VertexUIColorGradientInfo Gradient = { 0,0,0 };
		int TxtSize = 18;
		int Corner = 8;
	};
	class VertexUIColor {
	public:
		short r;
		short g;
		short b;
	};
	class VertexUIPos
	{
	public:
		int x = 0;
		int y = 0;
		int cx = 100;
		int cy = 100;
	};

	template < typename T >
	void ClearVector(std::vector< T >& vt)
	{
		std::vector< T > veTemp;
		veTemp.swap(vt);
	}

	typedef class VertexUIControl {
	public:
		virtual int CreateCtl(const wchar_t* type, HWND h, HDC hdc, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text, VertexUIColorInfo vci);
		virtual int CreateCtl(const wchar_t* type, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text, VertexUIColorInfo vci);
		virtual int AddEvent();
		virtual int SetInfo(VertexUIPos vp, const wchar_t* txt = nullptr, bool changepos = true)
		{
			this->IsInfoChanged = true;
			if (txt != nullptr)this->Text = txt;
			if (changepos == true)
			{
				this->x = vp.x;
				this->y = vp.y;
				this->sizex = vp.cx;
				this->sizey = vp.cy;
			}
			return 0;
		}
		virtual int SetInfo(int x, int y, int cx, int cy)
		{
			this->IsInfoChanged = true;
		
				this->x = x;
				this->y = y;
				this->sizex = cx;
				this->sizey = cy;
			
			return 0;
		}
		virtual void SetInheritance(VertexUIControl vuic)
		{
			this->CtlName = vuic.CtlName;
			if (this->Text == nullptr)Text = L"NULL";
			this->hWnd = vuic.hWnd;
			this->hdc = vuic.hdc;
		}
		virtual void CreateInheritedCtl(HWND hWnd, HDC hdc, VertexUIControl vuic)
		{
			CreateCtl(vuic.CtlName, hWnd, hdc, vuic.x, vuic.y, vuic.sizex, vuic.sizey, vuic.runfun, vuic.Text, vuic.pInfo);
		}
		virtual void CreateInheritedCtl(VertexUIControl vuic)
		{
			CreateCtl(vuic.CtlName, vuic.hWnd, vuic.hdc, vuic.x, vuic.y, vuic.sizex, vuic.sizey, vuic.runfun, vuic.Text, vuic.pInfo);
		}
	private:
		HWND hWnd;
		HDC hdc;
		const wchar_t* CtlName;
		int x;
		int y;
		int sizex;
		int sizey;
		std::function<void()> runfun;
		const wchar_t* Text;
		VertexUIColorInfo pInfo;
	protected:
		bool IsInfoChanged = false;
		HANDLE aHandle = 0;
		static DWORD WINAPI  AnimationTest(LPVOID pCtl);
		virtual void PlayAnimation()
		{
			this->aHandle = CreateThread(NULL, 0, AnimationTest, this, 0, NULL);

			//int id = Elements::Utils::GetRandomNumber(1000);
		}
	}VuiCtl, VuiControl;
	class DrawPanel
	{
	public:
		HWND hWnd;
		HDC hdc;
	};
	class VertexUILayoutInfo
	{
	public:
		VertexUILayoutInfo()
		{
		}
		VertexUILayoutInfo(int Mode, int x, int y, int cx, int cy,int interval=0)
		{
			this->Mode = Mode;
			this->x = x;
			this->y = y;
			this->uniCx = cx;
			this->uniCy = cy;
			this->interval = interval;
		}
		int Mode = 0;
		int x = 0;
		int y = 0;
		int uniCx = 0;
		int uniCy = 0;
		int interval = 0;
	};

	class VertexUIGroup
	{
	public:
		DrawPanel Handle()
		{
			DrawPanel p;
			p.hWnd = hWnd;
			p.hdc = hdc;
			return p;
		}
		void Handle(HWND h, HDC hdc)
		{
			this->hWnd = h; this->hdc = hdc;
		}
		void Add(VertexUIControl ctl)
		{
			//ctl.CreateInheritedCtl(this->Handle().hWnd, this->Handle().hdc, ctl);
			this->ctl.push_back(ctl);
		}
		std::vector<VertexUIControl>ctl;
	protected:
		HWND hWnd; HDC hdc;
	};
	class VertexUIPanel
	{
	public:
		int Set(HWND hWnd, HDC hdc)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->hdc = hdc;
			return 0;
		}
		int Set(HWND hWnd, HDC hdc,VertexUILayoutInfo lo)
		{
			ClearVector(Ctl);
			ClearVector(Group);
			ClearVector(CtlPtr);
			this->hWnd = hWnd;
			this->hdc = hdc;
			this->Layout = lo;
			return 0;
		}
		void Add(VertexUIGroup gp)
		{
			gp.Handle(this->hWnd, this->hdc);
			this->Group.push_back(gp);

			for (auto i : Group)
			{
				for (auto j : i.ctl)
				{
					j.CreateInheritedCtl(this->hWnd, this->hdc, j);
				}
			}

		}
		void Add(VertexUIControl ctl)
		{
			ctl.CreateInheritedCtl(hWnd, hdc, ctl);
			this->Ctl.push_back(ctl);
		}
		template <class T>
		void Add(T ctl)
		{
			if (Layout.Mode == VertexUI::Left)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right-Layout.x) /OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);
				OutputDebugString(L"CURRENT COUNT:\n");
				MonitorValue(0, count, 0, true);
				ctl->SetInfo(Layout.x + OneSize*(CtlPtr.size())-(count*CrLine)*OneSize, y+ CrLine*(Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
			if (Layout.Mode == VertexUI::Pushing)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				Layout.uniCx = rc.right * 0.25;
				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right - Layout.x) / OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);
				OutputDebugString(L"CURRENT COUNT:\n");
				MonitorValue(0, count, 0, true);
				ctl->SetInfo(Layout.x + OneSize * (CtlPtr.size()) - (count * CrLine) * OneSize, y + CrLine * (Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
			if (Layout.Mode == VertexUI::Flex)
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				
				int y = Layout.y;
				int OneSize = (Layout.uniCx + Layout.interval);
				int count = (rc.right - Layout.x) / OneSize;

				int CrLine = ceil((CtlPtr.size()) / count);

				ctl->SetInfo(Layout.x+((rc.right/2)-((count*OneSize)+ Layout.interval/2 )/2) + OneSize * (CtlPtr.size()) - (count * CrLine) * OneSize, y + CrLine * (Layout.uniCy + Layout.interval), Layout.uniCx, Layout.uniCy);
			}
			ctl->CreateInheritedCtl(hWnd, hdc, ctl);
			this->CtlPtr.push_back(ctl);
		}
		int AddEvent()
		{
			int ret = -1;
			if (Group.size() > 1)
			{
				for (auto i : Group)
				{
					for (auto j : i.ctl)
					{
						j.CreateInheritedCtl(hWnd, hdc, j);
						int preret = j.AddEvent();
						if (preret != -1)
						{
							ret = preret;
							break;
						}
					}
				}
			}
			if (Ctl.size() >= 1)
			{
				for (int i = 0; i < Ctl.size(); i++)
				{
					int preret = Ctl[i].AddEvent();
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}
			if (CtlPtr.size() >= 1)
			{
				for (int i = 0; i < CtlPtr.size(); i++)
				{
					int preret = CtlPtr[i]->AddEvent();
					if (preret != -1)
					{
						ret = preret;
						break;
					}
				}
			}
			return ret;
		}
	protected:
		HWND hWnd;
		HDC hdc;
		std::vector<VertexUIGroup>Group;
		std::vector<VertexUIControl>Ctl;
		std::vector<VertexUIControl*>CtlPtr;
		VertexUILayoutInfo Layout;
	};

	VertexUIColorInfo NCIX{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,0,0,18,8 };
	VertexUIColorInfo NCIW{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,0 };
	VertexUIColorInfo CNCIX{ VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITE,VERTEXUICOLOR_BLOOMLAVENDER,0 };
	class PanelHandleFun
	{
	public:
		HDC hdc;
		HDC hmdc;
		RECT rc;
		HBITMAP hpbp;
		HBITMAP hmbp;
		int x;
		int y;
		int cx;
		int cy;
		DRAWPANEL* exDraw;
		COLORREF c1;
		COLORREF c2;
		HWND h;
	};
	typedef struct PanelDCCache
	{
		HDC inhdc;//兼容VertexUI内建
		HDC hdc;
		HDC hMemDC;
	};
	int ScrollDepth;
	int ScrollUpdate = 0;
	const wchar_t* ScrollSingleID = L"vui!NULL";
	int ScrollSinglePage;
	int ScrollAreaOffset;
	int ScrollArea = 2000;
	int dystate = 1;
	bool AlwaysRedraw = true;
	const wchar_t* ListPanel = L"ListPanel";
	const wchar_t* ViewPanel = L"ViewPanel";
	const wchar_t* PanelID = L"Init";
	const wchar_t* PrevPanelID = L"Init";
	const wchar_t* ButtonText = L"Button";
	int g_hoverstate = 0;
	VertexUIPos SharedPos;
	namespace Panel
	{
		//CreatePanel only passed in parameters below:(HWND,HDC).

		//It convert (x,y,sizex,sizey) into RECT {x,y,x+sizex,y+sizey}.
		void RectTypeConvert(RECT& rc, int x, int y, int sizex, int sizey)
		{
			rc = { x,y,x + sizex,y + sizey };
		}
		RECT RectTypeConvert(int x, int y, int sizex, int sizey)
		{
			RECT rc = { x,y,x + sizex,y + sizey };
			return rc;
		}
		void Refresh(HWND hWnd)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			InvalidateRect(hWnd, &rc, 0);
		}
		//DrawRect
		void DrawRect( HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			RECT mRc;
			RectTypeConvert(mRc, x, y, sizex, sizey);
			HBRUSH hb = CreateSolidBrush(cl);
			HBRUSH bhb = (HBRUSH)SelectObject(hdc, hb);
			SelectObject(hdc, hb);
			FillRect(hdc, &mRc, hb);
			SelectObject(hdc, bhb);
			DeleteObject(hb);
		}
		void DrawRect(HWND h,HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			DrawRect(hdc, x, y, sizex,sizey,cl);
		}
#define CreateRect(hWnd,hdc, x, y,sizex,sizey,cl) DrawRect(hWnd,hdc, x, y,sizex,sizey,cl)
		void DrawHollowRect(HDC hdc, int x, int y, int cx, int cy, int p, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, p, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hdc, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)::GetCurrentObject(hdc, OBJ_BRUSH);

			SelectObject(hdc, hPen);
			SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
			Rectangle(hdc, x, y, x + cx, y + cy);


			SelectObject(hdc, hOldBrush);
			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
#define CreateHollowRect(hdc, x, y, cx, cy,  p, cl) DrawHollowRect(hdc, x, y, cx, cy,  p, cl)
		void DrawLine(HDC hdc,/*POINT* pt*/int x, int y, int x1, int y1, int p, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, p, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hdc, OBJ_PEN);

			SelectObject(hdc, hPen);

			MoveToEx(hdc, x, y, 0);
			LineTo(hdc, x1, y1);


			SelectObject(hdc, hOldPen);
			DeleteObject(hPen);
		}
		void CreateRectArea(HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl)
		{
			RECT mRc;
			RectTypeConvert(mRc, x, y, sizex, sizey);
			HBRUSH hb = CreateSolidBrush(cl);
			HBRUSH bhb = (HBRUSH)SelectObject(hdc, hb);
			SelectObject(hdc, hb);
			FillRect(hdc, &mRc, hb);
			SelectObject(hdc, bhb);
			DeleteObject(hb);
		}

		//Fill the whole window
		void CreateFillArea(HWND h, HDC hdc, COLORREF cl)
		{
			RECT Rc;
			GetClientRect(h, &Rc);
			DrawRect(hdc, Rc.left, Rc.top, Rc.right, Rc.bottom, cl);
		}

		//Text
		void TextPreDraw(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -18;
				wcscpy_s(lf.lfFaceName, L"Segoe UI");
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}

		//Flags: 0.ToLeft 1.Center 
		void TextPreDrawEx(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, int sz, int flag, COLORREF cl)
		{
			if (txt == nullptr)return;
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -sz;
				//lf.lfWeight = 10;
				wcscpy_s(lf.lfFaceName, L"Segoe UI");
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			if (flag == 0)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_VCENTER);
			}
			if (flag == 1)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			if (flag == 2)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE);
			}
			if (flag == 3)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_CENTER | DT_VCENTER);
			}
			if (flag == 4)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, 0);
			}
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}
		void _TextPreDrawEx(HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* txt, int sz, const wchar_t* Font, int flag, COLORREF cl)
		{
			if (txt == nullptr)return;
			RECT rc;
			RectTypeConvert(rc, x, y, sizex, sizey);
			LOGFONT lf;
			HFONT hFont = 0;
			SetTextColor(hdc, cl);
			SetBkMode(hdc, TRANSPARENT);
			if (hFont == 0)
			{
				memset(&lf, 0, sizeof(LOGFONT));
				lf.lfHeight = -sz;
				if (flag == 5)
				{
					lf.lfStrikeOut = TRUE;
					flag = 0;
				}
				//lf.lfQuality = ANTIALIASED_QUALITY;
				wcscpy_s(lf.lfFaceName, Font);
				hFont = CreateFontIndirect(&lf);  // create the font
			}
			HFONT old = (HFONT)SelectObject(hdc, hFont);
			if (flag == 0)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_VCENTER);
			}
			if (flag == 1)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
			if (flag == 2)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_SINGLELINE);
			}
			if (flag == 3)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_CENTER | DT_VCENTER);
			}
			if (flag == 4)
			{
				DrawText(hdc, txt, wcslen(txt), &rc, DT_VCENTER);
			}
			DeleteObject(hFont);
			SelectObject(hdc, old);
		}
		int TextDrawOuter(HDC hDrawingDC, int x, int y, int cx, int cy, int sz, int mode, const wchar_t* txt, const wchar_t* font = L"Segoe UI", int outsz = 1, COLORREF inC = 0, COLORREF OutC = VERTEXUICOLOR_WHITE)
		{
			::BeginPath(hDrawingDC);
			_TextPreDrawEx(hDrawingDC, x, y, cx, cy, txt, sz, font, mode, 0);
			::EndPath(hDrawingDC);
			HPEN hp = CreatePen(PS_SOLID, outsz, OutC);
			HPEN hOld = (HPEN)SelectObject(hDrawingDC, hp);
			HBRUSH hb = CreateSolidBrush(inC);
			HBRUSH HbR = (HBRUSH)SelectObject(hDrawingDC, hb);
			::StrokeAndFillPath(hDrawingDC);
			SelectObject(hDrawingDC, hOld);
			SelectObject(hDrawingDC, HbR);
			DeleteObject(hb);
			DeleteObject(hp);
			return 0;
		}
		//Only straight Line
		void PanelDrawSTLine(HDC pDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine)
		{
			if (Y0 > Y1) { int Temp = Y0; Y0 = Y1; Y1 = Temp; Temp = X0; X0 = X1; X1 = Temp; }SetPixel(pDC, X0, Y0, clrLine);
			int XDir, DeltaX = X1 - X0; if (DeltaX >= 0) { XDir = 1; }
			else { XDir = -1; DeltaX = 0 - DeltaX; }int DeltaY = Y1 - Y0;
			if (DeltaY == 0) { while (DeltaX-- != 0) { X0 += XDir; SetPixel(pDC, X0, Y0, clrLine); }return; }
			if (DeltaX == 0) { do { Y0++; SetPixel(pDC, X0, Y0, clrLine); } while (--DeltaY != 0); return; }
			if (DeltaX == DeltaY) { do { X0 += XDir; Y0++; SetPixel(pDC, X0, Y0, clrLine); } while (--DeltaY != 0); return; }SetPixel(pDC, X1, Y1, clrLine);
		}
#define SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
		HBITMAP ExtractFileIcon(LPCTSTR pszPath, UINT nWidth, UINT nHeight)
		{
			HBITMAP hBitmap = NULL;
			if ((NULL != pszPath) && (nWidth > 0.0) && (nHeight > 0.0))
			{
				IShellItemImageFactory* psif = NULL;
				SIZE size = { nWidth, nHeight };
				HRESULT hr = ::SHCreateItemFromParsingName(pszPath, NULL, IID_PPV_ARGS(&psif));
				if (SUCCEEDED(hr) && (NULL != psif))
				{
					psif->GetImage(size, SIIGBF_ICONONLY, &hBitmap);
					//OutputDebugString(L"114514");//测试完成
				}
				else
				{
					hBitmap = 0;
				}
				//psif->Release();
				SAFE_RELEASE(psif);
			}

			return hBitmap;
		}
		void DisplayIco(HDC hdc, RECT rc, HBITMAP hb)
		{
			HDC hMemDC = CreateCompatibleDC(hdc);               // 创建一个兼容内存DC
			HGDIOBJ hOldObj = SelectObject(hMemDC, hb);   // 将g_bitmap选入到这个内存DC

			//BitBlt(hdc, x, y, ICON_WIDTH, ICON_HEIGHT, hMemDC, 0, 0, SRCCOPY);

			BLENDFUNCTION ftn = { 0 };
			ftn.BlendOp = AC_SRC_OVER;                           // 目前只能设置这个值
			ftn.AlphaFormat = AC_SRC_ALPHA;                      // 也只能设置这个值
			ftn.BlendFlags = 0;                                  // 必须为0
			ftn.SourceConstantAlpha = 255;                       // 指定源图片的alpha

			// 调用这个函数来进行Alpha混合
			AlphaBlend(hdc, rc.left + 2, rc.top + 2, 32, 32, hMemDC, 0, 0, 32, 32, ftn);

			SelectObject(hMemDC, hOldObj);
			DeleteObject(hMemDC);
			DeleteObject(hOldObj);
			DeleteObject(hb);
		}
		void DisplayIcoEx(HDC hdc, RECT rc, HBITMAP hb)
		{
			HDC hMemDC = CreateCompatibleDC(hdc);               // 创建一个兼容内存DC
			HGDIOBJ hOldObj = SelectObject(hMemDC, hb);   // 将g_bitmap选入到这个内存DC

			//BitBlt(hdc, x, y, ICON_WIDTH, ICON_HEIGHT, hMemDC, 0, 0, SRCCOPY);

			BLENDFUNCTION ftn = { 0 };
			ftn.BlendOp = AC_SRC_OVER;                           // 目前只能设置这个值
			ftn.AlphaFormat = AC_SRC_ALPHA;                      // 也只能设置这个值
			ftn.BlendFlags = 0;                                  // 必须为0
			ftn.SourceConstantAlpha = 255;                       // 指定源图片的alpha

			// 调用这个函数来进行Alpha混合
			AlphaBlend(hdc, rc.left + 2, rc.top + 2, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, RCWID, rc.bottom - rc.top, ftn);

			SelectObject(hMemDC, hOldObj);
			DeleteObject(hMemDC);
			DeleteObject(hOldObj);
			DeleteObject(hb);
		}
		//Frame
		HICON GetFileIcon(const wchar_t* strFilePath, BOOL bLarge)
		{
			SHFILEINFO SHFI;  ZeroMemory(&SHFI, sizeof(SHFI));  DWORD_PTR ret = ::SHGetFileInfo(strFilePath, 0, &SHFI, sizeof(SHFI),
				SHGFI_ICON | (bLarge ? SHGFI_LARGEICON : SHGFI_SMALLICON));  if (ret != 0) { return SHFI.hIcon; }  return NULL;
		}
		void DisplayIcon(HDC hdc, const wchar_t* path, int x, int y, int sz)
		{
			//正式代码
			if (path == std::wstring(L"NULL"))return;
			if (path == std::wstring(L""))return;
			if (path == std::wstring(L" "))return;
			HICON hIcon = GetFileIcon(path, 1);
			if (hIcon != NULL)
				DrawIconEx(hdc, x, y, hIcon, sz, sz, 0, NULL, DI_NORMAL);
			DestroyIcon(hIcon);
		}
		void CreatehIcon(HDC hdc, HICON hIcon, int x, int y, int sz)
		{
			if (hIcon != NULL)
				DrawIconEx(hdc, x, y, hIcon, sz, sz, 0, NULL, DI_NORMAL);
		}
		void PanelDrawOutFrame(HWND h, HDC hdc, COLORREF cl)
		{
			RECT rc;
			GetClientRect(h, &rc);
			CreateHollowRect(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 1, cl);
			/*
		PanelDrawSTLine(hdc, 0, 0, rc.right, 0, cl);
		PanelDrawSTLine(hdc, 0, 0, 0, rc.bottom, cl);
		PanelDrawSTLine(hdc, 0, rc.bottom - 1, rc.right - rc.left, rc.bottom - 1, cl);
		PanelDrawSTLine(hdc, rc.right - 1, 0, rc.right - 1, rc.bottom - 1, cl);
		*/
		}
		void PanelDrawCloseBtn(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1, NULL);
			LineTo(hdc, rc.right - inframepos, rc.bottom - inframepos);

			MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		void PanelDrawDisplayBtn(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, int mdpos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.left + (x1 / 2) - inframepos, rc.top + (y1 / 2) - inframepos + mdpos);
			MoveToEx(hdc,rc.right- (x1/2.5)-1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.right - (x1 / 2) - inframepos - 1, rc.bottom - (y1 / 2) - inframepos + mdpos);
			//MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			//LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		void PanelDrawDisplayBtn2(HWND h, HDC hdc, int x, int y, int x1, int y1, int inframepos, int mdpos, COLORREF cl)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, x1, y1);
			HPEN hpen;
			hpen = CreatePen(PS_SOLID, 1, cl);
			HPEN prhp = (HPEN)SelectObject(hdc, hpen);
			SelectObject(hdc, hpen);
			MoveToEx(hdc, rc.left + inframepos + 1, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.left + (x1 / 2) - inframepos, rc.top + (y1 / 2) - inframepos + mdpos);
			MoveToEx(hdc, rc.right - (x1 / 2) - 0, rc.top + inframepos + 1 + mdpos, NULL);
			LineTo(hdc, rc.right - (x1 / 2) - inframepos - 1, rc.bottom - (y1 / 2) - inframepos + mdpos);
			//MoveToEx(hdc, rc.left + inframepos + 1, rc.bottom - inframepos - 1, NULL);
			//LineTo(hdc, rc.right - inframepos, rc.top + inframepos);
			DeleteObject(hpen);
			SelectObject(hdc, prhp);
		}
		//Button
		void CreateSimpleButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, const wchar_t* s)
		{
			CreateRect(h, hdc, x, y, sizex, sizey, VERTEXUI_GREENSEA);
			TextPreDraw(hdc, x, y, sizex, sizey, s, VERTEXUI_WHITE);
		}

		//Custom Color Button
		void CreateSimpleButtonEx(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, const wchar_t* s, int ctsz = 0, COLORREF tClr = VERTEXUICOLOR_WHITE)
		{
			int tsz = 0;
			CreateRect(h, hdc, x, y, sizex, sizey, cl);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, tClr);
		}
		void CreateAlphaMask(HDC hdc, int x, int y, int sizex, int sizey, int a, COLORREF cl);
		void DrawLinearGradient(HDC pDC, int x, int y, int cx, int cy, COLORREF clr1, COLORREF clr2, int angle);
		void CreateTransparentButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, const wchar_t* s, int a, int ctsz = 0)
		{
			int tsz = 0;
			CreateAlphaMask(hdc, x, y, sizex, sizey, a, cl);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, VERTEXUI_WHITE);
		}
		void CreateGradientButton(HWND h, HDC hdc, int x, int y, int sizex, int sizey, COLORREF cl, COLORREF clTo, int deg, const wchar_t* s, int ctsz = 0, COLORREF tClr = VERTEXUICOLOR_WHITE)
		{
			int tsz = 0;
			DrawLinearGradient(hdc, x, y, sizex, sizey, cl, clTo, deg);
			if (sizey > 23)
			{
				tsz = 18;
			}
			else
			{
				tsz = 16;
			}
			if (ctsz != 0)tsz = ctsz;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsz, 1, tClr);
		}
		void DrawRoundRect(HDC hDrawingDC, int x, int y, int cx, int cy, int cornersize, COLORREF cl)
		{
			HPEN hGreenPen = ::CreatePen(PS_SOLID, 2, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)CreateSolidBrush(cl);

			::SelectObject(hDrawingDC, hGreenPen);
			::SelectObject(hDrawingDC, hOldBrush);
			::RoundRect(hDrawingDC, x, y, x + cx, y + cy, cornersize, cornersize);


			::SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			::SelectObject(hDrawingDC, hOldPen);
			::DeleteObject(hGreenPen);
		}
		void DrawHollowRoundRect(HDC hDrawingDC, int x, int y, int cx, int cy, int cornersize, int pensize, COLORREF cl)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, pensize, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)::GetCurrentObject(hDrawingDC, OBJ_BRUSH);

			SelectObject(hDrawingDC, hPen);
			SelectObject(hDrawingDC, (HBRUSH)GetStockObject(NULL_BRUSH));
			RoundRect(hDrawingDC, x, y, x + cx, y + cy, cornersize, cornersize);


			SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			SelectObject(hDrawingDC, hOldPen);
			DeleteObject(hPen);
		}
		void CreateRoundButton(HDC hdc, int x, int y, int cx, int cy, int cornersize, const wchar_t* txt, COLORREF cl)
		{
			DrawRoundRect(hdc, x, y, cx, cy, cornersize, cl);
			TextPreDraw(hdc, x, y, cx, cy, txt, VERTEXUICOLOR_WHITE);
		}
		void CreateRoundButtonEx(HDC hdc, int x, int y, int cx, int cy, int cornersize, const wchar_t* txt, int txtsz, COLORREF cl, COLORREF tcl = VERTEXUICOLOR_WHITE)
		{
			DrawRoundRect(hdc, x, y, cx, cy, cornersize, cl);
			TextPreDrawEx(hdc, x, y, cx, cy, txt, txtsz, 1, tcl);
		}
		void DrawPloygon(HDC hDrawingDC, const POINT* pt, int ang, COLORREF cl)
		{
			HPEN hGreenPen = ::CreatePen(PS_SOLID, 2, cl);

			HPEN hOldPen = (HPEN)::GetCurrentObject(hDrawingDC, OBJ_PEN);
			HBRUSH hOldBrush = (HBRUSH)CreateSolidBrush(cl);
			SelectObject(hDrawingDC, hGreenPen);
			SelectObject(hDrawingDC, hOldBrush);
			Polygon(hDrawingDC, pt, ang);
			SelectObject(hDrawingDC, hOldBrush);
			DeleteObject(hOldBrush);
			SelectObject(hDrawingDC, hOldPen);
			DeleteObject(hGreenPen);
		}
		void CreateSTab(HDC hdc, int x, int y, bool isopen, int scale = 1)
		{
			if (isopen == false)
			{
				DrawRoundRect(hdc, x * scale, y * scale, 50 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_MIDNIGHTPLUS);
				DrawRoundRect(hdc, x * scale, y * scale, 20 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_GREENSEA);
			}
			if (isopen == true)
			{
				DrawRoundRect(hdc, x * scale, y * scale, 50 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_MIDNIGHTPLUS);
				DrawRoundRect(hdc, (x + 30) * scale, y * scale, 20 * scale, 20 * scale, 15 * scale, VERTEXUICOLOR_GREENSEA);
			}
		}
		void CreateSimpleHollowButton(HDC hdc, int x, int y, int sizex, int sizey, int p, const wchar_t* s, COLORREF cl, COLORREF tcl = VERTEXUICOLOR_WHITE, int tsize = 18)
		{
			if (sizex < 0)sizex = 0;
			CreateHollowRect(hdc, x, y, sizex, sizey, p, cl);
			if (tsize < 0)tsize = 0;
			TextPreDrawEx(hdc, x, y, sizex, sizey, s, tsize, 1, tcl);
		}
		void CreateCtl(HWND hWnd, HDC hdc, VUICTL vctl)
		{
			if (vctl.CtlName == L"Button")
			{
				CreateSimpleButton(hWnd, hdc, vctl.x, vctl.y, vctl.sizex, vctl.sizey, vctl.Text);
			}
		}
		HBITMAP hwndToHbitmap(HWND hwnd, HDC hDC)
		{

			//HDC hDC = GetDC(hwnd);


			RECT rect;

			GetClientRect(hwnd, &rect);


			HDC hDCMem = CreateCompatibleDC(hDC);


			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);


			HBITMAP hOldMap = (HBITMAP)SelectObject(hDCMem, hBitMap);


			BitBlt(hDCMem, 0, 0, rect.right, rect.bottom, hDC, 0, 0, SRCCOPY);
			hBitMap = (HBITMAP)SelectObject(hDCMem, hOldMap);
			DeleteDC(hDCMem);
			return hBitMap;
		}
		HBITMAP DCToHbitmap(HDC hDC, int cx, int cy)
		{

			//HDC hDC = GetDC(hwnd);




			HDC hDCMem = CreateCompatibleDC(hDC);


			HBITMAP hBitMap = CreateCompatibleBitmap(hDC, cx, cy);


			HBITMAP hOldMap = (HBITMAP)SelectObject(hDCMem, hBitMap);


			BitBlt(hDCMem, 0, 0, cx, cy, hDC, 0, 0, SRCCOPY);
			hBitMap = (HBITMAP)SelectObject(hDCMem, hOldMap);
			DeleteDC(hDCMem);
			return hBitMap;
		}
		//Create a Drawing Panel.
		template<class draw>
		void _CreatePanel(HWND h, HDC hdc, draw DrawFun)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, hPreBmp);

			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);


		}
		void CreatePanelEx(HWND h, HDC hdc, DRAWPANEL DrawFun, int x = 0, int y = 0, int xOf = 0, int yOf = 0)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			if (xOf == 0)xOf = rc.right - rc.left;
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, yOf);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, x, y, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);


			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);
		}
		void CreateAreaPanel(HWND h, HDC hdc, DRAWPANEL DrawFun, int x, int y, int cx, int cy, int xOf = 0, int yOf = 0, HDC CachedDC = 0)
		{
			if (0 == CachedDC)
			{
				HDC         hMemDC;
				HBITMAP     hBmpMem;
				HBITMAP     hPreBmp;
				RECT rc;
				GetClientRect(h, &rc);
				hMemDC = CreateCompatibleDC(hdc);

				hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);

				hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
				//On hMemDC.
				DrawFun(h, hMemDC);

				BitBlt(hdc, x, y, cx, cy, hMemDC, xOf, yOf, SRCCOPY);


				SelectObject(hMemDC, hPreBmp);


				DeleteObject(hBmpMem);

				DeleteDC(hMemDC);
			}
			else 	BitBlt(hdc, x, y, cx, cy, CachedDC, xOf, yOf, SRCCOPY);
		}
		//
		void CreatePanel(HWND h, HDC hdc, DRAWPANEL DrawFun)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			RECT rc;
			GetClientRect(h, &rc);
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			DrawFun(h, hMemDC);

			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);


			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);
		}

		//


		void SetPanelID(const wchar_t* id)
		{
			PrevPanelID = PanelID;
			PanelID = id;
		}
		void SwitchPanel(const wchar_t* ID)
		{
			RECT rc;
			GetClientRect(GhWnd, &rc);
			SetPanelID(ID);
			InvalidateRect(GhWnd, &rc, 1);
		}
		void RefreshDisplay(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			InvalidateRect(h, &rc, 0);
		}
		void RefreshWindow(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 1;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshWindowAndDC(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 3;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshCache(HWND h)
		{
			RECT rc;
			GetClientRect(h, &rc);
			dystate = 2;
			InvalidateRect(h, &rc, 0);
		}
		void RefreshAreaFromCache(HWND h, int x, int y, int cx, int cy)
		{
			RECT rc;
			RectTypeConvert(rc, x, y, cx, cy);
			InvalidateRect(h, &rc, 0);
			SendMessage(h, WM_PAINT, 0, 0);
		}
	}
	//--
	class VertexUIControllerObject
	{
	public:
		void SetPanelID(const wchar_t* id)
		{
			this->PrevPanelID = this->PanelID;
			this->PanelID = id;
		}
		void SwitchPanel(const wchar_t* ID)
		{
			RECT rc;
			GetClientRect(this->hWnd, &rc);
			SetPanelID(ID);
			InvalidateRect(this->hWnd, &rc, 1);
		}
		void SetOrigin(HWND hWnd)
		{
			this->hWnd = hWnd;
		}
		int IsPanel(const wchar_t* p)
		{
			if (this->PanelID == p)return 1;
			else return 0;
		}
		const wchar_t* gTxt = L"qwq";
	private:
		HWND hWnd;
		const wchar_t* PanelID;
		const wchar_t* PrevPanelID;
	};

}
#ifdef FullDesign
namespace VertexUI::Panel
{
	void CreateAlphaMaskFromDC(HDC hdc, int x, int y, int cx, int cy, int alpha, HDC cl);
	void AreaBlur(HDC hdc, VertexUIPos vp, int a, int b, int th);
	class LimpidDesignCtlEnhancement {
	public:
		int Alpha;
		int blurRad;
		int Factor;
		int ExFlag = 0;
	};
	class VertexUIControlDrawingBoard
	{
	public:


		void RoundButton(HWND hWnd, HDC hdc, VertexUIPos vp, int csz, const wchar_t* txt, VertexUIColorInfo vci = { VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,1,16 })
		{
			this->vp = vp;
			this->vci = vci;
			this->txt = txt;
			this->csz = csz;
			this->nh = hdc;
			CreateAA(hWnd, hdc, vp.x + 1, vp.y + 1, vp.cx, vp.cy, this->ButtonDrawingBoard);
		}
		void LimpidCtl(HWND hWnd, HDC hdc, VertexUIPos vp, int csz, LimpidDesignCtlEnhancement ldce, const wchar_t* txt, VertexUIColorInfo vci = { VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,1,16 })
		{
			this->vp = vp;
			this->vci = vci;
			this->txt = txt;
			this->csz = csz;
			this->nh = hdc;
			this->ldce = ldce;
			CreateAA(hWnd, hdc, vp.x + 1, vp.y + 1, vp.cx, vp.cy, this->LimpidDrawingBoard);
		}
		void HollowRoundButton(HWND hWnd, HDC hdc, VertexUIPos vp, int csz, int psz, const wchar_t* txt, VertexUIColorInfo vci = { VERTEXUICOLOR_LAVENDER,VERTEXUICOLOR_WHITEDRAW,VERTEXUICOLOR_BLOOMLAVENDER,1,16 })
		{
			this->vp = vp;
			this->vci = vci;
			this->txt = txt;
			this->csz = csz;
			this->nh = hdc;
			this->psz = psz;
			CreateAA(hWnd, hdc, vp.x + 1, vp.y + 1, vp.cx, vp.cy, this->HollowButtonDrawingBoard);
		}
		void BoxShadowFrame(HWND hWnd, HDC hdc, VertexUIPos vp, int r)
		{
			//CreateBoxShadow
		}
	private:
		static int psz;
		static int csz;
		static VertexUIPos vp;
		static VertexUIColorInfo vci;
		static LimpidDesignCtlEnhancement ldce;
		static const wchar_t* txt;
		static HDC nh;
		static void ButtonDrawingBoard(HWND hWnd, HDC hdc, int s = 4)
		{
			static HBITMAP prebmp;
			prebmp = hwndToHbitmap(hWnd, nh);
			HDC hMemDC1 = CreateCompatibleDC(hdc);
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
			StretchBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);
			SelectObject(hMemDC1, prebmp);
			DeleteObject(prebmp);
			DeleteDC(hMemDC1);
			CreateRoundButtonEx(hdc, 8, 8, (vp.cx - 7) * s, (vp.cy - 7) * s, csz * s, txt, vci.TxtSize * s, vci.BkColor, vci.TxtColor);
		}
		static void IconDrawingBoard(HWND hWnd, HDC hdc, int s = 4)
		{
			static HBITMAP prebmp;
			prebmp = hwndToHbitmap(hWnd, nh);
			HDC hMemDC1 = CreateCompatibleDC(hdc);
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
			StretchBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);
			SelectObject(hMemDC1, prebmp);
			DeleteObject(prebmp);
			DeleteDC(hMemDC1);
			HBITMAP hb = ExtractFileIcon(txt, (vp.cx - 6) * s, (vp.cy - 6) * s); DisplayIcoEx(hdc, { 0,0,(vp.cx - 6) * s,(vp.cy - 6) * s }, hb);
			DeleteObject(hb);
		}
		static void LimpidDrawingBoard(HWND hWnd, HDC hdc, int s = 4)
		{
			static HBITMAP prebmp;
			prebmp = hwndToHbitmap(hWnd, nh);
			HDC hMemDC1 = CreateCompatibleDC(hdc);
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
			StretchBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);


			HDC         hMemDC2;
			HBITMAP     hBmpMem2;
			HBITMAP     hPreBmp2;

			hMemDC2 = CreateCompatibleDC(hdc);

			hBmpMem2 = CreateCompatibleBitmap(hdc, vp.cx * s, vp.cy * s);

			hPreBmp2 = (HBITMAP)SelectObject(hMemDC2, hBmpMem2);


			HDC         hMemDC3;
			HBITMAP     hBmpMem3;
			HBITMAP     hPreBmp3;

			hMemDC3 = CreateCompatibleDC(hdc);

			hBmpMem3 = CreateCompatibleBitmap(hdc, vp.cx * s, vp.cy * s);

			hPreBmp3 = (HBITMAP)SelectObject(hMemDC3, hBmpMem3);


			//On hMemDC.
			// //DC1不可用
			//先在DC3画黑白图
			//将白色去掉,画到带有背景的DC2
			CreateRect(0, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(0, 0, 0));
			CreateRoundButtonEx(hMemDC3, 8, 8, (vp.cx - 7) * s, (vp.cy - 7) * s, csz * s, L"", 0, RGB(255, 255, 255), RGB(255, 255, 255));

			StretchBlt(hMemDC2, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);
			if (ldce.blurRad != 0)AreaBlur(hMemDC2, { 0,0,vp.cx * s,vp.cy * s }, ldce.blurRad, ldce.blurRad * 1.5 + (ldce.Factor - 1) + s, ldce.ExFlag);
			CreateAlphaMask(hMemDC2, 0, 0, vp.cx * s, vp.cy * s, ldce.Alpha, vci.BkColor);
			TextPreDrawEx(hMemDC2, 0, 0, vp.cx * s, vp.cy * s, txt, vci.TxtSize * s, 1, vci.TxtColor);
			TransparentBlt(hMemDC2, 0, 0, vp.cx * s, vp.cy * s, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(255, 255, 255));
			TransparentBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC2, 0, 0, vp.cx * s, vp.cy * s, RGB(0, 0, 0));
			//CreateAlphaMaskFromDC(hdc, 0, 0, vp.cx * s, vp.cy * s, ldce.Alpha, hMemDC2);

			SelectObject(hMemDC1, prebmp);
			DeleteObject(prebmp);
			DeleteDC(hMemDC1);


			SelectObject(hMemDC2, hPreBmp2);


			DeleteObject(hBmpMem2);

			DeleteDC(hMemDC2);


			SelectObject(hMemDC3, hPreBmp3);


			DeleteObject(hBmpMem3);

			DeleteDC(hMemDC3);


			//CreateRoundButtonEx(hdc, 8, 8, (vp.cx - 8) * s, (vp.cy - 8) * s, csz * s, txt, vci.TxtSize * s, vci.BkColor, vci.TxtColor);
		}
		static void HollowButtonDrawingBoard(HWND hWnd, HDC hdc, int s = 4)
		{
			static HBITMAP prebmp;
			prebmp = hwndToHbitmap(hWnd, nh);
			HDC hMemDC1 = CreateCompatibleDC(hdc);
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
			StretchBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);
			SelectObject(hMemDC1, prebmp);
			DeleteObject(prebmp);
			DeleteDC(hMemDC1);
			DrawHollowRoundRect(hdc, 8, 8, (vp.cx - 8) * s, (vp.cy - 8) * s, csz * s, psz * s, vci.BkColor);
			TextPreDrawEx(hdc, vp.x, vp.y, vp.cx, vp.cy, txt, vci.TxtSize, 1, vci.TxtColor);
		}
		static void AAQBoard(HWND hWnd, HDC hdc, int s = 4)
		{
			static HBITMAP prebmp;
			prebmp = hwndToHbitmap(hWnd, nh);
			HDC hMemDC1 = CreateCompatibleDC(hdc);
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
			StretchBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC1, vp.x, vp.y, vp.cx, vp.cy, SRCCOPY);
			SelectObject(hMemDC1, prebmp);
			DeleteObject(prebmp);
			DeleteDC(hMemDC1);
		}
	};
	int VertexUIControlDrawingBoard::psz;
	int VertexUIControlDrawingBoard::csz;
	VertexUIPos VertexUIControlDrawingBoard::vp;
	VertexUIColorInfo VertexUIControlDrawingBoard::vci;
	HDC VertexUIControlDrawingBoard::nh;
	const wchar_t* VertexUIControlDrawingBoard::txt;
	LimpidDesignCtlEnhancement VertexUIControlDrawingBoard::ldce;

	VertexUIControlDrawingBoard vcdb;
	void DrawHQRoundRect(HWND hWnd, HDC hdc,int x,int y,int cx,int cy,int Corner,COLORREF clr)
	{
		vcdb.RoundButton(hWnd, hdc, { x,y,cx,cy }, Corner, 0, { clr });
	}
	void ContrastPolygon(HWND hWnd, HDC hdc, VertexUIPos vp, const POINT* pt, int ang)
	{
		int s = 1;



		HDC         hMemDC3;
		HBITMAP     hBmpMem3;
		HBITMAP     hPreBmp3;

		hMemDC3 = CreateCompatibleDC(hdc);

		hBmpMem3 = CreateCompatibleBitmap(hdc, vp.cx * s, vp.cy * s);

		hPreBmp3 = (HBITMAP)SelectObject(hMemDC3, hBmpMem3);


		//On hMemDC.
		// //DC1不可用
		//先在DC3画黑白图
		//将白色去掉,画到带有背景的DC2
		CreateRect(0, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(0, 0, 0));

		//HERE
		DrawPloygon(hMemDC3, pt, ang, RGB(255, 255, 255));

		TransparentBlt(hdc, 0, 0, vp.cx * s, vp.cy * s, hMemDC3, 0, 0, vp.cx * s, vp.cy * s, RGB(255, 255, 255));
		//CreateAlphaMaskFromDC(hdc, 0, 0, vp.cx * s, vp.cy * s, ldce.Alpha, hMemDC2);



		SelectObject(hMemDC3, hPreBmp3);


		DeleteObject(hBmpMem3);

		DeleteDC(hMemDC3);


		//CreateRoundButtonEx(hdc, 8, 8, (vp.cx - 8) * s, (vp.cy - 8) * s, csz * s, txt, vci.TxtSize * s, vci.BkColor, vci.TxtColor);
	}
}
#endif
int VertexUI::VertexUIControl::CreateCtl(const wchar_t* type, HWND h, HDC hdc, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text = L"NULL", VertexUIColorInfo vci = {})
{
	this->CtlName = type;
	if (IsInfoChanged == false)
	{
		this->x = x;
		this->y = y;
		this->sizex = wid;
		this->sizey = height;
		this->Text = Text;
	}
	this->runfun = func;
	this->hdc = hdc;
	this->hWnd = h;
	this->pInfo = vci;
	if (CtlName == L"Button")
	{
		if (vci.Gradient.From && vci.Gradient.To != 0)VertexUI::Panel::CreateGradientButton(hWnd, hdc, this->x, this->y, this->sizex, this->sizey, vci.Gradient.From, vci.Gradient.To, vci.Gradient.Degree, this->Text, 0, vci.TxtColor);
		else VertexUI::Panel::CreateSimpleButtonEx(hWnd, hdc, this->x, this->y, this->sizex, this->sizey, vci.BkColor, this->Text, 0, vci.TxtColor);
	}
	if (CtlName == L"Button2")
	{

		VertexUI::Panel::vcdb.RoundButton(hWnd, hdc, { this->x, this->y, this->sizex, this->sizey }, vci.Corner, this->Text, vci);
	}
	if (CtlName == L"Text")
	{
		VertexUI::Panel::TextPreDrawEx(hdc, this->x, this->y, this->sizex, this->sizey, this->Text, vci.TxtSize, 1, vci.TxtColor);
	}
	if (CtlName == L"LeftText")
	{
		VertexUI::Panel::TextPreDrawEx(hdc, this->x, this->y, this->sizex, this->sizey, this->Text, vci.TxtSize, 0, vci.TxtColor);
	}
	return 0;
}
int VertexUI::VertexUIControl::CreateCtl(const wchar_t* type, int x, int y, int wid, int height, std::function<void()> func, const wchar_t* Text = L"NULL", VertexUIColorInfo vci = {})
{
	this->CtlName = type;
	if (IsInfoChanged == false)
	{
		this->x = x;
		this->y = y;
		this->sizex = wid;
		this->sizey = height;
		this->Text = Text;
	}
	this->runfun = func;
	this->pInfo = vci;
	return 0;
}
#ifdef test__
void TESTDrawLine(HDC pDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine);
#endif
#define _Delay(_x) Sleep(_x);
namespace VertexUI::Click
{
	extern int KeepDownMsg;
}
namespace VertexUI::Panel
{
	void XSleep(UINT Delay_ms)
	{
		DWORD dwTick = GetTickCount64() + Delay_ms;
		while (GetTickCount64() < dwTick)
		{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(0);
		}
	}
	double CalcEaseOutCurve(double t, double b, double c, double d) {
		return c * ((t = t / d - 1) * t * t + 1) + b;
	}
	double CalcOutCurve4(double t, double b, double  c, double d) {
		return -c * ((t = t / d - 1) * t * t * t - 1) + b;
	}
	double CalcEaseInOutCurve(double t, double b, double c, double d) {
		t += 10;
		if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;
		return c / 2 * ((t -= 2) * t * t + 2) + b;

	}
	void CtlMoveInAnimation(HWND hWnd, RECT rc, int& animated, int MoveNum = 0)
	{
		for (int t = 1; t < 40; t++)
		{
			animated = CalcEaseOutCurve(t, 0, 40, 40) - MoveNum; InvalidateRect(hWnd, &rc, 0); XSleep(10);
		}
	}

	class AnimationController
	{
	public:
		static int Process;
		static bool Flag;
		int CachePullDown(HWND h, RECT rc)
		{
			for (int t = 1; t < 20; t++)
			{
				double ra;
				ra = CalcEaseOutCurve(t, 0, rc.bottom + 1, 20); RefreshAreaFromCache(h, 0, 0, rc.right - rc.left, ra); _Delay(10);
			}
			RefreshAreaFromCache(h, 0, 0, rc.right - rc.left, rc.bottom - rc.top);
			return 0;
		}
		int CachePullUp(HWND h, RECT rc)
		{
			for (int t = 1; t < 20; t++)
			{
				double ra;
				ra = CalcEaseOutCurve(t, 0, rc.bottom - 40, 20); RefreshAreaFromCache(h, 0, rc.bottom - ra, rc.right - rc.left, ra); _Delay(10);
			}
			return 0;
		}
		void FadeIn(HDC hdc, RECT rc, HDC hMemDC, int ti = 100)
		{
			BLENDFUNCTION bf = { 0 };
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 1;
			bf.AlphaFormat = 0;
			for (int t = 1; t < t + 1; t++)
			{
				double value;
				value = CalcEaseInOutCurve(t, 0, 50, ti);
				bf.SourceConstantAlpha = value;
				AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
				if (value > 49)break;
				Sleep(1);
			}
			bf.SourceConstantAlpha = 255;
			AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		}
		void AreaFadeIn(HDC hdc, int xClip, int yClip, VertexUIPos vp, HDC hMemDCx, HWND hWnd = 0)
		{
			if (hWnd == 0)hWnd = GhWnd;


			static HBITMAP FinalBlur1;
			static int  ocx, ocy;
			/*高斯模糊处理部分*/


			HDC hMemDC2 = CreateCompatibleDC(hdc);
			HBITMAP hBmpMem2 = CreateCompatibleBitmap(hdc, vp.cx, vp.cy);
			HBITMAP hPreBmp2 = (HBITMAP)SelectObject(hMemDC2, hBmpMem2);
			BitBlt(hMemDC2, vp.x, vp.y, vp.cx, vp.cy, hMemDCx, xClip, yClip, SRCCOPY);

			HDC hMemDC1 = CreateCompatibleDC(hdc);
			FinalBlur1 = hwndToHbitmap(hWnd, hdc); ocx = vp.cx; ocy = vp.cy;
			HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, FinalBlur1);

			BLENDFUNCTION bf = { 0 };
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 1;
			bf.AlphaFormat = 0;
			for (int t = 1; t < t + 1; t++)
			{
				HDC         hMemDC;
				HBITMAP     hBmpMem;
				HBITMAP     hPreBmp;
				hMemDC = CreateCompatibleDC(hdc);

				hBmpMem = CreateCompatibleBitmap(hdc, vp.cx, vp.cy);

				hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
				//On hMemDC.
				double value;
				value = CalcEaseInOutCurve(t, 0, 255, 30);
				bf.SourceConstantAlpha = value;

				BitBlt(hMemDC, 0, 0, vp.cx, vp.cy, hMemDC1, 0, 0, SRCCOPY);

				AlphaBlend(hMemDC, (255 * 0.5) - value * 0.5, 0, vp.cx, vp.cy, hMemDC2, 0, 0, vp.cx, vp.cy, bf);


				BitBlt(hdc, 0, 0, vp.cx, vp.cy, hMemDC, 0, 0, SRCCOPY);


				SelectObject(hMemDC, hPreBmp);


				DeleteObject(hBmpMem);

				DeleteDC(hMemDC);
				Sleep(1);
				if (value > 254)break;

			}
			bf.SourceConstantAlpha = 255;
			AlphaBlend(hdc, 0, 0, vp.cx, vp.cy, hMemDCx, 0, 0, vp.cx, vp.cy, bf);
			SelectObject(hMemDC1, hPreBmp1);
			DeleteDC(hMemDC1);
			DeleteObject(FinalBlur1);
			SelectObject(hMemDC2, hPreBmp2);
			DeleteDC(hMemDC2);
			DeleteObject(hBmpMem2);
		}
		static void SetGlobalAnimationTimer(HWND hWnd)
		{
			SetTimer(hWnd, 10, 10, 0);
			Flag = true;
		}
		static void KillGlobalAnimationTimer(HWND hWnd)
		{
			KillTimer(hWnd, 10);
			Flag = false;
			Process = 0;
		}
	};
	int AnimationController::Process = 0;
	bool AnimationController::Flag = false;
	int anistat = 0;
	DWORD WINAPI VUIPAnimationThread(LPVOID pf)
	{
		PanelHandleFun* p = (PanelHandleFun*)pf;
		HDC hdc = p->hdc;
		HDC hMemDC = p->hmdc;
		RECT rc = p->rc;
		HBITMAP hPreBmp = p->hpbp;
		HBITMAP hBmpMem = p->hmbp;
		if (rc.right - rc.left >= 750)
		{
#ifndef _MINANIMATION
			BitBlt(hdc, rc.right - 2, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 5, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 18, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 36, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 59, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 100, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 158, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 213, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 300, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 350, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 400, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 450, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
			BitBlt(hdc, rc.right - 530, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(8);
#endif
			BitBlt(hdc, 300, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
#ifdef _MINANIMATION
			XSleep(10);
			BitBlt(hdc, 295, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			XSleep(10);
			BitBlt(hdc, 282, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
#endif
		}
		XSleep(8);
		BitBlt(hdc, 262, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 243, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 214, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 171, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 116, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 71, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 41, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 29, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 17, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 11, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 8, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 5, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 3, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 2, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 1, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
		XSleep(8);
		BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);
		return 0;
	}
	void VUIBlendInAnimation(HDC hdc, RECT rc, HDC hMemDCx, HWND hWnd = 0)
	{
		if (hWnd == 0)hWnd = GhWnd;
		HDC ndc = GetDC(hWnd);

		static HBITMAP FinalBlur1;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC1 = CreateCompatibleDC(hdc);
		FinalBlur1 = hwndToHbitmap(hWnd, hdc); ocx = rc.right; ocy = rc.bottom;
		HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, FinalBlur1);




		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		for (int t = 1; t < t + 1; t++)
		{
			HDC         hMemDC;
			HBITMAP     hBmpMem;
			HBITMAP     hPreBmp;
			hMemDC = CreateCompatibleDC(hdc);

			hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			double value;
			value = CalcEaseInOutCurve(t, 0, 255, 30);
			bf.SourceConstantAlpha = value;

			BitBlt(hMemDC, 0, 0, RCWID, rc.bottom - rc.top, hMemDC1, 0, 0, SRCCOPY);

			AlphaBlend(hMemDC, 0, (255 * 2) - value * 2, rc.right - rc.left, rc.bottom - rc.top, hMemDCx, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);


			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);


			DeleteObject(hBmpMem);

			DeleteDC(hMemDC);
			Sleep(1);
			if (value > 254)break;

		}
		bf.SourceConstantAlpha = 255;
		AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDCx, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		SelectObject(hMemDC1, hPreBmp1);
		DeleteDC(hMemDC1);
		DeleteObject(FinalBlur1);
		ReleaseDC(hWnd, ndc);
		/*
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		Sleep(10);
		bf.SourceConstantAlpha = 10;
		AlphaBlend(hdc, 40, 40, rc.right - rc.left - 40 * 2, rc.bottom - rc.top - 40 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 20;
		AlphaBlend(hdc, 35, 35, rc.right - rc.left - 35 * 2, rc.bottom - rc.top - 35 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 30;
		AlphaBlend(hdc, 30, 30, rc.right - rc.left - 30 * 2, rc.bottom - rc.top - 30 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 40;
		AlphaBlend(hdc, 25, 25, rc.right - rc.left - 25 * 2, rc.bottom - rc.top - 25 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 55;
		AlphaBlend(hdc, 20, 20, rc.right - rc.left - 20 * 2, rc.bottom - rc.top - 20 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 67;
		AlphaBlend(hdc, 16, 16, rc.right - rc.left - 16 * 2, rc.bottom - rc.top - 16 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 91;
		AlphaBlend(hdc, 13, 13, rc.right - rc.left - 13 * 2, rc.bottom - rc.top - 13 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 110;
		AlphaBlend(hdc, 10, 10, rc.right - rc.left - 10 * 2, rc.bottom - rc.top - 10 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 150;
		AlphaBlend(hdc, 7, 7, rc.right - rc.left - 7 * 2, rc.bottom - rc.top - 7 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 210;
		AlphaBlend(hdc, 5, 5, rc.right - rc.left - 5 * 2, rc.bottom - rc.top - 5 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 230;
		AlphaBlend(hdc, 3, 3, rc.right - rc.left - 3 * 2, rc.bottom - rc.top - 3 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 240;
		AlphaBlend(hdc, 2, 2, rc.right - rc.left - 2 * 2, rc.bottom - rc.top - 2 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 249;
		AlphaBlend(hdc, 1, 1, rc.right - rc.left - 1 * 2, rc.bottom - rc.top - 1 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 255;
		AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		*/
	}
	void VUIBlendInAnimation3(HDC hdc, RECT rc, HDC hMemDCx, HWND hWnd = 0)
	{
		if (hWnd == (HWND)0)hWnd = GhWnd;
		HDC ndc = GetDC(hWnd);

		static HBITMAP FinalBlur1;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC1 = CreateCompatibleDC(hdc);
		FinalBlur1 = hwndToHbitmap(hWnd, hdc); ocx = rc.right; ocy = rc.bottom;
		HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, FinalBlur1);




		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;

		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
		for (int t = 1; t < t + 1; t++)
		{

			hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
			//On hMemDC.
			double value;
			value = CalcEaseOutCurve(t, 0, 255, 20);
			bf.SourceConstantAlpha = value;

			if (t <= 10)
			{
				double __ = CalcEaseOutCurve(t, 0, 60, 10);
				::SetStretchBltMode(hMemDC, HALFTONE);
				::SetBrushOrgEx(hMemDC, 0, 0, NULL);
				StretchBlt(hMemDC, __, __, RCWID - __ * 2, (rc.bottom - rc.top) - __ * 2, hMemDC1, 0, 0, RCWID, (rc.bottom - rc.top), SRCCOPY);
			}
			//BitBlt(hMemDC, 0, 0, RCWID, rc.bottom - rc.top, hMemDC1, 0, 0, SRCCOPY);

			AlphaBlend(hMemDC, (255 - value) / 4, (255 - value) / 4, rc.right - rc.left - ((255) - value) / 2, rc.bottom - rc.top - ((255) - value) / 2, hMemDCx, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);


			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


			SelectObject(hMemDC, hPreBmp);





			Sleep(1);
			if (value > 254)break;

		}
		DeleteObject(hBmpMem);
		DeleteDC(hMemDC);
		bf.SourceConstantAlpha = 255;
		AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDCx, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		SelectObject(hMemDC1, hPreBmp1);
		DeleteDC(hMemDC1);
		DeleteObject(FinalBlur1);
		ReleaseDC(hWnd, ndc);
	}
	void VUIBlendInAnimation2(HDC hdc, RECT rc, HDC hMemDC)
	{
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		Sleep(10);
		bf.SourceConstantAlpha = 10;
		AlphaBlend(hdc, 40, 40, rc.right - rc.left - 40 * 2, rc.bottom - rc.top - 40 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 20;
		AlphaBlend(hdc, 35, 35, rc.right - rc.left - 35 * 2, rc.bottom - rc.top - 35 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 30;
		AlphaBlend(hdc, 30, 30, rc.right - rc.left - 30 * 2, rc.bottom - rc.top - 30 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 40;
		AlphaBlend(hdc, 25, 25, rc.right - rc.left - 25 * 2, rc.bottom - rc.top - 25 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 55;
		AlphaBlend(hdc, 20, 20, rc.right - rc.left - 20 * 2, rc.bottom - rc.top - 20 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 67;
		AlphaBlend(hdc, 16, 16, rc.right - rc.left - 16 * 2, rc.bottom - rc.top - 16 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 91;
		AlphaBlend(hdc, 13, 13, rc.right - rc.left - 13 * 2, rc.bottom - rc.top - 13 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 110;
		AlphaBlend(hdc, 10, 10, rc.right - rc.left - 10 * 2, rc.bottom - rc.top - 10 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 150;
		AlphaBlend(hdc, 7, 7, rc.right - rc.left - 7 * 2, rc.bottom - rc.top - 7 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 210;
		AlphaBlend(hdc, 5, 5, rc.right - rc.left - 5 * 2, rc.bottom - rc.top - 5 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 230;
		AlphaBlend(hdc, 3, 3, rc.right - rc.left - 3 * 2, rc.bottom - rc.top - 3 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 240;
		AlphaBlend(hdc, 2, 2, rc.right - rc.left - 2 * 2, rc.bottom - rc.top - 2 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 249;
		AlphaBlend(hdc, 1, 1, rc.right - rc.left - 1 * 2, rc.bottom - rc.top - 1 * 2, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
		Sleep(10);
		bf.SourceConstantAlpha = 255;
		AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
	}
	void VUIFadeInAnimation(HDC hdc, RECT rc, HDC hMemDC, int ti = 100)
	{
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		for (int t = 1; t < t + 1; t++)
		{
			double value;
			value = CalcEaseInOutCurve(t, 0, 255, ti);
			bf.SourceConstantAlpha = value;
			AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
			if (value > 254)break;
			XSleep(10);
		}
		bf.SourceConstantAlpha = 255;
		AlphaBlend(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, bf);
	}
	template<class draw>
	int CreatePanelAnimation(HWND h, HDC hdc, draw DrawFun)
	{
		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		//InvalidateRect(h, &rc, 1);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.
		DrawFun(h, hMemDC);
		if (anistat == 1)
		{
			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			anistat = 0;
			return -1;
		}
		if (anistat == 0)
		{
			PanelHandleFun p{};
			p.hdc = hdc;
			p.hmdc = hMemDC;
			p.rc = rc;
			p.hpbp = hPreBmp;
			p.hmbp = hBmpMem;
			HANDLE thread = CreateThread(NULL, NULL, VUIPAnimationThread, &p, 0, 0);
			if (thread)
			{
				WaitForSingleObject(thread, INFINITE);
				CloseHandle(thread);
			}

			anistat = 1;
			return -1;
		}

		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);

		return 0;
	}
	template<class draw>
	int CreatePanelAnimationEx(HWND h, HDC hdc, draw DrawFun, int Flag)
	{
		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		//InvalidateRect(h, &rc, 1);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.
		DrawFun(h, hMemDC);
		if (anistat == 1)
		{
			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			anistat = 0;
			return -1;
		}
		if (anistat == 0)
		{
			if (Flag == 2)
			{
				VUIBlendInAnimation(hdc, rc, hMemDC);
			}
			if (Flag == 3)
			{
				VUIBlendInAnimation2(hdc, rc, hMemDC);
			}
			if (Flag == 4)
			{
				AnimationController ac;
				ac.FadeIn(hdc, rc, hMemDC, 30);

			}

			anistat = 1;
			return -1;
		}

		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);

		return 0;
	}
	int _VertexUI_PanelFlag = 0;
	void CreatePanelByFlag(HWND h, HDC hdc, DRAWPANEL DrawFun, int Flag = 0)
	{

		//On hMemDC.
		if (PanelID == PrevPanelID)
		{
			_CreatePanel(h, hdc, DrawFun);
			anistat = 0;
		}
		if (PanelID != PrevPanelID)
		{
			if (Flag == 0)
			{
				CreatePanelAnimation(h, hdc, DrawFun);
			}
			if (Flag == 1)
			{
				_CreatePanel(h, hdc, DrawFun);
			}
			if (Flag == 2)
			{
				CreatePanelAnimationEx(h, hdc, DrawFun, 2);
			}
			PrevPanelID = PanelID;
			anistat = 0;
		}
	}
	HDC inhdc;
	HDC GlobalSharedDC;
	HBITMAP inbmp;
	HBITMAP CopyBitmap(HBITMAP hbm) {
		HDC hdcSrc = CreateCompatibleDC(NULL);
		HDC hdcDst = CreateCompatibleDC(NULL);
		HBITMAP hbmOld, hbmOld2, hbmNew;
		BITMAP bm;
		GetObject(hbm, sizeof(bm), &bm);
		hbmOld = (HBITMAP)SelectObject(hdcSrc, hbm);
		hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes,
			bm.bmBitsPixel,
			NULL);
		hbmOld2 = (HBITMAP)SelectObject(hdcDst, hbmNew);
		BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);
		SelectObject(hdcSrc, hbmOld);
		SelectObject(hdcDst, hbmOld2);
		DeleteDC(hdcSrc);
		DeleteDC(hdcDst);
		return hbmNew;
	}
	template<class draw>
	void CreatePanelDynamic(HWND h, HDC hdc, draw DrawFun, int Flag = 0, int updateFlag = 0)
	{

		//On hMemDC.
		if (PanelID == PrevPanelID)
		{
			if (updateFlag == 2)
			{
				_CreatePanel(h, hdc, DrawFun);
			}
			else
			{
				if (dystate == 0)
				{
					//BitBlt
					RECT rc;
					GetClientRect(h, &rc);
					HDC         hMemDC;
					HBITMAP     hBmpMem;
					HBITMAP     hPreBmp;

					hMemDC = CreateCompatibleDC(hdc);

					hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

					hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
					//On hMemDC.

					BitBlt(hMemDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, inhdc, 0, 0, SRCCOPY);
					BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


					SelectObject(hMemDC, hPreBmp);


					DeleteObject(hBmpMem);

					DeleteDC(hMemDC);
				}
				if (dystate == 1) //First time
				{
					DeleteDC(inhdc);
					HBITMAP     hBmpMem;
					HBITMAP     hPreBmp;
					RECT rc;
					GetClientRect(h, &rc);
					inhdc = CreateCompatibleDC(hdc);

					hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

					hPreBmp = (HBITMAP)SelectObject(inhdc, hBmpMem);
					//On hMemDC.
					DrawFun(h, inhdc);

					BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, inhdc, 0, 0, SRCCOPY);


					//SelectObject(inhdc, hPreBmp);


					DeleteObject(hBmpMem);
					dystate = 0;
				}
				if (dystate == 2) //First time
				{
					DeleteDC(inhdc);
					HBITMAP     hBmpMem;
					HBITMAP     hPreBmp;
					RECT rc;
					GetClientRect(h, &rc);
					inhdc = CreateCompatibleDC(hdc);
					hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
					hPreBmp = (HBITMAP)SelectObject(inhdc, hBmpMem);
					//On hMemDC.
					DrawFun(h, inhdc);
					DeleteObject(hBmpMem);
					dystate = 0;
				}
			}
			anistat = 0;
		}
		if (PanelID != PrevPanelID)
		{
			if (Flag == 0)
			{
				CreatePanelAnimation(h, hdc, DrawFun);
			}
			if (Flag == 1)
			{
				_CreatePanel(h, hdc, DrawFun);
			}
			if (Flag == 2)
			{
				CreatePanelAnimationEx(h, hdc, DrawFun, 2);
			}
			if (Flag == 3)
			{
				CreatePanelAnimationEx(h, hdc, DrawFun, 3);
			}
			if (Flag == 4)
			{
				CreatePanelAnimationEx(h, hdc, DrawFun, 4);
			}
			if (Flag == 104)
			{
				CreatePanelAnimationEx(h, hdc, DrawFun, 4);
				AnimationController ac;
				ac.SetGlobalAnimationTimer(h);
			}
			if (updateFlag == 0)
			{
				dystate = 1;
			}
			PrevPanelID = PanelID;
			anistat = 0;
		}
	}
	void CreateButton(HWND hWnd, HDC hdc, int x, int y, int sizex, int sizey, int cid, const wchar_t* CTLTXT)
	{
		//VERTEXUICTL btn;
		//btn.Text = L"114514";
		CreateSimpleButton(hWnd, hdc, x, y, sizex, sizey, CTLTXT);
	}


	void DrawGradient(HDC pDC, RECT rRect, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = rRect.bottom - rRect.top;

		int iWidth = rRect.right - 1;

		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = rRect.left; i <= iWidth; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, i, rRect.top, 1, iHeight);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	COLORREF GetGradientValue(COLORREF crFrom, COLORREF crTo, int iWidth, int stage)
	{


		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = 0; i <= stage; i++, iR += idR, iG += idG, iB += idB)
		{
			if (i == stage)return(RGB(iR / 256, iG / 256, iB / 256));
		}

	}
	void DrawGradientEx(HDC pDC, int x, int y, int cx, int cy, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = cy;

		int iWidth = cx;

		//

		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);

		//

		int idR = (256 * (GetRValue(crTo) - iR)) / iWidth;

		int idG = (256 * (GetGValue(crTo) - iG)) / iWidth;

		int idB = (256 * (GetBValue(crTo) - iB)) / iWidth;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = x; i <= x + iWidth; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, i, y, 1, iHeight);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	void DrawGradientToBottomEx(HDC pDC, int x, int y, int cx, int cy, COLORREF crFrom, COLORREF crTo)
	{

		int iHeight = cy;

		int iWidth = cx;
		//
		int iR = GetRValue(crFrom);

		int iG = GetGValue(crFrom);

		int iB = GetBValue(crFrom);
		//
		int idR = (256 * (GetRValue(crTo) - iR)) / iHeight;

		int idG = (256 * (GetGValue(crTo) - iG)) / iHeight;

		int idB = (256 * (GetBValue(crTo) - iB)) / iHeight;



		iR *= 256;

		iG *= 256;

		iB *= 256;

		// ->

		for (int i = y; i <= y + iHeight; i++, iR += idR, iG += idG, iB += idB)
		{
			RECT rcx;
			RectTypeConvert(rcx, x, i, iWidth, 1);
			HBRUSH hbr = CreateSolidBrush(RGB(iR / 256, iG / 256, iB / 256));
			HBRUSH hOld = (HBRUSH)SelectObject(pDC, hbr);
			FillRect(pDC, &rcx, hbr);
			DeleteObject(hbr);
			SelectObject(pDC, hOld);
		}

	}
	void DrawLinearGradient(HDC pDC, int x, int y, int cx, int cy, COLORREF clr1, COLORREF clr2, int angle)
	{
		int width = cx;
		int height = cy;
		int r1 = GetRValue(clr1);
		int r2 = GetRValue(clr2);
		int g1 = GetGValue(clr1);
		int g2 = GetGValue(clr2);
		int b1 = GetBValue(clr1);
		int b2 = GetBValue(clr2);

		int rSpan = r2 - r1;
		int gSpan = g2 - g1;
		int bSpan = b2 - b1;

		int len = 0;
		int len_clr = 0;
		if (angle == 0)len_clr = len = height;
		else if (angle == 90)len_clr = len = width;
		else len_clr = len = width + height / tan((double)angle * 3.14 / 180);

		HDC dcMem = CreateCompatibleDC(pDC);

		HBITMAP bmp = CreateCompatibleBitmap(pDC, cx, cy);

		HBITMAP hPreBmp = (HBITMAP)SelectObject(dcMem, bmp);
		//开始内存绘制
		for (int i = 0; i < len; i++)
		{
			COLORREF clr = RGB(r1 + (i * rSpan) / len_clr, g1 + (i * gSpan) / len_clr, b1 + (i * bSpan) / len_clr);
			HPEN pen = CreatePen(PS_SOLID, 2, clr);
			SelectObject(dcMem, pen);
			if (angle == 0)
			{
				MoveToEx(dcMem, 0, i, 0);
				LineTo(dcMem, width, i);
			}
			else
			{
				SelectObject(dcMem, pen);
				MoveToEx(dcMem, i, 0, 0);
				int y = (int)i * tan((double)angle * 3.14 / 180);
				LineTo(dcMem, 0, y);
			}
			DeleteObject(pen);
		}
		BitBlt(pDC, x, y, cx, cy, dcMem, 0, 0, SRCCOPY);
		SelectObject(dcMem, hPreBmp);


		DeleteObject(bmp);

		DeleteDC(dcMem);

	}
	void DrawAlphaShadowToRightSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cy, COLORREF cl)
	{
		for (int i = 0; i <= 10; i++)
		{
			CreateAlphaMask(hdc, x + i, y, 1, cy, (10 - i) * ShadowAlphaFactor, VERTEXUICOLOR_DARKEN);
		}
	}
	void DrawAlphaShadowToBottomSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cx, COLORREF cl)
	{
		for (int i = 0; i <= 10; i++)
		{
			CreateAlphaMask(hdc, x, y + i, cx, 1, (10 - i) * ShadowAlphaFactor, VERTEXUICOLOR_DARKEN);
		}
	}
	void DrawAlphaShadowToTopSample(HDC hdc, int x, int y, int ShadowAlphaFactor, int cx, COLORREF cl,int b)
	{
		for (int i = 0; i <= b; i++)
		{
			CreateAlphaMask(hdc, x, y - i-1, cx, 1, (b - i) * ShadowAlphaFactor, cl);
		}
	}
	void _RegisterScrollBarPanel(const wchar_t* id, int p = -1)
	{
		ScrollSingleID = id;
		ScrollSinglePage = p;
	}
	void MonitorValue(HWND h, int i, HDC dc = (HDC)0, bool ConsoleMode = false)
	{
		wchar_t a[128];
		_itow(i, a, 10);
		if (ConsoleMode == true) {
			OutputDebugString(a);
			OutputDebugString(L"\n");
			return;
		}
		if (dc != HDC(0))
		{
			CreateRect(h, dc, 40, 40, 100, 40, VERTEXUICOLOR_WHITE);
			TextPreDrawEx(dc, 40, 40, 100, 40, a, 18, 1, VERTEXUICOLOR_GREENSEA);
		}
		else
		{
			HDC hdc = GetDC(h);
			CreateRect(h, hdc, 20, 20, 100, 40, VERTEXUICOLOR_WHITE);
			TextPreDrawEx(hdc, 20, 20, 100, 40, a, 18, 1, VERTEXUICOLOR_GREENSEA);
			ReleaseDC(h, hdc);
		}
	}
#define MointorValue MonitorValue
	void CreateAlphaMask(HDC hdc, int x, int y, int cx, int cy, int alpha, COLORREF cl)
	{
		int ocx, ocy;
		ocx = cx; ocy = cy;
		if (cx < 0)cx = abs(cx);
		if (cy < 0)cy = abs(cy);
		if (ocx < 0)x -= cx;
		if (ocy < 0)y -= cy;
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);//获取整个屏幕,之后绘制.
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);

		CreateRectArea(hMemDC, 0, 0, cx, cy, cl);
		//CreateRect(hWnd, hMemDC, ((rc.right - rc.left)/2) - (800 / 2), rc.bottom - 230, 800, 100, RGB(20, 20, 20));
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;

		AlphaBlend(hdc, x, y, cx, cy, hMemDC, 0, 0, cx, cy, bf);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hBmpMem);
		DeleteDC(hMemDC);//透明度混合
	}
	void CreateAlphaMaskFromDC(HDC hdc, int x, int y, int cx, int cy, int alpha, HDC cl)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);//获取整个屏幕,之后绘制.
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);

		//CreateRect(hWnd, hMemDC, ((rc.right - rc.left)/2) - (800 / 2), rc.bottom - 230, 800, 100, RGB(20, 20, 20));
		BLENDFUNCTION bf = { 0 };
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = 1;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = alpha;
		BitBlt(hMemDC, 0, 0, cx, cy, cl, 0, 0, SRCCOPY);
		AlphaBlend(hdc, x, y, cx, cy, hMemDC, 0, 0, cx, cy, bf);
		SelectObject(hMemDC, hPreBmp);
		DeleteObject(hBmpMem);
		DeleteDC(hMemDC);//透明度混合
	}
	HBITMAP CreateGDIBitmap(int nWid, int nHei, void** ppBits)
	{
		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = nWid;
		bmi.bmiHeader.biHeight = -nHei;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = 0;

		HDC hdc = GetDC(NULL);
		HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
		ReleaseDC(NULL, hdc);
		return hBmp;
	}
	int in_test()
	{
		return 0;
	}
	typedef class VERTEXUILISTPANEL
	{
		typedef int RUNFUN;
	public:
		std::vector<const wchar_t*> listtxt = { 128,(L"") };
		std::vector<std::function<void()>> listclick = { 128,(in_test) };
		void Add(const wchar_t* txt, std::function<void()> fun)
		{
			id++;
			listtxt[id] = txt;
			listclick[id] = fun;
		}
		COLORREF PanelBkColor = VERTEXUICOLOR_DARKNIGHT;
	protected:
		int id=0;
	}tagVertexUIList;
	typedef void (DRAWLISTPANEL)(HWND, HDC, VERTEXUILISTPANEL);
	bool ListPanelBlured = false;
#ifdef FullDesign
#define _SAFE_DELETE(a) if( (a) != NULL ) delete (a); (a) = NULL;
#include "../GaussBlurFilter.hpp"
	namespace _FullDesign
	{
		HBITMAP GaussianBlurFunc(HDC hDC, int x, int y, int iWidth, int iHeight)
		{
			static HBITMAP hBitMap;
			DeleteObject(hBitMap);

			HDC hMemDC = CreateCompatibleDC(hDC);
			hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
			HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
			BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, 0, 0, SRCCOPY);
			int size = iWidth * iHeight * 5;
			unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

			int nBytes = GetBitmapBits(hBitMap, size, pBits);

			//GaussianBlur(pBits, iWidth, iHeight, 4,40); //单线程高斯模糊 不明白为啥要开

			unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
			CGaussBlurFilter<double> _filter;
			_filter.SetSigma(3.5); // 设置高斯半径
			_filter.SetMultiThreads(true, 4);
			_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
			SetBitmapBits(hBitMap, size, nBits);
			SelectObject(hMemDC, hPreBmp);
			DeleteObject(hMemDC);

			_SAFE_DELETE(pBits);
			_SAFE_DELETE(nBits);
			//delete[]pBits;
			return hBitMap;
		}
		HBITMAP _GaussianBlurFunc(HDC hDC, int x, int y, int iWidth, int iHeight, double s = 3.5, bool IsLowQuality = false, int lqNum = 8, bool lqButhq = false, bool MultiThread = true)
		{
			if (IsLowQuality == false)lqNum = 1;
			iWidth /= lqNum; iHeight /= lqNum;
			static HBITMAP hBitMap;
			DeleteObject(hBitMap);
			HDC hMemDC = CreateCompatibleDC(hDC);
			hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
			HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
			if (IsLowQuality == true) {
				if (lqButhq == true)
				{
					::SetStretchBltMode(hMemDC, HALFTONE);
					::SetBrushOrgEx(hMemDC, 0, 0, NULL);
				}StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
			}
			else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
			int size = iWidth * iHeight * 4;
			clock_t time = clock();
			unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

			int nBytes = GetBitmapBits(hBitMap, size, pBits);


			unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
			CGaussBlurFilter<double> _filter;
			_filter.SetSigma(s); // 设置高斯半径
			if (MultiThread != false)_filter.SetMultiThreads(true, 4);
			_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
			SetBitmapBits(hBitMap, size, nBits);


			clock_t etime = clock();
			int ntime = etime - time;
			//SetBitmapBits(hBitMap, size, pBits);

			wchar_t timec[20];
			_itow(ntime, timec, 10);
			OutputDebugString(L"Blur Time:");
			OutputDebugString(timec);
			OutputDebugString(L"\n");
			//SelectObject(hdcMem, hOld);
			SelectObject(hMemDC, hPreBmp);
			DeleteObject(hMemDC);

			_SAFE_DELETE(pBits);
			_SAFE_DELETE(nBits);
			//delete[]pBits;
			return hBitMap;
		}
		//防止Bmp冲突（（
		HBITMAP _GaussianBlurFunc2(HDC hDC, int x, int y, int iWidth, int iHeight, int s = 3.5, bool IsLowQuality = false, int lqNum = 8, bool lqButhq = false, bool MultiThread = true)
		{
			if (IsLowQuality == false)lqNum = 1;
			iWidth /= lqNum; iHeight /= lqNum;
			static HBITMAP hBitMap;
			DeleteObject(hBitMap);
			HDC hMemDC = CreateCompatibleDC(hDC);
			hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
			HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
			if (IsLowQuality == true) {
				if (lqButhq == true)
				{
					::SetStretchBltMode(hMemDC, HALFTONE);
					::SetBrushOrgEx(hMemDC, 0, 0, NULL);
				}StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
			}
			else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
			int size = iWidth * iHeight * 4;
			clock_t time = clock();
			unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

			int nBytes = GetBitmapBits(hBitMap, size, pBits);


			unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
			CGaussBlurFilter<double> _filter;
			_filter.SetSigma(s); // 设置高斯半径
			if (MultiThread != false)_filter.SetMultiThreads(true, 4);
			_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
			SetBitmapBits(hBitMap, size, nBits);


			clock_t etime = clock();
			int ntime = etime - time;
			//SetBitmapBits(hBitMap, size, pBits);

			wchar_t timec[20];
			_itow(ntime, timec, 10);
			OutputDebugString(L"Blur Time:");
			OutputDebugString(timec);
			OutputDebugString(L"\n");
			//SelectObject(hdcMem, hOld);
			SelectObject(hMemDC, hPreBmp);
			DeleteObject(hMemDC);

			_SAFE_DELETE(pBits);
			_SAFE_DELETE(nBits);
			//delete[]pBits;
			return hBitMap;
		}
		void _GaussianBitsBlurFunc(UINT* pb, int x, int y, int iWidth, int iHeight, int s = 3.5, bool MultiThread = true)
		{

			int size = iWidth * iHeight * 4;
			clock_t time = clock();



			UINT* nBits = (UINT*)malloc(size);  //在堆上申请
			CGaussBlurFilter<double> _filter;
			_filter.SetSigma(s); // 设置高斯半径
			if (MultiThread != false)_filter.SetMultiThreads(true, 4);
			_filter.Filter(pb, nBits, iWidth, iHeight, 32);


			clock_t etime = clock();
			int ntime = etime - time;
			//SetBitmapBits(hBitMap, size, pBits);

			wchar_t timec[20];
			_itow(ntime, timec, 10);
			OutputDebugString(L"Blur Time:");
			OutputDebugString(timec);
			OutputDebugString(L"\n");
			//SelectObject(hdcMem, hOld);
			pb = nBits;
			_SAFE_DELETE(nBits);
			//delete[]pBits;
			return;
		}
		HBITMAP _GaussianBlurFunc_Old(HDC hDC, int x, int y, int iWidth, int iHeight, int s = 3.5, bool IsLowQuality = false, int lqNum = 8)
		{
			if (IsLowQuality == false)lqNum = 1;
			iWidth /= lqNum; iHeight /= lqNum;
			HBITMAP hBitMap;
			HDC hMemDC = CreateCompatibleDC(hDC);
			hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); // 创建与设备描述表兼容的位图
			HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap);
			if (IsLowQuality == true)StretchBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, iWidth * lqNum, iHeight * lqNum, SRCCOPY);
			else BitBlt(hMemDC, 0, 0, iWidth, iHeight, hDC, x, y, SRCCOPY);
			int size = iWidth * iHeight * 4;
			clock_t time = clock();
			unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请

			int nBytes = GetBitmapBits(hBitMap, size, pBits);


			unsigned char* nBits = (unsigned char*)malloc(size);  //在堆上申请
			CGaussBlurFilter<double> _filter;
			_filter.SetSigma(s); // 设置高斯半径
			_filter.SetMultiThreads(true, 4);
			_filter.Filter(pBits, nBits, iWidth, iHeight, 32);
			SetBitmapBits(hBitMap, size, nBits);


			clock_t etime = clock();
			int ntime = etime - time;
			//SetBitmapBits(hBitMap, size, pBits);

			wchar_t timec[20];
			_itow(ntime, timec, 10);
			OutputDebugString(timec);
			//SelectObject(hdcMem, hOld);
			SelectObject(hMemDC, hPreBmp);
			DeleteObject(hMemDC);

			_SAFE_DELETE(pBits);
			_SAFE_DELETE(nBits);
			//delete[]pBits;
			return hBitMap;
		}
		unsigned char* freeTransform(unsigned char* in, int cx, int cy, POINTFLOAT* fourCorners, POINTFLOAT origin)
		{
			unsigned char* ret = new unsigned char[cx * cy * 4]; // new出来的内存是0，无需memset
			POINTFLOAT fourLines[4]{};
			for (size_t i = 0; i < 4; i++)
			{
				if (i == 3)
				{
					fourLines[i].x = fourCorners[0].x - fourCorners[i].x;
					fourLines[i].y = fourCorners[0].y - fourCorners[i].y;
				}
				else
				{
					fourLines[i].x = fourCorners[i + 1].x - fourCorners[i].x;
					fourLines[i].y = fourCorners[i + 1].y - fourCorners[i].y;
				}
				fourLines[i].x /= sqrtf(powf(fourLines[i].x, 2) + powf(fourLines[i].y, 2));
				fourLines[i].y /= sqrtf(powf(fourLines[i].x, 2) + powf(fourLines[i].y, 2));
			}
			double distances[4]{};
			double tmpx, tmpy;
			int a, b;
			for (size_t y = 0; y < cy; y++)
			{
				for (size_t x = 0; x < cx; x++)
				{
					for (size_t i = 0; i < 4; i++)
					{
						distances[i] = abs(((origin.x - fourCorners[i].x) * fourLines[i].y) - ((origin.y - fourCorners[i].y) * fourLines[i].x));
					}
					tmpx = cx * distances[4] / (distances[4] + distances[2]);
					tmpy = cy * distances[1] / (distances[1] + distances[3]);
					if (tmpx >= 0 && tmpx < cx && tmpy >= 0 && tmpy < cy) // 在图片范围内
					{
						a = (y * cx + x) * 4;
						b = (tmpy * cx + tmpx) * 4;
						ret[a] = in[b];
						ret[a + 1] = in[b + 1];
						ret[a + 2] = in[b + 2];
						ret[a + 3] = in[b + 3];
					}
				}
			}
			return ret;
		}

		HBITMAP CalcTransformBmp(HDC hDC, int x, int y, int iWidth, int iHeight, POINTFLOAT* To, POINTFLOAT From)
		{
			static HBITMAP hBitMap; DeleteObject(hBitMap); HDC hMemDC = CreateCompatibleDC(hDC); hBitMap = CreateCompatibleBitmap(hDC, iWidth, iHeight); HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, hBitMap); BitBlt(hMemDC, 0, 0, iWidth, iHeight, GlobalSharedDC, x, y, SRCCOPY);

			int size = iWidth * iHeight * 4;
			clock_t time = clock();
			unsigned char* pBits = (unsigned char*)malloc(size);  //在堆上申请
			int nBytes = GetBitmapBits(hBitMap, size, pBits);


			unsigned char* nBits;
			nBits = freeTransform(pBits, iWidth, iHeight, To, From);
			SetBitmapBits(hBitMap, size, nBits);

			clock_t etime = clock(); int ntime = etime - time; wchar_t timec[20]; _itow(ntime, timec, 10); OutputDebugString(L"Transform Time:"); OutputDebugString(timec); OutputDebugString(L"\n");
			DeleteObject(hMemDC); _SAFE_DELETE(pBits); _SAFE_DELETE(nBits);
			return hBitMap;
		}
	}
	void TestTransform(HDC hdc, VertexUIPos vp)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		POINTFLOAT p[4];
		p[0] = {10,0 };
		p[1] = { 400,0 };
		p[2] = {400,400};
		p[3] = { 0,400 };

		FinalBlur = _FullDesign::CalcTransformBmp(hdc, 0, 0, 600, 600, p, { 80,80 });
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		BitBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC, 0, 0,  SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void AreaBlur(HDC hdc, VertexUIPos vp, int  a, int b, int th)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _FullDesign::_GaussianBlurFunc2(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC, 0, 0, vp.cx / b, vp.cy / b, SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void AreaBlur3(HDC hdc, VertexUIPos vp, double a, int b, int th)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _FullDesign::_GaussianBlurFunc(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC, 0, 0, vp.cx / b, vp.cy / b, SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void AreaBlurBmp(HDC hdc, HBITMAP& Back, VertexUIPos vp, int a, int b, int th)
	{
		Back = _FullDesign::_GaussianBlurFunc2(hdc, vp.x, vp.y, vp.cx, vp.cy, a, true, b, true, th);
	}
	void AreaBlur2(HDC hdc, HDC oDC, VertexUIPos vp, int a, int b, int th, int Exp)
	{

		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		FinalBlur = _FullDesign::_GaussianBlurFunc2(oDC, vp.x, vp.y, vp.cx * Exp, vp.cy * Exp, a, true, b + Exp, true, th);
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, vp.x * Exp, vp.y * Exp, vp.cx * Exp, vp.cy * Exp, hMemDC, 0, 0, vp.cx / (b + Exp), vp.cy / (b + Exp), SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
		DeleteObject(FinalBlur);
	}
	void ResumeDCFromBitmap(HDC hdc, VertexUIPos vp, HBITMAP prebmp, int xExt = 0, int yExt = 0)
	{
		HDC hMemDC1 = CreateCompatibleDC(hdc);
		HBITMAP hPreBmp1 = (HBITMAP)SelectObject(hMemDC1, prebmp);
		BitBlt(hdc, vp.x, vp.y, vp.cx, vp.cy, hMemDC1, xExt, yExt, SRCCOPY);
		SelectObject(hMemDC1, prebmp);
		DeleteDC(hMemDC1);
	}



#endif
	void _VUIListPanel(HWND hWnd, HDC hdc, VERTEXUILISTPANEL vlp)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		int tsz = 40;
		//CreateFillArea(hWnd, hdc, vlp.PanelBkColor);
#ifdef FullDesign
		static HBITMAP FinalBlur;
		static int  ocx, ocy;
		/*高斯模糊处理部分*/
		HDC hMemDC = CreateCompatibleDC(hdc);
		if (ListPanelBlured == false) { HDC nh = GetDC(hWnd); FinalBlur = _FullDesign::_GaussianBlurFunc(nh, 0, 0, RCWID * 8, 80 * 8, 6, true, 8, true); ListPanelBlured = true; ocx = rc.right; ocy = rc.bottom; ReleaseDC(hWnd, nh); }
		HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, NULL);
		StretchBlt(hdc, 0, 0, RCWID, rc.bottom - rc.top, hMemDC, 0, 0, ocx / 8, ocy / 8, SRCCOPY);
		SelectObject(hMemDC, hPreBmp);
		DeleteDC(hMemDC);
#endif
		CreateAlphaMask(hdc, 0, 40, RCWID, (rc.bottom - rc.top), 200, vlp.PanelBkColor);
		for (int i = 1; i <= 128; i++)
		{
			if (vlp.listtxt[i] == L"")
			{
				break;
			}
			CreateAlphaMask(hdc, 60, i * 60 + tsz + 20, (rc.right) - 120, 40, 150, VuiCalcShadow(VERTEXUICOLOR_MIDNIGHT));
			AreaBlur(hdc, { 60 - 15, i * 60 + tsz + 20 - 15, (rc.right) - 120 + 30, 40 + 30 }, 3, 2, 0);
			CreateAlphaMask(hdc, 60, i * 60 + tsz + 20, (rc.right) - 120, 40, 200, VuiMediumChangeToLight(VERTEXUICOLOR_MIDNIGHTPLUS, 40));
			TextPreDrawEx(hdc, 70, i * 60 + tsz + 5 + 20, 512, 24, vlp.listtxt[i], 20, 0, VERTEXUICOLOR_WHITE);
		}
		//CreateAlphaMask(hdc, 1, tsz, rc.right - rc.left, 40, 150,RGB(SetColorBright(vuicolor.bg_r,-12), SetColorBright(vuicolor.bg_g, -12), SetColorBright(vuicolor.bg_b, -12)));
		CreateRect(0, hdc, 58, tsz + 20, 114, 35, VuiCalcShadow(VERTEXUICOLOR_MIDNIGHT));
		AreaBlur(hdc, { 57 - 15, tsz + 10, 142, 56 }, 3, 2, 0);
		vcdb.RoundButton(hWnd, hdc, { 57,tsz + 10,120,50 }, 18, L"", { VuiDarkChangeToMedium(RGB(SetColorBright(vuicolor.bg_r, -12), SetColorBright(vuicolor.bg_g, -12), SetColorBright(vuicolor.bg_b, -12)),30) });

		TextPreDrawEx(hdc, 61, tsz + 11, 40, 40, L"﹤", 35, 1, VERTEXUICOLOR_BLOOMLAVENDER);
		TextPreDrawEx(hdc, 80, tsz + 4 + 10, 80, 40, L"返回", 17, 1, VERTEXUICOLOR_BLOOMLAVENDER);
		//CreateSimpleButtonEx(hWnd, hdc, 1, tsz, 40, 40, VERTEXUICOLOR_GREENSEA, L"<", 30);
	}
	tagVertexUIList VertexUIList;
	void _CreateListPanel(HWND h, HDC hdc, VERTEXUILISTPANEL vlp, DRAWPANEL exDraw = 0)
	{

		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.

		_VUIListPanel(h, hMemDC, vlp);
		if (exDraw != 0)
		{
			exDraw(h, hMemDC);
		}
		BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);
	}
	int _CreateListPanelAnimation(HWND h, HDC hdc, VERTEXUILISTPANEL vlp, DRAWPANEL exDraw = 0)
	{
		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		//InvalidateRect(h, &rc, 1);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.
		_VUIListPanel(h, hMemDC, vlp);
		if (exDraw != 0)
		{
			exDraw(h, hMemDC);
		}

		if (anistat == 1)
		{
			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			anistat = 0;
			return -1;
		}
		if (anistat == 0)
		{
			VUIBlendInAnimation(hdc, rc, hMemDC);
			anistat = 1;
			return -1;
		}

		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);

		return 0;
	}

	void CreateListPanel(HWND h, VERTEXUILISTPANEL vlp)
	{
		ListPanelBlured = false;
		VertexUIList = {};
		VertexUIList = vlp;
		SwitchPanel(ListPanel);
	}
	void CreateListPanelByFlag(HWND h, HDC hdc, VERTEXUILISTPANEL vlp, DRAWPANEL DrawFun, int Flag = 0)
	{

		//On hMemDC.
		if (PanelID == PrevPanelID)
		{
			_CreateListPanel(h, hdc, vlp, DrawFun);
			anistat = 0;
		}
		if (PanelID != PrevPanelID)
		{
			if (Flag == 0)
			{
				_CreateListPanelAnimation(h, hdc, vlp, DrawFun);
			}
			if (Flag == 1)
			{
				_CreateListPanel(h, hdc, vlp, DrawFun);
			}
			PrevPanelID = PanelID;
			anistat = 0;
		}
	}
#define ListPanelEvent \
if (PanelID == ListPanel)\
{\
	RECT rc;\
	for (int i = 1; i <= 128; i++)\
	{\
\
		int tsz = 40;\
		if (VertexUIList.listtxt[i] == L"")\
		{\
			break;\
		}\
		AddBoxClickArea_Animation(hWnd, lParam, 60, i * 60 + tsz+20, (winrc.right)-120, 40, VertexUIList.listclick[i])\
\
	}\
AddBoxClickAreaDDraw(hWnd,lParam,58,40+10,120,40,GoPagePrev,[hWnd]{HDC hdc=GetDC(hWnd);vcdb.LimpidCtl(hWnd, hdc, {57,40+10,120,50},18,{50,4,1},L"",{VERTEXUICOLOR_WHITE});TextPreDrawEx(hdc, 61, 40 + 11, 40, 40, L"﹤", 35, 1, VERTEXUICOLOR_WHITE);TextPreDrawEx(hdc, 80, 40 + 4 + 10, 80, 40, L"返回", 17, 1, VERTEXUICOLOR_WHITE); ReleaseDC(hWnd,hdc);})\
\
}\

	void VUIStdBanner(HWND hWnd, HDC hdc)
	{
		CreateFillArea(hWnd, hdc, VERTEXUICOLOR_LDARK);
	}
	typedef struct VERTEXUIVIEWPANEL
	{
		typedef int RUNFUN;
		std::vector<const wchar_t*> viewtxt = { 128,(L"") };
		std::vector<int(*)()> viewclick = { 128,(in_test) };
		std::vector<DRAWPANEL*>viewimg{ 128,(VUIStdBanner) };
		COLORREF PanelBkColor = VERTEXUICOLOR_DARKNIGHT;

	}tagVertexUIView;

	typedef void (DRAWVIEWPANEL)(HWND, HDC, VERTEXUIVIEWPANEL);
	void _VUIViewPanel(HWND hWnd, HDC hdc, VERTEXUIVIEWPANEL vlp)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		int tsz = 40;
		CreateFillArea(hWnd, hdc, vlp.PanelBkColor);
		for (int i = 1, line = 0, x = 0; i <= 128; i++)
		{
			int ToLeft = (rc.right - rc.left) / 2 - (740 / 2);
			if (vlp.viewtxt[i] == L"")
			{
				break;
			}
			if ((i - 1) % 3 == 0)
			{
				if (i != 0 && i != 1 && i != 2 && i != 3)line++;
				x = 0;
			}
			else
			{
				x += 1;
			}
			CreateRect(hWnd, hdc, ToLeft + x * 250, 60 + tsz + line * 150, 240, 140, VERTEXUICOLOR_SHADOWNIGHT);
			CreateAreaPanel(hWnd, hdc, vlp.viewimg[i], ToLeft + x * 250, 60 + tsz + line * 150, 240, 110);
			TextPreDrawEx(hdc, ToLeft + x * 250 + 10, 60 + tsz + 110 + line * 150, 240, 24, vlp.viewtxt[i], 20, 0, VERTEXUICOLOR_WHITEDRAW);
		}
		CreateRect(hWnd, hdc, 1, tsz, rc.right - rc.left, 40, VuiMediumChangeToLight(VERTEXUICOLOR_MIDNIGHTPLUS, 40));
		CreateSimpleButtonEx(hWnd, hdc, 1, tsz, 40, 40, VuiMediumChangeToLight(VERTEXUICOLOR_MIDNIGHTPLUS, 50), L"<", 30);
	}
	tagVertexUIView VertexUIView;
	void _CreateViewPanel(HWND h, HDC hdc, VERTEXUIVIEWPANEL vlp, DRAWPANEL exDraw = 0)
	{
		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.

		_VUIViewPanel(h, hMemDC, vlp);
		if (exDraw != 0)
		{
			exDraw(h, hMemDC);
		}
		BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);


		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);
	}
	int _CreateViewPanelAnimation(HWND h, HDC hdc, VERTEXUIVIEWPANEL vlp, DRAWPANEL exDraw = 0)
	{
		HDC         hMemDC;
		HBITMAP     hBmpMem;
		HBITMAP     hPreBmp;
		RECT rc;
		GetClientRect(h, &rc);
		//InvalidateRect(h, &rc, 1);
		hMemDC = CreateCompatibleDC(hdc);

		hBmpMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

		hPreBmp = (HBITMAP)SelectObject(hMemDC, hBmpMem);
		//On hMemDC.
		_VUIViewPanel(h, hMemDC, vlp);
		if (exDraw != 0)
		{
			exDraw(h, hMemDC);
		}

		if (anistat == 1)
		{
			BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY);
			anistat = 0;
			return -1;
		}
		if (anistat == 0)
		{
			VUIBlendInAnimation(hdc, rc, hMemDC);
			anistat = 1;
			return -1;
		}

		SelectObject(hMemDC, hPreBmp);


		DeleteObject(hBmpMem);

		DeleteDC(hMemDC);

		return 0;
	}

	void CreateViewPanel(HWND h, VERTEXUIVIEWPANEL vlp)
	{
		VertexUIView = {};
		VertexUIView = vlp;
		SwitchPanel(ViewPanel);
	}
	void CreateViewPanelByFlag(HWND h, HDC hdc, VERTEXUIVIEWPANEL vlp, DRAWPANEL DrawFun, int Flag = 0)
	{

		//On hMemDC.
		if (PanelID == PrevPanelID)
		{
			_CreateViewPanel(h, hdc, vlp, DrawFun);
			anistat = 0;
		}
		if (PanelID != PrevPanelID)
		{
			if (Flag == 0)
			{
				_CreateViewPanelAnimation(h, hdc, vlp, DrawFun);
			}
			if (Flag == 1)
			{
				_CreateViewPanel(h, hdc, vlp, DrawFun);
			}
			PrevPanelID = PanelID;
			anistat = 0;
		}
	}
#define ViewPanelEvent \
if (PanelID == ViewPanel)\
{\
	RECT rc = {0}; RECT winrc;GetClientRect(hWnd,&winrc);\
			for (int i = 1,line = 0,x=0; i <= 128; i++)\
			{\
int tsz=40;\
				int ToLeft = (winrc.right - winrc.left) / 2 - (740 / 2);\
				if (VertexUIView.viewtxt[i] == L"")\
				{\
					break;\
				}\
				if ((i - 1) % 3 == 0)\
				{\
					if (i != 0 && i != 1 && i != 2 && i != 3)line++;\
					x = 0;\
				}\
				else\
				{\
					x += 1;\
				}\
				AddBoxClickArea(hWnd, lParam, ToLeft+x*250, 60 + tsz + line*150, 240, 140, VertexUIView.viewclick[i])\
			}\
AddBoxClickArea(hWnd,lParam,1,40,40,40,GoPagePrev)\
\
}\



}

#ifdef VERTEXUI_DEVMODE
using namespace::VertexUI;
using namespace::VertexUI::Panel;

//Other


#endif

#ifdef test__
void TESTDrawLine(HDC pDC, int X0, int Y0, int X1, int Y1, COLORREF clrLine)
{
	HDC mDC = pDC;
	if (Y0 > Y1)
	{
		int Temp = Y0; Y0 = Y1; Y1 = Temp;
		Temp = X0; X0 = X1; X1 = Temp;
	}

	SetPixel(pDC, X0, Y0, clrLine);

	int XDir, DeltaX = X1 - X0;
	if (DeltaX >= 0)
	{
		XDir = 1;
	}
	else
	{
		XDir = -1;
		DeltaX = 0 - DeltaX;
	}

	int DeltaY = Y1 - Y0;
	if (DeltaY == 0)
	{
		/* Horizontal line */
		while (DeltaX-- != 0)
		{
			X0 += XDir;
			SetPixel(pDC, X0, Y0, clrLine);
		}
		return;
	}
	if (DeltaX == 0)
	{
		/* Vertical line */
		do
		{
			Y0++;
			SetPixel(pDC, X0, Y0, clrLine);
		} while (--DeltaY != 0);
		return;
	}

	if (DeltaX == DeltaY)
	{
		/* Diagonal line */
		do
		{
			X0 += XDir;
			Y0++;
			SetPixel(pDC, X0, Y0, clrLine);
		} while (--DeltaY != 0);
		return;
	}

	unsigned short ErrorAdj;
	unsigned short ErrorAccTemp, Weighting;


	unsigned short ErrorAcc = 0;

	BYTE rl = GetRValue(clrLine);
	BYTE gl = GetGValue(clrLine);
	BYTE bl = GetBValue(clrLine);
	double grayl = rl * 0.299 + gl * 0.587 + bl * 0.114;

	if (DeltaY > DeltaX)
	{

		ErrorAdj = ((unsigned long)DeltaX << 16) / (unsigned long)DeltaY;
		while (--DeltaY) {
			ErrorAccTemp = ErrorAcc;
			ErrorAcc += ErrorAdj;
			if (ErrorAcc <= ErrorAccTemp) {

				X0 += XDir;
			}
			Y0++;

			Weighting = ErrorAcc >> 8;
			assert(Weighting < 256);
			assert((Weighting ^ 255) < 256);

			COLORREF clrBackGround = ::GetPixel(mDC, X0, Y0);
			BYTE rb = GetRValue(clrBackGround);
			BYTE gb = GetGValue(clrBackGround);
			BYTE bb = GetBValue(clrBackGround);
			double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

			BYTE rr = (rb > rl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
			BYTE gr = (gb > gl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
			BYTE br = (bb > bl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (bl - bb) + bb)));
			SetPixel(pDC, X0, Y0, RGB(rr, gr, br));

			clrBackGround = ::GetPixel(mDC, X0 + XDir, Y0);
			rb = GetRValue(clrBackGround);
			gb = GetGValue(clrBackGround);
			bb = GetBValue(clrBackGround);
			grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

			rr = (rb > rl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (rl - rb) + rb)));
			gr = (gb > gl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (gl - gb) + gb)));
			br = (bb > bl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (bl - bb) + bb)));
			SetPixel(pDC, X0 + XDir, Y0, RGB(rr, gr, br));
		}

		SetPixel(pDC, X1, Y1, clrLine);
		return;
	}

	ErrorAdj = ((unsigned long)DeltaY << 16) / (unsigned long)DeltaX;

	while (--DeltaX) {
		ErrorAccTemp = ErrorAcc;
		ErrorAcc += ErrorAdj;
		if (ErrorAcc <= ErrorAccTemp) {

			Y0++;
		}
		X0 += XDir;

		Weighting = ErrorAcc >> 8;
		assert(Weighting < 256);
		assert((Weighting ^ 255) < 256);

		COLORREF clrBackGround = ::GetPixel(mDC, X0, Y0);
		BYTE rb = GetRValue(clrBackGround);
		BYTE gb = GetGValue(clrBackGround);
		BYTE bb = GetBValue(clrBackGround);
		double grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

		BYTE rr = (rb > rl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (rl - rb) + rb)));
		BYTE gr = (gb > gl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (gl - gb) + gb)));
		BYTE br = (bb > bl ? ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl < grayb ? Weighting : (Weighting ^ 255))) / 255.0 * (bl - bb) + bb)));

		SetPixel(pDC, X0, Y0, RGB(rr, gr, br));

		clrBackGround = ::GetPixel(mDC, X0, Y0 + 1);
		rb = GetRValue(clrBackGround);
		gb = GetGValue(clrBackGround);
		bb = GetBValue(clrBackGround);
		grayb = rb * 0.299 + gb * 0.587 + bb * 0.114;

		rr = (rb > rl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (rb - rl) + rl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (rl - rb) + rb)));
		gr = (gb > gl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (gb - gl) + gl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (gl - gb) + gb)));
		br = (bb > bl ? ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (bb - bl) + bl)) : ((BYTE)(((double)(grayl < grayb ? (Weighting ^ 255) : Weighting)) / 255.0 * (bl - bb) + bb)));

		SetPixel(pDC, X0, Y0 + 1, RGB(rr, gr, br));
	}

	SetPixel(pDC, X1, Y1, clrLine);
}
#endif
