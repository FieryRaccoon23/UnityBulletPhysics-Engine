#include "SharedMemory/SharedMemory.h"

int _tmain()
{
    while (true)
    {
        if (!SharedMemory::IfMapOpened())
        {
            SharedMemory::OpenShareFileMapping();
        }
        else
        {
            SharedMemory::ReadData();
        }
    }

    SharedMemory::UnMapFileMapping();
    return 0;
}