/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#ifndef COMM_BASE_H
#define COMM_BASE_H

#include "config.h"

#include <string>

/**
 * @brief The CCommSerial class abstracts the serial communication using an specific library for it.
 *
 * The CCommSerial class offers an OS independant interface to provide the classical methods of
 * serial communication; that is, read bytes, write bytes, open, close, etc.
 *
 */

class CCommSerial {

protected:
    /**
     * @brief timeOut time in milliseconds for the I/O operations to be considered failed (the other
     * endpoint not responding, for instance).
     */
    uint timeOut;

    /**
     * Listener to be executed wheneve this class believes the UIs should be updated. If NULL, no
     * listener will be called at any moment.
     */
    void (*m_listener)(void* param);

    /**
     * @brief m_parameter parameter to be send with the listener execution. For customization
     * purposes.
     */
    void *m_parameter;

public:
    /**
     * @brief CCommSerial creates a new instance of the CCommSerial class. This instance has yet to
     * be configured and open to work properly.
     */
    CCommSerial();

    /**
     * @brief ~CCommSerial deallocates this instance of the CCommSerial class. It should perform a
     * close of the underlying device, and maybe, deallocate some ojects allocated dinamically.
     */
    virtual ~CCommSerial();

    /**
     * Deprecated: this method (and concept) is to be removed in future versions of this
     * application, in favor of multi threading.
     *
     * @brief setCallback sets a listener function to be executed to perform updates to the GUI. It
     * will be called many times, before and after I/O, as well as near any heavy operations (to
     * try to keep UIs as responsive as possible).
     * @param parameter optional parameter to be send along with the listener execution, for custom
     * usage (like specifying a sender or )
     */
    void setCallback(void(*listener)(void*p), void *parameter);

    /**
     * @brief setTimeout sets a time in milliseconds for I/O operations to be considered failed.
     * @param dwTimeOut Time in milliseconds for any I/O operation of this object to be considered
     * failed, and thus, aborted. Any timed out operation will be aborted and any bytes read or
     * written may be lost. It is responsability of the calling code to check whether the I/O
     * operation was succeded or failed, by inspecting how many bytes were transferred.
     */
    void setTimeout(uint dwTimeOut);

    /**
     * @brief open opens and prepares this CCommSerial object to perform I/O operations.
     * @param port port of the device for connecting (in Windows environments, it's similar to COM1
     * or COM2).
     * @param dwBaudrate speed of the serial communication.
     * @return true if the device was open successfully, otherwise false.
     */
    virtual bool open(const std::string &port, uint dwBaudrate) = 0;

    /**
     * @brief close closes this CCommSerial object, to disable I/O operations of the underlying
     * device.
     * @return true if the object was open and it got closed. If it was unopened, returns false.
     */
    virtual bool close() = 0;

    /**
     * @brief write sends bytes through the underlying device and reports a count of how many bytes
     * were successfully transferred. This method tries to send all the specified bytes, until all
     * of them are sent or the timeout time has passed, whatever happens first.
     * @param buffer memory address where the bytes to be send are to be taken. It should point to a
     * valid memory address, of at least nSize reserved bytes.
     * @param nSize number of bytes to be send.
     * @return number of bytes successfully send through the underlying device, which may be less
     * than the specified nSize value. If the value returned is smaller than nSize, it means an I/O
     * error ocurred and the sending was aborted. If it's equal, it can be considered a successfull
     * operation.
     */
    virtual int64 write(uchar *buffer, uint nSize) = 0;

    /**
     * @brief read receives bytes through the underlying device and reports a count of how many
     * bytes were successfully transferred. This method tries to read all the specified bytes, until
     * all of them are received or the timeout time has passed, whatever happens first.
     * @param buffer memory address where the received bytes are to be placed. It should point to a
     * valid memory address containing at least nSize reserved bytes.
     * @param nSize number of bytes to read.
     * @return  number of bytes successfully read from the underlying device, which may be less than
     * the specified nSize value. If the value returned is smaller than nSize, it means an I/O error
     * ocurred and the reading of the remaining bytes was aborted. If it's the same, it can be
     * considered a successfull operation.
     */
    virtual int64 read(uchar *buffer, uint nSize) = 0;
};

#endif // COMM_BASE_H
