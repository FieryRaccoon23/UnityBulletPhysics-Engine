#include "SharedMemory.h"
#include "BulletVisualDebugger/VulkanVisualDisplay/VisualDebugInfo.h"
#include <iostream>
#include <string>

TCHAR memMapName[] = TEXT("VisualDebuggerMemMap");

HANDLE SharedMemory::m_HMapFile = nullptr;
VisualData* SharedMemory::m_PBuf = nullptr;

void SharedMemory::CreateShareFileMapping()
{
    m_HMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    
        NULL,                    
        PAGE_READWRITE,          
        0,                       
        sizeof(VisualData),                
        memMapName);             

    if (m_HMapFile == NULL)
    {
        _tprintf(TEXT("Could not create file mapping object (%d).\n"),
            GetLastError());
        return;
    }

    m_PBuf = (struct VisualData*)MapViewOfFile(m_HMapFile,
        FILE_MAP_ALL_ACCESS, 
        0,
        0,
        sizeof(VisualData));

    if (m_PBuf == NULL)
    {
        _tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());

        CloseHandle(m_HMapFile);

        return;
    }
}

void SharedMemory::UnMapFileMapping()
{
    if (m_PBuf)
    {
        UnmapViewOfFile(m_PBuf);
    }
    if (m_HMapFile)
    {
        CloseHandle(m_HMapFile);
    }
}

void SharedMemory::ReadData()
{
    if (m_PBuf && m_PBuf->state != 0)
    {
        //std::cout << m_PBuf->data[0] << ", " << m_PBuf->data[1] << ", " << m_PBuf->data[2] << std::endl;
        RefreshMemoryData();
    }
}

void SharedMemory::RefreshMemoryData()
{
    if (m_PBuf)
    {
        m_PBuf->state = static_cast<int>(VisualDebugState::Ready);
        std::fill(std::begin(m_PBuf->data), std::end(m_PBuf->data), 0);
    }
}

bool AreSame(float a, float b)
{
    return fabs(a - b) < std::numeric_limits<float>::epsilon();
}
