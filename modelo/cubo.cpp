#include "cubo.h"

Cubo::Cubo()
{

}

void Cubo::desenhar()
{
    //sistema local
    glPushMatrix();

        if(getSelecionado()){
            glColor3f(1.0, 0.0, 0.0);
        }else{
            glColor3d(0.4, 0.4, 0.4);
        }

        //composicao de transformacoes
        glTranslated(getTranslado_x(), getTranslado_z(), getTranslado_z());
        glRotated(getAngulo_z(),0,0,1);
        glRotated(getAngulo_y(),0,1,0);
        glRotated(getAngulo_x(),1,0,0);
        glScaled(
            getEscala_x(),
            getEscala_y(),
            getEscala_z()
        );

        glutSolidCube(getEscala_x());

    glPopMatrix();
}
