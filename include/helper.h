/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#ifndef HELPER_H
#define HELPER_H

#include "config.h"
#include "utils.h"

#include <vector>

#define EXTENSION_DATABASE "Database Files (*.db *.dat)"
#define EXTENSION_TEMPLATE "Template Files (*.dat)"
#define EXTENSION_IMAGE    "Image Files (*.png)"


class Helper {
public:
    enum ImageType {
        Image256,
        Image320,
        None
    };

    Config consoleConfig;

private:

    std::vector<uchar> image256;
    std::vector<uchar> image320;

    ImageType lastType;

public:
    explicit Helper();
    virtual ~Helper();

    void saveLastImage(const std::string &filename);

    void setResult(const std::string &line1, const std::string &line2="");
    void setImage(uchar *image, ImageType type);

    std::string getComPort();
    uint getComBaudrate();
    uint getId();

    bool confirm(const std::string &message, const std::string &title="Confirmation");
    std::string getOpenFilename(const std::string &exts);
    std::string getSaveFilename(const std::string &exts, const std::string &suggested=".");


};

#endif // HELPER_H
