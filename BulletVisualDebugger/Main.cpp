#include "SharedMemory/SharedMemory.h"

int _tmain()
{
    SharedMemory::CreateShareFileMapping();
    SharedMemory::RefreshMemoryData();

    while (true)
    {
        SharedMemory::ReadData();
    }

    SharedMemory::UnMapFileMapping();
    return 0;
}