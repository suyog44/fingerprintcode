/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "config.h"

#ifndef RS232_COMM_SERIAL_H
#define RS232_COMM_SERIAL_H

#ifdef RS232_SERIAL_COMM

#include "rs232.h"

#include "commbase.h"

/**
  * Given that the RS232 library requires an identifier for each open COM port, this is hardcoded
  * since this application does not open more than one connection at the same time.
  */
#define SINGLETON_PORT_ID 1

/**
 * @brief The Rs232CommSerial class implements a serial communication by using the external library
 * RS232 http://www.teuniz.net/RS-232/.
 *
 * This class implements the CCommSerial interface, see CCommSerial for more information on the
 * contract of this class.
 */
class Rs232CommSerial : public CCommSerial {
private:

    /**
     * @brief lastOpen id of the last open connection, which is useful to know whether the
     * connection is already open.
     */
    int lastOpen;

public:
    Rs232CommSerial();

    virtual ~Rs232CommSerial();

    virtual bool open(const std::string &port, uint dwBaudrate);

    virtual bool close();

    virtual int64 write(uchar *buffer, uint nSize);

    virtual int64 read(uchar *buffer, uint nSize);
};

#endif // RS232_SERIAL_COMM

#endif // RS232_COMM_SERIAL_H
