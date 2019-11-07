#include <iostream>
#include <math.h>
#include "CameraDistante.h"

using namespace std;

bool incluirObjeto = true;

//variaveis globais
int slices = 16;
int stacks = 16;

float trans_obj = false;

bool desenhaEixo = true;

float desl = 0.0; //0.001;

float delta = 5.0;

//ponto em coords locais, a ser calculado em coords de mundo
float pl[4] = { 0.0, 0.0, 0.0, 1.0 };
//ponto em coords globais, resultado da conversao a partir de pl
float pg[4] = { 0.0, 0.0, 0.0, 1.0 };

bool lbpressed = false;
bool mbpressed = false;
bool rbpressed = false;

float last_x = 0.0;
float last_y = 0.0;

Camera* cam = new CameraDistante();
float savedCamera[9];

Camera* cam2 = new CameraDistante(-3,2,-5, 0,0,0, 0,1,0);
//Camera* cam2 = new CameraDistante(2,1,0, 2,1,-5, -1,0,0);
bool manual_cam = false; //visualizar atraves da camera manual (cam2)?
bool change_manual_cam = false; //alterar a camera manual (cam2)?

int vp_manual_cam_x = 0;
int vp_manual_cam_y = 0;
int vp_manual_cam_w = 0;
int vp_manual_cam_h = 0;

#include <vector>
#include "Camera.h"
#include <gui_glut/gui.h>

vector<Camera*> cameraOrtho;
vector<Camera*> cameraP;
vector<Camera*> cameraOb;
int marcaCameraOb = -1;
int marcaCamera = -1;
int marcaCameraO = -1;
int marcaCameraP = -1;
int posSelecionado = -1;

/** -------------------------------------------------------- **/
#include <fstream>

#include "modelo/modelo.h"
#include "modelo/casa.h"
#include "modelo/cubo.h"
#include "modelo/bule.h"
#include "modelo/objmodelloader.h"

#include<vector>
vector<Modelo*> listaModelos;
int indice_obj_selecionado = 0;

void salvaArquivo(){
    ofstream myfile ("../Trabalho01CG/estado.txt");
    if (myfile.is_open())
    {
        if(listaModelos.empty()){
            cout << "lista vazia" << endl;
        }
        else{
            for (int index = 0; index < listaModelos.size(); ++index) {

                myfile << listaModelos.at(index)->getNome() << " ";

                myfile << listaModelos.at(index)->getTranslado_x() << " ";
                myfile << listaModelos.at(index)->getTranslado_y() << " ";
                myfile << listaModelos.at(index)->getTranslado_z() << " ";

                myfile << listaModelos.at(index)->getAngulo_x() << " ";
                myfile << listaModelos.at(index)->getAngulo_y() << " ";
                myfile << listaModelos.at(index)->getAngulo_z() << " ";

                myfile << listaModelos.at(index)->getEscala_x() << " ";
                myfile << listaModelos.at(index)->getEscala_y() << " ";
                myfile << listaModelos.at(index)->getEscala_z() << " ";

                myfile << "\n";
            }
        }
        myfile.close();
    }
    else{
        cout << "Erro de leitura";
    }
}

