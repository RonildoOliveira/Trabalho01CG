#ifndef CASA_H
#define CASA_H

#include "modelo.h"

class Casa : public Modelo
{
public:
    Casa();
    void desenhar() override;
    void face();
};

#endif // CASA_H
