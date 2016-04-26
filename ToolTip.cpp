/****************************************************************************
**
** ToolTip.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TToolTip ToolTip;
//---------------------------------------------------------------------------
TToolTip::TToolTip()
: m_SeqIndex(0), m_Data(L""), m_ClilocID(0), m_MaxWidth(0), m_X(0), m_Y(0),
m_Use(false), m_Type(SOT_NO_OBJECT), m_Timer(0)
{
	Texture.Clear();
}
//---------------------------------------------------------------------------
TToolTip::~TToolTip()
{
	Texture.Clear();
}
//---------------------------------------------------------------------------
void TToolTip::Set(wstring str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth, int x, int y)
{
	m_Use = !(m_Timer > GetTickCount());

	if (type == m_Type && seqIndex == m_SeqIndex) //��� ���������
		return;

	m_Timer = GetTickCount() + ConfigManager.ToolTipsDelay;
	m_Use = false;
	m_Type = type;
	m_SeqIndex = seqIndex;
	m_Data = str;
	m_ClilocID = 0;
	m_MaxWidth = maxWidth;
	m_X = x;
	m_Y = y;

	Texture.Clear();

	FontManager->GenerateW((BYTE)ConfigManager.ToolTipsTextFont, Texture, m_Data.c_str(), ConfigManager.ToolTipsTextColor, 5, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);
	//FontManager->GenerateW(0, Texture, m_Data.c_str(), 0x0482, 30, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);
}
//---------------------------------------------------------------------------
void TToolTip::Set(DWORD clilocID, string str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth, int x, int y)
{
	Set(ClilocManager->Cliloc(g_Language)->GetW(clilocID, str), type, seqIndex, maxWidth, x, y);

	m_ClilocID = clilocID;
}
//---------------------------------------------------------------------------
void TToolTip::Draw(int cursorWidth, int cursorHeight)
{
	if (!m_Use || !ConfigManager.UseToolTips)
		return;

	if (!m_MaxWidth)
		m_MaxWidth = FontManager->GetWidthW((BYTE)ConfigManager.ToolTipsTextFont, m_Data.c_str(), m_Data.length());

	if (Texture.Empty())
		FontManager->GenerateW((BYTE)ConfigManager.ToolTipsTextFont, Texture, m_Data.c_str(), ConfigManager.ToolTipsTextColor /*0x0482*/, 30, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);

	if (!Texture.Empty())
	{
		int x = m_X;
		int y = m_Y;

		if (!x)
			x = g_MouseX - (Texture.Width + 8);

		if (!y)
			y = g_MouseY - (Texture.Height + 8);

		if (y < 0)
		{
			y = m_Y;
			y = g_MouseY + cursorHeight;
		}
		
		if (x < 0)
		{
			x = m_X;
			x = g_MouseX + cursorWidth;
		}
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

		g_GL.DrawPolygone(x, y, Texture.Width + 8, Texture.Height + 8);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		glDisable(GL_BLEND);

		int tempX = x + 6;
		int tempY = y + 4;

		g_GL.Draw(Texture.Texture, tempX, tempY, Texture.Width, Texture.Height);
	}

	m_Use = false;
}
//---------------------------------------------------------------------------