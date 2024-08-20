#pragma once
#include <quicklog/common.h>
#include <quicklog/details/file_api.h>

#include <fstream>
namespace quicklog{

inline File_API::File_API(std::string path)
    : m_filePath(std::filesystem::absolute(path))
{
    
}
File_API::~File_API()
{
    close();
}
inline void File_API::setFilePath(const std::string path)
{
    m_filePath = std::filesystem::absolute(path);
}
inline void File_API::open()
{
    close();

    for (int tries = 0; tries < m_tries; ++tries)
    {
        //如果路径不存在则创建相应路径
        if (!std::filesystem::exists(m_filePath))
        {
            std::filesystem::path createDir(m_filePath.parent_path());
            std::filesystem::create_directories(createDir);
            std::ofstream createFile(m_filePath.string());
            if (!createFile.is_open())
            {
                throw(log_exception("file create falied!"));
            }
            createFile.close();
        }
        else
        {

#ifdef USE_FILE_CPP
            m_ofs.open(m_filePath.string(), std::ios_base::app);
            if (m_ofs.is_open())
                return;
#else
            fopen_s(&m_pfile, m_filePath.string().c_str(), "ab");
            if (m_pfile != nullptr)
                return;
#endif
            
        }       
        std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
    }
    throw(log_exception("file open failed!"));
    

}

inline void File_API::reopen()
{
    if (m_filePath.empty()) 
    {
        throw(log_exception("file reopen failed!"));
    }
    this->open();
}

inline void File_API::flush()
{
#ifdef USE_FILE_CPP
    m_ofs.flush();
#else
    std::fflush(m_pfile);
#endif  
}

inline void File_API::close()
{
#ifdef USE_FILE_CPP
    if (m_ofs.is_open())
    {
        m_ofs.close();
    }
#else
    if (m_pfile != nullptr) 
    {       
        std::fclose(m_pfile);
        m_pfile = nullptr;       
    }
#endif  
}

inline bool File_API::copy(const std::string newPath)
{
    auto copyPath(std::filesystem::absolute(newPath));
    if (!std::filesystem::exists(copyPath))
    {
        if (std::filesystem::is_directory(copyPath))
        {
            std::filesystem::path copy_createDir(copyPath.string());
            std::filesystem::create_directories(copy_createDir);
        }
        else
        {
            std::filesystem::path copy_createDir(copyPath.parent_path());
            std::filesystem::create_directories(copy_createDir);
        }
    }  
    std::filesystem::copy(m_filePath,copyPath);
}

inline bool File_API::exists(const std::string newPath)
{
    auto checkPath(std::filesystem::absolute(newPath));
    return std::filesystem::exists(checkPath);
}

inline void File_API::remove()
{
    std::filesystem::remove(m_filePath);
    m_filePath.clear();
}

inline void File_API::rename(const std::string newFileName)
{
    auto newPath(m_filePath.parent_path());
    newPath.append(newFileName);
    std::filesystem::rename(m_filePath, newPath);
}

inline void File_API::write(const std::string& content)
{
#ifdef USE_FILE_CPP
    if (!m_ofs.is_open())
    {
        return;
    }
    else
    {
        size_t content_size = content.size();
        m_ofs.write(content.c_str(), content_size);
    }
#else

    if (m_pfile == nullptr)
    {
        return;
    }
    else
    {
        size_t content_size = content.size();
        std::fwrite(content.c_str(), sizeof(char), content_size, m_pfile);
    }
#endif     
}

inline size_t File_API::size() const
{
#ifdef USE_FILE_CPP
    if (!m_ofs.is_open())
    {
        throw(log_exception("Cannot use size() on closed file"));
}
    return std::filesystem::file_size(m_filePath);
#else
    if (m_pfile == nullptr) 
    {
        throw(log_exception("Cannot use size() on closed file"));
    }
    return std::filesystem::file_size(m_filePath);
#endif 
    
}

inline std::string File_API::filePath() const
{
    return m_filePath.string();
}

inline std::string File_API::fileDir() const
{
    return m_filePath.parent_path().string();
}

inline std::string File_API::fileName() const
{
    return m_filePath.filename().string();
}

inline std::string File_API::fileStem() const
{
    return m_filePath.stem().string();
}

inline std::string File_API::fileEXT() const
{
    return m_filePath.extension().string();
}

}//namespace quicklog