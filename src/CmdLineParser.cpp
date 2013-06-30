#include "CmdLineParser.hpp"
#include <string>
#include <vector>
#include <algorithm>

class CmdLineParserImpl
{
public:
    CmdLineParserImpl()
    {
    }

    void Parse(int argc, const wchar_t** argv)
    {
        m_fileNames.clear();

        for(int i=0; i<argc; ++i)
        {
            m_fileNames.push_back(argv[i]);
        }
    }

    size_t GetFileNameCount() const
    {
        return m_fileNames.size();
    }

    const wchar_t* GetFileName(size_t i) const
    {
        return (i<m_fileNames.size()) ? m_fileNames[i].c_str() : NULL;
    }

    void ConvertBackSlashes()
    {
        for(int i=0; i<m_fileNames.size(); ++i)
        {
            std::wstring &s = m_fileNames[i];
            std::replace( s.begin(), s.end(), L'/', L'\\'); // replace all 'x' to 'y'
        }
    }

private:
    typedef std::vector<std::wstring> TWStrVector;
    TWStrVector m_fileNames;
};

CmdLineParser::CmdLineParser()
{
    pImpl = new CmdLineParserImpl();
}

CmdLineParser::~CmdLineParser()
{
    delete pImpl;
    pImpl = NULL;
}

void CmdLineParser::Parse(int argc, const wchar_t** argv)
{
    pImpl->Parse(argc, argv);
}

size_t CmdLineParser::GetFileNameCount() const 
{
    return pImpl->GetFileNameCount();
}

const wchar_t* CmdLineParser::GetFileName(size_t i) const 
{ 
    return pImpl->GetFileName(i); 
}

void CmdLineParser::ConvertBackSlashes()
{
    pImpl->ConvertBackSlashes();
}

