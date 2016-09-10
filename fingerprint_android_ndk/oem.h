#ifndef OEM_H
#define OEM_H

#include "config.h"
#include "commbase.h"
#include "oemp.h"
#include "utils.h"

#include <cstdio>

/**
 * All the different possible commands to be senf to the fingerpring, as stated in its datasheet.
 */
enum {
    CMD_NONE                = 0x00,
    CMD_OPEN                = 0x01,
    CMD_CLOSE               = 0x02,
    CMD_USB_INTERNAL_CHECK  = 0x03,
    CMD_CHANGE_BAUDRATE     = 0x04,

    CMD_CMOS_LED            = 0x12,

    CMD_ENROLL_COUNT        = 0x20,
    CMD_CHECK_ENROLLED      = 0x21,
    CMD_ENROLL_START        = 0x22,
    CMD_ENROLL1             = 0x23,
    CMD_ENROLL2             = 0x24,
    CMD_ENROLL3             = 0x25,
    CMD_IS_PRESS_FINGER     = 0x26,

    CMD_DELETE              = 0x40,
    CMD_DELETE_ALL          = 0x41,

    CMD_VERIFY              = 0x50,
    CMD_IDENTIFY            = 0x51,
    CMD_VERIFY_TEMPLATE     = 0x52,
    CMD_IDENTIFY_TEMPLATE   = 0x53,

    CMD_CAPTURE             = 0x60,

    CMD_GET_IMAGE           = 0x62,
    CMD_GET_RAWIMAGE        = 0x63,

    CMD_GET_TEMPLATE        = 0x70,
    CMD_ADD_TEMPLATE        = 0x71,
    CMD_GET_DATABASE_START  = 0x72,
    CMD_GET_DATABASE_END    = 0x73,

    CMD_FW_UPDATE           = 0x80,
    CMD_ISO_UPDATE          = 0x81,

    ACK_OK                  = 0x30,
    NACK_INFO               = 0x31,
};

/**
 * All different possible acknowledge responses to be received from the fingerprint sensor, as
 * stated by its datasheed.
 */
enum {
        NACK_NONE                = 0x1000,
        NACK_TIMEOUT,
        NACK_INVALID_BAUDRATE,
        NACK_INVALID_POS,
        NACK_IS_NOT_USED,
        NACK_IS_ALREADY_USED,
        NACK_COMM_ERR,
        NACK_VERIFY_FAILED,
        NACK_IDENTIFY_FAILED,
        NACK_DB_IS_FULL,
        NACK_DB_IS_EMPTY,
        NACK_TURN_ERR,
        NACK_BAD_FINGER,
        NACK_ENROLL_FAILED,
        NACK_IS_NOT_SUPPORTED,
        NACK_DEV_ERR,
        NACK_CAPTURE_CANCELED,
        NACK_INVALID_PARAM,
        NACK_FINGER_IS_NOT_PRESSED,
};

/**
 * Handy enum for passing erros at application level (the fingerprint does not use this enum).
 */
enum {
        OEM_NONE                    = -2000,
        OEM_COMM_ERR,
};

/**
  * Holds some the information of the fingerprint sensor.
  */
typedef struct _devinfo {
    uint FirmwareVersion;
    uint IsoAreaMaxSize;
    uchar DeviceSerialNumber[16];
}devinfo;

#define FP_MAX_USERS        200
#define FP_TEMPLATE_SIZE    498
#define EEPROM_SIZE         16
#define FP_TEMPLATE_DB_SIZE (FP_TEMPLATE_SIZE * (FP_MAX_USERS+1) + (FP_MAX_USERS+1))

#define IMG8BIT_SIZE       256*256

/**
 * @brief The Oem class implements the high level communication with the fingerprint sensor, using
 * its communication protocol (see Oemp class).
 *
 * The lifecycle of a tipycal applications using this fingerprint sensor communication is as
 * follows:
 *
 * 1. Plug the sensor to the computer.
 * 2. Open the serial port.
 * 3. Start the communication protocol (this sets the desired baudrate automatically).
 * 4. Send commands (this implies receiving an ack).
 * 5. Reading back the result (if the ack was OK).
 * 6. Goto 4 if the termination condition was not met.
 * 7. Stop the communication protocol.
 * 8. Close the serial port.
 * 9. At this point the sensor can be unplugged safely.
 *
 * In general, the methods in this class will yield an integer result, with a value of 0 for
 * succesfull execution of commands and < 0 for errors (communication error, non good ack responses,
 * non good packet headers, etc). For checking the responses, it can be against the gwLastAckParam
 * which contains results for OK acks (like getting the number of users enrolled, if the finger is
 * pressed and small responses like those). For getting large results like getting images, the
 * corresponding variable should automatically be populated by this class.
 *
 * All methods in this class use the gwLastAck to indicate good/non good response. It is
 * responsability of the calling code to check whether the response was OK and handle the error
 * properly.
 */
