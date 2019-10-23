#include "modelo.h"

Modelo::Modelo(){}

float Modelo::getAngulo_x() const
{
    return angulo_x;
}

void Modelo::setAngulo_x(float value)
{
    angulo_x = value;
}

float Modelo::getAngulo_y() const
{
    return angulo_y;
}

void Modelo::setAngulo_y(float value)
{
    angulo_y = value;
}

float Modelo::getAngulo_z() const
{
    return angulo_z;
}

void Modelo::setAngulo_z(float value)
{
    angulo_z = value;
}

float Modelo::getTranslado_x() const
{
    return translado_x;
}

void Modelo::setTranslado_x(float value)
{
    translado_x = value;
}

float Modelo::getTranslado_y() const
{
    return translado_y;
}

void Modelo::setTranslado_y(float value)
{
    translado_y = value;
}

float Modelo::getTranslado_z() const
{
    return translado_z;
}

void Modelo::setTranslado_z(float value)
{
    translado_z = value;
}

float Modelo::getEscala_x() const
{
    return escala_x;
}

void Modelo::setEscala_x(float value)
{
    escala_x = value;
}

float Modelo::getEscala_y() const
{
    return escala_y;
}

void Modelo::setEscala_y(float value)
{
    escala_y = value;
}

float Modelo::getEscala_z() const
{
    return escala_z;
}

void Modelo::setEscala_z(float value)
{
    escala_z = value;
}

bool Modelo::getMostraEixos() const
{
    return mostraEixos;
}

void Modelo::setMostraEixos(bool value)
{
    mostraEixos = value;
}

bool Modelo::getSelecionado() const
{
    return selecionado;
}

void Modelo::setSelecionado(bool value)
{
    selecionado = value;
}

std::string Modelo::getNome() const
{
    return nome;
}

void Modelo::setNome(const std::string &value)
{
    nome = value;
}
