/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "config.h"

#ifdef RS232_SERIAL_COMM

#include "commbase_rs232.h"
#include "utils.h"

Rs232CommSerial::Rs232CommSerial() :
    lastOpen(-1)
{
}

Rs232CommSerial::~Rs232CommSerial() {
    close();
}

bool Rs232CommSerial::open(const std::string &port, uint dwBaudrate) {
    if (lastOpen != -1) {
#ifdef OUTPUT_DEBUG
        printf("Port already open, closing it first!\n");
#endif
        close();
    }

    int r = RS232_OpenComport(port.c_str(), SINGLETON_PORT_ID, dwBaudrate, "8N1");
    if (r == 0) {
#ifdef OUTPUT_DEBUG
        printf("Port open OK!");
#endif
        lastOpen = SINGLETON_PORT_ID;
        return true;
    }

    return false;
}

bool Rs232CommSerial::close() {
    if (lastOpen != -1) {
        RS232_CloseComport(lastOpen);
        lastOpen = -1;
        return true;
    }
    return false;
}

int64 Rs232CommSerial::write(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    printf("Sending data to open port, %d bytes\n", nSize);
#endif

    int64 ds = Utils::currentMillis();
    const uint chunkSize = 512;

    uint written = 0;
    while(written < nSize && (Utils::currentMillis() - ds < timeOut)) {

        int64 w = RS232_SendBuf(lastOpen, (uchar*)(buffer+written), std::min(chunkSize, nSize-written));
        if (w < 0) {
            break;
        }
        written += w;

        if (m_listener) {
            (*m_listener)(m_parameter);
        }

        // TODO: maybe add a pause here to allow the reading from the other side
    }
#ifdef OUTPUT_DEBUG
    printf("written: %d\n", written);
#endif
    return written;
}

int64 Rs232CommSerial::read(uchar *buffer, uint nSize) {
#ifdef OUTPUT_DEBUG
    printf("trying to read %d bytes from open port\n", nSize);
#endif

    int64 ds = Utils::currentMillis();
    const uint chunkSize = 512;

    uint read = 0;
    while (read < nSize && (Utils::currentMillis() - ds < timeOut)) {
#ifdef OUTPUT_DEBUG
        printf("Waiting some bytes\n");
#endif

        int r = RS232_PollComport(lastOpen, (uchar*)(buffer+read), std::min(chunkSize, nSize-read));
        if(r < 0) {
            break;
        }

        read += r;

        if (m_listener) {
            (*m_listener)(m_parameter);
        }

        // TODO: may add a pause here to allow the writing from the other side
    }

#ifdef OUTPUT_DEBUG
    printf("Total bytes read: %d out of %d\n", read, nSize);
    printf("{");
    for (uint i = 0; i < read; i++) {
        printf(" %d", (int)(buffer[i]));
    }
    printf("}\n");
#endif

    return read;
}

#endif // RS232_SERIAL_COMM
