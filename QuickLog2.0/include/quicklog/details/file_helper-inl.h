#pragma once
#include <quicklog/details/tool.h>
#include <quicklog/common.h>
#include <quicklog/details/file_helper.h>

namespace quicklog{
inline file_helper::file_helper(std::string fileName)
    : m_fileName(fileName)
    {}
file_helper::~file_helper()
{
    close();
}
//(throw)
inline void file_helper::open(const std::string fileName)
{
    close();
    m_fileName = fileName;

    //auto* mode = SPDLOG_FILENAME_T("ab");
    //auto* trunc_mode = SPDLOG_FILENAME_T("wb");
    for (int tries = 0; tries < m_tries; ++tries) 
    {
        // 若目录不存在则创建
        if (getFile(m_fileName))
        {
#ifdef USE_FILE_CPP
            m_ofs.open(m_fileName,std::ios_base::app);
#else
            fopen_s(&m_pfile, m_fileName.c_str(), "ab");
#endif            
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
    }
    m_fileName = "";
    throw(log_exception(m_fileName+":文件打开失败"));  
}

inline void file_helper::reopen()
{
    if (m_fileName.empty()) 
    {
        throw(log_exception("文件重新打开失败 - 文件并未成功打开"));
    }
    this->open(m_fileName);
}
inline void file_helper::flush()
{
#ifdef USE_FILE_CPP
    m_ofs.flush();
#else
    std::fflush(m_pfile);
#endif  
}
inline void file_helper::close()
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
inline void file_helper::write(const std::string& content)
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
        std::fwrite(content.c_str(), 1, content_size, m_pfile);
    }


#endif  
    
}

inline size_t file_helper::fileSize() const
{
#ifdef USE_FILE_CPP
    if (!m_ofs.is_open())
    {
        throw(log_exception("Cannot use size() on closed file " + m_fileName));
}
    return getFileSize(m_fileName);
#else
    if (m_pfile == nullptr) 
    {
        throw(log_exception("Cannot use size() on closed file " + m_fileName));
    }
    return getFileSize(m_fileName);
#endif 
    
}
inline const std::string& file_helper::fileName() const
{ 
    return m_fileName; 
}
inline std::tuple<std::string, std::string> file_helper::split_EXT(const std::string& fileName)
{
    // treat cases like "/etc/rc.d/somelogfile or "/abc/.hiddenfile"
    std::string only_fileName(split_floder(fileName));
    auto EXT_pos = only_fileName.rfind('.');
    // finally - return a valid base and extension tuple
    return std::make_tuple(only_fileName.substr(0, EXT_pos), only_fileName.substr(EXT_pos));
}
}//namespace quicklog