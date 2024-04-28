#include "Public/File/yjw_file_reader.h"
#include <fstream>

namespace yjw
{
    FileBlob::FileBlob(char* data, int size)
    {
        m_data = data;
        m_size = size;
    }

    void FileBlob::Release()
    {
        if (m_data)
        {
            free(m_data);
        }
    }

    bool FileBlob::Read(void* target, int size)
    {
        if (m_data && m_current_pos + size < m_size)
        {
            memcpy(target, m_data + m_current_pos, size);
            m_current_pos += size;
            return true;
        }
        else
        {
            return false;
        }
    }

    FileBlob FileReader::ReadHoleFile(const char* url)
    {
        std::ifstream file(url, std::ios::binary);
        file.seekg(0, file.end);
        int fileSize = file.tellg();
        file.seekg(0, file.beg);
        char* data = (char*)malloc(fileSize);
        if (!file.read(data, fileSize))
        {
            auto rs = file.fail();
            free(data);
            data = nullptr;
            fileSize = 0;
        }
        file.close();

        return FileBlob(data, fileSize);
    }
}