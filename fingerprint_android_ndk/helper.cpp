/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "helper.h"
#include "lodepng.h"
#include <vector>
#include <cstdio>

Helper::Helper() :

    image256(256*256*4, 0xFF),
    image320(320*240*4, 0xFF),

    lastType(None)
{

}

Helper::~Helper() {
}
void Helper::saveLastImage(const std::string &_filename) {
    std::string filename = _filename;

    if(Utils::endsWith(Utils::toLower(filename), ".png")) {
        filename += ".png";
    }

    std::vector<uchar> png;
    uint error;

    if (lastType == Image256) {
        error = lodepng::encode(png, image256, 256, 256);
    } else if (lastType == Image320) {
        error = lodepng::encode(png, image320, 320, 240);
    } else {
#ifdef OUTPUT_DEBUG
        printf("Error, cannot save last image because there's no such image\n");
#endif
        return;
    }

    //if there's an error, display it
    if(error) {
        printf("encoder error %d : %s\n", error, lodepng_error_text(error));
    } else {
        lodepng::save_file(png, filename);
    }
}

void Helper::setResult(const std::string &line1, const std::string &line2) {
    std::string r = line1 + "\n" + line2;
#ifdef OUTPUT_DEBUG
    printf("setResult:%s\n", r.c_str());
#endif

#ifndef MODE_WINDOW
    printf(">> %s\n", line1.c_str());
    if (!line2.empty()) {
        printf(">> %s\n", line2.c_str());
    }
#endif
}

void Helper::setImage(uchar *image, ImageType type) {
    std::vector<uchar> *addr = NULL;
    uint total = 0;
    lastType = type;
    if (type == Image256) {
        addr = &image256;
        total = 256*256;
    } else if (type == Image320) {
        addr = &image320;
        total = 320*240;
    }
    if (addr) {
        std::vector<uchar> &img = *addr;
        for (uint i = 0; i < total; i++) {
            img[i*4+0] = image[i]; // R
            img[i*4+1] = image[i]; // G
            img[i*4+2] = image[i]; // B
        }
    }
}

std::string Helper::getComPort() {
    return consoleConfig.port;
}

uint Helper::getComBaudrate() {
    return consoleConfig.baudrate;
}

uint Helper::getId() {
    int id = consoleConfig.id;
    return std::min(199, std::max(0, id));
}

bool Helper::confirm(const std::string &message, const std::string &title) {
#ifdef OUTPUT_DEBUG
    printf("Because this is a console app, returning fixed value (%s, %s)\n", message.c_str(), title.c_str());
#endif
    return true;
}

std::string Helper::getOpenFilename(const std::string &exts) {
#ifdef OUTPUT_DEBUG
    printf("Because this is a console app, returning fixed value (%s)\n", exts.c_str());
#endif
    return consoleConfig.filename;
}

std::string Helper::getSaveFilename(const std::string &exts, const std::string &suggested) {
#ifdef OUTPUT_DEBUG
    printf("Because this is a console app, returning fixed value (%s, %s)", exts.c_str(), suggested.c_str());
#endif
    return consoleConfig.filename;
}
