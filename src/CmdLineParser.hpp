#ifndef _CMDLINEPARSER_HPP_
#define _CMDLINEPARSER_HPP_

class CmdLineParser
{
public:
    CmdLineParser();
    ~CmdLineParser();

    void Parse(int argc, const wchar_t** argv);

    void ConvertBackSlashes();

    size_t GetFileNameCount() const;
    const wchar_t* GetFileName(size_t i) const;

private:
    class CmdLineParserImpl* pImpl;
};



#endif // _CMDLINEPARSER_HPP_
