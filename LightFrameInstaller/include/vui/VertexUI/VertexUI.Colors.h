﻿#pragma once
#include "framework.h"
 
class VertexUIColorStandard
{
public:
	//42 47 56
	//38 41 54
	//241 242 244
	class titlebar
	{
	public:
		//158 142 250
		int oR = 158;
		int oG = 142;
		int oB = 250;
		int bg_r = 158;
		int bg_g = 142;
		int bg_b = 250;
		int bg_d1_r, bg_d1_g, bg_d1_b = 10;
		int bg_d2_r, bg_d2_g, bg_d2_b = 30;
	};
	titlebar title;
	int bg_r = 38;
	int bg_g = 41;
	int bg_b = 54;
	int bg_d1_r, bg_d1_g, bg_d1_b = 10;
	int bg_d2_r, bg_d2_g, bg_d2_b = 30;

	int ctl_d1_r, ctl_d1_g, ctl_d1_b;
	int ctl_blur_r, ctl_blur_g, ctl_blur_b;

	int txt_r, txt_g, txt_b;
};
VertexUIColorStandard vuicolor;

int GetMaxValue(int num, int max)
{
	if (num >= max)return max;
	else return num;
}
int GetMinValue(int num, int min)
{
	if (num <= min)return min;
	else return num;
}

int SetColorBright(int c,int b,bool DarkModeNotChange=false)
{
	if (c > 128) { if (c - b >= 255)return 255; else return c - b; }
	else if (DarkModeNotChange == false)return c + b;
	else return c;
}

int SetThemedColorBright(int c, int b, int Themed,bool DarkModeNotChange = false)
{
	if (c > 128) { return Themed; }
	else if (DarkModeNotChange == false)return c + b;
	else return c;
}

int VuiBlackOrWhiteOpposite(int c)
{
	if (c > 128)return 20;
	else return 244;
}

int VuiBlackOrWhiteMedium(int c)
{
	if (c > 128)return 128;
	else return 15;
}
unsigned long VuiColorAverage(unsigned long Clr)
{
	return (GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3;
}
unsigned long VuiMediumChangeToLight(unsigned long Clr,int Val=30)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)return Clr;
	else return RGB(GetMaxValue(GetRValue(Clr)+Val,255), GetMaxValue(GetGValue(Clr)+Val, 255), GetMaxValue(GetBValue(Clr)+Val, 255));
}
unsigned long VuiDarkChangeToMedium(unsigned long Clr, int Val = 30)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) >= 128)return Clr;
	return RGB(GetMaxValue(GetRValue(Clr) + Val, 255), GetMaxValue(GetGValue(Clr) + Val, 255), GetMaxValue(GetBValue(Clr) + Val, 255));
}
unsigned long VuiCalcShadow(unsigned long Clr)
{
	if (((GetRValue(Clr) + GetGValue(Clr) + GetBValue(Clr)) / 3) <= 128)return RGB(25, 25, 27);
	else
		if (((GetRValue(Clr) < GetBValue(Clr)-8) && GetGValue(Clr) < GetBValue(Clr)-8))return RGB(GetRValue(Clr) - 40, GetGValue(Clr) - 40, GetBValue(Clr) - 20);
		else if (((GetGValue(Clr) < GetRValue(Clr) - 8) && GetBValue(Clr) < GetRValue(Clr) - 8))return RGB(GetRValue(Clr) - 20, GetGValue(Clr) - 40, GetBValue(Clr) - 40);
		else if (((GetRValue(Clr) < GetGValue(Clr) - 8) && GetBValue(Clr) < GetGValue(Clr) - 5))return RGB(GetRValue(Clr) - 40, GetGValue(Clr) - 20, GetBValue(Clr) - 40);
		else return RGB(190, 190, 194);
}
unsigned long VuiAddColorValue(unsigned long Clr, int r, int g, int b)
{
	int nR, nG, nB;
	nR = GetMaxValue(GetRValue(Clr)+r,255);
	nG = GetMaxValue(GetGValue(Clr)+g,255);
	nB = GetMaxValue(GetBValue(Clr)+b,255);
	return RGB(nR, nG, nB);
}

unsigned long VuiFadeColor(unsigned long Clr, int val)
{
	int nR, nG, nB;
	if (VuiColorAverage(Clr) > 128)val = -val;
	else val = val;
	nR = GetMaxValue(GetRValue(Clr) + val, 255);
	nG = GetMaxValue(GetGValue(Clr) + val, 255);
	nB = GetMaxValue(GetBValue(Clr) + val, 255);
	return RGB(nR, nG, nB);
}
int VuiAOrBOrCMedium(int c,int a,int b,int c1,bool DNC=false)
{
	if (c > 128)
	{
		if (c > 200)return c1;
		else return a;
	}
	else return b;
}
bool GetBitColor(BYTE* byte,  int x, int y, unsigned int& iRGB)
{ 
	int nPixelSize = 4;
	unsigned int* iBtRGB;

	iBtRGB = (unsigned int*)(byte + (y  * nPixelSize + x * nPixelSize));
	iRGB = RGB(GetBValue(iBtRGB[0]), GetGValue(iBtRGB[0]), GetRValue(iBtRGB[0]));
	//if (iBtRGB[0] == iRGB[0])
		return true;
	//else
		//return false;
}
#define VERTEXUICOLOR_THEME RGB(228,212,255)

