#pragma once
#include "Module/yjw_module.h"
#include "yjw_file_reader.h"

namespace yjw
{
    class FileSystemModule : public ModuleInterface<FileSystemModule>
    {
    public:
        virtual void StartUp() override;
        virtual void ShutDown() override;
        virtual void ClientTick(float DeltaTime) override {};
        virtual const char* GetModuleName() override { return "FileSystemModule"; }

        FileReader& GetFileReader();
    private:
        FileReader m_file_reader;
    };
}
