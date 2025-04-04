#include "Engine/Utils/Public/Serialize/Stream.h"
#include <fstream>

namespace yjw
{
    void Stream::LoadFromFile(char* url)
    {
        std::ifstream file(url, std::ios::binary);
        file.seekg(0, file.end);
        int fileSize = file.tellg();
        file.seekg(0, file.beg);
        m_data.resize(fileSize);
        if (!file.read(m_data.data(), fileSize))
        {
            auto rs = file.fail();
            m_data.clear();
            file.close();
        }
        file.close();
    }

    void Stream::SaveToFile(char* url)
    {
        std::ofstream file(url, std::ios::binary);
        if (file.write(m_data.data(), m_data.size()))
        {
            auto rs = file.fail();
            file.close();
        }
        file.close();
    }
}