void VuiColorSystemInit()
{
	vuicolor.bg_d1_r = SetColorBright(vuicolor.bg_r, 10);
	vuicolor.bg_d1_g = SetColorBright(vuicolor.bg_g, 10);
	vuicolor.bg_d1_b = SetColorBright(vuicolor.bg_b, 10);
	vuicolor.bg_d2_r = SetColorBright(vuicolor.bg_r, 30);
	vuicolor.bg_d2_g = SetColorBright(vuicolor.bg_g, 30);
	vuicolor.bg_d2_b = SetColorBright(vuicolor.bg_b, 30);

	if (vuicolor.bg_r <= 128)
	{
		vuicolor.title.bg_r = vuicolor.bg_r;
		vuicolor.title.bg_g = vuicolor.bg_g;
		vuicolor.title.bg_b = vuicolor.bg_b;
	}
	else
	{
		vuicolor.title.bg_r = vuicolor.title.oR;
		vuicolor.title.bg_g = vuicolor.title.oG;
		vuicolor.title.bg_b = vuicolor.title.oB;
	}
	vuicolor.title.bg_d1_r = SetColorBright(vuicolor.title.bg_r, 10);
	vuicolor.title.bg_d1_g = SetColorBright(vuicolor.title.bg_g, 10);
	vuicolor.title.bg_d1_b = SetColorBright(vuicolor.title.bg_b, 10);
	vuicolor.title.bg_d2_r = SetColorBright(vuicolor.title.bg_r, 30);
	vuicolor.title.bg_d2_g = SetColorBright(vuicolor.title.bg_g, 30);
	vuicolor.title.bg_d2_b = SetColorBright(vuicolor.title.bg_b, 30);


	vuicolor.ctl_d1_r = SetColorBright(vuicolor.bg_r, 35);
	vuicolor.ctl_d1_g = SetColorBright(vuicolor.bg_g, 35);
	vuicolor.ctl_d1_b = SetColorBright(vuicolor.bg_b, 35);
	vuicolor.ctl_blur_r = SetColorBright(vuicolor.bg_r,13);
	vuicolor.ctl_blur_g = SetColorBright(vuicolor.bg_g,13);
	vuicolor.ctl_blur_b = SetColorBright(vuicolor.bg_b,13);
	
	vuicolor.txt_r = VuiBlackOrWhiteOpposite(vuicolor.bg_r);
	vuicolor.txt_g = VuiBlackOrWhiteOpposite(vuicolor.bg_g);
	vuicolor.txt_b =VuiBlackOrWhiteOpposite(vuicolor.bg_b);
}

#define VERTEXUICOLOR_DARKEN RGB(20,20,20)

#define VERTEXUICOLOR_TABDARKEN RGB(57, 62, 71)

#define VERTEXUICOLOR_WHITEDRAW RGB(244,244,244)

#define VERTEXUICOLOR_WHITE RGB(vuicolor.txt_r,vuicolor.txt_g,vuicolor.txt_b)

#define VERTEXUICOLOR_SEA RGB(45,137,239)

#define VERTEXUICOLOR_DEEPSEA RGB(15,107,209)

#define VERTEXUICOLOR_LAVENDER RGB(108,92,200)

#define VERTEXUICOLOR_LAVENDER_ RGB(128,112,220)

#define VERTEXUICOLOR_BLOOMLAVENDER RGB(158,142,250)

#define VERTEXUICOLOR_GREENSEA RGB(26,188,156)

#define VERTEXUICOLOR_GREENDEEPSEA RGB(4,138,106)

#define VERTEXUICOLOR_REALLYGREENDEEPSEA RGB(68,102,103)

#define VERTEXUICOLOR_DARKENX RGB(10,10,10)

#define VERTEXUICOLOR_PEACHRED RGB(232,77,61)

#define VERTEXUICOLOR_MIDNIGHT RGB(vuicolor.bg_d1_r, vuicolor.bg_d1_g, vuicolor.bg_d1_b)

#define VERTEXUICOLOR_MIDNIGHTPLUS RGB(vuicolor.bg_d2_r, vuicolor.bg_d2_g, vuicolor.bg_d2_b)

#define VERTEXUICOLOR_DARKNIGHT RGB(vuicolor.bg_r, vuicolor.bg_g, vuicolor.bg_b)

#define VERTEXUICOLOR_FOREST RGB(38,212,110)

#define VERTEXUICOLOR_DAWN RGB(255,115,119)

#define VERTEXUICOLOR_LDARK RGB(42+35,47+35,56+35)

#define VERTEXUICOLOR_OTHER_GITHUB RGB(88,166,255)

#define VERTEXUI_DARKEN RGB(20,20,20)

#define VERTEXUI_WHITE RGB(244,244,244)

#define VERTEXUI_GREENSEA RGB(26,188,156)

#define VERTEXUICOLOR_SHADOWNIGHT RGB(57,62,71)


COLORREF VertexUIColor_Main;