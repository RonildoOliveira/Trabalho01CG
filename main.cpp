#include <GL/glut.h>
#include <iostream>
#include <iomanip>

using namespace std;

#include "CameraDistante.h"
#include "Desenha.h"

//variaveis globais
int width = 320;
int height = 240;

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

/** -------------------------------------------------------- **/
#include <fstream>

#include "modelo/modelo.h"
#include "modelo/casa.h"
#include "modelo/cubo.h"
#include "modelo/bule.h"

/* Modelos */
Casa * casa = new Casa();
Cubo * cubo = new Cubo();
Bule * bule = new Bule();

#include<vector>
vector<Modelo*> listaModelos;
int indice_obj_selecionado = 0;


void salvaArquivo(){
    ofstream myfile ("../Trabalho01CG/estado.txt");
    if (myfile.is_open())
    {
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

void mult_matriz_vetor(float res[4], float matriz[16], float entr[4]) {
    for (int i = 0; i < 4; i++) {
        res[i] = 0.0;
        for (int j = 0; j < 4; j++) {
            //res[i] += matriz[4*i+j] * entr[j];
            res[i] += matriz[4*j+i] * entr[j]; //matriz^T.entr
        }
    }
}

void mostra_matriz_transform(float matriz[16], bool transposta = true) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (!transposta) {
                cout << setiosflags (ios::fixed) << setprecision(2) << matriz[4*i+j] << "  ";
            } else {
                cout << setiosflags (ios::fixed) << setprecision(2) << matriz[4*j+i] << "  "; //matriz^T
            }
        }
        cout << "\n";
    }
    //cout << "\n";
}

void imprime_coords_locais_globais()
{
    //imprimindo coords locais e coords globais
      //locais
        cout << "Coords locais: " << pl[0] << ", " << pl[1] << ", " << pl[2] << "\n";
      //globais
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
              //composicao de transformacoes
              glTranslated(
                          listaModelos.at(indice_obj_selecionado)->getTranslado_x(),
                          listaModelos.at(indice_obj_selecionado)->getTranslado_y(),
                          listaModelos.at(indice_obj_selecionado)->getTranslado_z()
                          );
              glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_z(),0,0,1);
              glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_y(),0,1,0);
              glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_x(),1,0,0);
            float mudanca_sist_coords[16];
            glGetFloatv(GL_MODELVIEW_MATRIX,mudanca_sist_coords);
            cout << "Matriz mudanca sist coords local=>global (T.Rz.Ry.Rx):\n";
            mostra_matriz_transform(mudanca_sist_coords);
            mult_matriz_vetor(pg,mudanca_sist_coords,pl);
            cout << "Coords globais: " << pg[0] << ", " << pg[1] << ", " << pg[2] << "\n\n";
        glPopMatrix();
}

void desenha_camera(float tam) {
    GLUquadricObj *quad = gluNewQuadric();

    glPushMatrix();
        Desenha::drawBox(-tam,-tam,-tam,tam,tam,tam);
        glTranslatef(0,0,-2*tam);
        Desenha::gluClosedCylinder(quad,tam,tam/2,tam,slices,stacks);
    glPopMatrix();

    gluDeleteQuadric( quad );
}

void transformacao_camera_2_global(Vetor3D e, Vetor3D c, Vetor3D u, bool mostra_matriz = false)
{
    Vetor3D z_ = e - c;
        z_.normaliza();
    Vetor3D x_ = u ^ z_;
        x_.normaliza();
    Vetor3D y_ = z_ ^ x_;
        //y_.normaliza();

    //matriz de transformacao
        float transform[16] = {
                                x_.x,    y_.x,    z_.x,    e.x,
                                x_.y,    y_.y,    z_.y,    e.y,
                                x_.z,    y_.z,    z_.z,    e.z,
                                0.0,    0.0,    0.0,    1.0
                             };
        glMultTransposeMatrixf( transform );

    if (mostra_matriz) {
        cout << "Matriz mudanca sist coords camera2=>global (R t = x' y' z' e):\n";
        mostra_matriz_transform(transform,false);
        cout << "\n";
    }
}

