/***********************************************************************************
**
** GumpOptions.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpOptions.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/PacketManager.h"
#include "../Managers/SoundManager.h"
#include "../Managers/ColorManager.h"
#include "../Managers/OptionsMacroManager.h"
#include "../Managers/MacroManager.h"
#include "GumpSelectColor.h"
#include "GumpSelectFont.h"
#include "GumpMenubar.h"
#include "../Network/Packets.h"
#include "../GLEngine/GLTextureCircleOfTransparency.h"
#include "../Container.h"
#include "../OrionWindow.h"
#include "../OrionUO.h"
#include "../Macro.h"
#include "../TextEngine/GameConsole.h"
//----------------------------------------------------------------------------------
const ushort g_OptionsTextColor = 0;
//----------------------------------------------------------------------------------
CGumpOptions::CGumpOptions(uint serial, short x, short y)
: CGump(GT_OPTIONS, serial, x, y), m_LastChangeMacroTime(0),
m_MacroSelection(0), m_MacroElement(0), m_MacroListOffset(0), m_MacroListCount(0),
m_MacroListOffsetYStart(0), m_MacroListOffsetYEnd(0), m_MacroListNameOffset(0),
m_MacroPointer(NULL), m_MacroObjectPointer(NULL)
{
	m_Page = 1;

	//Body
	Add(new CGUIResizepic(0, 0x0A28, 40, 0, 550, 450));



	//Left page buttons

	//Sound and Music
	CGUIButton *button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_1, 0x00DA, 0x00DA, 0x00DA, 0, 45));
	button->ToPage = 1;
	//Orion's configuration
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_2, 0x00DC, 0x00DC, 0x00DC, 0, 111));
	button->ToPage = 2;
	//Language
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_3, 0x00DE, 0x00DE, 0x00DE, 0, 177));
	button->ToPage = 3;
	//Chat
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_4, 0x00E0, 0x00E0, 0x00E0, 0, 243));
	button->ToPage = 4;
	//Macro Options
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_5, 0x00ED, 0x00ED, 0x00ED, 0, 309));
	button->ToPage = 5;



	//Right page buttons

	//Interface
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_6, 0x00E2, 0x00E2, 0x00E2, 576, 45));
	button->ToPage = 6;
	//Display
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_7, 0x00E4, 0x00E4, 0x00E4, 576, 111));
	button->ToPage = 7;
	//Reputation System
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_8, 0x00E6, 0x00E6, 0x00E6, 576, 177));
	button->ToPage = 8;
	//Miscellaneous
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_9, 0x00E8, 0x00E8, 0x00E8, 576, 243));
	button->ToPage = 9;
	//Filter Options
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_PAGE_10, 0x00EB, 0x00EB, 0x00EB, 576, 309));
	button->ToPage = 10;



	Add(new CGUIButton(ID_GO_CANCEL, 0x00F3, 0x00F2, 0x00F1, 154, 405));
	Add(new CGUIButton(ID_GO_APPLY, 0x00EF, 0x00EE, 0x00F0, 248, 405));
	Add(new CGUIButton(ID_GO_DEFAULT, 0x00F6, 0x00F5, 0x00F4, 346, 405));
	Add(new CGUIButton(ID_GO_OKAY, 0x00F9, 0x00F7, 0x00F8, 443, 405));



	DrawPage1(); //Sound and Music
	DrawPage2(); //Pop-up Help
	DrawPage3(); //Language
	DrawPage4(); //Chat
	DrawPage5(); //Macro Options
	DrawPage6(); //Interface
	DrawPage7(); //Display
	DrawPage8(); //Reputation System
	DrawPage9(); //Miscellaneous
	DrawPage10(); //Filter Options
}
//----------------------------------------------------------------------------------
CGumpOptions::~CGumpOptions()
{
}
//----------------------------------------------------------------------------
void CGumpOptions::Init()
{
	g_OptionsMacroManager.LoadFromMacro();

	m_MacroPointer = (CMacro*)g_OptionsMacroManager.m_Items;
	m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

	m_MacroKey->m_Entry.SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);

	RedrawMacroData();
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage1()
{
	//Sound and Music
	Add(new CGUIPage(1));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Sound and Music", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the sound and music you will hear while playing Ultima Online.", 30, 500);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_SOUND_ON_OFF, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.Sound;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Sound on/off");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 112));
	text->CreateTextureW(0, L"Sound volume");

	m_SliderSound = (CGUISlider*)Add(new CGUISlider(ID_GO_P1_SOUND_VOLUME, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 133, 90, 0, 255, g_OptionsConfig.SoundVolume));
	m_SliderSound->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_MUSIC_ON_OFF, 0x00D2, 0x00D3, 0x00D2, 64, 151));
	checkbox->Checked = g_OptionsConfig.Music;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 151));
	text->CreateTextureW(0, L"Music on/off");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 173));
	text->CreateTextureW(0, L"Music volume");

	m_SliderMusic = (CGUISlider*)Add(new CGUISlider(ID_GO_P1_MUSIC_VOLUME, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 194, 90, 0, 255, g_OptionsConfig.MusicVolume));
	m_SliderMusic->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_PLAY_FOOTSTEP_SOUNDS, 0x00D2, 0x00D3, 0x00D2, 64, 212));
	checkbox->Checked = g_OptionsConfig.FootstepsSound;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 212));
	text->CreateTextureW(0, L"Play footstep sounds");


	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P1_PLAY_COMBAT_MUSIC, 0x00D2, 0x00D3, 0x00D2, 64, 232));
	checkbox->Checked = g_OptionsConfig.CombatMusic;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 232));
	text->CreateTextureW(0, L"Play combat music");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage2()
{
	//Orion's configuration
	Add(new CGUIPage(2));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Orion's configuration", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings configure the Orion UO Client.");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 90));
	text->CreateTextureW(0, L"FPS rate:");

	m_SliderClientFPS = (CGUISlider*)Add(new CGUISlider(ID_GO_P2_CLIENT_FPS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 111, 90, 16, 64, g_OptionsConfig.ClientFPS));
	m_SliderClientFPS->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage3()
{
	//Language
	Add(new CGUIPage(3));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Language", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"The language you use when playing UO is obtained from your Operating System settings.", 30, 480);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P3_USE_TOOLTIP, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.UseToolTips;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Use Tool-tips");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 112));
	text->CreateTextureW(0, L"Delay befor Tool-tip appears");

	m_SliderTooltipDelay = (CGUISlider*)Add(new CGUISlider(ID_GO_P3_DELAY_BEFORE_TOOLTIP, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 133, 90, 0, 5000, g_OptionsConfig.ToolTipsDelay));
	m_SliderTooltipDelay->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);

	Add(new CGUIButton(ID_GO_P3_TEXT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 151));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.ToolTipsTextColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ToolTipsTextColor);

	m_ColorTooltipText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P3_TEXT_COLOR, g_OptionsConfig.ToolTipsTextColor, 67, 154, 13, 14, color));
	m_ColorTooltipText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 151));
	text->CreateTextureW(0, L"Color of Tool-tip text");

	Add(new CGUIButton(ID_GO_P3_TEXT_FONT, 0x00D0, 0x00D0, 0x00D0, 64, 173));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 173));
	text->CreateTextureW(0, L"Font for Tool-tips");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage4()
{
	//Chat
	Add(new CGUIPage(4));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Chat", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the interface display for the chat system.");


	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 90));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);

	m_ColorInputText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR, g_OptionsConfig.ChatColorInputText, 67, 93, 13, 14, color));
	m_ColorInputText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 90));
	text->CreateTextureW(0, L"Input text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 1, 0x00D4, 0x00D4, 0x00D4, 64, 109));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMenuOption = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 1, g_OptionsConfig.ChatColorInputText, 67, 112, 13, 14, color));
	m_ColorMenuOption->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 109));
	text->CreateTextureW(0, L"Menu option color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 2, 0x00D4, 0x00D4, 0x00D4, 64, 128));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorPlayerColorInMemberList = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 2, g_OptionsConfig.ChatColorInputText, 67, 131, 13, 14, color));
	m_ColorPlayerColorInMemberList->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 128));
	text->CreateTextureW(0, L"Player color in member list");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 3, 0x00D4, 0x00D4, 0x00D4, 64, 147));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChatText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 3, g_OptionsConfig.ChatColorInputText, 67, 150, 13, 14, color));
	m_ColorChatText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 147));
	text->CreateTextureW(0, L"Chat text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 4, 0x00D4, 0x00D4, 0x00D4, 64, 166));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorPlayerNameWithoutSpeakingPrivileges = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 4, g_OptionsConfig.ChatColorInputText, 67, 169, 13, 14, color));
	m_ColorPlayerNameWithoutSpeakingPrivileges->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 166));
	text->CreateTextureW(0, L"Player name without speaking privileges", 30, 160);

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 5, 0x00D4, 0x00D4, 0x00D4, 64, 201));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMutedText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 5, g_OptionsConfig.ChatColorInputText, 67, 204, 13, 14, color));
	m_ColorMutedText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 201));
	text->CreateTextureW(0, L"Muted text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 6, 0x00D4, 0x00D4, 0x00D4, 64, 220));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChannelModeratorName = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 6, g_OptionsConfig.ChatColorInputText, 67, 223, 13, 14, color));
	m_ColorChannelModeratorName->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 220));
	text->CreateTextureW(0, L"Channel moderator name");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 7, 0x00D4, 0x00D4, 0x00D4, 64, 239));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorChannelModeratorText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 7, g_OptionsConfig.ChatColorInputText, 67, 242, 13, 14, color));
	m_ColorChannelModeratorText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 239));
	text->CreateTextureW(0, L"Channel moderator text");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 8, 0x00D4, 0x00D4, 0x00D4, 64, 258));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMyName = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 8, g_OptionsConfig.ChatColorInputText, 67, 261, 13, 14, color));
	m_ColorMyName->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 258));
	text->CreateTextureW(0, L"My name's color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 9, 0x00D4, 0x00D4, 0x00D4, 64, 277));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorMyText = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 9, g_OptionsConfig.ChatColorInputText, 67, 280, 13, 14, color));
	m_ColorMyText->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 277));
	text->CreateTextureW(0, L"My text color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 10, 0x00D4, 0x00D4, 0x00D4, 64, 296));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorSystemMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 10, g_OptionsConfig.ChatColorInputText, 67, 299, 13, 14, color));
	m_ColorSystemMessage->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 88, 296));
	text->CreateTextureW(0, L"System message color");



	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 11, 0x00D4, 0x00D4, 0x00D4, 300, 90));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorTextOutputBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 11, g_OptionsConfig.ChatColorInputText, 303, 93, 13, 14, color));
	m_ColorTextOutputBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 90));
	text->CreateTextureW(0, L"Text Output Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 12, 0x00D4, 0x00D4, 0x00D4, 300, 109));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorTextInputBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 12, g_OptionsConfig.ChatColorInputText, 303, 112, 13, 14, color));
	m_ColorTextInputBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 109));
	text->CreateTextureW(0, L"Text Input Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 13, 0x00D4, 0x00D4, 0x00D4, 300, 128));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorUserListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 13, g_OptionsConfig.ChatColorInputText, 303, 131, 13, 14, color));
	m_ColorUserListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 128));
	text->CreateTextureW(0, L"User List Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 14, 0x00D4, 0x00D4, 0x00D4, 300, 147));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorConferenceListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 14, g_OptionsConfig.ChatColorInputText, 303, 150, 13, 14, color));
	m_ColorConferenceListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 147));
	text->CreateTextureW(0, L"Conference List Background Color");

	Add(new CGUIButton(ID_GO_P4_TEXT_COLOR + 15, 0x00D4, 0x00D4, 0x00D4, 300, 166));

	if (g_OptionsConfig.ChatColorInputText != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.ChatColorInputText);
	else
		color = 0xFF7F7F7F;

	m_ColorCommandListBackground = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P4_TEXT_COLOR + 15, g_OptionsConfig.ChatColorInputText, 303, 169, 13, 14, color));
	m_ColorCommandListBackground->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 166));
	text->CreateTextureW(0, L"Command List Background Color");



	Add(new CGUIButton(ID_GO_P4_TEXT_FONT, 0x00D0, 0x00D0, 0x00D0, 300, 192));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 324, 192));
	text->CreateTextureW(0, L"Chat font");
}
//----------------------------------------------------------------------------
void CGumpOptions::RedrawMacroData()
{
	m_MacroDataBox->Clear();

	bool alt = false;
	bool ctrl = false;
	bool shift = false;
	ushort key = 0;

	if (m_MacroPointer != NULL)
	{
		alt = m_MacroPointer->Alt;
		ctrl = m_MacroPointer->Ctrl;
		shift = m_MacroPointer->Shift;
		key = m_MacroPointer->Key;
	}

	m_MacroCheckboxShift->Checked = shift;
	m_MacroCheckboxAlt->Checked = alt;
	m_MacroCheckboxCtrl->Checked = ctrl;

	CMacroObject *obj = m_MacroObjectPointer;

	if (obj != NULL)
	{
		int captionY = 22;
		int posY = 60;
		int posX = 164;

		//Macro Options
		int boxPosY = posY + 127;
		int arrowPosY = posY + 133;
		bool drawEmptyBox = true;

		if (obj->m_Prev != NULL)
			m_MacroDataBox->Add(new CGUIButton(ID_GO_P5_BUTTON_UP, 0x0983, 0x0984, 0x0984, 292, 113));

		/*int macroCount = 0;

		while (obj != NULL && macroCount < maxMacroDraw)
		{
			Orion->DrawGump(0x098D, 0, posX, boxPosY);
			FontManager->DrawA(1, TMacro::GetActionName(obj->Code), 0x0386, posX + 4, boxPosY + 5, 98, TS_CENTER, UOFONT_FIXED);
			Orion->DrawGump(0x0985, 0, posX + 94, arrowPosY);

			char hasMenu = obj->HasSubMenu;
			if (hasMenu)
			{
				Orion->DrawGump(0x098E, 0, posX + 112, boxPosY);

				if (hasMenu == 1) //����� ��������� ����
				{
					Orion->DrawGump(0x0985, 0, posX + 274, arrowPosY);

					FontManager->DrawA(1, TMacro::GetAction(obj->SubCode), 0x0386, posX + 118, boxPosY + 5, 150, TS_LEFT, UOFONT_FIXED);
				}
				else
				{
					g_GL.Scissor((int)g_GumpTranslateX + posX + 118, (int)g_GumpTranslateY + boxPosY + 5, 150, 20);

					((TMacroObjectEntry*)obj)->TextEntry->DrawA(1, 0x0386, posX + 118, boxPosY + 5, TS_LEFT, UOFONT_FIXED);

					glDisable(GL_SCISSOR_TEST);
				}
			}

			boxPosY += 26;
			arrowPosY += 26;
			macroCount++;

			if (obj->m_Next == NULL)
				break;

			obj = (TMacroObject*)obj->m_Next;
		}

		if (macroCount >= maxMacroDraw)
		{
			drawEmptyBox = false;
			Orion->DrawGump(0x0985 + (int)(g_GumpSelectElement == ID_GO_P5_BUTTON_DOWN), 0, posX + 292, boxPosY);
		}
		else if (obj->Code == MC_NONE)
			drawEmptyBox = false;
	}

	if (drawEmptyBox)
	{
		Orion->DrawGump(0x098D, 0, posX, boxPosY);
		Orion->DrawGump(0x0985, 0, posX + 94, arrowPosY);
	}

	if (m_MacroSelection)
	{
		bool isAction = false;
		if (m_MacroSelection == 0x20000000) //Action
		{
			posX += 116;
			isAction = true;
		}

		int textIndex = m_MacroListOffset;

		posY -= 60;
		posY += m_MacroListOffsetYStart;

		//Top
		Orion->DrawGump(0x09B5, 0, posX - 5, posY - 11);

		if (textIndex > 0)
			Orion->DrawGump(0x0983, 0, posX + 85, posY - 8);

		int ofs = 0;

		int count = m_MacroListMaxCount;

		if (count > m_MacroListCount)
			count = m_MacroListCount;

		IFOR(i, 0, count)
		{
			gumpID = 0x09B6 + ofs;
			ofs = (ofs + 1) % 3;

			int itemPosY = posY + (i * 15);

			Orion->DrawGump(gumpID, 0, posX, itemPosY);

			int textNameIndex = textIndex + m_MacroListNameOffset;

			if (textIndex < m_MacroListCount)
			{
				if (g_GumpSelectElement >= ID_GO_P5_SELECTION)
				{
					if (textNameIndex == (g_GumpSelectElement - ID_GO_P5_SELECTION))
						g_GL.DrawPolygone(posX + 4, itemPosY, 150, 14);
				}

				if (isAction) //Action
				{
					if (textNameIndex < 210)
					{
						FontManager->DrawA(1, TMacro::GetAction(textNameIndex), 0x0386, posX + 4, itemPosY);
						textIndex++;
					}
				}
				else //Action name
				{
					if (textNameIndex < 60)
					{
						FontManager->DrawA(1, TMacro::GetActionName(textNameIndex), 0x0386, posX + 4, itemPosY);
						textIndex++;
					}
				}
			}
		}

		int offsBottomY = (m_MacroListOffsetYEnd - m_MacroListOffsetYStart);

		//Bottom
		Orion->DrawGump(0x09B9, 0, posX - 5, posY + offsBottomY);

		if (m_MacroListOffset + m_MacroListMaxCount < m_MacroListCount)
			Orion->DrawGump(0x0985, 0, posX + 85, posY + offsBottomY - 2);*/
	}
	
	/*const int maxMacroDraw = 7;

	int captionY = 22;
	int posY = captionY + 38;
	int posX = 30;

	//Macro Options
	if (mode)
	{
		posX += 134;

		int boxPosY = posY + 127;
		int arrowPosY = posY + 133;
		bool drawEmptyBox = true;
		
		TMacroObject *obj = m_MacroObjectPointer;
		if (obj != NULL)
		{
			if (obj->m_Prev != NULL)
				Orion->DrawGump(0x0983 + (int)(g_GumpSelectElement == ID_GO_P5_BUTTON_UP), 0, posX + 292, posY + 113);
			
			int macroCount = 0;

			while (obj != NULL && macroCount < maxMacroDraw)
			{
				Orion->DrawGump(0x098D, 0, posX, boxPosY);
				FontManager->DrawA(1, TMacro::GetActionName(obj->Code), 0x0386, posX + 4, boxPosY + 5, 98, TS_CENTER, UOFONT_FIXED);
				Orion->DrawGump(0x0985, 0, posX + 94, arrowPosY);

				char hasMenu = obj->HasSubMenu;
				if (hasMenu)
				{
					Orion->DrawGump(0x098E, 0, posX + 112, boxPosY);

					if (hasMenu == 1) //����� ��������� ����
					{
						Orion->DrawGump(0x0985, 0, posX + 274, arrowPosY);
						
						FontManager->DrawA(1, TMacro::GetAction(obj->SubCode), 0x0386, posX + 118, boxPosY + 5, 150, TS_LEFT, UOFONT_FIXED);
					}
					else
					{
						g_GL.Scissor((int)g_GumpTranslateX + posX + 118, (int)g_GumpTranslateY + boxPosY + 5, 150, 20);

						((TMacroObjectEntry*)obj)->TextEntry->DrawA(1, 0x0386, posX + 118, boxPosY + 5, TS_LEFT, UOFONT_FIXED);

						glDisable(GL_SCISSOR_TEST);
					}
				}

				boxPosY += 26;
				arrowPosY += 26;
				macroCount++;

				if (obj->m_Next == NULL)
					break;

				obj = (TMacroObject*)obj->m_Next;
			}
			
			if (macroCount >= maxMacroDraw)
			{
				drawEmptyBox = false;
				Orion->DrawGump(0x0985 + (int)(g_GumpSelectElement == ID_GO_P5_BUTTON_DOWN), 0, posX + 292, boxPosY);
			}
			else if (obj->Code == MC_NONE)
				drawEmptyBox = false;
		}

		if (drawEmptyBox)
		{
			Orion->DrawGump(0x098D, 0, posX, boxPosY);
			Orion->DrawGump(0x0985, 0, posX + 94, arrowPosY);
		}

		if (m_MacroSelection)
		{
			bool isAction = false;
			if (m_MacroSelection == 0x20000000) //Action
			{
				posX += 116;
				isAction = true;
			}

			int textIndex = m_MacroListOffset;

			posY -= 60;
			posY += m_MacroListOffsetYStart;
				
			//Top
			Orion->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			if (textIndex > 0)
				Orion->DrawGump(0x0983, 0, posX + 85, posY - 8);
				
			int ofs = 0;

			int count = m_MacroListMaxCount;

			if (count > m_MacroListCount)
				count = m_MacroListCount;

			IFOR(i, 0, count)
			{
				gumpID = 0x09B6 + ofs;
				ofs = (ofs + 1) % 3;

				int itemPosY = posY + (i * 15);

				Orion->DrawGump(gumpID, 0, posX, itemPosY);
				
				int textNameIndex = textIndex + m_MacroListNameOffset;

				if (textIndex < m_MacroListCount)
				{
					if (g_GumpSelectElement >= ID_GO_P5_SELECTION)
					{
						if (textNameIndex == (g_GumpSelectElement - ID_GO_P5_SELECTION))
							g_GL.DrawPolygone(posX + 4, itemPosY, 150, 14);
					}

					if (isAction) //Action
					{
						if (textNameIndex < 210)
						{
							FontManager->DrawA(1, TMacro::GetAction(textNameIndex), 0x0386, posX + 4, itemPosY);
							textIndex++;
						}
					}
					else //Action name
					{
						if (textNameIndex < 60)
						{
							FontManager->DrawA(1, TMacro::GetActionName(textNameIndex), 0x0386, posX + 4, itemPosY);
							textIndex++;
						}
					}
				}
			}

			int offsBottomY = (m_MacroListOffsetYEnd - m_MacroListOffsetYStart);

			//Bottom
			Orion->DrawGump(0x09B9, 0, posX - 5, posY + offsBottomY);

			if (m_MacroListOffset + m_MacroListMaxCount < m_MacroListCount)
				Orion->DrawGump(0x0985, 0, posX + 85, posY + offsBottomY - 2);
		}
	}
	else
	{
		int LSG = 0;
		
		if (Orion->GumpPixelsInXY(0x099C, posX + 152, posY))
			LSG = ID_GO_P5_BUTTON_ADD; //Add
		else if (Orion->GumpPixelsInXY(0x099F, posX + 205, posY))
			LSG = ID_GO_P5_BUTTON_DELETE; //Delete
		else if (Orion->GumpPixelsInXY(0x09A2, posX + 273, posY))
			LSG = ID_GO_P5_BUTTON_PREVEOUS; //Preveous
		else if (Orion->GumpPixelsInXY(0x09A5, posX + 357, posY))
			LSG = ID_GO_P5_BUTTON_NEXT; //Next
		else if (Orion->GumpPixelsInXY(0x098B, posX + 133, posY + 52))
			LSG = ID_GO_P5_KEY_BOX; //Key box
		else if (Orion->GumpPixelsInXY(0x0867, posX + 248, posY + 19))
			LSG = ID_GO_P5_BUTTON_SHIFT; //Shift
		else if (Orion->GumpPixelsInXY(0x0867, posX + 248, posY + 47))
			LSG = ID_GO_P5_BUTTON_ALT; //Shift
		else if (Orion->GumpPixelsInXY(0x0867, posX + 248, posY + 75))
			LSG = ID_GO_P5_BUTTON_CTRL; //Shift

		posX += 134;

		int boxPosY = posY + 127;
		int arrowPosY = posY + 133;
		bool drawEmptyBox = true;
		
		TMacroObject *obj = m_MacroObjectPointer;
		if (obj != NULL)
		{
			if (obj->m_Prev != NULL && Orion->GumpPixelsInXY(0x0983, posX + 292, posY + 113))
				LSG = ID_GO_P5_BUTTON_UP; //UP arrow
			
			int macroCount = 0;

			while (obj != NULL && macroCount < maxMacroDraw)
			{
				if (Orion->GumpPixelsInXY(0x098D, posX, boxPosY))
					LSG = ID_GO_P5_LEFT_BOX + macroCount; //Left macro box

				char hasMenu = obj->HasSubMenu;
				if (hasMenu)
				{
					if (Orion->GumpPixelsInXY(0x098E, posX + 112, boxPosY))
						LSG = ID_GO_P5_RIGHT_BOX + macroCount; //Right macro box
				}

				boxPosY += 26;
				arrowPosY += 26;
				macroCount++;
					
				if (obj->m_Next == NULL)
					break;

				obj = (TMacroObject*)obj->m_Next;
			}
			
			if (macroCount >= maxMacroDraw)
			{
				drawEmptyBox = false;

				if (Orion->GumpPixelsInXY(0x0985, posX + 292, boxPosY))
					LSG = ID_GO_P5_BUTTON_DOWN; //Down arrow
			}
			else if (obj->Code == MC_NONE)
				drawEmptyBox = false;

			if (macroCount >= maxMacroDraw || obj->Code == MC_NONE)
				drawEmptyBox = false;
		}

		if (drawEmptyBox)
		{
			if (Orion->GumpPixelsInXY(0x098D, posX, boxPosY))
				LSG = ID_GO_P5_EMPTY_BOX; //Empty macro box
		}

		if (m_MacroSelection)
		{
			bool isAction = false;
			if (m_MacroSelection == 0x20000000) //Action
			{
				posX += 116;
				isAction = true;
			}

			int textIndex = m_MacroListOffset;

			posY -= 60;
			posY += m_MacroListOffsetYStart;
				
			int ofs = 0;

			int count = m_MacroListMaxCount;

			if (count > m_MacroListCount)
				count = m_MacroListCount;

			IFOR(i, 0, count)
			{
				if (Orion->GumpPixelsInXY(0x09B6 + ofs, posX, posY + (i * 15)))
					LSG = ID_GO_P5_SELECTION + textIndex + m_MacroListNameOffset;
					
				ofs = (ofs + 1) % 3;
				textIndex++;
			}
		}

		return LSG;
	}

	return 0;*/
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage5()
{
	Add(new CGUIPage(5));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Macro Options", 30, 460, TS_CENTER);

	Add(new CGUIButton(ID_GO_P5_BUTTON_ADD, 0x099C, 0x099E, 0x099D, 152, 60));
	CGUIButton *button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_DELETE, 0x099F, 0x09A1, 0x09A0, 205, 60));
	button->ProcessPressedState = true;
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_PREVEOUS, 0x09A2, 0x09A4, 0x09A3, 273, 60));
	button->ProcessPressedState = true;
	button = (CGUIButton*)Add(new CGUIButton(ID_GO_P5_BUTTON_NEXT, 0x09A5, 0x09A7, 0x09A6, 357, 60));
	button->ProcessPressedState = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 134, 82));
	text->CreateTextureW(0, L"Keystroke");

	//KeyBox
	Add(new CGUIGumppic(0x098B, 133, 112));
	m_MacroKey = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P5_KEY_BOX, 0x0386, 0x0386, 0x0386, 138, 117, 56, false, 1, TS_LEFT, UOFONT_FIXED));
	m_MacroKey->CheckOnSerial = true;
	Add(new CGUIHitBox(ID_GO_P5_KEY_BOX, 133, 112, 63, 23));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 200, 111));
	text->CreateTextureW(0, L"Key");

	//Shift checkbox
	m_MacroCheckboxShift = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_SHIFT, 0x0867, 0x0869, 0x0867, 248, 79));
	m_MacroCheckboxShift->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 82));
	text->CreateTextureW(0, L"Shift");

	//Alt checkbox
	m_MacroCheckboxAlt = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_ALT, 0x0867, 0x0869, 0x0867, 248, 107));
	m_MacroCheckboxAlt->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 111));
	text->CreateTextureW(0, L"Alt");

	//Ctrl checkbox
	m_MacroCheckboxCtrl = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P5_BUTTON_CTRL, 0x0867, 0x0869, 0x0867, 248, 135));
	m_MacroCheckboxCtrl->GraphicSelected = 0x0868;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 280, 140));
	text->CreateTextureW(0, L"Ctrl");

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 134, 163));
	text->CreateTextureW(0, L"Actions");

	m_MacroDataBox = (CGUIDataBox*)Add(new CGUIDataBox());
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage6()
{
	//Interface
	Add(new CGUIPage(6));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Interface", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These options affect your interface.");

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_ENABLE_PATHFINDING, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.EnablePathfind;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Enable pathfinding with double-right-click");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_HOLD_TAB_FOR_COMBAT, 0x00D2, 0x00D3, 0x00D2, 64, 110));
	checkbox->Checked = g_OptionsConfig.HoldTabForCombat;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 110));
	text->CreateTextureW(0, L"Hold down TAB key for combat, instead of tapping it to toggle combat mode", 30, 450);

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_OFFSET_INTERFACE_WINDOWS, 0x00D2, 0x00D3, 0x00D2, 64, 146));
	checkbox->Checked = g_OptionsConfig.OffsetInterfaceWindows;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 146));
	text->CreateTextureW(0, L"Offset interface windows rather than perfectly stacking them");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS, 0x00D2, 0x00D3, 0x00D2, 64, 166));
	checkbox->Checked = g_OptionsConfig.AutoArrange;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 166));
	text->CreateTextureW(0, L"Automatically arrange minimized windows");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_ALWAYS_RUN, 0x00D2, 0x00D3, 0x00D2, 64, 186));
	checkbox->Checked = g_OptionsConfig.AlwaysRun;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 186));
	text->CreateTextureW(0, L"Your character will always run if this is checked");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_DISABLE_MENUBAR, 0x00D2, 0x00D3, 0x00D2, 64, 206));
	checkbox->Checked = g_OptionsConfig.DisableMenubar;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 206));
	text->CreateTextureW(0, L"Disable the Menu Bar");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS, 0x00D2, 0x00D3, 0x00D2, 64, 226));
	checkbox->Checked = g_OptionsConfig.GrayOutOfRangeObjects;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 226));
	text->CreateTextureW(0, L"Gray out of range objects");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM, 0x00D2, 0x00D3, 0x00D2, 64, 246));
	checkbox->Checked = g_OptionsConfig.DisableNewTargetSystem;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 246));
	text->CreateTextureW(0, L"Disable New Target System");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_OBJECT_HANDLES, 0x00D2, 0x00D3, 0x00D2, 64, 266));
	checkbox->Checked = g_OptionsConfig.ObjectHandles;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 266));
	text->CreateTextureW(0, L"Object Handles");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_REDUCE_FPS_UNACTIVE_WINDOW, 0x00D2, 0x00D3, 0x00D2, 64, 286));
	checkbox->Checked = g_OptionsConfig.ReduceFPSUnactiveWindow;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 286));
	text->CreateTextureW(0, L"Reduce FPS when Window is Unactive");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON, 0x00D2, 0x00D3, 0x00D2, 64, 306));
	checkbox->Checked = g_OptionsConfig.ItemPropertiesIcon;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 306));
	text->CreateTextureW(0, L"Display Item Properties Icon");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS, 0x00D2, 0x00D3, 0x00D2, 64, 326));
	checkbox->Checked = g_OptionsConfig.HoldShiftForContextMenus;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 326));
	text->CreateTextureW(0, L"Hold Shift For Context Menus");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING, 0x00D2, 0x00D3, 0x00D2, 64, 346));
	checkbox->Checked = g_OptionsConfig.HoldShiftForEnablePathfind;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 346));
	text->CreateTextureW(0, L"Hold Shift For Enable Pathfinding");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage7()
{
	//Display
	Add(new CGUIPage(7));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Display", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These options affect your display, and adjusting some of them may improve your graphics performance.", 30, 500);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 90));
	text->CreateTextureW(0, L"Game Play Window Size");

	Add(new CGUIResizepic(ID_GO_P7_GAME_WINDOW_WIDTH, 0x0BB8, 64, 112, 60, 22));
	m_GameWindowWidth = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P7_GAME_WINDOW_WIDTH, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 68, 114));
	m_GameWindowWidth->CheckOnSerial = true;
	m_GameWindowWidth->m_Entry.MaxLength = 3000;
	m_GameWindowWidth->m_Entry.NumberOnly = true;
	m_GameWindowWidth->m_Entry.SetText(std::to_wstring(g_OptionsConfig.GameWindowWidth));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 126, 112));
	text->CreateTextureW(0, L"x");

	Add(new CGUIResizepic(ID_GO_P7_GAME_WINDOW_HEIGHT, 0x0BB8, 139, 112, 60, 22));
	m_GameWindowHeight = (CGUITextEntry*)Add(new CGUITextEntry(ID_GO_P7_GAME_WINDOW_HEIGHT, g_OptionsTextColor, g_OptionsTextColor, g_OptionsTextColor, 143, 114));
	m_GameWindowHeight->CheckOnSerial = true;
	m_GameWindowHeight->m_Entry.MaxLength = 3000;
	m_GameWindowHeight->m_Entry.NumberOnly = true;
	m_GameWindowHeight->m_Entry.SetText(std::to_wstring(g_OptionsConfig.GameWindowHeight));



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 140));
	text->CreateTextureW(0, L"Adjust how long speech remains on screen");

	m_SliderSpeeckDuration = (CGUISlider*)Add(new CGUISlider(ID_GO_P7_AJUST_LONG_SPEECH, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 160, 90, 0, 999, g_OptionsConfig.SpeechDelay));
	m_SliderSpeeckDuration->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_SCALE_SPEECH_DURATION, 0x00D2, 0x00D3, 0x00D2, 64, 182));
	checkbox->Checked = g_OptionsConfig.ScaleSpeechDelay;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 182));
	text->CreateTextureW(0, L"Scale speech duration based on length");



	Add(new CGUIButton(ID_GO_P7_SPEECH_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 204));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.SpeechColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.SpeechColor);

	m_ColorSpeech = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_SPEECH_COLOR, g_OptionsConfig.SpeechColor, 67, 207, 13, 14, color));
	m_ColorSpeech->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 204));
	text->CreateTextureW(0, L"Speech Color");

	Add(new CGUIButton(ID_GO_P7_EMOTE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 223));

	if (g_OptionsConfig.EmoteColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.EmoteColor);
	else
		color = 0xFF7F7F7F;

	m_ColorEmote = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_EMOTE_COLOR, g_OptionsConfig.EmoteColor, 67, 226, 13, 14, color));
	m_ColorEmote->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 223));
	text->CreateTextureW(0, L"Emote Color");

	Add(new CGUIButton(ID_GO_P7_PARTY_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 242));

	if (g_OptionsConfig.PartyMessageColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.PartyMessageColor);
	else
		color = 0xFF7F7F7F;

	m_ColorPartyMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_PARTY_MESSAGE_COLOR, g_OptionsConfig.PartyMessageColor, 67, 245, 13, 14, color));
	m_ColorPartyMessage->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 242));
	text->CreateTextureW(0, L"Party Message Color");



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_DARK_NIGHTS, 0x00D2, 0x00D3, 0x00D2, 64, 264));
	checkbox->Checked = g_OptionsConfig.DarkNights;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 264));
	text->CreateTextureW(0, L"Dark Nights");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_COLORED_LIGHTING, 0x00D2, 0x00D3, 0x00D2, 64, 284));
	checkbox->Checked = g_OptionsConfig.ColoredLighting;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 284));
	text->CreateTextureW(0, L"Colored Lighting");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_CHARACTERS_ANIMATION_DELAY, 0x00D2, 0x00D3, 0x00D2, 64, 304));
	checkbox->Checked = g_OptionsConfig.StandartCharactersAnimationDelay;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 304));
	text->CreateTextureW(0, L"Standard characters animation delay");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_ITEMS_ANIMATION_DELAY, 0x00D2, 0x00D3, 0x00D2, 64, 324));
	checkbox->Checked = g_OptionsConfig.StandartItemsAnimationDelay;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 324));
	text->CreateTextureW(0, L"Standard items animation delay");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_LOCK_GAME_WINDOW_RESIZING, 0x00D2, 0x00D3, 0x00D2, 64, 344));
	checkbox->Checked = g_OptionsConfig.LockResizingGameWindow;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 344));
	text->CreateTextureW(0, L"Lock game window resizing");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_LOCK_GUMP_MOVING, 0x00D2, 0x00D3, 0x00D2, 64, 364));
	checkbox->Checked = g_OptionsConfig.LockGumpsMoving;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 364));
	text->CreateTextureW(0, L"Lock gumps moving");

	if (g_PacketManager.ClientVersion >= CV_6000)
	{
		Add(new CGUIButton(ID_GO_P7_GUILD_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 354, 204));

		if (g_OptionsConfig.GuildMessageColor != 0xFFFF)
			color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.GuildMessageColor);
		else
			color = 0xFF7F7F7F;

		m_ColorGuildMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_GUILD_MESSAGE_COLOR, g_OptionsConfig.GuildMessageColor, 357, 207, 13, 14, color));
		m_ColorGuildMessage->CallOnMouseUp = true;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 204));
		text->CreateTextureW(0, L"Guild Message Color");

		Add(new CGUIButton(ID_GO_P7_ALLIANCE_MESSAGE_COLOR, 0x00D4, 0x00D4, 0x00D4, 354, 223));

		if (g_OptionsConfig.AllianceMessageColor != 0xFFFF)
			color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.AllianceMessageColor);
		else
			color = 0xFF7F7F7F;

		m_ColorAllianceMessage = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P7_ALLIANCE_MESSAGE_COLOR, g_OptionsConfig.AllianceMessageColor, 357, 226, 13, 14, color));
		m_ColorAllianceMessage->CallOnMouseUp = true;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 223));
		text->CreateTextureW(0, L"Alliance Message Color");

		checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_IGNORE_GUILD_MESSAGE, 0x00D2, 0x00D3, 0x00D2, 354, 245));
		checkbox->Checked = g_OptionsConfig.IgnoreGuildMessage;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 245));
		text->CreateTextureW(0, L"Ignore Guild Messages");

		checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P7_IGNORE_ALLIANCE_MESSAGE, 0x00D2, 0x00D3, 0x00D2, 354, 265));
		checkbox->Checked = g_OptionsConfig.IgnoreAllianceMessage;

		text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 376, 265));
		text->CreateTextureW(0, L"Ignore Alliance Messages");
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage8()
{
	//Reputation System
	Add(new CGUIPage(8));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Reputation System", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"These settings affect the reputation system, which is Ultima Online's system for controlling antisocial behavior.", 30, 500);



	Add(new CGUIButton(ID_GO_P8_INNOCENT_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 90));

	uint color = 0xFF7F7F7F;

	if (g_OptionsConfig.InnocentColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.InnocentColor);

	m_ColorInnocent = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_INNOCENT_COLOR, g_OptionsConfig.InnocentColor, 67, 93, 13, 14, color));
	m_ColorInnocent->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Innocent highlight color");

	Add(new CGUIButton(ID_GO_P8_FRIENDLY_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 109));

	if (g_OptionsConfig.FriendlyColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.FriendlyColor);
	else
		color = 0xFF7F7F7F;

	m_ColorFriendly = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_FRIENDLY_COLOR, g_OptionsConfig.FriendlyColor, 67, 112, 13, 14, color));
	m_ColorFriendly->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 109));
	text->CreateTextureW(0, L"Friendly guilds highlight color");

	Add(new CGUIButton(ID_GO_P8_SOMEONE_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 128));

	if (g_OptionsConfig.SomeoneColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.SomeoneColor);
	else
		color = 0xFF7F7F7F;

	m_ColorSomeone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_SOMEONE_COLOR, g_OptionsConfig.SomeoneColor, 67, 131, 13, 14, color));
	m_ColorSomeone->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 128));
	text->CreateTextureW(0, L"Someone that can be attacked color");

	Add(new CGUIButton(ID_GO_P8_CRIMINAL_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 147));

	if (g_OptionsConfig.CriminalColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.CriminalColor);
	else
		color = 0xFF7F7F7F;

	m_ColorCriminal = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_CRIMINAL_COLOR, g_OptionsConfig.CriminalColor, 67, 150, 13, 14, color));
	m_ColorCriminal->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 147));
	text->CreateTextureW(0, L"Criminal highlight color");

	Add(new CGUIButton(ID_GO_P8_ENEMY_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 166));

	if (g_OptionsConfig.EnemyColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.EnemyColor);
	else
		color = 0xFF7F7F7F;

	m_ColorEnemy = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_ENEMY_COLOR, g_OptionsConfig.EnemyColor, 67, 169, 13, 14, color));
	m_ColorEnemy->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 166));
	text->CreateTextureW(0, L"Enemy guildmembers color");

	Add(new CGUIButton(ID_GO_P8_MURDERER_COLOR, 0x00D4, 0x00D4, 0x00D4, 64, 185));

	if (g_OptionsConfig.MurdererColor != 0xFFFF)
		color = g_ColorManager.GetPolygoneColor(5, g_OptionsConfig.MurdererColor);
	else
		color = 0xFF7F7F7F;

	m_ColorMurderer = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(ID_GO_P8_MURDERER_COLOR, g_OptionsConfig.MurdererColor, 67, 188, 13, 14, color));
	m_ColorMurderer->CallOnMouseUp = true;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 185));
	text->CreateTextureW(0, L"Murderer highlight color");



	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P8_QUERY_CRIMINAL_ACTIONS, 0x00D2, 0x00D3, 0x00D2, 64, 204));
	checkbox->Checked = g_OptionsConfig.CriminalActionsQuery;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 204));
	text->CreateTextureW(0, L"Query before performing criminal actions");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage9()
{
	//Miscellaneous
	Add(new CGUIPage(9));

	CGUIText *text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 84, 22));
	text->CreateTextureW(0, L"Miscellaneous", 30, 460, TS_CENTER);

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 44));
	text->CreateTextureW(0, L"Miscellaneous options.", 30, 500);

	CGUICheckbox *checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_SHOW_APPROACHING_NAMES, 0x00D2, 0x00D3, 0x00D2, 64, 90));
	checkbox->Checked = g_OptionsConfig.ShowIncomingNames;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 90));
	text->CreateTextureW(0, L"Show Names of Approaching Players");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY, 0x00D2, 0x00D3, 0x00D2, 64, 110));
	checkbox->Checked = g_OptionsConfig.UseCircleTrans;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 110));
	text->CreateTextureW(0, L"Use circle of transparency");



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 132));
	text->CreateTextureW(0, L"Set radius of transparency circle");

	m_SliderCircleTransparency = (CGUISlider*)Add(new CGUISlider(ID_GO_P9_TRANSPARENCY_RADIUS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 153, 90, 0, 200, g_OptionsConfig.CircleTransRadius));
	m_SliderCircleTransparency->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 64, 175));
	text->CreateTextureW(0, L"Inform me when my skills improve this much (in tenths)");

	m_SliderInformSkills = (CGUISlider*)Add(new CGUISlider(ID_GO_P9_INFORM_SKILLS, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 64, 196, 90, 0, 100, g_OptionsConfig.SkillReport));
	m_SliderInformSkills->SetTextParameters(true, STP_RIGHT, 0, g_OptionsTextColor, true);



	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_INFORM_STATS, 0x00D2, 0x00D3, 0x00D2, 64, 216));
	checkbox->Checked = g_OptionsConfig.StatReport;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 216));
	text->CreateTextureW(0, L"Inform me of increases in strength, dexterity, and intelligence.");

	checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GO_P9_CONSOLE_ENTER, 0x00D2, 0x00D3, 0x00D2, 64, 236));
	checkbox->Checked = g_OptionsConfig.ConsoleNeedEnter;

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 236));
	text->CreateTextureW(0, L"Console need press 'Enter' to activate it.");

	Add(new CGUIButton(ID_GO_P9_SPEECH_FONT, 0x00D0, 0x00D0, 0x00D0, 64, 262));

	text = (CGUIText*)Add(new CGUIText(g_OptionsTextColor, 86, 262));
	text->CreateTextureW(0, L"Set the font for speech");
}
//----------------------------------------------------------------------------
void CGumpOptions::DrawPage10()
{
	//Filter Options
}
//----------------------------------------------------------------------------
void CGumpOptions::UpdateColor(const SELECT_COLOR_GUMP_STATE &state, const ushort &color)
{
	switch (state)
	{
		case SCGS_OPT_TOOLTIP_TEXT:
		{
			g_OptionsConfig.ToolTipsTextColor = color;
			m_ColorTooltipText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_INPUT_TEXT:
		{
			g_OptionsConfig.ChatColorInputText = color;
			m_ColorInputText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MENU_OPTION:
		{
			g_OptionsConfig.ChatColorMenuOption = color;
			m_ColorMenuOption->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_IN_MEMBER_LIST:
		{
			g_OptionsConfig.ChatColorPlayerInMemberList = color;
			m_ColorPlayerColorInMemberList->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_TEXT:
		{
			g_OptionsConfig.ChatColorText = color;
			m_ColorChatText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_WITHOUT_PRIV:
		{
			g_OptionsConfig.ChatColorPlayerNameWithout = color;
			m_ColorPlayerNameWithoutSpeakingPrivileges->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MUTED_TEXT:
		{
			g_OptionsConfig.ChatColorMuted = color;
			m_ColorMutedText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_NAME:
		{
			g_OptionsConfig.ChatColorChannelModeratorName = color;
			m_ColorChannelModeratorName->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_TEXT:
		{
			g_OptionsConfig.ChatColorChannelModeratorText = color;
			m_ColorChannelModeratorText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MY_NAME:
		{
			g_OptionsConfig.ChatColorMyName = color;
			m_ColorMyName->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_MY_TEXT:
		{
			g_OptionsConfig.ChatColorMyText = color;
			m_ColorMyText->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_SYSTEM_MESSAGE:
		{
			g_OptionsConfig.ChatColorSystemMessage = color;
			m_ColorSystemMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_OUTPUT:
		{
			g_OptionsConfig.ChatColorBGOutputText = color;
			m_ColorTextOutputBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_INPUT:
		{
			g_OptionsConfig.ChatColorBGInputText = color;
			m_ColorTextInputBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_USER_LIST:
		{
			g_OptionsConfig.ChatColorBGUserList = color;
			m_ColorUserListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_CONF_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			m_ColorConferenceListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_CHAT_BG_COMMAND_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			m_ColorCommandListBackground->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_SPEECH:
		{
			g_OptionsConfig.SpeechColor = color;
			m_ColorSpeech->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_EMOTE:
		{
			g_OptionsConfig.EmoteColor = color;
			m_ColorEmote->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_PARTY_MESSAGE:
		{
			g_OptionsConfig.PartyMessageColor = color;
			m_ColorPartyMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_GUILD_MESSAGE:
		{
			g_OptionsConfig.GuildMessageColor = color;
			m_ColorGuildMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_DISPLAY_ALLIANCE_MESSAGE:
		{
			g_OptionsConfig.AllianceMessageColor = color;
			m_ColorAllianceMessage->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_INNOCENT:
		{
			g_OptionsConfig.InnocentColor = color;
			m_ColorInnocent->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_FRIENDLY:
		{
			g_OptionsConfig.FriendlyColor = color;
			m_ColorFriendly->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_SOMEONE:
		{
			g_OptionsConfig.SomeoneColor = color;
			m_ColorSomeone->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_CRIMINAL:
		{
			g_OptionsConfig.CriminalColor = color;
			m_ColorCriminal->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_ENEMY:
		{
			g_OptionsConfig.EnemyColor = color;
			m_ColorEnemy->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		case SCGS_OPT_REPSYS_MURDERER:
		{
			g_OptionsConfig.MurdererColor = color;
			m_ColorMurderer->UpdateColor(color, g_ColorManager.GetPolygoneColor(5, color));
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::OnLeftMouseDown()
{
	/*if (g_LastSelectedObject == ID_GO_P5_BUTTON_PREVEOUS) //Preveous button
	{
		if (m_MacroPointer->m_Prev != NULL)
		{
			m_MacroPointer = (TMacro*)m_MacroPointer->m_Prev;
			m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

			TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
		}

		m_LastChangeMacroTime = g_Ticks + CHANGE_MACRO_DELAY;
	}
	else if (g_LastSelectedObject == ID_GO_P5_BUTTON_NEXT) //Next button
	{
		if (m_MacroPointer->m_Next != NULL)
		{
			m_MacroPointer = (TMacro*)m_MacroPointer->m_Next;
			m_MacroObjectPointer = (TMacroObject*)m_MacroPointer->m_Items;

			TextEntryMacro->SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);
		}

		m_LastChangeMacroTime = g_Ticks + CHANGE_MACRO_DELAY;
	}
	else if (g_LastSelectedObject >= ID_GO_P5_LEFT_BOX && g_LastSelectedObject < ID_GO_P5_RIGHT_BOX) //Left action box
	{
		m_MacroElement = g_LastSelectedObject - ID_GO_P5_LEFT_BOX;
		m_MacroSelection = 0x10000000;
		m_MacroListOffset = 0;
		m_MacroListCount = 60;
		m_MacroListNameOffset = 0;
		m_MacroListOffsetYStart = 92;
		m_MacroListOffsetYEnd = 392;
	}
	else if (g_LastSelectedObject >= ID_GO_P5_RIGHT_BOX && g_LastSelectedObject < ID_GO_P5_EMPTY_BOX) //Right action box
	{
		m_MacroElement = g_LastSelectedObject - ID_GO_P5_RIGHT_BOX;
		
		TMacroObject *obj = m_MacroObjectPointer;
					
		IFOR(i, 0, (int)m_MacroElement)
		{
			if (obj->m_Next == NULL)
				break;

			obj = (TMacroObject*)obj->m_Next;
		}
		
		m_MacroListCount = 0;
		m_MacroListOffset = 0;
		m_MacroListNameOffset = 0;

		TMacro::GetBoundByCode(obj->Code, m_MacroListCount, m_MacroListNameOffset);

		if (m_MacroListCount)
		{
			m_MacroSelection = 0x20000000;

			if (m_MacroListCount >= m_MacroListMaxCount)
			{
				m_MacroListOffsetYStart = 92;
				m_MacroListOffsetYEnd = 392;
			}
			else
			{
				int startY = 187 + (m_MacroElement * 26) + 26;

				int heightWindow = m_MacroListCount * 15;

				if (heightWindow + startY > 392)
				{
					m_MacroListOffsetYStart = 392 - heightWindow;
					m_MacroListOffsetYEnd = 392;
				}
				else
				{
					m_MacroListOffsetYStart = startY;
					m_MacroListOffsetYEnd = startY + heightWindow;
				}
			}
		}
		else
		{
			m_MacroSelection = 0;
			m_MacroListOffsetYStart = 0;
			m_MacroListOffsetYEnd = 0;

			if (obj->HasSubMenu == 2)
			{
				int x = g_MouseX - (X + 110);
				int y = 0; //g_MouseY - (Y + 57);

				((TMacroObjectEntry*)obj)->TextEntry->OnClick(this, 1, false, x, y, TS_LEFT, UOFONT_FIXED);
			}
		}
	}
	else if (g_LastSelectedObject == ID_GO_P5_EMPTY_BOX) //Empty left action box
	{
		m_MacroSelection = 0x40000000;
		m_MacroListOffset = 0;
		m_MacroListCount = 60;
		m_MacroListNameOffset = 0;
		m_MacroListOffsetYStart = 92;
		m_MacroListOffsetYEnd = 392;
	}*/
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GO_PAGE_7)
	{
		m_GameWindowWidth->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowWidth));
		m_GameWindowHeight->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowHeight));
	}
	else if (serial == ID_GO_CANCEL) //Cancel
	{
		g_OptionsMacroManager.Clear();
		m_RemoveMark = true;
	}
	else if (serial == ID_GO_APPLY) //Apply
		ApplyPageChanges();
	else if (serial == ID_GO_DEFAULT) //Default
	{
		switch (m_Page)
		{
			case 1:
			{
				g_OptionsConfig.DefaultPage1();
				break;
			}
			case 2:
			{
				g_OptionsConfig.DefaultPage2();
				break;
			}
			case 3:
			{
				g_OptionsConfig.DefaultPage3();
				break;
			}
			case 4:
			{
				g_OptionsConfig.DefaultPage4();
				break;
			}
			case 6:
			{
				g_OptionsConfig.DefaultPage6();
				break;
			}
			case 7:
			{
				g_OptionsConfig.DefaultPage7();

				m_GameWindowWidth->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowWidth));
				m_GameWindowHeight->m_Entry.SetText(std::to_string(g_OptionsConfig.GameWindowHeight));
				break;
			}
			case 8:
			{
				g_OptionsConfig.DefaultPage8();
				break;
			}
			case 9:
			{
				g_OptionsConfig.DefaultPage9();
				break;
			}
			default:
				break;
		}
	}
	else if (serial == ID_GO_OKAY) //Okay
	{
		/*IFOR(i, 0, 10)
		{
			m_Page = i;
			ApplyPageChanges();
		}*/

		ApplyPageChanges();
		g_OptionsMacroManager.Clear();

		m_RemoveMark = true;
	}
	else
	{
		switch (m_Page)
		{
			case 1: //Sound and Music
			{
				break;
			}
			case 2: //Orion's configuration
			{
				break;
			}
			case 3: //Language
			{
				if (serial == ID_GO_P3_TEXT_COLOR) //Color of Tool-tip text
					g_GumpManager.AddGump(new CGumpSelectColor(g_PlayerSerial, 100, 100, SCGS_OPT_TOOLTIP_TEXT));
				else if (serial == ID_GO_P3_TEXT_FONT) //Font
					g_GumpManager.AddGump(new CGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_TOOLTIP));

				break;
			}
			case 4: //Chat
			{
				if (serial >= ID_GO_P4_TEXT_COLOR) //Input text color
					g_GumpManager.AddGump(new CGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_CHAT_INPUT_TEXT + (serial - ID_GO_P4_TEXT_COLOR))));
				else if (serial == ID_GO_P4_TEXT_FONT) //Font
					g_GumpManager.AddGump(new CGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_CHAT));

				break;
			}
			case 5: //Macro Options
			{
				if (serial == ID_GO_P5_BUTTON_ADD) //Add button
				{
					m_MacroPointer = CMacro::CreateBlankMacro();
					m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;
					g_OptionsMacroManager.Add(m_MacroPointer);

					m_MacroKey->m_Entry.SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);

					RedrawMacroData();
				}
				else if (serial == ID_GO_P5_BUTTON_DELETE) //Delete button
				{
					CMacro *newpointer = (CMacro*)m_MacroPointer->m_Next;

					if (newpointer == NULL)
						newpointer = (CMacro*)m_MacroPointer->m_Prev;

					g_OptionsMacroManager.Delete(m_MacroPointer);

					if (newpointer == NULL)
					{
						newpointer = CMacro::CreateBlankMacro();
						g_OptionsMacroManager.Add(newpointer);
					}

					m_MacroPointer = newpointer;
					m_MacroObjectPointer = (CMacroObject*)m_MacroPointer->m_Items;

					m_MacroKey->m_Entry.SetText(m_HotkeyText[m_MacroPointer->Key & 0xFF]);

					RedrawMacroData();
				}
				else if (serial == ID_GO_P5_BUTTON_UP) //Prev button
				{
					if (m_MacroObjectPointer->m_Prev != NULL)
					{
						m_MacroObjectPointer = (CMacroObject*)m_MacroObjectPointer->m_Prev;
						RedrawMacroData();
					}
				}
				else if (serial == ID_GO_P5_BUTTON_DOWN) //Next button
				{
					if (m_MacroObjectPointer->m_Next != NULL)
					{
						m_MacroObjectPointer = (CMacroObject*)m_MacroObjectPointer->m_Next;
						RedrawMacroData();
					}
				}
				/*else if (serial >= ID_GO_P5_SELECTION) //Action selection
				{
					DWORD index = g_LastSelectedObject - ID_GO_P5_SELECTION;

					switch (m_MacroSelection)
					{
						case 0x10000000:
						case 0x40000000:
						{
							TMacroObject *obj = m_MacroObjectPointer;

							if (m_MacroSelection == 0x40000000)
							{
								obj = TMacro::CreateMacro((MACRO_CODE)index);
								m_MacroPointer->Add(obj);
							}
							else
							{
								IFOR(i, 0, (int)m_MacroElement)
								{
									if (obj->m_Next == NULL)
									{
										obj = TMacro::CreateMacro((MACRO_CODE)index);
										m_MacroPointer->Add(obj);

										break;
									}

									obj = (TMacroObject*)obj->m_Next;
								}
							}
							
							if (obj->Code != (MACRO_CODE)index)
							{
								TMacroObject *newobj = TMacro::CreateMacro((MACRO_CODE)index);

								if (obj == m_MacroObjectPointer)
									m_MacroObjectPointer = newobj;
								
								m_MacroPointer->ChangeObject(obj, newobj);
							}

							break;
						}
						case 0x20000000:
						{
							TMacroObject *obj = m_MacroObjectPointer;
							
							IFOR(i, 0, (int)m_MacroElement)
							{
								if (obj->m_Next == NULL)
									break;

								obj = (TMacroObject*)obj->m_Next;
							}

							obj->SubCode = (MACRO_SUB_CODE)index;

							break;
						}
						default:
							break;
					}
				}*/

				break;
			}
			case 6: //Interface
			{
				break;
			}
			case 7: //Display
			{
				if (serial >= ID_GO_P7_SPEECH_COLOR && serial <= ID_GO_P7_ALLIANCE_MESSAGE_COLOR) //Speech color
					g_GumpManager.AddGump(new CGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_DISPLAY_SPEECH + (serial - ID_GO_P7_SPEECH_COLOR))));

				break;
			}
			case 8: //Reputation System
			{
				if (serial >= ID_GO_P8_INNOCENT_COLOR && serial <= ID_GO_P8_MURDERER_COLOR) //Innocent highlight color
					g_GumpManager.AddGump(new CGumpSelectColor(g_PlayerSerial, 100, 100, (SELECT_COLOR_GUMP_STATE)(SCGS_OPT_REPSYS_INNOCENT + (serial - ID_GO_P8_INNOCENT_COLOR))));

				break;
			}
			case 9: //Miscellaneous
			{
				if (serial == ID_GO_P9_SPEECH_FONT) //Speech Font
					g_GumpManager.AddGump(new CGumpSelectFont(g_PlayerSerial, 320, 240, SFGS_OPT_MISCELLANEOUS));

				break;
			}
			case 10: //Filter Options
			{
				break;
			}
			default:
				break;
		}
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_CHECKBOX_EVENT_C
{
	switch (m_Page)
	{
		case 1: //Sound and Music
		{
			if (serial == ID_GO_P1_SOUND_ON_OFF) //Sound on/off
				g_OptionsConfig.Sound = state;
			else if (serial == ID_GO_P1_MUSIC_ON_OFF) //Music on/off
				g_OptionsConfig.Music = state;
			else if (serial == ID_GO_P1_PLAY_FOOTSTEP_SOUNDS) //Play footstep sounds
				g_OptionsConfig.FootstepsSound = state;
			else if (serial == ID_GO_P1_PLAY_COMBAT_MUSIC) //Play combat music
				g_OptionsConfig.CombatMusic = state;

			break;
		}
		case 2: //Orion's configuration
		{
			break;
		}
		case 3: //Language
		{
			if (serial == ID_GO_P3_USE_TOOLTIP) //Use Tool-tips
				g_OptionsConfig.UseToolTips = state;

			break;
		}
		case 4: //Chat
		{

			break;
		}
		case 5: //Macro Options
		{
			if (serial == ID_GO_P5_BUTTON_SHIFT) //Shift checkbox
				m_MacroPointer->Shift = state;
			else if (serial == ID_GO_P5_BUTTON_ALT) //Alt checkbox
				m_MacroPointer->Alt = state;
			else if (serial == ID_GO_P5_BUTTON_CTRL) //Ctrl checkbox
				m_MacroPointer->Ctrl = state;

			break;
		}
		case 6: //Interface
		{
			if (serial == ID_GO_P6_ENABLE_PATHFINDING) //Enable pathfinding with double-right-click
				g_OptionsConfig.EnablePathfind = state;
			else if (serial == ID_GO_P6_HOLD_TAB_FOR_COMBAT) //Hold down TAB key for combat, instead of tapping it to toggle combat mode
				g_OptionsConfig.HoldTabForCombat = state;
			else if (serial == ID_GO_P6_OFFSET_INTERFACE_WINDOWS) //Offset interface windows rather than perfectly stacking them
				g_OptionsConfig.OffsetInterfaceWindows = state;
			else if (serial == ID_GO_P6_AUTO_ARRANGE_MINIMIZED_WINDOWS) //Automatically arrange minimized windows
				g_OptionsConfig.AutoArrange = state;
			else if (serial == ID_GO_P6_ALWAYS_RUN) //Your character will always run if this is checked
				g_OptionsConfig.AlwaysRun = state;
			else if (serial == ID_GO_P6_DISABLE_MENUBAR) //Disable the Menu Bar
				g_OptionsConfig.DisableMenubar = state;
			else if (serial == ID_GO_P6_GRAY_OUT_OF_RANGE_OBJECTS) //Gray out of range objects
				g_OptionsConfig.GrayOutOfRangeObjects = state;
			else if (serial == ID_GO_P6_DISABLE_NEW_TARGET_SYSTEM) //Disable New Target System
				g_OptionsConfig.DisableNewTargetSystem = state;
			else if (serial == ID_GO_P6_OBJECT_HANDLES) //Object Handles
				g_OptionsConfig.ObjectHandles = state;
			else if (serial == ID_GO_P6_REDUCE_FPS_UNACTIVE_WINDOW) //Reduce FPS when Window is Unactive
				g_OptionsConfig.ReduceFPSUnactiveWindow = state;
			else if (serial == ID_GO_P6_DISPLAY_ITEM_PROPERTIES_ICON) //Display Item Properties Icon
				g_OptionsConfig.ItemPropertiesIcon = state;
			else if (serial == ID_GO_P6_HOLD_SHIFT_FOR_CONTEXT_MENUS) //Hold Shift For Context Menus
				g_OptionsConfig.HoldShiftForContextMenus = state;
			else if (serial == ID_GO_P6_HOLD_SHIFT_FOR_ENABLE_PATHFINDING) //Hold Shift For Enable Pathfinding
				g_OptionsConfig.HoldShiftForEnablePathfind = state;

			break;
		}
		case 7: //Display
		{
			if (serial == ID_GO_P7_SCALE_SPEECH_DURATION) //Scale speech duration based on length
				g_OptionsConfig.ScaleSpeechDelay = state;
			else if (serial == ID_GO_P7_IGNORE_GUILD_MESSAGE) //Ignore Guild Messages
				g_OptionsConfig.IgnoreGuildMessage = state;
			else if (serial == ID_GO_P7_IGNORE_ALLIANCE_MESSAGE) //Ignore Alliance Messages
				g_OptionsConfig.IgnoreAllianceMessage = state;
			else if (serial == ID_GO_P7_DARK_NIGHTS) //Dark Nights
				g_OptionsConfig.DarkNights = state;
			else if (serial == ID_GO_P7_COLORED_LIGHTING) //Colored Lighting
				g_OptionsConfig.ColoredLighting = state;
			else if (serial == ID_GO_P7_CHARACTERS_ANIMATION_DELAY) //Standard characters animation delay
				g_OptionsConfig.StandartCharactersAnimationDelay = state;
			else if (serial == ID_GO_P7_ITEMS_ANIMATION_DELAY) //Standard items animation delay
				g_OptionsConfig.StandartItemsAnimationDelay = state;
			else if (serial == ID_GO_P7_LOCK_GAME_WINDOW_RESIZING) //Lock game window resizing
				g_OptionsConfig.LockResizingGameWindow = state;
			else if (serial == ID_GO_P7_LOCK_GUMP_MOVING) //Lock gump moving
				g_OptionsConfig.LockGumpsMoving = state;

			break;
		}
		case 8: //Reputation System
		{
			if (serial == ID_GO_P8_QUERY_CRIMINAL_ACTIONS) //Query before performing criminal actions
				g_OptionsConfig.CriminalActionsQuery = state;

			break;
		}
		case 9: //Miscellaneous
		{
			if (serial == ID_GO_P9_SHOW_APPROACHING_NAMES) //Show Names of Approaching Players
				g_OptionsConfig.ShowIncomingNames = state;
			else if (serial == ID_GO_P9_USE_CIRCLE_OF_TRANSPARENCY) //Use circle of transparency
				g_OptionsConfig.UseCircleTrans = state;
			else if (serial == ID_GO_P9_INFORM_STATS) //Inform me of increases in strength, dexterity, and intelligence.
				g_OptionsConfig.StatReport = state;
			else if (serial == ID_GO_P9_CONSOLE_ENTER) //Console need press 'Enter' to activate it.
				g_OptionsConfig.ConsoleNeedEnter = state;

			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_SLIDER_CLICK_EVENT_C
{
	OnSliderMove(serial);
}
//----------------------------------------------------------------------------
void CGumpOptions::GUMP_SLIDER_MOVE_EVENT_C
{
	switch (m_Page)
	{
		case 1: //Sound and Music
		{
			if (serial == ID_GO_P1_SOUND_VOLUME)
				g_OptionsConfig.SoundVolume = m_SliderSound->Value;
			else if (serial == ID_GO_P1_MUSIC_VOLUME)
				g_OptionsConfig.MusicVolume = m_SliderMusic->Value;

			break;
		}
		case 2: //Orion's configuration
		{
			if (serial == ID_GO_P2_CLIENT_FPS)
				g_OptionsConfig.ClientFPS = m_SliderClientFPS->Value;

			break;
		}
		case 3: //Language
		{
			if (serial == ID_GO_P3_DELAY_BEFORE_TOOLTIP)
				g_OptionsConfig.ToolTipsDelay = m_SliderTooltipDelay->Value;

			break;
		}
		case 4: //Chat
		{

			break;
		}
		case 5: //Macro Options
		{
			break;
		}
		case 6: //Interface
		{
			break;
		}
		case 7: //Display
		{
			break;
		}
		case 8: //Reputation System
		{
			break;
		}
		case 9: //Miscellaneous
		{
			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	if (g_EntryPointer == &m_GameWindowWidth->m_Entry || g_EntryPointer == &m_GameWindowHeight->m_Entry)
	{
		if (wParam >= '0' && wParam <= '9')
		{
			g_EntryPointer->Insert(wParam);

			int val = atoi(g_EntryPointer->c_str());

			if (val > 2000)
				g_EntryPointer->Remove(true);
			else
				m_WantRedraw = true;
		}
	}
	/*else if (g_EntryPointer != &m_MacroKey->m_Entry)
	{
		CMacroObject *obj = m_MacroPointer->EntryPointerHere();

		if (obj != NULL)
		{
			bool canAdd = false;

			switch (obj->Code)
			{
				case MC_DELAY:
				case MC_SET_UPDATE_RANGE:
				case MC_MODIFY_UPDATE_RANGE:
				{
					if (wParam >= '0' && wParam <= '9')
						canAdd = true;

					break;
				}
				case MC_SAY:
				case MC_EMOTE:
				case MC_WHISPER:
				case MC_YELL:
				{
					canAdd = true;

					break;
				}
				default:
					break;
			}

			if (canAdd)
			{
				g_EntryPointer->Insert(wParam);
				m_WantRedraw = true;
			}
		}
	}*/
}
//----------------------------------------------------------------------------
void CGumpOptions::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (g_EntryPointer == &m_MacroKey->m_Entry)
	{
		m_MacroPointer->Key = wParam & 0xFF;

		m_MacroKey->m_Entry.SetText(m_HotkeyText[wParam & 0xFF]);

		m_WantRedraw = true;
	}
	else
	{
		if (wParam == VK_RETURN)
		{
			if (g_ConfigManager.ConsoleNeedEnter)
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true;
		}
		else
			g_EntryPointer->OnKey(this, wParam);
	}
}
//----------------------------------------------------------------------------
void CGumpOptions::ApplyPageChanges()
{
	switch (m_Page)
	{
		case 1: //Sound and Music
		{		
			//������ ��������� ����� �������� � ������� ��������
			if (g_ConfigManager.SoundVolume != g_OptionsConfig.SoundVolume)
			{
				g_ConfigManager.SoundVolume = g_OptionsConfig.SoundVolume;
				g_Orion.AdjustSoundEffects(g_Ticks + 100000, g_SoundManager.GetVolumeValue());
			}

			//������ ��������� ����� ������ � ������� ������
			if (g_ConfigManager.MusicVolume != g_OptionsConfig.MusicVolume)
			{
				g_ConfigManager.MusicVolume = g_OptionsConfig.MusicVolume;
				g_SoundManager.SetMusicVolume(g_SoundManager.GetVolumeValue(-1, true));
			}
			
		    //��������� ���� ��������.
			if (g_ConfigManager.Sound && !g_OptionsConfig.Sound)
				g_Orion.AdjustSoundEffects(g_Ticks + 100000);

			g_ConfigManager.Sound = g_OptionsConfig.Sound;


			//��������� ���� ������.
			if (g_ConfigManager.Music && !g_OptionsConfig.Music)
			{
				g_SoundManager.StopMusic();
				g_SoundManager.StopWarMusic();
			}

			g_ConfigManager.Music = g_OptionsConfig.Music;

			g_ConfigManager.FootstepsSound = g_OptionsConfig.FootstepsSound;
			g_ConfigManager.CombatMusic = g_OptionsConfig.CombatMusic;

			break;
		}
		case 2: //Orion's configuration
		{
			g_ConfigManager.ClientFPS = g_OptionsConfig.ClientFPS;

			break;
		}
		case 3: //Language
		{
			g_ConfigManager.UseToolTips = g_OptionsConfig.UseToolTips;
			g_ConfigManager.ToolTipsTextColor = g_OptionsConfig.ToolTipsTextColor;
			g_ConfigManager.ToolTipsTextFont = g_OptionsConfig.ToolTipsTextFont;
			g_ConfigManager.ToolTipsDelay = g_OptionsConfig.ToolTipsDelay;

			break;
		}
		case 4: //Chat
		{
			g_ConfigManager.ChatColorInputText = g_OptionsConfig.ChatColorInputText;
			g_ConfigManager.ChatColorMenuOption = g_OptionsConfig.ChatColorMenuOption;
			g_ConfigManager.ChatColorPlayerInMemberList = g_OptionsConfig.ChatColorPlayerInMemberList;
			g_ConfigManager.ChatColorText = g_OptionsConfig.ChatColorText;
			g_ConfigManager.ChatColorPlayerNameWithout = g_OptionsConfig.ChatColorPlayerNameWithout;
			g_ConfigManager.ChatColorMuted = g_OptionsConfig.ChatColorMuted;
			g_ConfigManager.ChatColorChannelModeratorName = g_OptionsConfig.ChatColorChannelModeratorName;
			g_ConfigManager.ChatColorChannelModeratorText = g_OptionsConfig.ChatColorChannelModeratorText;
			g_ConfigManager.ChatColorMyName = g_OptionsConfig.ChatColorMyName;
			g_ConfigManager.ChatColorMyText = g_OptionsConfig.ChatColorMyText;
			g_ConfigManager.ChatColorSystemMessage = g_OptionsConfig.ChatColorSystemMessage;
			g_ConfigManager.ChatColorBGOutputText = g_OptionsConfig.ChatColorBGOutputText;
			g_ConfigManager.ChatColorBGInputText = g_OptionsConfig.ChatColorBGInputText;
			g_ConfigManager.ChatColorBGUserList = g_OptionsConfig.ChatColorBGUserList;
			g_ConfigManager.ChatColorBGConfList = g_OptionsConfig.ChatColorBGConfList;
			g_ConfigManager.ChatColorBGCommandList = g_OptionsConfig.ChatColorBGCommandList;
			g_ConfigManager.ChatFont = g_OptionsConfig.ChatFont;

			break;
		}
		case 5: //Macro Options
		{
			g_MacroManager.LoadFromOptions();
			break;
		}
		case 6: //Interface
		{
			g_ConfigManager.EnablePathfind = g_OptionsConfig.EnablePathfind;
			g_ConfigManager.HoldTabForCombat = g_OptionsConfig.HoldTabForCombat;

			if (g_ConfigManager.OffsetInterfaceWindows != g_OptionsConfig.OffsetInterfaceWindows && g_OptionsConfig.OffsetInterfaceWindows)
				g_ContainerRect.MakeDefault();

			g_ConfigManager.OffsetInterfaceWindows = g_OptionsConfig.OffsetInterfaceWindows;
			g_ConfigManager.AutoArrange = g_OptionsConfig.AutoArrange;
			g_ConfigManager.AlwaysRun = g_OptionsConfig.AlwaysRun;
			g_ConfigManager.DisableMenubar = g_OptionsConfig.DisableMenubar;
			g_ConfigManager.GrayOutOfRangeObjects = g_OptionsConfig.GrayOutOfRangeObjects;
			g_ConfigManager.DisableNewTargetSystem = g_OptionsConfig.DisableNewTargetSystem;
			g_ConfigManager.ObjectHandles = g_OptionsConfig.ObjectHandles;
			g_ConfigManager.ReduceFPSUnactiveWindow = g_OptionsConfig.ReduceFPSUnactiveWindow;
			g_ConfigManager.ItemPropertiesIcon = g_OptionsConfig.ItemPropertiesIcon;
			g_ConfigManager.HoldShiftForContextMenus = g_OptionsConfig.HoldShiftForContextMenus;
			g_ConfigManager.HoldShiftForEnablePathfind = g_OptionsConfig.HoldShiftForEnablePathfind;

			if (g_OptionsConfig.DisableMenubar)
				g_GumpManager.CloseGump(g_PlayerSerial, 0, GT_MENUBAR);
			else
				g_GumpManager.AddGump(new CGumpMenubar(g_PlayerSerial, 0, 0));

			break;
		}
		case 7: //Display
		{
			int curX = 0;

			if (m_GameWindowWidth->m_Entry.Length())
				curX = atoi(m_GameWindowWidth->m_Entry.c_str());

			if (curX < 640)
				curX = 640;
			else if (curX > (g_OrionWindow.Size.Width - 20))
				curX = g_OrionWindow.Size.Width - 20;

			g_OptionsConfig.GameWindowWidth = curX;
			g_ConfigManager.GameWindowWidth = curX;
			
			int curY = 0;
			
			if (m_GameWindowHeight->m_Entry.Length())
				curY = atoi(m_GameWindowHeight->m_Entry.c_str());

			if (curY < 480)
				curY = 480;
			else if (curY > (g_OrionWindow.Size.Height - 40))
				curY = (g_OrionWindow.Size.Height - 40);

			g_OptionsConfig.GameWindowHeight = curY;
			g_ConfigManager.GameWindowHeight = curY;

			if (g_PacketManager.GetClientVersion() >= CV_200)
				CPacketGameWindowSize().Send();

			g_ConfigManager.SpeechDelay = g_OptionsConfig.SpeechDelay;
			g_ConfigManager.ScaleSpeechDelay = g_OptionsConfig.ScaleSpeechDelay;
			g_ConfigManager.SpeechColor = g_OptionsConfig.SpeechColor;
			g_ConfigManager.EmoteColor = g_OptionsConfig.EmoteColor;
			g_ConfigManager.PartyMessageColor = g_OptionsConfig.PartyMessageColor;
			g_ConfigManager.GuildMessageColor = g_OptionsConfig.GuildMessageColor;
			g_ConfigManager.AllianceMessageColor = g_OptionsConfig.AllianceMessageColor;
			g_ConfigManager.IgnoreGuildMessage = g_OptionsConfig.IgnoreGuildMessage;
			g_ConfigManager.IgnoreAllianceMessage = g_OptionsConfig.IgnoreAllianceMessage;
			g_ConfigManager.DarkNights = g_OptionsConfig.DarkNights;
			g_ConfigManager.ColoredLighting = g_OptionsConfig.ColoredLighting;
			g_ConfigManager.StandartCharactersAnimationDelay = g_OptionsConfig.StandartCharactersAnimationDelay;
			g_ConfigManager.StandartItemsAnimationDelay = g_OptionsConfig.StandartItemsAnimationDelay;
			g_ConfigManager.LockResizingGameWindow = g_OptionsConfig.LockResizingGameWindow;
			g_ConfigManager.LockGumpsMoving = g_OptionsConfig.LockGumpsMoving;
			
			break;
		}
		case 8: //Reputation System
		{
			g_ConfigManager.InnocentColor = g_OptionsConfig.InnocentColor;
			g_ConfigManager.FriendlyColor = g_OptionsConfig.FriendlyColor;
			g_ConfigManager.SomeoneColor = g_OptionsConfig.SomeoneColor;
			g_ConfigManager.CriminalColor = g_OptionsConfig.CriminalColor;
			g_ConfigManager.EnemyColor = g_OptionsConfig.EnemyColor;
			g_ConfigManager.MurdererColor = g_OptionsConfig.MurdererColor;
			g_ConfigManager.CriminalActionsQuery = g_OptionsConfig.CriminalActionsQuery;

			break;
		}
		case 9: //Miscellaneous
		{
			g_ConfigManager.ShowIncomingNames = g_OptionsConfig.ShowIncomingNames;
			g_ConfigManager.UseCircleTrans = g_OptionsConfig.UseCircleTrans;
			g_ConfigManager.StatReport = g_OptionsConfig.StatReport;
			g_ConfigManager.ConsoleNeedEnter = g_OptionsConfig.ConsoleNeedEnter;
			g_ConfigManager.CircleTransRadius = g_OptionsConfig.CircleTransRadius;
			g_ConfigManager.SkillReport = g_OptionsConfig.SkillReport;
			g_ConfigManager.SpeechFont = g_OptionsConfig.SpeechFont;

			g_CircleOfTransparency.Create(g_ConfigManager.CircleTransRadius);

			break;
		}
		case 10: //Filter Options
		{
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
const char *CGumpOptions::m_HotkeyText[0x100] =
{
	"",         //0x00
	"Left Mouse",   //0x01
	"Right Mouse",  //0x02
	"Ctrl-Break",   //0x03
	"Middle Mouse", //0x04
	"0x05",         //0x05
	"0x06",         //0x06
	"0x07",         //0x07
	"Backspace",    //0x08
	"Tab",          //0x09
	"0x0a",         //0x0a
	"0x0b",         //0x0b
	"Clear",        //0x0c
	"Enter",        //0x0d
	"0x0e",         //0x0e
	"0x0f",         //0x0f
	"Shift",        //0x10
	"Ctrl",         //0x11
	"Alt",          //0x12
	"Pause",        //0x13
	"Caps Lock",    //0x14
	"0x15",         //0x15
	"0x16",         //0x16
	"0x17",         //0x17
	"0x18",         //0x18
	"0x19",         //0x19
	"0x1a",         //0x1a
	"Esc",          //0x1b
	"0x1c",         //0x1c
	"0x1d",         //0x1d
	"0x1e",         //0x1e
	"0x1f",         //0x1f
	"Space",        //0x20
	"Page Up",      //0x21
	"Page Down",    //0x22
	"End",          //0x23
	"Home",         //0x24
	"Left Arrow",   //0x25
	"Up Arrow",     //0x26
	"Right Arrow",  //0x27
	"Down Arrow",   //0x28
	"Select",       //0x29
	"OEM 2A",       //0x2a
	"Execute",      //0x2b
	"Print Screen", //0x2c
	"Ins",          //0x2d
	"Del",          //0x2e
	"Help",         //0x2f
	"0",            //0x30
	"1",            //0x31
	"2",            //0x32
	"3",            //0x33
	"4",            //0x34
	"5",            //0x35
	"6",            //0x36
	"7",            //0x37
	"8",            //0x38
	"9",            //0x39
	"0x3a",         //0x3a
	"0x3b",         //0x3b
	"0x3c",         //0x3c
	"0x3d",         //0x3d
	"0x3e",         //0x3e
	"0x3f",         //0x3f
	"0x40",         //0x40
	"A",            //0x41
	"B",            //0x42
	"C",            //0x43
	"D",            //0x44
	"E",            //0x45
	"F",            //0x46
	"G",            //0x47
	"H",            //0x48
	"I",            //0x49
	"J",            //0x4a
	"K",            //0x4b
	"L",            //0x4c
	"M",            //0x4d
	"N",            //0x4e
	"O",            //0x4f
	"P",            //0x50
	"Q",            //0x51
	"R",            //0x52
	"S",            //0x53
	"T",            //0x54
	"U",            //0x55
	"V",            //0x56
	"W",            //0x57
	"X",            //0x58
	"Y",            //0x59
	"Z",            //0x5a
	"Left Win",     //0x5b
	"Right Win",    //0x5c
	"Apps",         //0x5d
	"0x5e",         //0x5e
	"0x5f",         //0x5f
	"Num 0",        //0x60
	"Num 1",        //0x61
	"Num 2",        //0x62
	"Num 3",        //0x63
	"Num 4",        //0x64
	"Num 5",        //0x65
	"Num 6",        //0x66
	"Num 7",        //0x67
	"Num 8",        //0x68
	"Num 9",        //0x69
	"Num *",        //0x6a
	"Num +",        //0x6b
	"Separator",    //0x6c
	"Num -",        //0x6d
	"Num .",        //0x6e
	"Num /",        //0x6f
	"F1",           //0x70
	"F2",           //0x71
	"F3",           //0x72
	"F4",           //0x73
	"F5",           //0x74
	"F6",           //0x75
	"F7",           //0x76
	"F8",           //0x77
	"F9",           //0x78
	"F10",          //0x79
	"F11",          //0x7a
	"F12",          //0x7b
	"Wheel Click",	//"F13",	//0x7c
	"Wheel Up",		//"F14",	//0x7d
	"Wheel Down",	//"F15",	//0x7e
	"Mouse+ Forward",//"F16",	//0x7f
	"Mouse+ Back",	//"F17",	//0x80
	"F18",          //0x81
	"F19",          //0x82
	"F20",          //0x83
	"F21",          //0x84
	"F22",          //0x85
	"F23",          //0x86
	"F24",          //0x87
	"0x88",         //0x88
	"0x89",         //0x89
	"0x8a",         //0x8a
	"0x8b",         //0x8b
	"0x8c",         //0x8c
	"0x8d",         //0x8d
	"0x8e",         //0x8e
	"0x8f",         //0x8f
	"Num Lock",      //0x90
	"Scroll Lock",   //0x91
	"0x92",         //0x92
	"0x93",         //0x93
	"0x94",         //0x94
	"0x95",         //0x95
	"0x96",         //0x96
	"0x97",         //0x97
	"0x98",         //0x98
	"0x99",         //0x99
	"0x9a",         //0x9a
	"0x9b",         //0x9b
	"0x9c",         //0x9c
	"0x9d",         //0x9d
	"0x9e",         //0x9e
	"0x9f",         //0x9f
	"0xa0",         //0xa0
	"0xa1",         //0xa1
	"0xa2",         //0xa2
	"0xa3",         //0xa3
	"0xa4",         //0xa4
	"0xa5",         //0xa5
	"0xa6",         //0xa6
	"0xa7",         //0xa7
	"0xa8",         //0xa8
	"0xa9",         //0xa9
	"0xaa",         //0xaa
	"0xab",         //0xab
	"0xac",         //0xac
	"0xad",         //0xad
	"0xae",         //0xae
	"0xaf",         //0xaf
	"0xb0",         //0xb0
	"0xb1",         //0xb1
	"0xb2",         //0xb2
	"0xb3",         //0xb3
	"0xb4",         //0xb4
	"0xb5",         //0xb5
	"0xb6",         //0xb6
	"0xb7",         //0xb7
	"0xb8",         //0xb8
	"0xb9",         //0xb9
	";",            //0xba
	"=",            //0xbb
	",",            //0xbc
	"-",            //0xbd
	".",            //0xbe
	"/",            //0xbf
	"`",            //0xc0
	"0xc1",         //0xc1
	"0xc2",         //0xc2
	"0xc3",         //0xc3
	"0xc4",         //0xc4
	"0xc5",         //0xc5
	"0xc6",         //0xc6
	"0xc7",         //0xc7
	"0xc8",         //0xc8
	"0xc9",         //0xc9
	"0xca",         //0xca
	"0xcb",         //0xcb
	"0xcc",         //0xcc
	"0xcd",         //0xcd
	"0xce",         //0xce
	"0xcf",         //0xcf
	"0xd0",         //0xd0
	"0xd1",         //0xd1
	"0xd2",         //0xd2
	"0xd3",         //0xd3
	"0xd4",         //0xd4
	"0xd5",         //0xd5
	"0xd6",         //0xd6
	"0xd7",         //0xd7
	"0xd8",         //0xd8
	"0xd9",         //0xd9
	"0xda",         //0xda
	"[",            //0xdb
	"\\",            //0xdc
	"]",            //0xdd
	"'",            //0xde
	"OEM DF",       //0xdf
	"OEM E0",       //0xe0
	"OEM E1",       //0xe1
	"OEM E2",       //0x32
	"OEM E3",       //0xe3
	"OEM E4",       //0xe4
	"0xe5",         //0xe5
	"OEM E6",       //0xe6
	"0xe7",         //0xe7
	"0xe8",         //0xe8
	"OEM E9",       //0xe9
	"OEM EA",       //0xea
	"OEM EB",       //0xeb
	"OEM EC",       //0xec
	"OEM ED",       //0xed
	"OEM EE",       //0xee
	"OEM EF",       //0xef
	"OEM F0",       //0xf0
	"OEM F1",       //0xf1
	"OEM F2",       //0xf2
	"OEM F3",       //0xf3
	"OEM F4",       //0xf4
	"OEM F5",       //0xf5
	"Attn",         //0xf6
	"Crsel",        //0xf7
	"Exsel",        //0xf8
	"Erase EOF",    //0xf9
	"Play",         //0xfa
	"Zoom",         //0xfb
	"0xfc",         //0xfc
	"PA1",          //0xfd
	"Clear",        //0xfe
	"0xff"          //0xff
};
//----------------------------------------------------------------------------