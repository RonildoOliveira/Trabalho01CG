#include "casa.h"

Casa::Casa()
{
    setNome("casa");
}

void Casa::desenhar(){

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

        glPushMatrix();
            face();
            glRotatef(90.0,0.0,1.0,0.0);
            glTranslatef(0.0,0.0,1.0);
            face();
            glTranslatef(0.0,0.0,-1.0);
            glRotatef(180.0,0.0,1.0,0.0);
            glTranslatef(-1.0,0.0,0.0);
            face();
            glTranslatef(0.0,0.0,-1.0);
            glRotatef(-90.0,0.0,1.0,0.0);
            face();
            glTranslatef(0.0,1.0,0.0);
            glRotatef(-90.0,1.0,0.0,0.0);
            face();
            glTranslatef(0.0,0.0,-1.0);
            glRotatef(180.0,0.0,1.0,0.0);
            glTranslatef(-1.0,0.0,0.0);
            face();
            glTranslatef(0.0,-0.20711,-0.79285);
            glRotatef(-45.0+180,1.0,0.0,0.0);
            glTranslatef(0.0,-1.0,0.0);
            glDisable(GL_CULL_FACE);
            face();
            glRotatef(90,1.0,0.0,0.0);
            glTranslatef(0.0,-1.0,0.0);
            face();
            glEnable(GL_CULL_FACE);
        glPopMatrix();
        glBegin(GL_TRIANGLES);
            glNormal3f(-1.0,0.0,0.0);
            glVertex3f(0.0,1.0,0.0);
            glVertex3f(0.0,1.5,-0.5);
            glVertex3f(0.0,1.0,-1.0);
        glEnd();
        glBegin(GL_TRIANGLES);
            glNormal3f(1.0,0.0,0.0);
            glVertex3f(1.0,1.0,-1.0);
            glVertex3f(1.0,1.5,-0.5);
            glVertex3f(1.0,1.0,0.0);
        glEnd();
    glPopMatrix();
}

void Casa::face()
{
    glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,1.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(1.0,0.0,0.0);
        glVertex3f(1.0,1.0,0.0);
        glVertex3f(0.0,1.0,0.0);
    glEnd();
}
