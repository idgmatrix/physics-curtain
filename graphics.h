// '�ٶ��� ������ Ŀ��' ���� ���α׷�
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// �� ���α׷��� �ҽ��� �������� ���α׷��� ���θ� �������θ� ����Ͻ� �� �ְ�
// �̿��� �ٸ� �������� ����� �� �����ϴ�. �ٸ� �������� ����ϰ��� �� ����
// �ݵ�� ���۱����� �輺�� (kaswan@hitel.net)���� ������ �ϼż� ����� �����ž� �մϴ�.  

#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct CURRENTCOLOR{
	BYTE r;
	BYTE g;
	BYTE b;
} CurrentColor;

extern BOOL bIsActive;
extern int DIBSectionWidth, DIBSectionHeight;
extern BYTE *Bits;
extern UINT scanline;
extern HDC hScreenDC;
extern CurrentColor color;

void	InitGraphics(HWND hWnd, int width, int height);
void	DeinitGraphics(HWND hWnd);
HBITMAP MakeDIBSection(int width, int height, BYTE **pBits);
void	UpdateFrame(void);
void	DisplayFPS(void);
void	ResetFrameCounter(void);
void	SetColorRGB(BYTE r, BYTE g, BYTE b);
void	ClearDIBSection(BYTE r, BYTE g, BYTE b);

#endif