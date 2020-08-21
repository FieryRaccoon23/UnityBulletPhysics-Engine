#pragma once

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <array>

#define CAMERA_PARAMETERS 12

struct VisualData {
	int state;
	std::array<float, CAMERA_PARAMETERS> data;
};

struct SharedMemory
{
public:
	static void CreateShareFileMapping();
	static void UnMapFileMapping();
	static void ReadData();
	static void RefreshMemoryData();
private:
	static HANDLE m_HMapFile;
	static VisualData* m_PBuf;
};