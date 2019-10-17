#ifndef CAMERADISTANTE_H
#define CAMERADISTANTE_H

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Camera.h"

class CameraDistante : public Camera
{
   public:

      CameraDistante();
      CameraDistante(Vetor3D e, Vetor3D c, Vetor3D u);
      CameraDistante(GLfloat ex, GLfloat ey, GLfloat ez, GLfloat cx, GLfloat cy, GLfloat cz, GLfloat ux, GLfloat uy, GLfloat uz);
      ~CameraDistante(){};

      //vetor Vec e u sempre perpendiculares e u sempre unitario
      virtual void zoom(GLfloat new_y, GLfloat last_y);
      virtual void translatex(GLfloat new_x, GLfloat last_x);
      virtual void translatey(GLfloat new_y, GLfloat last_y);
      virtual void rotatex(GLfloat new_y, GLfloat last_y);
      virtual void rotatey(GLfloat new_x, GLfloat last_x);
      virtual void rotatez(GLfloat new_x, GLfloat last_x);
      virtual Vetor3D getPickedPoint(GLfloat x, GLfloat y);

};

#endif
