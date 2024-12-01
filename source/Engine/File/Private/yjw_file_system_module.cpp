#include "Engine/File/Public/yjw_file_system_module.h"

namespace yjw
{
    void FileSystemModule::StartUp()
    {

    }

    void FileSystemModule::ShutDown()
    {

    }

    FileReader& FileSystemModule::GetFileReader()
    {
        return m_file_reader;
    }
}
