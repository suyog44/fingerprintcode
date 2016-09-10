/*****************************************************
Modified By : B Suyog
github: www.github.com/suyog44
mail id: suyogburadkar@gmail.com

Contribution is needed to maintain code. Interested
people can support it on GITHUB.COM
        
*****************************************************/

#include "commbase.h"

CCommSerial::CCommSerial() {
}

CCommSerial::~CCommSerial() {

}

void CCommSerial::setCallback(void(*listener)(void*p), void *parameter) {
    m_listener = listener;
    m_parameter = parameter;
}

void CCommSerial::setTimeout(uint dwTimeOut) {
    timeOut = dwTimeOut;
}
