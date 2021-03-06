﻿/***********************************************************************************
**
** GLTexture.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLTEXTURE_H
#define GLTEXTURE_H
//----------------------------------------------------------------------------------
typedef vector<uchar> HIT_MAP_TYPE;
//----------------------------------------------------------------------------------
class CGLTexture
{
	//!Габариты текстуры
	SETGET(short, Width, 0);
	SETGET(short, Height, 0);

	SETGET(short, ImageOffsetX, 0);
	SETGET(short, ImageOffsetY, 0);

	SETGET(short, ImageWidth, 0);
	SETGET(short, ImageHeight, 0);

	//!Буфер вершин
	SETGET(GLuint, VertexBuffer, 0);

	//!Буфер вершин для зеркального отображения анимации
	SETGET(GLuint, MirroredVertexBuffer, 0);

public:
	CGLTexture();
	virtual ~CGLTexture();

	GLuint Texture{ 0 };

	HIT_MAP_TYPE m_HitMap;

	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);
	virtual void Draw(const int &x, const int &y, int width, int height, const bool &checktrans = false);

	virtual void DrawRotated(const int &x, const int &y, const float &angle);

	virtual void DrawTransparent(const int &x, const int &y, const bool &stencil = true);

	virtual bool Select(int x, int y, const bool &pixelCheck = true);

	virtual void Clear();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