void carregaArquivo(){
    std::ifstream file("../Trabalho01CG/estado.txt");
        if (!file) {
            cout << "Erro de leitura";
        }

        string nomeModelo;
        float tx, ty, tz = 0;
        float ax, ay, az = 0;
        float sx, sy, sz = 0;

        while(!file.eof()){

            file >> nomeModelo;

            if(nomeModelo == "bule"){
                file >> tx >> ty >> tz;
                file >> ax >> ay >> az;
                file >> sx >> sy >> sz;

                Bule * bule = new Bule();
                bule->setAngulo_x(ax);
                bule->setAngulo_y(ay);
                bule->setAngulo_z(az);

                bule->setEscala_x(sx);
                bule->setEscala_y(sy);
                bule->setEscala_z(sz);

                bule->setTranslado_x(tx);
                bule->setTranslado_x(ty);
                bule->setTranslado_x(tz);

                listaModelos.push_back(bule);
            }

            else if(nomeModelo == "batmovel") {
                file >> tx >> ty >> tz;
                file >> ax >> ay >> az;
                file >> sx >> sy >> sz;

                ObjModelLoader * batmovel = new ObjModelLoader("../Trabalho01CG/modelos_3d/batmobile.obj", "batmovel");

                batmovel->setAngulo_x(ax);
                batmovel->setAngulo_y(ay);
                batmovel->setAngulo_z(az);

                batmovel->setEscala_x(sx);
                batmovel->setEscala_y(sy);
                batmovel->setEscala_z(sz);

                batmovel->setTranslado_x(tx);
                batmovel->setTranslado_y(ty);
                batmovel->setTranslado_z(tz);

                listaModelos.push_back(batmovel);
            }
            else if(nomeModelo == "cubo") {
                file >> tx >> ty >> tz;
                file >> ax >> ay >> az;
                file >> sx >> sy >> sz;

                Cubo * cubo = new Cubo();

                cubo->setAngulo_x(ax);
                cubo->setAngulo_y(ay);
                cubo->setAngulo_z(az);

                cubo->setEscala_x(sx);
                cubo->setEscala_y(sy);
                cubo->setEscala_z(sz);

                cubo->setTranslado_x(tx);
                cubo->setTranslado_y(ty);
                cubo->setTranslado_z(tz);

                listaModelos.push_back(cubo);
            }
            else if(nomeModelo == "casa") {
                file >> tx >> ty >> tz;
                file >> ax >> ay >> az;
                file >> sx >> sy >> sz;

                Casa * casa = new Casa();

                casa->setAngulo_x(ax);
                casa->setAngulo_y(ay);
                casa->setAngulo_z(az);

                casa->setEscala_x(sx);
                casa->setEscala_y(sy);
                casa->setEscala_z(sz);

                casa->setTranslado_x(tx);
                casa->setTranslado_y(ty);
                casa->setTranslado_z(tz);

                listaModelos.push_back(casa);
            }
        }
}

void carregaCamera(){
    std::ifstream file("../Trabalho01CG/camera.txt");
    string nomeModelo;

    GLfloat ex;
    GLfloat ey;
    GLfloat ez;
    GLfloat cx;
    GLfloat cy;
    GLfloat cz;
    GLfloat ux;
    GLfloat uy;
    GLfloat uz;

    if (!file) {
        cout << "Erro de leitura";
    }

    while(!file.eof()){

        file >> nomeModelo;
        file >> ex >> ey >> ez;
        file >> cx >> cy >> cz;
        file >> ux >> uy >> uz;

        cam = new CameraDistante(ex,ey,ez, cx,cy,cz, ux,uy,uz);
    }
}

void salvaCamera(){
    ofstream myfile ("../Trabalho01CG/camera.txt");

    myfile << "CameraDistante";

    if (myfile.is_open())
    {
        //ecu
        myfile << " " << cam->e.x;
        myfile << " " << cam->e.y;
        myfile << " " << cam->e.z;

        myfile << " " << cam->c.x;
        myfile << " " << cam->c.y;
        myfile << " " << cam->c.z;

        myfile << " " << cam->u.x;
        myfile << " " << cam->u.y;
        myfile << " " << cam->u.z;

        myfile.close();
    }

    else cout << "Erro de leitura";
}

//-------------------sombra-------------------
bool drawShadow = false;
bool pontual = true;
float k = 0.0;
//-------------------sombra-------------------


void sombra(GLfloat plano[4], GLfloat lightPos[4]){
    glPushMatrix();

            GLfloat sombra[4][4];
            GUI::shadowMatrix(sombra,plano,lightPos);
            glMultTransposeMatrixf( (GLfloat*)sombra );

        glDisable(GL_LIGHTING);
        glColor3d(0.0,0.0,0.0);
        if (drawShadow) {
            bool aux = glutGUI::draw_eixos;
            glutGUI::draw_eixos = false;
            for (int i = 0; i < listaModelos.size(); ++i) {

                //if(listaModelos[i]->sombra){
                if(true){
                    glPushMatrix();
                        listaModelos[i]->desenhar();
                    glPopMatrix();

                }
            }
            glutGUI::draw_eixos = aux;
        }
        glEnable(GL_LIGHTING);
    glPopMatrix();

}