void transformacao_global_2_camera(Vetor3D e, Vetor3D c, Vetor3D u, bool mostra_matriz = false)
{
    Vetor3D z_ = e - c;
        z_.normaliza();
    Vetor3D x_ = u ^ z_;
        x_.normaliza();
    Vetor3D y_ = z_ ^ x_;
        //y_.normaliza();
    Vetor3D t;
        t.x = x_ * (e * -1);
        t.y = y_ * (e * -1);
        t.z = z_ * (e * -1);

    //matriz de transformacao
        float transform[16] = {
                                x_.x,    x_.y,    x_.z,    t.x,
                                y_.x,    y_.y,    y_.z,    t.y,
                                z_.x,    z_.y,    z_.z,    t.z,
                                0.0,    0.0,    0.0,    1.0
                             };
        glMultTransposeMatrixf( transform );

    if (mostra_matriz) {
        cout << "Matriz mudanca sist coords global=>camera2 (R^T -R^T.t):\n";
        mostra_matriz_transform(transform,false);
        cout << "\n";
    }
}


/* GLUT callback Handlers */

void resize(int w, int h)
{
    width = w;
    height = h;
}

void displayInit( float ar_width = width, float ar_height = height, bool only_projection = false )
{
    if (!only_projection) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, width, height);
    }

    const float ar = ar_height>0 ? (float) ar_width / (float) ar_height : 1.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.,ar,0.1,1000.);
}

void displayEnd()
{
    glutSwapBuffers();
}

void displayInner(bool manual_cam)
{
    //tempo
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    listaModelos.at(indice_obj_selecionado)->
            setAngulo_x(listaModelos.at(indice_obj_selecionado)->getAngulo_x() + desl);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //transformacao de camera
        if (!manual_cam) {
            //definindo posicao e orientacao da camera
            gluLookAt(cam->e.x,cam->e.y,cam->e.z, cam->c.x,cam->c.y,cam->c.z, cam->u.x,cam->u.y,cam->u.z);
            //transformacao_global_2_camera(cam->e,cam->c,cam->u);
        } else {
            //definindo posicao e orientacao da camera 2 "manualmente"
            transformacao_global_2_camera(cam2->e,cam2->c,cam2->u);
        }

    //sistema global
    glPushMatrix();
        //desenhando eixos do sistema de coordenadas global
          Desenha::drawEixos( 0.5 );
        //chao
        glColor3d(0.3,0.3,0.3);
        Desenha::drawGrid( 10, 0, 10, 1 );
    glPopMatrix();

    glPushMatrix();
    for (int var = 0; var < listaModelos.size(); ++var) {
        if(indice_obj_selecionado % listaModelos.size() == var){
            listaModelos.at(var)->setSelecionado(
                        ! listaModelos.at(var)->getSelecionado()
                        );
        }
        listaModelos.at(var)->desenhar();
    }
    glPopMatrix();

    //sistema local da camera
    glPushMatrix();
        //definindo sistema de coords atraves do glulookat (eye,center,up)
        if (!manual_cam) {
            transformacao_camera_2_global(cam2->e,cam2->c,cam2->u);
        } else {
            transformacao_camera_2_global(cam->e,cam->c,cam->u);
        }
        //desenhando eixos do sistema de coordenadas local da camera
          Desenha::drawEixos( 0.5 );
        //desenhando camera
        glColor3d(0.5,0.5,0.0);
        desenha_camera(0.2);
    glPopMatrix();
}

void display()
{
    //visao de duas camera (duas viewports), viewport auxiliar sobrepondo a principal
    //permitindo alterar posicionamento e dimensoes da viewport auxiliar
    displayInit();
    glViewport(0, 0, width, height);
    displayInner(manual_cam);

    glScissor(vp_manual_cam_x, vp_manual_cam_y + 3*height/4, vp_manual_cam_w + width/4, vp_manual_cam_h + height/4);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    displayInit(vp_manual_cam_w + width/4, vp_manual_cam_h + height/4, true);
    glViewport(vp_manual_cam_x, vp_manual_cam_y + 3*height/4, vp_manual_cam_w + width/4, vp_manual_cam_h + height/4);
    displayInner(!manual_cam);

    displayEnd();
}

