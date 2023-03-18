#pragma once
#include "VertexUI/VertexUI.min.h"
#include "LightFrame.Window.h"
#include "VertexUI/vui.parser/vui.parser.hpp"
#include "Vui.Foundation/Encoding.h"
#include "LightFrame.Data.h"
#include <istream>
#include <string>
namespace Elements
{
	int BoxEdgeValueCvt(VertexUIPos vp, int val, int wh, bool Absolute = false)/*0:宽度 1:高度*/
	{
		int n = val;
		if (val < 0)
		{
			if (wh == 0)n = vp.cx + val + 1;
			if (wh == 1)n = vp.cy + val + 1;
		}
		return n;
	}
	int ClientEdgeValueCvt(HWND h, int val, int wh)/*0:宽度 1:高度*/
	{
		RECT rc;
		GetClientRect(h, &rc);
		int n = BoxEdgeValueCvt({0,0,rc.right,rc.bottom},val,wh);
		return n;
	}
	int ClientEdgeValueCvt(HWND hWnd,VertexUIPos vp, int val, int wh, bool UseSize = false)/*0:宽度 1:高度*/
	{
		int n;
		if(UseSize==false)n = ClientEdgeValueCvt(hWnd,  val, wh);
		if (UseSize == true)n= BoxEdgeValueCvt(vp, val, wh);
		return n;
	}
	const wchar_t* ReadVui(const wchar_t* file)
	{
		std::string nfile;
		std::ifstream is;
		is.open(file, std::ios::in);
		std::string str; while (std::getline(is, str))
		{

			//状态表达:
			//0:当前正常读取
			//1:匹配到Name相同的部分,添加缓存并收尾后设置为0 QwQ
			//修改status的状态标志
			nfile.append(str);
			nfile.push_back('\n');
		}
		is.close();
		const wchar_t* n = vui::Encoding::Utf8().GetString(nfile.c_str());
		return n;
	}
	void TwinkleDoParse(const wchar_t*& file, const wchar_t*& i, int IsArsi, const wchar_t* Reg, const wchar_t* data, const wchar_t* _obj)
	{
		if (wcsstr(file, L"%20"))SetTagWW(file, L"%20", L" ");
		if (wcsstr(file, L"%Local"))SetTagWW(file, L"%Local", LocalCom);
		std::wstring LastData;
		const wchar_t* Info = L"vui!NULL";
		if (IsArsi == 0)
		{
			if (Reg != std::wstring(L"vui!NULL") || Reg != std::wstring(L""))
			{
				auto Obj_ = vui::parser::wfparser(std::wstring(file), std::wstring(Reg));
				Obj_.get(std::wstring(data), LastData, std::wstring(_obj));
			}
			else {
				auto Obj_ = vui::parser::wfparser(std::wstring(file));
				Obj_.get(std::wstring(data), LastData, std::wstring(_obj));
			}
			Info = LastData.c_str();
		}
		if (IsArsi == 1)
		{
			if (Reg != std::wstring(L"vui!NULL") || Reg != std::wstring(L""))
			{
				std::string qwq;
				std::string qaq;
				std::string arsifile;
				std::string arsiRegion;
				Wchar_tToString(arsifile, file);
				Wchar_tToString(arsiRegion, Reg);
				auto Obj_ = vui::parser::fparser(arsifile.c_str(), arsiRegion.c_str());

				Wchar_tToString(qwq, data);
				Wchar_tToString(qaq, _obj);
				Info = VUIGetObjectEx(Obj_, qwq.c_str(), qaq.c_str());
			}
			else {
				std::string arsifile;
				Wchar_tToString(arsifile, file);
				auto Obj_ = vui::parser::fparser(arsifile);
				std::string qwq;
				std::string qaq;
				Wchar_tToString(qwq, data);
				Wchar_tToString(qaq, _obj);
				Info = VUIGetObjectEx(Obj_, qwq.c_str(), qaq);
			}
		}
		if (wcsstr(Info, L"%20"))SetTagWW(Info, L"%20", L" ");
		i = Info;
	}
	template <class pT>
	void TwinkleGetIntFromObject(vui::parser::basic_value_pair<wchar_t> pair,pT& OriObj,int &n)
	{
		
		int num;
		std::wstring s;
		if (!pair.get(s))
		{
			pair.get(n);
		}
		else
		{
			wchar_t first[16];
			wchar_t second[16];
			swscanf(s.c_str(), L"%[^.].%s", &first, &second);
			for (auto obj : OriObj)
			{
				if (obj.name() == std::wstring(first))
				{
					for (auto pa : obj)
					{
						if (pa.name()==std::wstring(second))pa.get(num);
					}
				}
			}
		//	MonitorValue(num);
			n=num;
		}
		
	}
	template <class pT>
	void TwinkleGetPos(vui::parser::basic_value_pair<wchar_t> pair,pT& OriObj, int& x, int& y, int& cx, int& cy,const wchar_t* adjust=L"Left")
	{
		/*
		auto w = vui::parser::wparser(L"");
		for (auto obj : w)
		{
			for(auto pair:obj)
			{ }
		}
		*/
		/*
		if (pair.name() == std::wstring(L"x"))TwinkleGetIntFromObject(parser, pair, x);
		if (pair.name() == std::wstring(L"y"))TwinkleGetIntFromObject(parser, pair, y);
		if (pair.name() == std::wstring(L"cx"))TwinkleGetIntFromObject(parser, pair, cx);
		if (pair.name() == std::wstring(L"cy"))TwinkleGetIntFromObject(parser, pair, cy);
		*/
		/*
		Adjust属性下的负数坐标方案:
        Left(默认):从Box坐标边界(右/下)向左/上计算
        Center:负数为左 正数为右
        Right:从Box坐标起点(左/上)向右/下计算
		*/
		if (pair.name() == std::wstring(L"x"))TwinkleGetIntFromObject(pair,OriObj,x);
		if (pair.name() == std::wstring(L"y"))TwinkleGetIntFromObject(pair,OriObj,  y);
		if (pair.name() == std::wstring(L"cx"))TwinkleGetIntFromObject(pair,OriObj, cx);
		if (pair.name() == std::wstring(L"cy"))TwinkleGetIntFromObject(pair,OriObj, cy);

	}
	void TwinkleGetPos(vui::parser::basic_value_pair<wchar_t> pair,int& x, int& y, int& cx, int& cy)
	{
		/*
		auto w = vui::parser::wparser(L"");
		for (auto obj : w)
		{
			for(auto pair:obj)
			{ }
		}
		*/
		/*
		if (pair.name() == std::wstring(L"x"))TwinkleGetIntFromObject(parser, pair, x);
		if (pair.name() == std::wstring(L"y"))TwinkleGetIntFromObject(parser, pair, y);
		if (pair.name() == std::wstring(L"cx"))TwinkleGetIntFromObject(parser, pair, cx);
		if (pair.name() == std::wstring(L"cy"))TwinkleGetIntFromObject(parser, pair, cy);
		*/
		if (pair.name() == std::wstring(L"x"))pair.get(x) ;
		if (pair.name() == std::wstring(L"y"))pair.get(y);
		if (pair.name() == std::wstring(L"cx"))pair.get(cx);
		if (pair.name() == std::wstring(L"cy"))pair.get(cy);
	}
	/*
	* 解析模式:
	*/
	/*
		for (auto obj : parser)
	{
	}
	*/

