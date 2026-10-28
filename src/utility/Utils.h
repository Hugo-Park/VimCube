#ifndef UTILS_H
#define UTILS_H

namespace vimcube::utility {

    enum class OS { LINUX, MACOS, WINDOWS };

    OS getOperatingSystem();
    bool openBuyMeaCoffee();
}
#endif
