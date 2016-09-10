#ifndef OEMP_H
#define OEMP_H

#include "config.h"
#include "commbase.h"

#include <string>

// Header Of Cmd and Ack Packets
#define STX1				0x55	//Header1 
#define STX2				0xAA	//Header2

// Header Of Data Packet
#define STX3				0x5A	//Header1 
#define STX4				0xA5	//Header2

// Structure Of Cmd and Ack Packets 
typedef struct {		
    uchar 	Head1;
    uchar 	Head2;
    ushort	wDevId;
    uint	nParam;
    ushort	wCmd;// or nAck
    ushort 	wChkSum;
}
// TODO: add portable macros for specifying the alignment of structures.
__attribute__((packed))
SB_OEM_PKT;

#define SB_OEM_PKT_SIZE			12

#define SB_OEM_HEADER_SIZE		2
#define SB_OEM_DEV_ID_SIZE		2
#define SB_OEM_CHK_SUM_SIZE		2

#define PKT_ERR_START	-500
#define PKT_COMM_ERR	PKT_ERR_START+1
#define PKT_HDR_ERR		PKT_ERR_START+2
#define PKT_DEV_ID_ERR	PKT_ERR_START+3
#define PKT_CHK_SUM_ERR	PKT_ERR_START+4
#define PKT_PARAM_ERR	PKT_ERR_START+5
/**
 * @brief The Oemp class implements the low level communications protocol with the sensor. It
 * handles the sending and receiving of the data packets from and to the sensor. It performs as well
 * the checksum verification and opening and closing of the serial interface.
 *
 * The Oem class functionalities sits on top of this class.
 */
class Oemp {
private:
    /**
     * @brief calcChkSumOfCmdAckPkt calculates and returns the checksum verification for a command
     * packet.
     * @param pPkt address of the structure to calculate its checksum.
     * @return checksum (sum of the bytes) for the supplied structure.
     */
    static ushort calcChkSumOfCmdAckPkt(SB_OEM_PKT *pPkt);

    /**
     * @brief calcChkSumOfDataPkt calculates and returns the checksum verification for a data
     * packet.
     * @param pDataPkt memory address where the bytes of the data packet are stored. It should point
     * to a valid memory address of at least nSize bytes.
     * @param nSize number of bytes to be used in the checksum calculation.
     * @return check (sum of the bytes) for the supplied memory address.
     */
    static ushort calcChkSumOfDataPkt(uchar* pDataPkt, int nSize);

    /**
     * @brief serial_port communication object to send/receive bytes using serial communication.
     */
    CCommSerial *serial_port;

public:

    Oemp(CCommSerial *serial_port);

    /**
     * Deprecated: remove this stuff in favor of a multi threading design application.
     * @brief setCallback
     * @param parameter
     */
    void setCallback(void(*listener)(void*p), void *parameter);

    /**
     * @brief open opens the physical connection tunnel for communicating with the sensor.
     * @param port name of the port where this app is going to be talking to the sensor (on Windows
     * machines, it may be COM1, COM2, etc. and in UNIX ones, it will be /dev/tty1, /dev/tty2 and
     * so).
     * @param baudrate speed of the serial connection.
     * @return 0 if the connection was open successfully, otherwise, < 0.
     */
    int open(const std::string &port, uint baudrate);

    /**
     * @brief close closes the underlying serial connection and I/O operations won't be able to be
     * performed anymore. This means no more talking between computer and sensor.
     * @return 0 if the connection was open and it was closed correctly. If it was unopened or some
     * sort of error occurred, it will yield a value < 0.
     */
    int close();

    int checkCmdAckPkt(ushort wDevID, SB_OEM_PKT* pPkt);

    /**
     * @brief writeCmdOrAck creates a new packet and sends it under an open connection in order to
     * wait for an ack response. The packet is build with its checksum values for the sensor to
     * validate it. The reading of the ack response is not handled by this method, it is
     * responsability of the calling code to do such.
     * @param wDevID device id of the fingerprint sensor. By contract always use a value of 1.
     * @param wCmd id of the command to execute, as stated as hex values in the fingerprint
     * datasheet.
     * @param nParam additional information by command.
     * @return 0 if the transfer of the bytes of the command was successfully finished, or < 0 if
     * some sort of error ocurred.
     */
    int writeCmd(ushort wDevID, ushort wCmd, uint nParam);

    /**
     * @brief readCmdOrAck reads an ack packet response, and stores its ack and additional
     * parameters in the supplied memory addresses. This method assumes a command packet was send in
     * first place (so, it's possible to read an ack packet response).
     * @param wDevID device id of the fingerprint sensor. By contract always use a value of 1.
     * @param wAck status of the packet response. Check for a ACK_OK for successfull command
     * executions, ACK_INFO for detailed message errors, and so. Please refer to the Oem class for
     * specific error handling (which is mostly done in that class).
     * @param pnParam additional information to complement the provided ack status code. It may
     * carry information requested (like the users count, finger pressed status, etc.)
     * @return 0 if the transfer of the bytes of the ack packet was successfully finished and
     * there's a response status (ack of any kind and the packet integrity is verified), or < 0 if
     * some sort of error ocurred.
     */
    int readAck(ushort wDevID, ushort* wAck, int* pnParam);

    /**
     * @brief write method for sending data packets to the device under the serial connection. This
     * method sends the apropriate header and data, for the specified data packet protocol stated in
     * the datasheet of the sensor. Templates and other large size packets are suited to used this
     * method.
     * @param wDevID device id of the fingerprint sensor. By contract always use a value of 1.
     * @param pBuf memory address of the bytes to be send in the body of the data packet. It should
     * point to a memory address holding at least nSize valid reserved bytes.
     * @param nSize number of bytes to be send under the serial connection.
     * @return 0 for successfull transfer of the data packet, or -1 if some communication error
     * occurred. Note that the value returned won't match the count of the actual bytes written.
     */
    int write(ushort wDevID, uchar* pBuf, int nSize);

    /**
     * @brief read method for reading data packets from the device, under the serial connection.
     * This methods reads, parses and verifies the checksum of the data packet as specified in the
     * data packet protocol as stated in the datasheet of the sensor. Templates and images are
     * suited to be read in this fashion, pleae see the datasheet for specific usages by command.
     * @param wDevID device id of the fingerprint sensor. By contract always used a value of 1.
     * @param pBuf memory address where the read bytes are going to be placed. It should point to a
     * valid reserved memory address of at least nSize bytes.
     * @param nSize numbe of bytes to be read from serial connection (size of the body of the data
     * packet).
     * @return 0 for successfull transfer of the data packet, or -1 if some communication error
     * occurred. Note that the value returned won't match the actual count of bytes read from the
     * serial connection.
     */
    int read(ushort wDevID, uchar* pBuf, int nSize);
};

#endif // OEMP_H
