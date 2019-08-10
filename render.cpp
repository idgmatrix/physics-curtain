// '바람에 날리는 커텐' 데모 프로그램
// Copyright (c) 1999 Kim Seong Wan
// all rights reserved
// 이 프로그램의 소스는 개인적인 프로그래밍 공부를 목적으로만 사용하실 수 있고
// 이외의 다른 목적으로 사용할 수 없습니다. 다른 목적으로 사용하고자 할 경우는
// 반드시 저작권자인 김성완 (kaswan@hitel.net)에게 연락을 하셔서 허락을 받으셔야 합니다.  

#include "stdafx.h"
#include "particles.h"

int	 Coordx = 320, Coordy = 240;

void PutPixel(int x, int y);
void DrawHLine(int x1, int x2, int y);
void DrawVLine(int x, int y1, int y2);
void DrawBox(int x1, int y1, int x2, int y2);
void FillBox(int x1, int y1, int x2, int y2);
void DrawLine(int x1, int y1, int x2, int y2);

void RenderScene(void)
{
	static int counter = 0;
	int i,j;

	SetColorRGB(50,255,50);	
	//FillBox(0, 0, 10, 10);
	//PutPixel(255, 255);
	//DrawHLine(10, 200, 300);
	SetPhysicsWorld();
	//DrawVLine(542, 0, 250);
	//for ( i = 0; i <= 35; i ++)
	//	DrawLine(i * 15, harray[i], i * 15 + 15, harray[i+1]);

	SetColorRGB(255,255,255);	
	
	if ( counter == 0 )
	{
		for ( j = 0; j < 16; j++)
		{
			for ( i = 0; i < 16; i++)
				if ( i == 0 )
					CreateParticle(0.5, i*15 + 150, j*15 + 120, 0, 10, 0, 0, 10000);
				else
					CreateParticle(0.5, i*15 + 150, j*15 + 120, 0, 0, 0, 0, 10000);
		}
		counter++;
	}
	
	//for (i = 0; i < MAXPARTICLE; i ++)
	//{
	//	if ( pa[i].life > 0 ) PutPixel((int)pa[i].x, (int)pa[i].y);
	//}
	
	for (i = 0; i < 16; i++)
	{
		for ( j = 0; j < 15; j++)
		{
			DrawLine(pa[i*16+j].x, pa[i*16+j].y, pa[i*16+(j+1)].x, pa[i*16+(j+1)].y);
		}
	}
	
	for (j = 0; j < 16; j++)
	{
		for ( i = 0; i < 15; i++)
		{
			DrawLine(pa[i*16+j].x, pa[i*16+j].y, pa[(i+1)*16+j].x, pa[(i+1)*16+j].y);
		}
	}

	UpdateLocalForce();
	
	for (i = 16; i < MAXPARTICLE; i ++)
	//if ( i % 16 != 0)	
		MoveParticle(&pa[i]);
	
	DetectCollision();

}

void PutPixel(int x, int y)//, BYTE r, BYTE g, BYTE b)
{
	UINT offset;
	
	offset = scanline * y + x * 3;
	if ( offset > scanline * 479 + 639 * 3 ) return;
	*(Bits + offset + 0) = color.b;
	*(Bits + offset + 1) = color.g;
	*(Bits + offset + 2) = color.r;
}

void DrawHLine(int x1, int x2, int y) 
{
	UINT offset;
	
	offset = scanline * y + x1 * 3;
	
	for(int i = x1; i <= x2; i++)
	{
		*(Bits + offset + 0) = color.b;
		*(Bits + offset + 1) = color.g;
		*(Bits + offset + 2) = color.r;
		offset += 3;
	}
}

void DrawVLine(int x, int y1, int y2)
{
	UINT offset, offsetend;

	offset = scanline * y1 + x * 3;
	offsetend = scanline * y2 + x * 3;
	
	while(offset <= offsetend) 
	{
		*(Bits + offset + 0) = color.b;
		*(Bits + offset + 1) = color.g;
		*(Bits + offset + 2) = color.r;
		offset += scanline;
	}
}

void DrawBox(int x1, int y1, int x2, int y2)
{
	DrawVLine(x1, y1, y2);
	DrawVLine(x2, y1, y2);
	DrawHLine(x1, x2, y1);
	DrawHLine(x1, x2, y2);
}

void FillBox(int x1, int y1, int x2, int y2)
{
	UINT offset;
					
	offset = scanline * y1 +  x1 * 3;
	UINT nextline = scanline - (x2 - x1) * 3 - 3;

	for(int i = y1; i <= y2; i++)
	{
		for(int j = x1; j <= x2; j++)
		{
			*(Bits + offset + 0) = color.b;
			*(Bits + offset + 1) = color.g;
			*(Bits + offset + 2) = color.r;
			offset += 3;
		}
		offset += nextline;
	}
}

void DrawLine(int x1, int y1, int x2, int y2)
{
	int temp;

	if ( x1 > x2 )
	{
		temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	
	int difx = x2 - x1;
	int dify = y2 - y1;
	
	if ( abs(difx) > abs(dify) )
	{
		float alivation = (float)dify / (float)difx;
		float y = (float)y1;
		for (int x = x1; x <= x2; x++)
		{
			PutPixel(x, (int)y);
			y += alivation;
		}
	}
	else
	{
		if ( y1 > y2 )
		{
			temp = y1;
			y1 = y2;
			y2 = temp;

			temp = x1;
			x1 = x2;
			x2 = temp;
		}
	
		difx = x2 - x1;
		dify = y2 - y1;
	
		float alivation = (float)difx / (float)dify;
		float x = (float)x1;
		for (int y = y1; y <= y2; y++)
		{
			PutPixel((int)x, y );
			x += alivation;
		}
	}
}
