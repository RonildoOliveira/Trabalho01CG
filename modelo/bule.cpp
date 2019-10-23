#include "bule.h"

Bule::Bule()
{
    setNome("bule");
}

void Bule::desenhar()
{
    //sistema local
        glPushMatrix();

            glTranslated(getTranslado_x(), getTranslado_z(), getTranslado_z());
            glRotated(getAngulo_x(),1,0,0);
            glRotated(getAngulo_y(),0,1,0);
            glRotated(getAngulo_z(),0,0,1);
            glScaled(
                getEscala_x(),
                getEscala_y(),
                getEscala_z()
            );

            if(getSelecionado()){
                glColor3f(1.0, 0.0, 0.0);
            }else{
                glColor3d(0.4, 0.4, 0.4);
            }

            glutSolidTeapot(getEscala_x());
        glPopMatrix();
}