void desenha() {
    GUI::displayInit();
    GUI::drawOrigin(1);

    GUI::setColor(1,0.5,0.5);
    GUI::drawFloor();
    glPushMatrix();
        //-------------------sombra-------------------
        glTranslated(0.0,k-0.001,0.0);
        //-------------------sombra-------------------
    glPopMatrix();

    for (int i = 0; i < listaModelos.size(); ++i) {
        glPushMatrix();
            listaModelos[i]->desenhar();
        glPopMatrix();
    }

    //-------------------sombra1-------------------
    //definindo a luz que sera usada para gerar a sombra
    float lightPos[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano[4] = {0,1,0,-0.001}; //{Normal, x, y, z}
    sombra(plano, lightPos);

    //-------------------sombra1-------------------

    //-------------------sombra2-------------------
    //definindo a luz que sera usada para gerar a sombra
    float lightPos2[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano2[4] = {1,0,0,2.49}; //{Normal, x, y, z}
    sombra(plano2, lightPos2);

    //-------------------sombra2-------------------
    //-------------------sombra3-------------------
    //definindo a luz que sera usada para gerar a sombra
    float lightPos3[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano3[4] = {-1,0,0,2.49}; //{Normal, x, y, z}
    sombra(plano3, lightPos3);

    //-------------------sombra3-------------------
    //-------------------sombra4-------------------
    //definindo a luz que sera usada para gerar a sombra
    float lightPos4[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano4[4] = {0,sqrt(2)/2,sqrt(2)/2,1.32}; //{Normal, x, y, z}
    sombra(plano4, lightPos4);

    //-------------------sombra4-------------------

    if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
        listaModelos[posSelecionado]->setTranslado_x(listaModelos[posSelecionado]->getTranslado_x() + glutGUI::dtx);
        listaModelos[posSelecionado]->setTranslado_y(listaModelos[posSelecionado]->getTranslado_y() + glutGUI::dty);
        listaModelos[posSelecionado]->setTranslado_z(listaModelos[posSelecionado]->getTranslado_z() + glutGUI::dtz);

        listaModelos[posSelecionado]->setAngulo_x(listaModelos[posSelecionado]->getAngulo_x() + glutGUI::dax);
        listaModelos[posSelecionado]->setAngulo_y(listaModelos[posSelecionado]->getAngulo_y() + glutGUI::day);
        listaModelos[posSelecionado]->setAngulo_z(listaModelos[posSelecionado]->getAngulo_z() + glutGUI::daz);

        listaModelos[posSelecionado]->setEscala_x(listaModelos[posSelecionado]->getEscala_x() + glutGUI::dsx);
        listaModelos[posSelecionado]->setEscala_y(listaModelos[posSelecionado]->getEscala_y() + glutGUI::dsy);
        listaModelos[posSelecionado]->setEscala_z(listaModelos[posSelecionado]->getEscala_z() + glutGUI::dsz);

    }
    //Parede 1
    glPushMatrix();
    GUI::setColor(0,0,1);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(2.5,0,2.5);
        glVertex3f(2.5,3.5,2.5);
        glVertex3f(2.5,3.5,-2.5);
        glVertex3f(2.5,0,-2.5);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    GUI::setColor(1,0,1);

    //Parede 2
    //glTranslated(0,1.6,1);
    glRotated(-45,1,0,0);
    glTranslated(0,0,-1.33+2.5);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(2.5,0,-2.5);
        glVertex3f(2.5,3.5,-2.5);
        glVertex3f(-2.5,3.5,-2.5);
        glVertex3f(-2.5,0,-2.5);
    glEnd();
    glPopMatrix();
    //Parede 3

    GUI::setColor(1,0,0);

    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f(-2.5,0,2.5);
        glVertex3f(-2.5,0,-2.5);
        glVertex3f(-2.5,3.5,-2.5);
        glVertex3f(-2.5,3.5,2.5);
    glEnd();
    glPopMatrix();


    cout<<glutGUI::cam->e.x<<','<<glutGUI::cam->e.y<<','<<glutGUI::cam->e.z<<',';
    cout<<glutGUI::cam->c.x<<','<<glutGUI::cam->c.y<<','<<glutGUI::cam->c.z<<',';
    cout<<glutGUI::cam->u.x<<','<<glutGUI::cam->u.y<<','<<glutGUI::cam->u.z<<endl;
    GUI::displayEnd();

}

void teclado(unsigned char key, int x, int y) {
        GUI::keyInit(key,x,y);

    switch (key) {
        case 't':
            glutGUI::trans_obj = !glutGUI::trans_obj;
            break;
        case 'l':
            glutGUI::trans_luz = !glutGUI::trans_luz;
            break;

        case 'n':
            if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
                listaModelos[posSelecionado]->setSelecionado(false);
            }
            posSelecionado++;
            posSelecionado = posSelecionado%listaModelos.size();
            if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
                listaModelos[posSelecionado]->setSelecionado(true);
            }
            break;
        case 'b':
            if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
                listaModelos[posSelecionado]->setSelecionado(false);
            }
            posSelecionado--;
            if (posSelecionado < 0) {
                posSelecionado = listaModelos.size()-1;
            }
            if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
                listaModelos[posSelecionado]->setSelecionado(true);
            }
            break;

        case 'p':
            incluirObjeto = !incluirObjeto;
            break;
        case '1':
                listaModelos.push_back(new Bule());
            break;
        case '2':
                listaModelos.push_back(new Cubo());
            break;
        case '3':
                listaModelos.push_back(new Casa());
            break;
        //-------------------sombra-------------------
        case 's':
            drawShadow = !drawShadow;
            break;
        case 'm':
            pontual = !pontual;
            break;
        case 'K':
            k += 0.1;
            break;
        case 'k':
            k -= 0.1;
            break;

        case 'u':

            if (posSelecionado >= 0 and posSelecionado < listaModelos.size()) {
              //SOOOOOOOOOO
             //listaModelos[posSelecionado]->sombra = !listaModelos[posSelecionado]->sombra;
        }
        //-------------------sombra-------------------

    //________________________________________
        case '=':
            glutGUI::ortho = true;
            glutGUI::perspective= false;
            glutGUI::obliq = false;
            marcaCameraO++;

            if(marcaCameraO>=cameraOrtho.size()){
                marcaCameraO =0;
            }
            glutGUI::cam = new CameraDistante(cameraOrtho[marcaCameraO]->e, cameraOrtho[marcaCameraO]->c, cameraOrtho[marcaCameraO]->u);
            break;
        case '-':
            glutGUI::perspective = true;
            glutGUI::obliq = false;
            glutGUI::ortho = false;
            marcaCameraP++;

            if(marcaCameraP>=cameraP.size()){
                marcaCameraP =0;
            }
            glutGUI::cam = new CameraDistante(cameraP[marcaCameraP]->e, cameraP[marcaCameraP]->c, cameraP[marcaCameraP]->u);
            break;

        case ')':
            glutGUI::obliq = true;
            glutGUI::ortho = false;
            glutGUI::perspective = false;
            marcaCameraOb++;
            if(marcaCameraOb>=cameraOb.size()){
                marcaCameraOb = 0;
            }
            glutGUI::cam = new CameraDistante(cameraOb[marcaCameraOb]->e, cameraOb[marcaCameraOb]->c, cameraOb[marcaCameraOb]->u);
            break;

    //_______________________________________

        default:
            break;
    }
}


int main()
{
    //isometrica
    cameraOrtho.push_back(new CameraDistante(5.83183,5.56437,6.71984,0,1,0,-0.299168,0.889756,-0.344722));
    //trimetrica
    cameraOrtho.push_back(new CameraDistante(3.47912,3.0497,9.14847,0,1,0,-0.0728583,0.978768,-0.191584));
   //dimetrica
    cameraOrtho.push_back(new CameraDistante(-0.000199788,5.13796,9.10371,0,1,0,9.08104e-06,0.91037,-0.413795));
   //1ponto
   cameraP.push_back(new CameraDistante(4.57603,0.531044,0.0102363,0,1,0,0.101947,0.99479,0.000228048));
   //2ponto
   cameraP.push_back(new CameraDistante(2.57824,0,3.7864,0,1,0,-0.051313,0.995835,-0.0753582));
   //3ponto
   cameraP.push_back(new CameraDistante(3.01227,2.14268,3.28338,0,1,0,-0.167931,0.968656,-0.183046));
   cameraOb.push_back(new CameraDistante(0.950944,1.90821,10.3176,-1.82974,-0.763079,1.09083,-0.0770811,0.963661,-0.255767));

    GUI gui = GUI(800,600, desenha, teclado);
}

