#pragma once
#include "Engine/Utils/Public/Serialize/Archive.h"
#include <cstdio>
#include <vector>
#include <string>

namespace yjw
{
    class FileWriterArchive : public Archive
    {
    public:
        FileWriterArchive(const char* url);
        virtual ~FileWriterArchive();
        virtual void close() override;
        virtual ArchiveType GetArchiveType() { return ArchiveType::write; }
    private:
        virtual void SerializeInternal(void* data, int length) override;
        std::vector<char> m_buffer;
        std::string m_url;
    };

    class FileReaderArchive : public Archive
    {
    public:
        FileReaderArchive(const char* url);
        virtual ArchiveType GetArchiveType() { return ArchiveType::read; }
    private:
        virtual void SerializeInternal(void* data, int length) override;
        std::vector<char> m_buffer;
        char* m_cursor = nullptr;
    };
}