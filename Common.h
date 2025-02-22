#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <vector>
#include <queue>
#include <algorithm>
//#include <windows.h>


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object() = 0;
	
	virtual void Act() = 0;
	virtual void Draw(HDC hdc, RECT &paint_area) = 0;
};
//------------------------------------------------------------------------------------------------------------
class AString
{
public:
	AString();
	AString(const wchar_t *str);

	void Append(int value, int digits);
	void Append(wchar_t symbol);
	void Delete_Last_Symbol();
	const wchar_t *Get_Content();
	int Get_Length();
	void Clear();

private:
	std::wstring Content;
};
//------------------------------------------------------------------------------------------------------------
enum class EMessage_Type: unsigned char
{
	Mirror_Allowed,
	Catched_Fish,
	Level_Done
};
//------------------------------------------------------------------------------------------------------------
class AMessage
{
public:
	AMessage(EMessage_Type message_type);

	const EMessage_Type Message_Type;
};
//------------------------------------------------------------------------------------------------------------
class AsMessage_Manager
{
public:
	static void Add_Message(AMessage *message);
	static bool Get_Message(AMessage **message);

private:
	static std::queue<AMessage *> Messages_Queue;
};
//------------------------------------------------------------------------------------------------------------