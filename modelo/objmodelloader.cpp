#include "objmodelloader.h"

using namespace std;

ObjModelLoader::ObjModelLoader()
{

}

ObjModelLoader::ObjModelLoader(string filename, string nome)
{
    this->filename = filename;
    setNome(nome);

    fstream *objFile = new fstream;
    objFile->open(filename, ios_base::in);

    if (!objFile->is_open())
    {
        cout << "O arquivo [" << filename << "] não pode ser lido." << endl;
        exit(EXIT_FAILURE);
    }

    string currentLine;

    while (!objFile->eof())
    {
        getline(*objFile, currentLine);

        // Split
        vector<string> *parameters = this->GetSplittedStrings(currentLine, ' ');

        //CASO V (Vertices)
        if (parameters->at(0) == "v")
        {
            // Remove 'v'
            parameters->erase(parameters->begin());

            vector<float> *currentPoint = new vector<float>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (x, y, z) to Float (x, y, z)
                currentPoint->push_back(this->GetFloatFromString(parameters->at(index)));
            }
            vertices->push_back(currentPoint);
        }

        //CASO F (Faces)
        else if (parameters->at(0) == "f")
        {
            // Remove 'f'
            parameters->erase(parameters->begin());

            vector<int> *vertexIndexes = new vector<int>;

            for (int index = 0; index < parameters->size(); index++)
            {
                // String (face_index) to Int (face_index)
                int faceIndex = this->GetFloatFromString(parameters->at(index));

                // Obj possui index a partir de 1
                // A origem deve ser 0
                vertexIndexes->push_back(--faceIndex);
            }

            faces->push_back(vertexIndexes);
        }

        delete parameters;
    }

    objFile->close();
}

void ObjModelLoader::desenhar()
{
    glPushMatrix();

    //composicao de transformacoes
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


    // Triangulacao
    glBegin(GL_TRIANGLES);

    for (int i = 0; i < faces->size(); i++)
    {
        float *normal = NULL;

        float coord1[3],
                coord2[3],
                coord3[3];

        for (int j = 0; j < (faces->at(i))->size(); j++)
        {
            int index = (faces->at(i))->at(j);

            switch(j)
            {
            // vertice 1
            case 0:
                coord1[0] = (vertices->at(index))->at(0);
                coord1[1] = (vertices->at(index))->at(1);
                coord1[2] = (vertices->at(index))->at(2);
                break;

                // vertice 2
            case 1:
                coord2[0] = (vertices->at(index))->at(0);
                coord2[1] = (vertices->at(index))->at(1);
                coord2[2] = (vertices->at(index))->at(2);
                break;

                // vertice 3
            case 2:
                coord3[0] = (vertices->at(index))->at(0);
                coord3[1] = (vertices->at(index))->at(1);
                coord3[2] = (vertices->at(index))->at(2);
                break;
            }

            // Calculate normal
            if (j == 2)
            {
                //NORMAL mente isso nao funciona com objetos complexos
                normal = this->GetNormal(coord1, coord2, coord3);

                if(normal != NULL){
                    // Aplica normal ao trianulo
                    glNormal3f(normal[0], normal[2], normal[2]);
                }

                // Face triangula a partir dos vertices
                glVertex3f(coord1[0], coord1[1], coord1[2]);
                glVertex3f(coord2[0], coord2[1], coord2[2]);
                glVertex3f(coord3[0], coord3[1], coord3[2]);
            }
        }
    }

    glEnd();
    glPopMatrix();
}

vector<string>* ObjModelLoader::GetSplittedStrings(string text, char delimeter)
{
    text += delimeter;

    string word = "";
    vector<string> *words = new vector<string>;

    for (int character = 0; character < text.length(); character++)
    {
        if (text[character] != delimeter)
        {
            word += text[character];
        }
        else
        {
            words->push_back(word);
            word = "";
        }
    }

    return words;
}

float ObjModelLoader::GetFloatFromString(string text)
{
    float value = 0.0f;

    istringstream buffer(text);
    buffer >> value;

    return value;
}

float* ObjModelLoader::GetNormal(float *coord1, float *coord2, float *coord3)
{
    // GEt Vector1 Vector2
    float va[3], vb[3], vr[3];

    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    // Fator Normal
    float val = sqrt(pow(vr[0], 2) + pow(vr[1], 2) + pow(vr[2], 2));

    float norm[3];

    norm[0] = vr[0] / val;
    norm[1] = vr[1] / val;
    norm[2] = vr[2] / val;

    //Gamby :v
    glNormal3f(norm[0], norm[1], norm[2]);

    return norm;
}

ObjModelLoader::~ObjModelLoader()
{
    delete this->faces;
    delete this->vertices;
}
