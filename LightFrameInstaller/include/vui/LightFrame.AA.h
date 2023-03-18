#pragma once
#include "framework.h"
namespace AAIMG {
	HBITMAP m_hOldAABitmap;
	HBITMAP m_hAABitmap;
	HDC m_hAADC;
	HBITMAP m_hOldMemBitmap;
	HBITMAP m_hMemBitmap;
	HDC m_hMemDC;
}
typedef void (AAPANEL)(HWND, HDC, int);

void CreateAAImage(HWND h, HDC hAADC, int scale, AAPANEL Draw, int cx, int cy)
{

	//DC
	HDC hDC = GetDC(h);

	// Create temporary DC and bitmap
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hDC, scale * cx, scale * cy);
	HBITMAP hOldTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);

	// Release screen DC
	ReleaseDC(h, hDC);

	// Create drawing
	Draw(h, hTempDC, scale);

	/*	// Copy temporary DC to anti-aliazed DC
		startTime = GetTickCount();
		int oldStretchBltMode = ::SetStretchBltMode(hAADC, HALFTONE);
		::StretchBlt(hAADC, 0, 0, 300, 200, hTempDC, 0, 0, scale*300, scale*200, SRCCOPY);
		::SetStretchBltMode(hAADC, oldStretchBltMode);
		endTime = GetTickCount();
		m_dwScalingTime = endTime - startTime;*/

	/**/
		// Get source bits
	int srcWidth = scale * cx;
	int srcHeight = scale * cy;
	int srcPitch = srcWidth * 4;
	int srcSize = srcWidth * srcPitch;
	BYTE* lpSrcBits = (BYTE*)malloc(srcSize);
	//BYTE* lpSrcBits = new BYTE[srcSize];
	GetBitmapBits(hTempBitmap, srcSize, lpSrcBits);
	// Get destination bits
	int dstWidth = cx;
	int dstHeight = cy;
	int dstPitch = dstWidth * 4;
	int dstSize = dstWidth * dstPitch;
	BYTE* lpDstBits = (BYTE*)malloc(dstSize);
	//BYTE* lpDstBits = new BYTE[dstSize];
	HBITMAP hAABitmap = (HBITMAP)GetCurrentObject(hAADC, OBJ_BITMAP);
	GetBitmapBits(hAABitmap, dstSize, lpDstBits);

	int scaledPitch = scale * srcPitch;
	int scaledHeightXPitch = (srcHeight - 4) * dstPitch * scale;
	int scaledPixel = scale * 4;
	int scaledWidthXPixel = (dstWidth)*scaledPixel;
	int gridSize = scale * scale;
	int resultRed, resultGreen, resultBlue;
	int dstY = 0, dstOffset;
	int srcOffset;
	int tmpX, tmpY, tmpOffset;

	for (int srcY = scaledPitch; srcY < scaledHeightXPitch; srcY += scaledPitch)
	{
		int dstX = 0;
		for (int srcX = scaledPixel; srcX < scaledWidthXPixel; srcX += scaledPixel)
		{
			srcOffset = srcY + srcX;

			resultRed = resultGreen = resultBlue = 0;
			tmpY = -srcPitch;
			for (int i = 0; i < scale; i++)
			{
				tmpX = -4;
				for (int j = 0; j < scale; j++)
				{
					tmpOffset = tmpY + tmpX;

					resultRed += lpSrcBits[srcOffset + tmpOffset + 2];
					resultGreen += lpSrcBits[srcOffset + tmpOffset + 1];
					resultBlue += lpSrcBits[srcOffset + tmpOffset];

					tmpX += 4;
				}
				tmpY += srcPitch;
			}

			dstOffset = dstY + dstX;
			lpDstBits[dstOffset + 2] = (BYTE)(resultRed / gridSize);
			lpDstBits[dstOffset + 1] = (BYTE)(resultGreen / gridSize);
			lpDstBits[dstOffset] = (BYTE)(resultBlue / gridSize);
			dstX += 4;
		}

		dstY += dstPitch;
	}
	SetBitmapBits(hAABitmap, dstSize, lpDstBits);

	// Destroy source bits
	//delete[] lpSrcBits;
	free(lpSrcBits);
	// Destroy destination bits
	//delete[] lpDstBits;
	free(lpDstBits);


	// Destroy temporary DC and bitmap
	if (hTempDC)
	{
		::SelectObject(hTempDC, hOldTempBitmap);
		::DeleteDC(hTempDC);
		::DeleteObject(hTempBitmap);
	}
}
template<class draw>
void PreDrawAA(HWND h, draw aap, int cx, int cy,int scale=4)
{
	DeleteObject(AAIMG::m_hAABitmap);
	DeleteObject(AAIMG::m_hOldAABitmap);
	DeleteDC(AAIMG::m_hAADC);
	using namespace::AAIMG;
	HDC hDC = ::GetDC(h);
	RECT rc;
	GetClientRect(h, &rc);

	//m_hMemDC = ::CreateCompatibleDC(hDC);
	//m_hMemBitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
	//m_hOldMemBitmap = (HBITMAP)::SelectObject(m_hMemDC, m_hMemBitmap);

	m_hAADC = ::CreateCompatibleDC(hDC);
	m_hAABitmap = ::CreateCompatibleBitmap(hDC, cx, cy);
	m_hOldAABitmap = (HBITMAP)::SelectObject(m_hAADC, m_hAABitmap);

	::ReleaseDC(h, hDC);


	CreateAAImage(h, m_hAADC, scale, aap, cx, cy);
}
void CreateAA(HWND h, HDC hdc, int x, int y, int cx, int cy, AAPANEL Draw, int move = -5)
{
	int ncx = cx;
	if (cx < cy)ncx = cy;
	using namespace::AAIMG;
	PreDrawAA(h, Draw, ncx, cy);
	BitBlt(hdc, x, y, cx + move, cy + move, m_hAADC, 0, 0, SRCCOPY);
}
void CreateAALQ(HWND h, HDC hdc, int x, int y, int cx, int cy, AAPANEL Draw, int move = -5)
{
	int ncx = cx;
	if (cx < cy)ncx = cy;
	using namespace::AAIMG;
	PreDrawAA(h, Draw, ncx, cy,2);
	BitBlt(hdc, x, y, cx + move, cy + move, m_hAADC, 0, 0, SRCCOPY);
}
template<class draw>
void CreateAAXX(HWND h, HDC hdc, int x, int y, int cx, int cy, draw Draw, int move = -5)
{
	int ncx = cx;
	if (cx < cy)ncx = cy;
	using namespace::AAIMG;
	PreDrawAA(h, Draw, ncx, cy);
	BitBlt(hdc, x, y, cx + move, cy + move, m_hAADC, 0, 0, SRCCOPY);
}
void CreateAAX(HWND h, HDC hdc, int x, int y, int cx, int cy, AAPANEL Draw, int move = -5)
{
	int ncx = cx;
	if (cx < cy)ncx = cy;
	using namespace::AAIMG;
	PreDrawAA(h, Draw, ncx, cy);
	BitBlt(hdc, x, y, cx + move, cy + move, m_hAADC, 0, 0, SRCCOPY);
}

