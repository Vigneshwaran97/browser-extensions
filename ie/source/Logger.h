#ifndef __LOGGER_H__
#define __LOGGER_H__

using namespace ATL;

#define LOGGER_TIMESTAMP

#define INTRANET DWORD(1)
#define TRUSTED DWORD(2)
#define RESTRICTED DWORD(4)

#define INTRANET_ZONE 1
#define TRUSTED_SITES_ZONE 2
#define INTERNET_ZONE 3
#define RESTRICTED_SITES_ZONE 4

class Logger {
 public:
    enum Level {
        OFF = 0,
        CRITICAL,
        ERR,
        WARN,
        INFO,
        DBG,
        BG,
        FG,
        SYS,
        ALL
    };

    Logger(Level level, const std::wstring& filename = L"", const std::wstring& bgfilename = L"",
                        const std::wstring& fgfilename = L"", const std::wstring& sysfilename = L"",
                        const std::wstring& tablogfolder = L"");
    
    void initialize(const boost::filesystem::wpath& path);

    bool enabled;
    
    std::wstring info(const std::wstring& message) {
        write(message, Logger::INFO);
        return message;
    }
    std::wstring warn(const std::wstring& message) {
        write(message, Logger::WARN);
        return message;
    }
    std::wstring error(const std::wstring& message) {
        write(message, Logger::ERR);
        return message;
    }
    std::wstring debug(const std::wstring& message) {
        write(message, Logger::DBG);
        return message;
    }
    std::wstring logBackground(const std::wstring& message) {
        write(message, Logger::BG);
        return message;
    }
    std::wstring logForeground(const std::wstring& message) {
        write(message, Logger::FG);
        return message;
    }
    std::wstring logSystem(const std::wstring& message) {
        write(message, Logger::SYS);
        return message;
    }
    void logIESettings() {
        this->logSystem(L"---------- IE-MS Settings ----------");
        this->logIESetting(TEXT("Start Page"));
        this->logIESetting(TEXT("Enable Browser Extensions"));
        this->logSecurityFlags();
        this->logSecuritySites();
    }
    std::wstring logOnTab(const std::wstring& message, const std::wstring& onTabId) {
        writeOnTab(message, onTabId);
        return message;
    }
    void logIESetting(LPCTSTR hValueName);
    void logSecurityFlags();
    void logSecurityFlag(int zone);
    void logSecuritySites();

    // handy type parsers
    std::wstring parse(HRESULT hr);
    std::wstring parse(IDispatch *object);
    
    std::wstring GetLastError() {
        wchar_t *buf;
        DWORD error = ::GetLastError();
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                      (LPTSTR)&buf, 0, NULL);
        return std::wstring(buf);
    }
    

 private:
    void write(const std::wstring& message, Level level = Logger::DBG);
    void writeOnTab(const std::wstring& message, const std::wstring& onTabId);
    void logAllEnums(HKEY hKey);
    std::wstring getZoneName(const int zone);
    bool readRegistryW(HKEY hKey, LPCTSTR hValueName, LPBYTE dwReturn);
    std::wstring readPath(const wchar_t* pathname);
    std::wstring m_filename;
    std::wstring m_bgfilename;
    std::wstring m_fgfilename;
    std::wstring m_sysfilename;
    std::wstring m_tablogfolder;
    Level m_level;
    #ifdef LOGGER_TIMESTAMP
        double m_dAdjustment;
        LARGE_INTEGER m_llFreq;
        void timestamp(std::wofstream& fs);
        void timestampOnly(std::wofstream& fs);
    #endif // LOGGER_TIMESTAMP

 public:    
    typedef boost::shared_ptr<Logger> pointer;

    // helpers
    std::wstring stringify(TYPEDESC *typeDesc, ITypeInfo *pTypeInfo);
    std::wstring stringify(HREFTYPE refType, ITypeInfo *pti);
    std::wstring stringify(VARDESC *varDesc, ITypeInfo *pti);
    std::wstring stringify(int vt);

 protected:
    std::wstring limit(const std::wstring& s, size_t maxlen = 160) {
        size_t len = s.length();
        if (len < maxlen) {
            return s;
        }
        return s.substr(0, maxlen / 2) +  L" ... <schnip /> ... " + 
            s.substr(len - maxlen / 2);
    }
};


#endif /* __LOGGER_H__ */
