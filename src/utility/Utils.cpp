#include "Utils.h"
#include <string>
#include <cstdlib>

namespace vimcube::utility {
    
    OS getOperatingSystem()
    {
        #if defined(__APPLE__) && defined(__MACH__)
            return OS::MACOS;
        #elif defined(__linux__)
            return OS::LINUX;
        #elif defined(_WIN32) || defined(_WIN64)
            return OS::WINDOWS;
        #else
            return;
        #endif
    }

    bool openBuyMeaCoffee()
    {
        vimcube::utility::OS os = vimcube::utility::getOperatingSystem();
        if (os == vimcube::utility::OS::MACOS)
            std::system("open https://buymeacoffee.com/huyeong");
        else if (os == vimcube::utility::OS::LINUX)
            std::system("xdg-open https://buymeacoffee.com/huyeong");
        else if (os == vimcube::utility::OS::WINDOWS)
        {
            std::string command = "start \"\" \"https://buymeacoffee.com/huyeong\"";
            std::system(command.c_str());
        }
        else return false;
        return true;
    }
}