/*
void AverageBlt(HDC& dest,VertexUIPos vp, HDC& src, int scale)
{
	POINT p = { vp.x,vp.y };
	SIZE s = { vp.cx,vp.cy };
	HDC dst = dest;
	HDC sr = src;
	HBITMAP srb = (HBITMAP)GetCurrentObject(sr, OBJ_BITMAP);
	BITMAPINFO bis{ 0 };
	bis.bmiHeader.biSize = sizeof(bis.bmiHeader);
	GetDIBits(sr, srb, 0, 0, 0, &bis, DIB_RGB_COLORS);
	BYTE* lpSrcBits = new BYTE[bis.bmiHeader.biSizeImage];
	GetBitmapBits(srb, bis.bmiHeader.biSizeImage, lpSrcBits);
	HBITMAP srd = (HBITMAP)GetCurrentObject(dst, OBJ_BITMAP);
	BITMAPINFO bid{ 0 };
	bid.bmiHeader.biSize = sizeof(bid.bmiHeader);
	GetDIBits(dst, srd, 0, 0, 0, &bid, DIB_RGB_COLORS);
	BYTE* lpDstBits = new BYTE[bid.bmiHeader.biSizeImage];
	GetBitmapBits(srd, bid.bmiHeader.biSizeImage, lpDstBits);

	for (int dY = p.y; dY < min(bid.bmiHeader.biHeight, p.y + s.cy); dY++)
	{
		for (int dX = p.x; dX < min(bid.bmiHeader.biWidth, p.x + s.cx); dX++)
		{
			int dI = (dX + dY * bid.bmiHeader.biWidth) * 4;
			int sI = ((dX - p.x) + (dY - p.y) * bis.bmiHeader.biWidth) * scale * 4;
			int resultRed = 0;
			int resultGreen = 0;
			int resultBlue = 0;
			for (int i = 0; i < scale; i++)
			{
				for (int j = 0; j < scale; j++)
				{
					int sIo = sI + i * bis.bmiHeader.biWidth * 4 + j * 4;
#ifdef _DEBUG
					if (static_cast<DWORD>(sIo) > bis.bmiHeader.biSizeImage)
						sIo = sI;
#endif
					resultRed += lpSrcBits[sIo + 2];
					resultGreen += lpSrcBits[sIo + 1];
					resultBlue += lpSrcBits[sIo];
				}
			}
			lpDstBits[dI + 2] = (BYTE)(resultRed / scale / scale);
			lpDstBits[dI + 1] = (BYTE)(resultGreen / scale / scale);
			lpDstBits[dI] = (BYTE)(resultBlue / scale / scale);
		}
	}
	delete[] lpSrcBits;
	SetBitmapBits(srd, bid.bmiHeader.biSizeImage, lpDstBits);
	delete[] lpDstBits;
}
*/