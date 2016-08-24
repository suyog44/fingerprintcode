/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "oem.h"
#include "oemp.h"

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>

int Oem::commandRun(ushort wCmd, int nCmdParam) {
    int x;

    if((x = oemp.writeCmd(gwDevID, wCmd, nCmdParam)) < 0) {
#ifdef OUTPUT_DEBUG
        printf("returning OEM_COMM_ERR (if #1), x = %d\n", x);
#endif
        return OEM_COMM_ERR;
    }

    if((x = oemp.readAck(gwDevID, &gwLastAck, &gwLastAckParam)) < 0) {
#ifdef OUTPUT_DEBUG
        printf("returning OEM_COMM_ERR (if #2), x = %d\n", x);
#endif
        return OEM_COMM_ERR;
    }
#ifdef OUTPUT_DEBUG
    printf("returning 0 (OK)\n");
#endif
    return 0;
}

Oem::Oem(CCommSerial *serial_port) :
    gwDevID(1),
    gwLastAck(0),
    gwLastAckParam(0),

    m_listener(NULL),
    m_parameter(NULL),
    oemp(serial_port)
{
}

void Oem::setCallback(void(*listener)(void*p), void *parameter) {
    m_listener = listener;
    m_parameter = parameter;

    oemp.setCallback(listener, parameter);
}

int Oem::openPort(const std::string &port, uint baudrate) {
    if(oemp.open(port, 9600) < 0 ||
        changeBaudrate(baudrate) < 0 ||
        oemp.open(port, baudrate) < 0){
#ifdef OUTPUT_DEBUG
        printf("Device is not connected to serial port.\n");
#endif
        return -1;
    }
    return 0;
}

int Oem::closePort() {
    if(changeBaudrate(9600)<0) {
#ifdef OUTPUT_DEBUG
        printf("Error while changing the baudrate before closing\n");
#endif
        return -1;
    }
    return oemp.close();
}

int Oem::openOem() {
    if(commandRun(CMD_OPEN, 1) < 0) {
        return OEM_COMM_ERR;
    }
    if(oemp.read(gwDevID, (uchar*)&gDevInfo, sizeof(devinfo)) < 0) {
        return OEM_COMM_ERR;
    }
    return 0;
}

int Oem::closeOem() {
    return commandRun(CMD_CLOSE, 0);
}

int Oem::usbInternalCheck() {
    return commandRun(CMD_USB_INTERNAL_CHECK, gwDevID);
}

int Oem::changeBaudrate(int nBaudrate) {
#ifdef OUTPUT_DEBUG
    printf("changeBaudrate\n");
#endif
    return commandRun(CMD_CHANGE_BAUDRATE, nBaudrate);
}

int Oem::cmosLed(bool bOn) {
    return commandRun(CMD_CMOS_LED, bOn ? 1 : 0);
}

int Oem::enrollCount() {
    return commandRun(CMD_ENROLL_COUNT, 0);
}

int Oem::checkEnrolled(int nPos) {
    return commandRun(CMD_CHECK_ENROLLED, nPos);
}

int Oem::enrollStart(int nPos) {
    return commandRun(CMD_ENROLL_START, nPos);
}

