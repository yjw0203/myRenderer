#include "Engine/Utils/Public/Serialize/FileArchive.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

FileWriterArchive::FileWriterArchive(const char* url)
{
    m_url = url;
}

FileWriterArchive::~FileWriterArchive()
{
}

void FileWriterArchive::SerializeInternal(void* data, int length)
{
    const char* byteData = static_cast<const char*>(data);
    int current = m_buffer.size();
    if (m_buffer.size() + length > m_buffer.capacity())
    {
        m_buffer.reserve(2 * m_buffer.capacity());
    }
    m_buffer.insert(m_buffer.end(), byteData, byteData + length);
}

void FileWriterArchive::close()
{
    if (m_buffer.empty())
    {
        return;
    }

    std::filesystem::path path(m_url);
    std::filesystem::create_directories(path.parent_path());

    FILE* file = fopen(m_url.c_str(), "wb");
    if (!file) {
        std::cout << std::string("open file failed!") + std::string(m_url) << std::endl;
        return;
    }
    else
    {
        if (!m_buffer.empty()) {
            fwrite(m_buffer.data(), 1, m_buffer.size(), file);
            m_buffer.clear();
        }

        fclose(file);
    }
}

FileReaderArchive::FileReaderArchive(const char* url)
{
    FILE* file = fopen(url, "rb");
    if (!file) {
        std::cout << std::string("open file failed!") + std::string(url) << std::endl;
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    m_buffer.resize(fileSize);
    size_t bytesRead = fread(m_buffer.data(), 1, fileSize, file);
    if (bytesRead != static_cast<size_t>(fileSize)) {
        fclose(file);
        std::cout << std::string("file read faild!") + std::string(url) << std::endl;
        return;
    }

    fclose(file);

    m_cursor = m_buffer.data();

    if (!m_cursor)
    {
        std::cout << std::string("file is empty!") + std::string(url) << std::endl;
        return;
    }
}

void FileReaderArchive::SerializeInternal(void* data, int length)
{
    if (!m_cursor) {
        return;
    }

    if (static_cast<size_t>(length) > static_cast<size_t>(m_buffer.data() + m_buffer.size() - m_cursor)) {
        return;
    }

    memcpy(data, m_cursor, length);
    m_cursor += length;
}