class Oem {

public:
    uchar    gbyImg8bit[IMG8BIT_SIZE];          // Image data of a pre-processed image taken from the sensor (smaller but unaccurate)
    uchar    gbyImgRaw[320*240];                // Image data of a raw image taken from the sensor           (bigger but accurate)
    uchar    gbyTemplate[FP_TEMPLATE_SIZE];     // Holds the template (simplified fingerprint data) of just one fingerprint
    uchar    gbyTemplateDB[FP_TEMPLATE_DB_SIZE];// Holds the templates of every fingerprint (200)

    ushort  gwDevID;        // Id of the fingerprint sensor (defaults to 1)
    ushort  gwLastAck;      // Acnowledge response from the commucation protocol
    int     gwLastAckParam; // Additional data for the acnowledge response
    devinfo gDevInfo;       // Fingerprint factory information

private:
    /**
     * @brief oemp implements the low level communication protocol (sends and reads packets of
     * information using protocol specified in the datasheet).
     */
    Oemp oemp;

    /**
     * Listener to be executed whenever this class believes the GUIs should be refreshed.
     */
    void (*m_listener)(void* param);

    /**
     * @brief m_parameter memory address to be passed to the listener when it gets executed.
     */
    void *m_parameter;

    /**
     * @brief commandRun executes a generic command in the underlying serial communication. This is
     * basically a chunk of bytes being sent to the device, using serial communication. After
     * executing this method successfully, a packquet should be read back, with an ack response.
     * @param wCmd command to be send to the device, as stated in its datasheet.
     * @param nCmdParam additional arguments for the command being sent.
     * @return 0 if the command was executed successfully (succesfully sent to the device) and < 0
     * for an error of any kind.
     */
    int commandRun(ushort wCmd, int nCmdParam);

public:
    /**
     * @brief Oem constructs a new instance of Oem. Make sure to setting it up and opening its
     * serial communication before using it.
     * @param
     */
    Oem(CCommSerial *serial_port);

    /**
     * @brief setCallback specifies a custom function and parameter to be executed for refreshing
     * the UI during I/O operations. Using a NULL value will disable it.
     * @param parameter optional parameter to be passed when the listener gets executed. It may be
     * NULL.
     */
    void setCallback(void(*listener)(void*p), void *parameter);

    /**
     * @brief openPort opens the underlying serial communication with the specified port and
     * baudrate speed.
     * @param port port (on Windows it's similar to COM1, COM2 and on UNIX it's similar to
     * /dev/tty1, /dev/tty2).
     * @param baudrate speed for the serial connection (one of 9600, 19200, 38400, 57600 or 115200).
     * @return 0 if the serial port was open successfully or < 0 if an error ocurred.
     */
    int openPort(const std::string &port, uint baudrate);

    /**
     * @brief closePort closes the underlying serial connection.
     * @return 0 if the connection was succesfully closed or < 0 if an error occurred.
     */
    int closePort();

    /**
     * @brief openOem starts the communication with the sensor (it's mandatory to first open the
     * serial port, and then, start the communication protocol).
     * @return 0 if the device is setup correctly and can start receiving commands or < 0 if a
     * communication error occurred.
     */
    int openOem();

    /**
     * @brief closeOem stops the communication with the fingerprint sensor (this is usually issued
     * before closing the serial communication). Make sure to close the communication protocol
     * BEFORE closing the physical connection of the underlying serial device.
     * @return
     */
    int closeOem();

    /**
     * @brief usbInternalCheck performs some kind of interaction for USB mode operation. This USB
     * mode is not supported by this software (it realys only on serial communication).
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int usbInternalCheck();

    /**
     * @brief changeBaudrate changes the speed of the communication of the fingerprint. By default
     * is set to 9600 but it may be changed to allow faster data transfers (which is necessary for
     * downloading images fro mthe sensor to a computer). The first time is necessary to connect at
     * the current speed (9600), issue a command change the desired speed and then, reconnect to the
     * device using that speed.
     * @param nBaudrate speed in bauds of the communication between sensor and computer (one of
     * 9600, 19200, 38400, 57600 or 115200)
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int changeBaudrate(int nBaudrate);

    /**
     * @brief cmosLed turns on/off the backlit light led of the sensor.
     * @param bOn true for turn on the blue led of the sensor, or false to turn it off.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int cmosLed(bool bOn);

    /**
     * @brief enrollCount queries the fingerprint sensor for the count of enrolled users in its
     * database. The response value can be found in gwLastAckParam and it ranges from 0 minimum to
     * 200 maximum.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int enrollCount();

    /**
     * @brief checkEnrolled tells whether the specified id is enrolled in the fingerprint sensor
     * internal database.
     * @param nPos position of the user to be checked against the database (only 0<=nPos<=199).
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int checkEnrolled(int nPos);

    /**
     * @brief enrollStart starts the enrolling procedure for a fingerprint. Make sure to use an
     * unocuppied id, by using the checkEnrolled method first.
     * @param nPos id which the fingerprint is going to be associated to, after the enrollment
     * finishes correctly.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (or the id was already assigned to a previous fingerprint).
     */
    int enrollStart(int nPos);

