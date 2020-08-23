#include "SharedMemory.h"
#include "BulletVisualDebugger/VulkanVisualDisplay/VisualDebugInfo.h"
#include <iostream>
#include <string>

TCHAR memMapName[] = TEXT("VisualDebuggerMemMap");

bool SharedMemory::IfMapOpened()
{
    return m_IsMapOpen;
}

void SharedMemory::OpenShareFileMapping()
{
    m_HMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   
        FALSE,                 
        memMapName);

    if (m_HMapFile == NULL)
    {
        /*_tprintf(TEXT("Could not open file mapping object (%d).\n"),
            GetLastError());*/
        return;
    }

    m_PBuf = (struct VisualData*)MapViewOfFile(m_HMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(VisualData));

    if (m_PBuf == NULL)
    {
        /*_tprintf(TEXT("Could not map view of file (%d).\n"),
            GetLastError());*/

        CloseHandle(m_HMapFile);

        return;
    }

    m_IsMapOpen = true;
}

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

    m_IsMapOpen = true;
}

void SharedMemory::UnMapFileMapping()
{
    if (m_PBuf)
    {
        UnmapViewOfFile(m_PBuf);
    }
    if (m_HMapFile)
    {
        if (CloseHandle(m_HMapFile) == 0)
        {
            _tprintf(TEXT("Could not close the map handle (%d).\n"),
                GetLastError());
        }
    }

    m_PBuf = nullptr;
    m_IsMapOpen = false;
}

void SharedMemory::ReadData()
{
    if (m_PBuf && m_PBuf->state == static_cast<int>(VisualDebugState::DataPresent))
    {
        _tprintf(TEXT("State: %d.\n"),
            m_PBuf->state);
        _tprintf(TEXT("X: (%f), Y: (%f), Z: (%f).\n"),
            m_PBuf->data[0], m_PBuf->data[1], m_PBuf->data[2]);
        RefreshMemoryData();
    }
    else if (m_PBuf && m_PBuf->state == static_cast<int>(VisualDebugState::ClosingMemMap))
    {
        _tprintf(TEXT("Unity Stopped.\n"));
        RefreshMemoryData();
        UnMapFileMapping();
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
