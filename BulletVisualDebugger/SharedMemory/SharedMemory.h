#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <array>

#define CAMERA_PARAMETERS 12

enum class VisualDebugState
{
	Ready = 0,
	DataPresent,
	ClosingMemMap
};

struct VisualData {
	int state;
	std::array<float, CAMERA_PARAMETERS> data;
};

struct SharedMemory
{
public:
	static void CreateShareFileMapping();
	static void OpenShareFileMapping();
	static void UnMapFileMapping();
	static void ReadData();
	static void RefreshMemoryData();
	static bool IfMapOpened();

private:
	inline static HANDLE m_HMapFile = nullptr;
	inline static VisualData* m_PBuf = nullptr;
	inline static bool m_IsMapOpen = false;
};