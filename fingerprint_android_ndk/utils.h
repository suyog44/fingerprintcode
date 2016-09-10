/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#ifndef UTILS_H
#define UTILS_H

#include "config.h"

#include <string>

class Utils {

public:
    static int64 writeAll(const std::string &filename, uchar *buffer, uint size);

    static int64 readAll(const std::string &filename, uchar *buffer, uint size);

    static std::string formatSerialNumber(uchar *serialNum);

    static int64 currentMillis();

    static std::string format(const char *fmt, int x);
    static std::string format(const char *fmt, int x1, int x2);
    static std::string format(const char *fmt, int x1, int x2, int x3);

    static std::string format(const char *fmt, std::string x);
    static std::string format(const char *fmt, std::string x1, std::string x2);
    static std::string format(const char *fmt, std::string x1, std::string x2, std::string x3);

    static std::string toLower(const std::string &s);
    static std::string toUpper(const std::string &s);
    static bool endsWith(const std::string &s, const std::string &suffix);
};

#endif // UTILS_H