    /*AVW*/
    /**
     * @brief enrollNth part of the enrollment procedure, which registers the first, second and
     * third fingerprint for building the template which the sensor is going to store in its
     * internal database.
     * @param nPos Id of the user being enrolled (the same one used for the enrollStart process).
     * @param nTurn 1, 2 or 3, which indicates one of the three fingerprints used to build the
     * template used for registering a user.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (or the id was already assigned to a previous fingerprint).
     */
    int enrollNth(int nPos, int nTurn);

    /**
     * @brief isPressFinger tells whether there's a finger pressed against the fingerprint sensor,
     * which is useful to know whether start capturing a fingerprint for processing it (or verifying
     * or identifying or whatever).
     * @return if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int isPressFinger();

    /**
     * @brief deleteId deletes the fingerprint from the database, associated with the specified
     * user. Make sure first to check whether the user is enrolled using the checkEnrolled method.
     * @param nPos id to be deleted from the fingerprint internal database.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (or the id was not assigned to any fingerprint).
     */
    int deleteId(int nPos);

    /**
     * @brief deleteAll deletes the whole internal fingerprint database. After this, the enroll
     * count is reset to 0 and to perform verifying and identification, at least one user must be
     * enrolled first.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int deleteAll();

    /**
     * @brief verify checks whether the fingerprint being taken by the sensor matches with the
     * specified id, by looking at the internal database.
     * @param nPos id to be matched against to in the internal database. THe phisycal fingerprint
     * must match with the template in this position in the internal database.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (independent of whether the fingerprint matches or not).
     */
    int verify(int nPos);

    /**
     * @brief identify retrieves the id of the physical fingerprint currently being taken by the
     * sensor.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int identify();

    /**
     * @brief verifyTemplate checks whether the supplied template matches the id, by looking at the
     * internal fingerprint sensor database.
     * @param nPos id of the template to be matched against to in the internal database.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int verifyTemplate(int nPos);

    /**
     * @brief identifyTemplate retrieves the id of the supplied template data in the internal
     * database.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int identifyTemplate();

    /**
     * @brief capture issues a capturing command to take the fingerpint image and storing it in the
     * internal ram of the sensor.
     * @param bBest true, for more accurate image capturing (this is slower) and false for less
     * accurate image (this is faster).
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int capture(bool bBest);

    /**
     * @brief getImage issues a download of the sensor image. Make sure to call it after the capture
     * method, which prepares the sensor for the download (the fingerprint has to be acquired by the
     * sensor, first). Immediately after issuing this command, a heavy reading operation will be
     * performed, to download the image data. The data will be stored under the gbyImg8bit variable
     * with a 256x256 resolution image.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int getImage();

    /**
     * @brief getRawImage issues a download of the raw sensor image. Make sure to call it after the
     * capture method, which prepares the sensor for the download. Immediately after issuing this
     * command, a heavy reading operation will be performed, to download the raw image data. The
     * data will be stored under the gbyImgRaw variable with a 320x240 resolution image.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int getRawimage();

    /**
     * @brief getTemplate retrieves the template of the specified id. Make sure to check if the id
     * is enrolled by using checkEnrolled. The template will be stored under the gbyTemplate
     * variable.
     * @param nPos id of the template to be retrieved.
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (or the id was not associated in the internal database to any fingerprint).
     */
    int getTemplate(int nPos);

    /**
     * @brief addTemplate adds a new template using the specified id. Make sure to check the id is
     * not associated with any previous fingerprint by using the checkEnrolled method.
     * @param nPos id which the template is going to be associated to (taken from the gbyTemplate
     * variable).
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error
     * (or the id was associated in the internal database to another fingerprint).
     */
    int addTemplate(int nPos);

    /**
     * Deprecated: this command exists for historical reasons, currently does nothing.
     * @brief getDatabaseStart
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int getDatabaseStart();

    /**
     * Deprecated: this command exists for historical reasons, currently does nothing.
     * @brief getDatabaseStart
     * @return 0 if the device answered with an OK ack, or < 0 if there was a communication error.
     */
    int getDatabaseEnd();

//    int fwUpgrade(uchar* pBuf, int nLen);
//    int isoUpgrade(uchar* pBuf, int nLen);

    static std::string error(int nNackInfo, int nPos);

    int capturing(bool best, std::string &err);

    int downloadingImage(std::string &err);

    int loadingImage(std::string &err);

    int loadingImageRaw(std::string &err);

    int loadingImageLive(std::string &err);
};

#endif // OEM_H
