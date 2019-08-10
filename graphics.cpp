// '�ٶ��� ������ Ŀ��' ���� ���α׷�
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// �� ���α׷��� �ҽ��� �������� ���α׷��� ���θ� �������θ� ����Ͻ� �� �ְ�
// �̿��� �ٸ� �������� ����� �� �����ϴ�. �ٸ� �������� ����ϰ��� �� ����
// �ݵ�� ���۱����� �輺�� (kaswan@hitel.net)���� ������ �ϼż� ����� �����ž� �մϴ�.  

#include "stdafx.h"
#include <stdio.h>

extern void RenderScene(void);

HDC		hScreenDC;
HDC		hMemoryDC;
HBITMAP hDefaultBitmap;

BOOL	bIsActive;
UINT	frameCounter;
LARGE_INTEGER Frequency;
LARGE_INTEGER StartCounter, EndCounter;
	
int		DIBSectionWidth, DIBSectionHeight;
BYTE	*Bits;
UINT	scanline;
float fps;
CurrentColor color;


void	InitGraphics(HWND hWnd, int width, int height);
void	DeinitGraphics(HWND hWnd);
HBITMAP MakeDIBSection(int width, int height, BYTE **pBits);
void	UpdateFrame(void);
void	DisplayFPS(void);
void	PutNumber(int x, int y, int number);
void	PutNumber(int x, int y, UINT number);
void	PutNumber(int x, int y, float number);
void	ResetFrameCounter(void);
void	SetColorRGB(BYTE r, BYTE g, BYTE b);
void	ClearDIBSection(BYTE r, BYTE g, BYTE b);


void InitGraphics(HWND hWnd, int width, int height)
{
	hScreenDC = GetDC(hWnd);
	hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hDIBitmap = MakeDIBSection(width, height, &Bits);
	hDefaultBitmap = (HBITMAP)SelectObject(hMemoryDC, hDIBitmap);

	DIBSectionWidth = width;
	DIBSectionHeight = height;
	scanline= ((DIBSectionWidth * 3) + 3) &~3;

	QueryPerformanceFrequency(&Frequency);
	
	bIsActive = TRUE;
}

void DeinitGraphics(HWND hWnd)
{
	bIsActive = FALSE;

	SelectObject(hMemoryDC, hDefaultBitmap);
	DeleteObject(hDefaultBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(hWnd, hScreenDC);
}

HBITMAP MakeDIBSection(int width, int height, BYTE **pBits)
{
	BITMAPINFO BMInfo;

	BMInfo.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= width;
	BMInfo.bmiHeader.biHeight			= -height;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 24;
	BMInfo.bmiHeader.biCompression		= BI_RGB;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;

	return( CreateDIBSection(hScreenDC, (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)pBits, NULL, 0) );
}

void UpdateFrame(void)
{
	static float frametime;

	QueryPerformanceCounter(&StartCounter);
	
	frameCounter++;
	ClearDIBSection(10, 10, 100);

	RenderScene();
	//PutNumber(0, 40, (UINT)Frequency.LowPart);
	//PutNumber(0, 60, Frequency.HighPart);
	//PutNumber(0, 100, (float)frametime);
	//PutNumber(300, 0, (float)fps);
	DisplayFPS();

	BitBlt(hScreenDC, 0, 0, 639, 479, hMemoryDC, 0, 0, SRCCOPY);

	QueryPerformanceCounter(&EndCounter);
	frametime = (float)(EndCounter.LowPart - StartCounter.LowPart) / (float)Frequency.LowPart;
	//StartCounter.LowPart = EndCounter.LowPart;
	fps = 1 / frametime;
}

void ResetFrameCounter(void)
{
	frameCounter = 0;
}

void DisplayFPS(void)
{
	char string[20];

	sprintf(string, "FPS: %f", fps);
	TextOut(hMemoryDC, 250, 0, string, strlen(string));
}

void PutNumber(int x, int y, int number)
{
	char string[20];

	sprintf(string, "%d", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, UINT number)
{
	char string[20];

	sprintf(string, "%u", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, float number)
{
	char string[20];

	sprintf(string, "%f", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void SetColorRGB(BYTE r, BYTE g, BYTE b)
{
	color.r = r;
	color.g = g;	
	color.b = b;
}

void ClearDIBSection(BYTE r, BYTE g, BYTE b)
{
	UINT BytesPerScanline = (DIBSectionWidth * 3 + 3) & ~3;
	UINT offset = 0;

	while ( offset < BytesPerScanline )
	{
			*(Bits + offset + 0) = b;
			*(Bits + offset + 1) = g;
			*(Bits + offset + 2) = r;
			offset += 3;
	}
	offset = BytesPerScanline;

	for (int i = 0; i < DIBSectionHeight - 1; i++)
	{
		memcpy(Bits + offset, Bits, BytesPerScanline);
		offset += BytesPerScanline;
	}

}
