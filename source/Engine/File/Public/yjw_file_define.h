#pragma once

class FileBlob
{
public:
    FileBlob(char* data, int size);
    void Release();
    bool Read(void* target, int size);
private:
    char* m_data = nullptr;
    int m_current_pos = 0;
    int m_size = 0;
};
