/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "utils.h"

#include <fstream>
#include <string>
#include <sys/time.h>
#include <cstring>

int64 Utils::writeAll(const std::string &filename, uchar *buffer, uint size) {
    std::ofstream out;
    out.open(filename.c_str(), std::ios::out | std::ios::binary);
    if (out.good() && out.is_open()) {
        out.write((char*)buffer, size);
        if (out.bad()) {
            out.close();
            return -1;
        }
        return size;
    }
    return -1;
}

int64 Utils::readAll(const std::string &filename, uchar *buffer, uint size) {
    int64 read = 0;
    std::ifstream in;
    in.open(filename.c_str(), std::ios::in | std::ios::binary);
    if (in.good() && in.is_open()) {
        while (!in.eof() && in.read((char*)(buffer+read), size-read)) {
            int r  = in.gcount();
            if (r < 0) {
                break;
            }
            read += r;
        }
        in.close();
        return read;
    }
    return -1;
}

std::string Utils::formatSerialNumber(uchar *p) {
    static const char *DIGITS = "0123456789ABCDEF";
    std::string sn(16, '\0');
    for(int i = 0; i < 8; i++) {
        int pi = p[i];
        sn[i*2  ] = DIGITS[pi/16];
        sn[i*2+1] = DIGITS[pi%16];
    }
    return sn.substr(0, 8) + "-" + sn.substr(8);
}

int64 Utils::currentMillis() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (int64) ( ((int64)tp.tv_sec)*1000 + ((int64)tp.tv_usec)/1000 );
}

std::string Utils::format(const char *fmt, int x1) {
    char *buffer = new char[strlen(fmt) + 20];
    sprintf(buffer, fmt, x1);
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::format(const char *fmt, int x1, int x2) {
    char *buffer = new char[strlen(fmt) + 40];
    sprintf(buffer, fmt, x1, x2);
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::format(const char *fmt, int x1, int x2, int x3) {
    char *buffer = new char[strlen(fmt) + 60];
    sprintf(buffer, fmt, x1, x2, x3);
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::format(const char *fmt, std::string x1) {
    char *buffer = new char[strlen(fmt) + x1.size()];
    sprintf(buffer, fmt, x1.c_str());
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::format(const char *fmt, std::string x1, std::string x2) {
    char *buffer = new char[strlen(fmt) + x1.size() + x2.size()];
    sprintf(buffer, fmt, x1.c_str(), x2.c_str());
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::format(const char *fmt, std::string x1, std::string x2, std::string x3) {
    char *buffer = new char[strlen(fmt) + x2.size() + x3.size()];
    sprintf(buffer, fmt, x1.c_str(), x2.c_str(), x3.c_str());
    std::string s = std::string(buffer);
    delete[] buffer;
    return s;
}

std::string Utils::toLower(const std::string &_s) {
    std::string s = _s;
    size_t n = s.size();
    for(size_t i = 0; i < n; i++) {
        if (isalpha(s[i])) {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}

std::string Utils::toUpper(const std::string &_s) {
    std::string s = _s;
    size_t n = s.size();
    for(size_t i = 0; i < n; i++) {
        if (isalpha(s[i])) {
            s[i] = toupper(s[i]);
        }
    }
    return s;
}

bool Utils::endsWith(const std::string &s, const std::string &suffix) {
    if (suffix.size() > s.size()) {
        return false;
    }

    size_t m = s.size();
    size_t n = suffix.size();

    for (size_t i = 0; i < n; i++) {
        if (s[m-n-1+i] != suffix[i]) {
            return false;
        }
    }

    return true;
}
