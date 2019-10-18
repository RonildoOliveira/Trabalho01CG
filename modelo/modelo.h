#ifndef MODELO_H
#define MODELO_H

#include <GL/glut.h>
#include <bib/CameraDistante.h>
#include <bib/Desenha.h>
#include <string>

class Modelo
{
public:
    Modelo();
    virtual void desenhar() = 0;

    /*Angulo*/
    float getAngulo_x() const;
    void setAngulo_x(float value);

    float getAngulo_y() const;
    void setAngulo_y(float value);

    float getAngulo_z() const;
    void setAngulo_z(float value);

    /*Translado*/
    float getTranslado_x() const;
    void setTranslado_x(float value);

    float getTranslado_y() const;
    void setTranslado_y(float value);

    float getTranslado_z() const;
    void setTranslado_z(float value);

    /*Escala*/
    float getEscala_x() const;
    void setEscala_x(float value);

    float getEscala_y() const;
    void setEscala_y(float value);

    float getEscala_z() const;
    void setEscala_z(float value);

    /*Eixos*/
    bool getMostraEixos() const;
    void setMostraEixos(bool value);

    /*Selecionado*/
    bool getSelecionado() const;
    void setSelecionado(bool value);

    std::string getNome() const;
    void setNome(const std::string &value);

private:
    float angulo_x = 0.0;
    float angulo_y = 0.0;
    float angulo_z = 0.0;

    float translado_x = 0.0;
    float translado_y = 0.0;
    float translado_z = 0.0;

    float escala_x = 1.0;
    float escala_y = 1.0;
    float escala_z = 1.0;

    bool mostraEixos = false;
    bool selecionado = false;

    std::string nome;
};

#endif // MODELO_H
