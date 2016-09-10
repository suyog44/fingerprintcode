/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#ifndef CONFIG_H
#define CONFIG_H

typedef signed char int8;          /* 8 bit signed */
typedef unsigned char uint8;       /* 8 bit unsigned */
typedef unsigned char uchar;       /* 8 bit signed */

typedef short int16;               /* 16 bit signed */
typedef unsigned short uint16;     /* 16 bit unsigned */
typedef unsigned short ushort;     /* 16 bit unsigned */

typedef int int32;                 /* 32 bit signed */
typedef unsigned int uint32;       /* 32 bit unsigned */
typedef unsigned int uint;         /* 32 bit unsigned */

typedef long long int64;           /* 64 bit signed */
typedef unsigned long long uint64; /* 64 bit unsigned */

#include <string>

// this is defined at compiler level, to avoid polluting other apps
//#define MODE_WINDOW


/**
  * If defined, enables verbose debug output for troubleshooting. Please disable it for console
  * applications as it will get mixed with other console outputs. For WINDOW_MODE it's okay, but it
  * may present performance issues because it will spit out huge amounts of data to the console.
  */
//#define OUTPUT_DEBUG

// this is defined at compiler level, to avoid polluting other apps
/**
  */
//#define ENABLED_QT5
//#define RS232_SERIAL_COMM

class Config {
public:
    std::string port;
    uint baudrate;

    uint id;

    std::string filename;

    uint action;
};

#endif // CONFIG_H