/*AVW*/
int Oem::enrollNth(int nPos, int nTurn) {
    if (commandRun(CMD_ENROLL_START+nTurn, 0) < 0) {
#ifdef OUTPUT_DEBUG
        printf("oem_enroll_nth: returning OEM_COMM_ERR\n");
#endif
        return OEM_COMM_ERR;
    }

    if (nPos == -1 && nTurn == 3) {
        if(gwLastAck == ACK_OK) {
            if(oemp.read(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
#ifdef OUTPUT_DEBUG
                printf("oem_enroll_nth: returning OEM_COMM_ERR\n");
#endif
                return OEM_COMM_ERR;
            }
        }
    }

    return 0;
}

int Oem::isPressFinger() {
    return commandRun(CMD_IS_PRESS_FINGER, 0);
}

int Oem::deleteId(int nPos) {
    return commandRun(CMD_DELETE, nPos);
}

int Oem::deleteAll() {
    return commandRun(CMD_DELETE_ALL, 0);
}

int Oem::verify(int nPos) {
    return commandRun(CMD_VERIFY, nPos);
}

int Oem::identify() {
    return commandRun(CMD_IDENTIFY, 0);
}

int Oem::verifyTemplate(int nPos) {
    if(commandRun(CMD_VERIFY_TEMPLATE, nPos) < 0) {
        return OEM_COMM_ERR;
    }

    if(gwLastAck == ACK_OK) {
        if(oemp.write(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
            return OEM_COMM_ERR;
        }
        if(oemp.readAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
            return OEM_COMM_ERR;
        }
    }
    return 0;
}

int Oem::identifyTemplate() {
    if(commandRun(CMD_IDENTIFY_TEMPLATE, 0) < 0) {
        return OEM_COMM_ERR;
    }

    if(gwLastAck == ACK_OK) {
        if(oemp.write(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
            return OEM_COMM_ERR;
        }
        if(oemp.readAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
            return OEM_COMM_ERR;
        }
    }

    return 0;
}

int Oem::capture(bool bBest) {
    return commandRun(CMD_CAPTURE, bBest);
}

/*AVW*/
static uchar gbyImg256_2[202*258];
static uchar gbyImg256_tmp[258*202];

int Oem::getImage() {
    if(commandRun(CMD_GET_IMAGE, 0) < 0){
        return OEM_COMM_ERR;
    }

#ifdef OUTPUT_DEBUG
    printf("receiving %d , %d bytes\n", sizeof(gbyImg256_tmp), sizeof(gbyImg256_2));
#endif

    if(oemp.read(gwDevID, gbyImg256_tmp, sizeof(gbyImg256_tmp)) < 0) {
        return OEM_COMM_ERR;
    }

    // image rotate
    int i, j;

    for(i=0; i<202; i++) {
        for(j=0; j<258; j++) {
            gbyImg256_2[i*258+j] = gbyImg256_tmp[j*202+i];
        }
    }

    for(unsigned int i = 0; i < sizeof(gbyImg8bit); i++) {
        gbyImg8bit[i] = 161;
    }

    for(i=0; i<202; i++) {
        memcpy(&gbyImg8bit[256*(27 + i) + 0], &gbyImg256_2[i * 258 + 1], 256);
    }

    return 0;
}

/*AVW*/
static uchar gbyImgRaw2[240*320/4];

int Oem::getRawimage() {
    if(commandRun(CMD_GET_RAWIMAGE, 0) < 0) {
        return OEM_COMM_ERR;
    }

    if(oemp.read(gwDevID, gbyImgRaw2, sizeof(gbyImgRaw2)) < 0) {
        return OEM_COMM_ERR;
    }

    /*AVW*/
    for(unsigned int i = 0; i < sizeof(gbyImgRaw); i++) {
        gbyImgRaw[i] = 66;
    }

    int i, j;
    for(i=0; i<120; i++) {
        for(j=0; j<160; j++) {
            gbyImgRaw[320*(2*i+0)+(2*j+0)] = gbyImgRaw2[i*160+j];
            gbyImgRaw[320*(2*i+0)+(2*j+1)] = gbyImgRaw2[i*160+j];
            gbyImgRaw[320*(2*i+1)+(2*j+0)] = gbyImgRaw2[i*160+j];
            gbyImgRaw[320*(2*i+1)+(2*j+1)] = gbyImgRaw2[i*160+j];
        }
    }

    return 0;
}

int Oem::getTemplate(int nPos) {
    if(commandRun(CMD_GET_TEMPLATE, nPos) < 0) {
        return OEM_COMM_ERR;
    }

    if(gwLastAck == ACK_OK) {
        if(oemp.read(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
            return OEM_COMM_ERR;
        }
    }

    return 0;
}

int Oem::addTemplate(int nPos) {
    if(commandRun(CMD_ADD_TEMPLATE, nPos) < 0) {
        return OEM_COMM_ERR;
    }

    if(gwLastAck == ACK_OK) {
        if(oemp.write(gwDevID, &gbyTemplate[0], FP_TEMPLATE_SIZE) < 0) {
            return OEM_COMM_ERR;
        }
        if(oemp.readAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
            return OEM_COMM_ERR;
        }
    }

    return 0;
}

int Oem::getDatabaseEnd() {
    return commandRun(CMD_GET_DATABASE_END, 0);
}

int Oem::getDatabaseStart() {
    return commandRun(CMD_GET_DATABASE_START, 0);
}

/*
int Oem::fwUpgrade(uchar* pBuf, int nLen) {
    if(commandRun(CMD_FW_UPDATE, nLen) < 0) {
        return OEM_COMM_ERR;
    }
    int nSegSize = 0;
    int cbWrote = 0;
    if(gwLastAck == ACK_OK) {
        nSegSize = gwLastAckParam;    // flash segment size that's returned with ack.
        if(nSegSize <= 0) {
            return OEM_COMM_ERR;
        }

        // send firmware
        while(cbWrote < nLen) {
            if(cbWrote + nSegSize > nLen) {
                nSegSize = nLen - cbWrote;
            }
            if(oemp.write(gwDevID, &pBuf[cbWrote], nSegSize) < 0) {
                return OEM_COMM_ERR;
            }
            if(oemp.readCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
                return OEM_COMM_ERR;
            }
            cbWrote += nSegSize;
        }
    }
    return 0;
}

int Oem::isoUpgrade(uchar* pBuf, int nLen) {
    if(commandRun(CMD_ISO_UPDATE, nLen) < 0) {
        return OEM_COMM_ERR;
    }
    int nSegSize = 0;
    int cbWrote = 0;
    if(gwLastAck == ACK_OK) {
        nSegSize = gwLastAckParam;    // flash segment size that's returned with ack.
        if(nSegSize <= 0){
            return OEM_COMM_ERR;
        }

        // send firmware
        while(cbWrote < nLen) {
            if(cbWrote + nSegSize > nLen) {
                nSegSize = nLen - cbWrote;
            }

            if(oemp.write(gwDevID, &pBuf[cbWrote], nSegSize) < 0) {
                return OEM_COMM_ERR;
            }
            if(oemp.readCmdOrAck(gwDevID, &gwLastAck, &gwLastAckParam) < 0) {
                return OEM_COMM_ERR;
            }
            cbWrote += nSegSize;
        }
    }
    return 0;
}*/

std::string Oem::error(int nNackInfo, int nPos) {
    if (nNackInfo > NACK_NONE) {
        switch(nNackInfo) {
        case NACK_TIMEOUT:
            return "Time Out!";
        case NACK_INVALID_BAUDRATE:
            return "Invalid baudrate";
        case NACK_INVALID_POS:
            return "Invalid ID";
        case NACK_IS_NOT_USED:
            return Utils::format("ID = %d: is not used!", nPos);
        case NACK_IS_ALREADY_USED:
            return Utils::format("ID = %d: is already used!", nPos);
        case NACK_COMM_ERR:
            return "Communication error!";
        case NACK_VERIFY_FAILED:
            return Utils::format("ID = %d: NG (DOES NOT MATCH!)", nPos);
        case NACK_IDENTIFY_FAILED:
            return "NG!";
        case NACK_DB_IS_FULL:
            return "Database is full";
        case NACK_DB_IS_EMPTY:
            return "Database is empty";
        case NACK_TURN_ERR:
            return "The order of enrollment is incorrect!";
        case NACK_BAD_FINGER:
            return "Bad finger!";
        case NACK_ENROLL_FAILED:
            return "The enrollment is failed!";
        case NACK_IS_NOT_SUPPORTED:
            return "The command is not supported";
        case NACK_DEV_ERR:
            return "The device error!";
        case NACK_CAPTURE_CANCELED:
            return "Canceled!";
        case NACK_INVALID_PARAM:
            return "Invalid Firmware Image!";
        case NACK_FINGER_IS_NOT_PRESSED:
            return "Finger is not pressed!";
        default:
            return "Unknown Error";
        }
    } else if (nNackInfo < FP_MAX_USERS) {
        return Utils::format("ID = %d: is already enrolled!", nNackInfo);
    }
    return "I don't know!!!";
}

int Oem::capturing(bool best, std::string &err) {
    int64 st = Utils::currentMillis();
    while (Utils::currentMillis() - st < 5000) {
        if(capture(best ? 1 : 0) < 0) {
            err = "Communication error!";
            return -1;
        } else if(gwLastAck == ACK_OK) {
            return 0;
        }
    }
    err = "Timeout!";
    return -2;
}

int Oem::downloadingImage(std::string &err) {
    if (getImage() < 0) {
        err = "Communication error!";
        return -1;
    } else if (gwLastAck == NACK_INFO) {
        err = Oem::error(gwLastAckParam, 0);
        return -1;
    }
    err = "";
    return 0;
}

int Oem::loadingImage(std::string &err) {
    if(getImage() < 0) {
        err = "Communication error";
        return -1;
    } else if(gwLastAck == NACK_INFO) {
        err = Oem::error(gwLastAckParam, 0);
        return -1;
    }
    err = "";
    return 0;
}

int Oem::loadingImageRaw(std::string &err) {
    if (getRawimage() < 0) {
        err = "Communication error";
        return -1;
    } else if(gwLastAck == NACK_INFO) {
        err = Oem::error(gwLastAckParam, 0);
        return -1;
    }
    err = "";
    return 0;
}

int Oem::loadingImageLive(std::string &err) {
    if (getRawimage() < 0) {
        err = "Communication error!";
        return -1;
    } else if (gwLastAck == NACK_INFO) {
        err = Oem::error(gwLastAckParam, 0);
        return -1;
    }
    err = "";
    return 0;
}