void mouseButton(int button, int state, int x, int y) {
	// if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		// when the button is pressed
		if (state == GLUT_DOWN) {
            lbpressed = true;
		} else {// state = GLUT_UP
            lbpressed = false;
		}
	}
	// if the middle button is pressed
	if (button == GLUT_MIDDLE_BUTTON) {
		// when the button is pressed
		if (state == GLUT_DOWN) {
            mbpressed = true;
		} else {// state = GLUT_UP
            mbpressed = false;
		}
	}
	// if the left button is pressed
	if (button == GLUT_RIGHT_BUTTON) {
		// when the button is pressed
		if (state == GLUT_DOWN) {
            rbpressed = true;
        } else {
            // state = GLUT_UP
            rbpressed = false;
		}
	}

    last_x = x;
    last_y = y;
}

void mouseMove(int x, int y) {
    float fator = 100.0;
    if (lbpressed && !rbpressed && !mbpressed) {
        if (!trans_obj) {
            if (!manual_cam) {
                if (!change_manual_cam) {
                    cam->rotatex(y,last_y);
                    cam->rotatey(x,last_x);
                } else {
                    cam2->rotatex(last_y,y);
                    cam2->rotatey(last_x,x);
                }
            } else {
                if (!change_manual_cam) {
                    cam->rotatex(last_y,y);
                    cam->rotatey(last_x,x);
                } else {
                    cam2->rotatex(y,last_y);
                    cam2->rotatey(x,last_x);
                }
            }
        } else {
            fator = 10;
            listaModelos.at(indice_obj_selecionado)->
                    setAngulo_x(listaModelos.at(indice_obj_selecionado)->getAngulo_x() + (y - last_y)/fator);
            listaModelos.at(indice_obj_selecionado)->
                    setAngulo_y(listaModelos.at(indice_obj_selecionado)->getAngulo_y() + (x - last_x)/fator);
            fator = 100;
        }
    }
    fator = 100.0;
    if (!lbpressed && rbpressed && !mbpressed) {
        if (!trans_obj) {
            if (!manual_cam) {
                if (!change_manual_cam) {
                    cam->translatex(x,last_x);
                    cam->translatey(y,last_y);
                } else {
                    cam2->translatex(last_x,x);
                    cam2->translatey(last_y,y);
                }
            } else {
                if (!change_manual_cam) {
                    cam->translatex(last_x,x);
                    cam->translatey(last_y,y);
                } else {
                    cam2->translatex(x,last_x);
                    cam2->translatey(y,last_y);
                }
            }
        } else {
            listaModelos.at(indice_obj_selecionado)->
                    setTranslado_x(listaModelos.at(indice_obj_selecionado)->getTranslado_x() + (x - last_x)/fator);
            listaModelos.at(indice_obj_selecionado)->
                    setTranslado_y(listaModelos.at(indice_obj_selecionado)->getTranslado_y() + -(y - last_y)/fator);
            /*
            tx += (x - last_x)/fator;
            ty += -(y - last_y)/fator;
            */
        }
    }
    if (lbpressed && rbpressed && !mbpressed) {
        if (!trans_obj) {
            if (!manual_cam) {
                if (!change_manual_cam) {
                    cam->zoom(y,last_y);
                } else {
                    cam2->zoom(last_y,y);
                }
            } else {
                if (!change_manual_cam) {
                    cam->zoom(last_y,y);
                } else {
                    cam2->zoom(y,last_y);
                }
            }
        } else {
            //tz += (y - last_y)/fator;
            listaModelos.at(indice_obj_selecionado)->
                    setTranslado_z(listaModelos.at(indice_obj_selecionado)->getTranslado_z() + (y - last_y)/fator);
            fator = 10.0;
            //az += -(x - last_x)/fator;
            listaModelos.at(indice_obj_selecionado)->
                    setAngulo_z(listaModelos.at(indice_obj_selecionado)->getAngulo_z() + -(x - last_x)/fator);
            cout << ">>>" << indice_obj_selecionado << endl;
        }
    }
    fator = 100.0;
    if (!lbpressed && !rbpressed && mbpressed) {
        if (!trans_obj) {
            vp_manual_cam_x += (x - last_x);
            vp_manual_cam_y += -(y - last_y);
        } else {
            listaModelos.at(indice_obj_selecionado)->
                    setEscala_x(listaModelos.at(indice_obj_selecionado)->getEscala_x() + (x - last_x)/fator);
            listaModelos.at(indice_obj_selecionado)->
                    setEscala_y(listaModelos.at(indice_obj_selecionado)->getEscala_y() + (x - last_x)/fator);
            //sx += (x - last_x)/fator;
            //sy += -(y - last_y)/fator;
        }
    }
    if (lbpressed && !rbpressed && mbpressed) {
        if (!trans_obj) {
            vp_manual_cam_w += (x - last_x);
            vp_manual_cam_h += -(y - last_y);
        } else {
            //sz += (y - last_y)/fator;
            listaModelos.at(indice_obj_selecionado)->
                    setEscala_z(listaModelos.at(indice_obj_selecionado)->getEscala_z() + (y - last_y)/fator);
        }
    }
    if (!lbpressed && rbpressed && mbpressed) {
        if (!trans_obj) {
        } else {
            pl[0] += (x - last_x)/fator;
            pl[1] += -(y - last_y)/fator;
            imprime_coords_locais_globais();
        }
    }

    last_x = x;
    last_y = y;
}

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 13 :
        case 'q':
            exit(0);
        break;
        case 'n':
            //proximo indice do objeto selecionado
            indice_obj_selecionado ++;
            indice_obj_selecionado = indice_obj_selecionado % listaModelos.size();
            cout << indice_obj_selecionado << endl;
        break;
        case 'p':
            //anterior indice do objeto selecionado
            indice_obj_selecionado --;
            indice_obj_selecionado = indice_obj_selecionado % listaModelos.size();
            cout << indice_obj_selecionado << endl;
        break;
        case 'd':
            if(!listaModelos.empty()){
                listaModelos.erase(listaModelos.begin()+indice_obj_selecionado);
            }
        break;
        case 'o':
            salvaArquivo();
            cout << "salva arquivo" << endl;
        break;
        case 'F':
            glutFullScreen();
            break;
        case 'f':
            glutReshapeWindow(800,600);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;

        case ';':
            if(desl==0.0) desl = 0.0001;
            else desl = 0.0;
            break;

        case 'X':
            if(trans_obj){
                //ax+=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_x(listaModelos.at(indice_obj_selecionado)->getAngulo_x() + delta);
            }
            else { pl[0] += 0.01; imprime_coords_locais_globais(); };
            break;

        case 'Y':
            if(trans_obj){
                //ay+=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_y(listaModelos.at(indice_obj_selecionado)->getAngulo_y() + delta);
            }
            else { pl[1] += 0.01; imprime_coords_locais_globais(); };
            break;

        case 'Z':
            if(trans_obj){
                //az+=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_z(listaModelos.at(indice_obj_selecionado)->getAngulo_z() + delta);
            }
            else { pl[2] += 0.01; imprime_coords_locais_globais(); };
            break;

        case 'x':
            if(trans_obj){
                //ax-=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_x(listaModelos.at(indice_obj_selecionado)->getAngulo_x() - delta);
            }
            else { pl[0] -= 0.01; imprime_coords_locais_globais(); };
            break;

        case 'y':
            if(trans_obj){
                //ay-=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_y(listaModelos.at(indice_obj_selecionado)->getAngulo_y() - delta);
            }
            else { pl[1] -= 0.01; imprime_coords_locais_globais(); };
            break;

        case 'z':
            if(trans_obj){
                //az-=delta;
                listaModelos.at(indice_obj_selecionado)->
                        setAngulo_z(listaModelos.at(indice_obj_selecionado)->getAngulo_z() - delta);
            }
            else { pl[2] -= 0.01; imprime_coords_locais_globais(); };
            break;

        case 'i':
            listaModelos.at(indice_obj_selecionado)->setAngulo_x(0);
            listaModelos.at(indice_obj_selecionado)->setAngulo_y(0);
            listaModelos.at(indice_obj_selecionado)->setAngulo_z(0);

            listaModelos.at(indice_obj_selecionado)->setTranslado_x(0);
            listaModelos.at(indice_obj_selecionado)->setTranslado_y(0);
            listaModelos.at(indice_obj_selecionado)->setTranslado_z(0);

            listaModelos.at(indice_obj_selecionado)->setEscala_x(1);
            listaModelos.at(indice_obj_selecionado)->setEscala_y(1);
            listaModelos.at(indice_obj_selecionado)->setEscala_z(1);

            break;

        case 't':
            trans_obj = !trans_obj;
            break;

        case 'E':
            desenhaEixo = !desenhaEixo;
            break;

        case 'c':
            static int posCam = 0;
            posCam++;
            if (cam->estilo == 1) {
                delete cam;
                if (posCam%5==0) cam = new CameraDistante(); //CameraDistante(0,1,5, 0,1,0, 0,1,0);
                if (posCam%5==1) cam = new CameraDistante(5,1,0, 0,1,0, 0,1,0);
                if (posCam%5==2) cam = new CameraDistante(0,1,-5, 0,1,0, 0,1,0);
                if (posCam%5==3) cam = new CameraDistante(-5,1,0, 0,1,0, 0,1,0);
                if (posCam%5==4) cam = new CameraDistante(savedCamera[0],savedCamera[1],savedCamera[2],savedCamera[3],savedCamera[4],savedCamera[5],savedCamera[6],savedCamera[7],savedCamera[8]);
            } else if (cam->estilo == 2) {
                delete cam;
                cam = new CameraDistante();
            }
            break;

        case 'C':
            static int pos_manual_cam = 0;
            pos_manual_cam++;
            if (pos_manual_cam % 4 == 0) {
                cout << "Mostrando camera 1/Alterando camera 1:\n";
                change_manual_cam = false;
                manual_cam = false;
            }
            if (pos_manual_cam%4==1) {
                cout << "Mostrando camera 1/Alterando camera 2:\n";
                change_manual_cam = true;
                manual_cam = false;
            }
            if (pos_manual_cam%4==2) {
                cout << "Mostrando camera 2/Alterando camera 2:\n";
                change_manual_cam = true;
                manual_cam = true;
            }
            if (pos_manual_cam%4==3) {
                cout << "Mostrando camera 2/Alterando camera 1:\n";
                change_manual_cam = false;
                manual_cam = true;
            }
            //manual_cam = !manual_cam;
            if (manual_cam) {
                glPushMatrix();
                    transformacao_global_2_camera(cam2->e,cam2->c,cam2->u,true);
                glPopMatrix();
            } else {
                glPushMatrix();
                    transformacao_camera_2_global(cam2->e,cam2->c,cam2->u,true);
                glPopMatrix();
            }
            break;

        case 's':
            //save current camera location
            salvaCamera();
            /*
            savedCamera[0] = cam->e.x;
            savedCamera[1] = cam->e.y;
            savedCamera[2] = cam->e.z;
            savedCamera[3] = cam->c.x;
            savedCamera[4] = cam->c.y;
            savedCamera[5] = cam->c.z;
            savedCamera[6] = cam->u.x;
            savedCamera[7] = cam->u.y;
            savedCamera[8] = cam->u.z;
            */
            break;

        case 'm':
            if (!manual_cam) {
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                    glLoadIdentity();
                    glTranslated(
                                listaModelos.at(indice_obj_selecionado)->getTranslado_x(),
                                listaModelos.at(indice_obj_selecionado)->getTranslado_y(),
                                listaModelos.at(indice_obj_selecionado)->getTranslado_z()
                                );
                    glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_z(),0,0,1);
                    glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_y(),0,1,0);
                    glRotated(listaModelos.at(indice_obj_selecionado)->getAngulo_x(),1,0,0);
                    float transform[16];
                    glGetFloatv(GL_MODELVIEW_MATRIX,transform);
                    cout << "Matriz composicao de transformacoes (T.Rz.Ry.Rx.S):\n";
                    mostra_matriz_transform(transform);
                    cout << "\n";
                glPopMatrix();
            } else {
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                    glLoadIdentity();
                    gluLookAt(cam2->e.x,cam2->e.y,cam2->e.z, cam2->c.x,cam2->c.y,cam2->c.z, cam2->u.x,cam2->u.y,cam2->u.z);
                    float transform[16];
                    glGetFloatv(GL_MODELVIEW_MATRIX,transform);
                    cout << "Matriz gluLookAt:\n";
                    mostra_matriz_transform(transform);
                    cout << "\n";
                glPopMatrix();
            }
            break;
    }

    glutPostRedisplay();
}

void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    /** carregar modelos **/
    carregaArquivo();

    /** carregar camera **/
    carregaCamera();

    //chamadas de inicializacao da GLUT
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Trabalho CG");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    //chamadas de inicializacao da OpenGL
    glClearColor(1,1,1,1);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE); //mantem a qualidade da iluminacao mesmo quando glScalef eh usada

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    //definindo uma luz
    glEnable(GL_LIGHT0);

    const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    //iniciando o loop principal da glut
    glutMainLoop();

    return EXIT_SUCCESS;
}