	template<typename pT>
	void VertexUITwinklePreDrawTest(HWND hWnd, HDC hdc, pT& parser, const wchar_t* ExValue = 0, VertexUIPos vPos = {0,0,0,0},bool UseSize=false)
	{
		parser.parse();
		for (auto obj : parser)
		{
			if (obj.name() == std::wstring(L"Rect"))
			{
				int x, y, cx, cy;
				std::wstring col;
				std::wstring adjust=L"Left";
				std::wstring blur;
				std::wstring grad;
				int _LiD;
				int radius = 0;
				const wchar_t* color;
				const wchar_t* gradcolor;
				const wchar_t* blurchar;
				int Mode = 0;//0:纯色 1:渐变 2:模糊 3.壁纸半透明
				for (auto pair : obj)
				{
					if (pair.name() == std::wstring(L"Color")) { Mode = 0; pair.get(col); }
					if (pair.name() == std::wstring(L"Radius")) { if (!pair.get(radius))radius = 0; }
					if (pair.name() == std::wstring(L"Gradient")) { Mode = 1; pair.get(grad); }
					if (pair.name() == std::wstring(L"Blur")) { Mode = 2; pair.get(blur); }
					if (pair.name() == std::wstring(L"LimpidDesign")) { Mode = 3; pair.get(_LiD); }
					if (pair.name() == std::wstring(L"Align")) { pair.get(adjust); }
					TwinkleGetPos(pair, parser, x, y, cx, cy);




					//else MessageBox(hWnd, L"LightFrame.VertexUITwinkle.Parser | ERROR 0x1\n标识符未定义.请检查大小写和拼写问题,或参考文档.", pair.name().c_str(),MB_OK);
				}
				if (wcsstr(adjust.c_str(),L"CenterWidth"))
				{
					if (UseSize == true) {
						int pre = x;
						x = vPos.cx / 2 - cx / 2 + pre;
					}
					else
					{
						int pre = x;
						RECT rt;
						GetClientRect(hWnd, &rt);
						x = rt.right / 2 - cx / 2 + pre;
					}
				}
				if (wcsstr(adjust.c_str(),L"CenterHeight"))
				{
					if (UseSize == true) {
						int pre = y;
						y = vPos.cy / 2 - cy / 2 + pre;
					}
					else
					{
						int pre = y;
						RECT rt;
						GetClientRect(hWnd, &rt);
						y = rt.bottom / 2 - cy / 2 + pre;
					}
				}
				if (Mode == 0) {
					int r, g, b, a;
					color = col.c_str();
					if (!wcsncmp(color, L"RGBA", 4))swscanf(color, L"RGBA(%d,%d,%d,%d)", &r, &g, &b, &a);
					else { swscanf(color, L"RGB(%d,%d,%d)", &r, &g, &b); a = 0; }
					int Correct = RGB(r, g, b);
					if (a > 0)CreateAlphaMask(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), a, Correct);
					else if (radius <= 0)CreateRect(hWnd, hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), Correct);
					else vcdb.RoundButton(hWnd, hdc, { ClientEdgeValueCvt(hWnd, vPos, x, 0,UseSize) - 2, ClientEdgeValueCvt(hWnd, vPos, y, 1,UseSize) - 1, ClientEdgeValueCvt(hWnd, vPos, cx, 0,UseSize) + 6, ClientEdgeValueCvt(hWnd, vPos, cy, 1,UseSize) + 6 }, radius, L"", { (COLORREF)Correct });
				}
				if (Mode == 1) {
					int r, g, b, tr, tg, tb, d;
					gradcolor = grad.c_str();
					swscanf(gradcolor, L"%d;RGB(%d,%d,%d);RGB(%d,%d,%d)", &d, &r, &g, &b, &tr, &tg, &tb);
					int CorrectF = RGB(r, g, b);
					int CorrectT = RGB(tr, tg, tb);
					DrawLinearGradient(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), CorrectF, CorrectT, d);
				}
				if (Mode == 2) {
					int a, b, th;
					const wchar_t* blurchar = blur.c_str();
					swscanf(blurchar, L"Info(%d,%d,&d)", &a, &b, &th);

					static HBITMAP FinalBlur;
					static int  ocx, ocy;
					HDC hMemDC = CreateCompatibleDC(hdc);
					FinalBlur = _FullDesign::_GaussianBlurFunc2(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), a, true, b, true, th);
					HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
					::SetStretchBltMode(hdc, HALFTONE);
					::SetBrushOrgEx(hdc, 0, 0, NULL);
					StretchBlt(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), hMemDC, 0, 0, ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize) / b, ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize) / b, SRCCOPY);
					SelectObject(hMemDC, hPreBmp);
					DeleteDC(hMemDC);
					DeleteObject(FinalBlur);

				}
				if (Mode == 3) {
					//int scrx = GetSystemMetrics(SM_CXSCREEN); int scry = GetSystemMetrics(SM_CYSCREEN);
					RECT rc;
					GetWindowRect(hWnd, &rc);
					BitBlt(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), GlobalSharedDC, rc.left + ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), rc.top + ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), SRCCOPY);
					//MessageBox(0, 0,UseSize, 0,UseSize, 0,UseSize);
				}
			}
			if (obj.name() == std::wstring(L"Text"))
			{
				int x, y, cx, cy, sz; const wchar_t* txt = L"vui!NULL"; const wchar_t* font = L"Segoe UI";
				std::wstring ntxt;
				std::wstring nfont;
				std::wstring col;
				std::wstring time;
				for (auto pair : obj)
				{
					TwinkleGetPos(pair, x, y, cx, cy);
					if (pair.name() == std::wstring(L"Size")) { pair.get(sz); }
					if (pair.name() == std::wstring(L"Color")) { pair.get(col); }
					if (pair.name() == std::wstring(L"Font")) { if (!pair.get(nfont)) { font = L"Segoe UI"; } else font = nfont.c_str(); }
					if (pair.name() == std::wstring(L"Text")) {
						pair.get(ntxt);
						txt = ntxt.c_str();
					}
					if (pair.name() == std::wstring(L"Time")) {
						pair.get(time); const wchar_t* in = time.c_str();
						wchar_t ti[16];
						SYSTEMTIME time;
						GetLocalTime(&time);
						if (in == std::wstring(L"Hour"))wsprintf(ti, L"%02d", time.wHour);
						if (in == std::wstring(L"Minute"))wsprintf(ti, L"%02d", time.wMinute);
						if (in == std::wstring(L"Second"))wsprintf(ti, L"%02d", time.wSecond);
						txt = ti;
					}

				}
				if (wcsstr(font, L"%20"))SetTagWW(font, L"%20", L" ");
				if (wcsstr(txt, L"%20"))SetTagWW(txt, L"%20", L" ");
				if (wcsstr(txt, L"%10"))SetTagWW(txt, L"%10", L"\n");
				const wchar_t* color;
				int r, g, b = 20;
				color = col.c_str();
				swscanf(color, L"RGB(%d,%d,%d)", &r, &g, &b);
				int Correct = RGB(r, g, b);
				_TextPreDrawEx(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, y, 1, UseSize), ClientEdgeValueCvt(hWnd, vPos, cx, 0, UseSize), ClientEdgeValueCvt(hWnd, vPos, cy, 1, UseSize), txt, sz, font, 1, Correct);
			}
			if (obj.name() == std::wstring(L"Load"))
			{
				//Arsi意思是Ansi
				int x, y, cx, cy, IsArsi = 0; const wchar_t* file = L"vui!NULL"; const wchar_t* Reg = L""; const wchar_t* _obj = L"vui!NULL"; const wchar_t* data = L"vui!NULL"; const wchar_t* flag = L"vui!NULL";
				std::wstring nfile;
				std::wstring nobj;
				std::wstring nReg;
				std::wstring ndata;
				std::wstring nflag;
				std::wstring col;
				for (auto pair : obj)
				{
					TwinkleGetPos(pair, x, y, cx, cy);
					if (pair.name() == std::wstring(L"IsANSI"))pair.get(IsArsi);

					if (pair.name() == std::wstring(L"File")) { pair.get(nfile); file = nfile.c_str(); }
					if (pair.name() == std::wstring(L"Object")) { if (!pair.get(nobj)) { nobj = L"Object"; } else _obj = nobj.c_str(); }
					if (pair.name() == std::wstring(L"Region")) { if (pair.get(nReg)) { Reg = nReg.c_str(); } else Reg = L"vui!NULL"; }
					if (pair.name() == std::wstring(L"Get")) { pair.get(ndata); data = ndata.c_str(); }
					if (pair.name() == std::wstring(L"Flag")) { pair.get(nflag); flag = nflag.c_str(); }
				}
				const wchar_t* Info;
				TwinkleDoParse(file, Info, IsArsi, Reg, data, _obj);
				//Flag
				if (flag == std::wstring(L"MsgBox"))
				{
					MessageBox(hWnd, Info, Info, 0);
				}
				if (flag == std::wstring(L"DrawIcon"))
				{
					if (Info != std::wstring(L"NULL"))DisplayIcon(hdc, Info, x, y, cx);
				}
			}
		}
	}
	template<typename pT>
	void VertexUITwinklePreHitTest(HWND hWnd, pT& parser)
	{
		parser.parse();
		for (auto obj : parser)
		{
			if (obj.name() == std::wstring(L"Load"))
			{
				//Arsi意思是Ansi
				int x, y, cx, cy, IsArsi = 0; const wchar_t* file = L"vui!NULL"; const wchar_t* Reg = L""; const wchar_t* _obj = L"vui!NULL"; const wchar_t* data = L"vui!NULL"; const wchar_t* flag = L"vui!NULL";
				std::wstring nfile;
				std::wstring nobj;
				std::wstring nReg;
				std::wstring ndata;
				std::wstring nflag;
				std::wstring nexec;
				for (auto pair : obj)
				{
					TwinkleGetPos(pair, x, y, cx, cy);
					if (pair.name() == std::wstring(L"IsANSI"))pair.get(IsArsi);

					if (pair.name() == std::wstring(L"File")) { pair.get(nfile); file = nfile.c_str(); }
					if (pair.name() == std::wstring(L"Object")) { if (!pair.get(nobj)) { nobj = L"Object"; } else _obj = nobj.c_str(); }
					if (pair.name() == std::wstring(L"Region")) { if (pair.get(nReg)) { Reg = nReg.c_str(); } else Reg = L"vui!NULL"; }
					if (pair.name() == std::wstring(L"Get")) { pair.get(ndata); data = ndata.c_str(); }
					if (pair.name() == std::wstring(L"HitFlag")) { pair.get(nflag); flag = nflag.c_str(); }
				}
				const wchar_t* Info;
				TwinkleDoParse(file, Info, IsArsi, Reg, data, _obj);
				//Flag
				if (flag == std::wstring(L"Open"))
				{
					if (GetPtInfo(hWnd, x, y, cx, cy, RECT{ 0 }) == 1)
					{
						if (ClickMsg == 1)
						{
							ClickMsg = 0;
							if (wcsstr(Info, L".lnk"))
							{
								ShellExecute(0, L"open", L"explorer.exe", Info, 0, SW_SHOW);
							}
							else
							{
								ShellExecute(0, L"open", Info, 0, 0, SW_SHOW);
							}
						}
					}
				}

			}
		}
	}
	/*
	void VertexUITwinklePreDrawTestBACKUP__(HWND hWnd, HDC hdc, pT& obj)
	{
		int _max = 1000;
		//auto obj = vui::parser::wfparser(file);
		for (int i = 0; i <= _max; i++)
		{
			wchar_t ni[20]; _itow(i, ni, 1,true0);
			int x, y, cx, cy; wchar_t txt[512];
			if (obj.get(L"x", x, vui::parser::same_name_object<wchar_t>(L"Rect", ni)) != false)
			{
				obj.get(L"y", y, vui::parser::same_name_object<wchar_t>(L"Rect", ni));
				if (!obj.get(L"cx", cx, vui::parser::same_name_object<wchar_t>(L"Rect", ni)))
				{

				}
				obj.get(L"cy", cy, vui::parser::same_name_object<wchar_t>(L"Rect", ni));
				std::wstring col;
				std::wstring blur;
				std::wstring grad;
				const wchar_t* color;
				const wchar_t* gradcolor;
				if (obj.get(L"Gradient", grad, vui::parser::same_name_object<wchar_t>(L"Rect", ni)) != false) {
					int r, g, b, tr, tg, tb, d;
					gradcolor = grad.c_str();
					swscanf(gradcolor, L"%d;RGB(%d,%d,%d);RGB(%d,%d,%d)", &d, &r, &g, &b, &tr, &tg, &tb);
					int CorrectF = RGB(r, g, b);
					int CorrectT = RGB(tr, tg, tb);
					DrawLinearGradient(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0,true), ClientEdgeValueCvt(hWnd, vPos, y, 1,true), ClientEdgeValueCvt(hWnd, vPos, cx, 0,true), ClientEdgeValueCvt(hWnd, vPos, cy, 1,true), CorrectF, CorrectT, d);
				}
				if (obj.get(L"color", col, vui::parser::same_name_object<wchar_t>(L"Rect", ni)) != false) {
					int r, g, b;
					color = col.c_str();
					swscanf(color, L"RGB(%d,%d,%d)", &r, &g, &b);
					int Correct = RGB(r, g, b);
					CreateRect(hWnd, hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0,true), ClientEdgeValueCvt(hWnd, vPos, y, 1,true), ClientEdgeValueCvt(hWnd, vPos, cx, 0,true), ClientEdgeValueCvt(hWnd, vPos, cy, 1,true), Correct);
				}
				if (obj.get(L"Blur", blur, vui::parser::same_name_object<wchar_t>(L"Rect", ni)) != false) {
					int a, b, th;
					const wchar_t* blurchar = blur.c_str();
					swscanf(blurchar, L"Info(%d,%d,&d)", &a, &b, &th);

					static HBITMAP FinalBlur;
					static int  ocx, ocy;
					HDC hMemDC = CreateCompatibleDC(hdc);
					FinalBlur = _FullDesign::_GaussianBlurFunc2(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0,true), ClientEdgeValueCvt(hWnd, vPos, y, 1,true), ClientEdgeValueCvt(hWnd, vPos, cx, 0,true), ClientEdgeValueCvt(hWnd, vPos, cy, 1,true), a, true, b, true, th);
					HBITMAP hPreBmp = (HBITMAP)SelectObject(hMemDC, FinalBlur);
					::SetStretchBltMode(hdc, HALFTONE);
					::SetBrushOrgEx(hdc, 0,true, 0,true, NULL);
					StretchBlt(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0,true), ClientEdgeValueCvt(hWnd, vPos, y, 1,true), ClientEdgeValueCvt(hWnd, vPos, cx, 0,true), ClientEdgeValueCvt(hWnd, vPos, cy, 1,true), hMemDC, 0,true, 0,true, ClientEdgeValueCvt(hWnd, vPos, cx, 0,true) / b, ClientEdgeValueCvt(hWnd, vPos, cy, 1,true) / b, SRCCOPY);
					SelectObject(hMemDC, hPreBmp);
					DeleteDC(hMemDC);
					DeleteObject(FinalBlur);

				}
			}
			else {
				if (i != 0)break;
			}
		}
		for (int i = 0; i <= _max; i++)
		{
			wchar_t ni[20]; _itow(i, ni, 1,true0);
			int x, y, cx, cy, sz; wchar_t txt[512]; const wchar_t* font;
			if (obj.get(L"x", x, vui::parser::same_name_object<wchar_t>(L"Text", ni)) != false)
			{
				obj.get(L"y", y, vui::parser::same_name_object<wchar_t>(L"Text", ni));
				obj.get(L"cx", cx, vui::parser::same_name_object<wchar_t>(L"Text", ni));
				obj.get(L"cy", cy, vui::parser::same_name_object<wchar_t>(L"Text", ni));
				if (obj.get(L"size", sz, vui::parser::same_name_object<wchar_t>(L"Text", ni)) == false)sz = 18;
				std::wstring ntxt;
				std::wstring nfont;
				if (obj.get(L"font", nfont, vui::parser::same_name_object<wchar_t>(L"Text", ni)) == false)font = L"Segoe UI";
				else font = nfont.c_str();
				if (wcsstr(font, L"%20"))SetTagWW(font, L"%20", L" ");
				obj.get(L"text", ntxt, vui::parser::same_name_object<wchar_t>(L"Text", ni));
				wcscpy(txt, ntxt.c_str());
				if (wcsstr(txt, L"%20"))SetTagW(txt, L"%20", L" ");
				if (wcsstr(txt, L"%10"))SetTagW(txt, L"%10", L"\n");
				std::wstring col;
				const wchar_t* color;
				int r, g, b = 20;
				if (obj.get(L"color", col, vui::parser::same_name_object<wchar_t>(L"Text", ni)) != false) {
					color = col.c_str();
					swscanf(color, L"RGB(%d,%d,%d)", &r, &g, &b);
				}
				else
				{
					r = 20; g = 20; b = 20;
				}
				int Correct = RGB(r, g, b);
				_TextPreDrawEx(hdc, ClientEdgeValueCvt(hWnd, vPos, x, 0,true), ClientEdgeValueCvt(hWnd, vPos, y, 1,true), cx, cy, txt, sz, font, 1,true, Correct);
			}
			else {
				if (i != 0)break;
			}
		}
	}
	*/
	

	template<typename pT = vui::parser::wparser>
	void VertexUITwinkleDirectDraw(HWND hWnd, HDC hdc, const wchar_t* vui ,VertexUIPos vp = { 0,0,0,0 }, bool UseSize = false) noexcept
	{
		auto obj = vui::parser::wparser(vui);
		VertexUITwinklePreDrawTest(hWnd, hdc, obj,0,vp,UseSize);
		return;
	}
	void VertexUITwinkleDrawTest(HWND hWnd, HDC hdc, const wchar_t* file, VertexUIPos vp = {0,0,0,0},bool UseSize=false) noexcept
	{
		//MessageBox(0, (const wchar_t*)vui, 0,true, 0,true);
		const wchar_t* n = ReadVui(file);
		VertexUITwinkleDirectDraw(hWnd, hdc, n,vp,UseSize);
	}
	template<typename pT = vui::parser::wfparser>
	void VertexUITwinkleHitTest(HWND hWnd, const wchar_t* file) noexcept
	{
	    auto obj = vui::parser::wfparser(file);
		VertexUITwinklePreHitTest(hWnd, obj);
	}
}