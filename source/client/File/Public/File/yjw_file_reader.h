#pragma once
#include "yjw_file_define.h"

namespace yjw
{
    class FileReader
    {
    public:
        FileBlob ReadHoleFile(const char* url);
    };
}