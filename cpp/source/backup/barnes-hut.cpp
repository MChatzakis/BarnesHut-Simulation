#include <iostream>
#include <fstream>
#include <vector>

#include "barnes-hut.h"

using namespace std;

int iter = 1;
int curr = 0;

void printEntities(vector<PhysicalEntity>);
void splitUniverseToQuads(double dimensions, double xMiddle, double yMiddle);
vector<PhysicalEntity> readFile(std::string dataset);

int main(int argc, char **argv)
{
    //vector<PhysicalEntity> entities = readFile("./test1.txt");
    //printEntities(entities);

    splitUniverseToQuads(4, 0, 0);

    return 0;
}

void splitUniverseToQuads(double dim, double xMiddle, double yMiddle)
{
    double xStart = xMiddle - dim / 2;
    double xEnd = xMiddle + dim / 2;

    double yStart = yMiddle - dim / 2;
    double yEnd = yMiddle + dim / 2;

    cout << "---------------------\n";
    cout << "Quad 1:\n";
    cout << "   xe [ " << xMiddle << "," << xEnd << "]\n";
    cout << "   ye [ " << yMiddle << "," << yEnd << "]\n";

    cout << "Quad 2:\n";
    cout << "   xe [ " << xStart << "," << xMiddle << "]\n";
    cout << "   ye [ " << yMiddle << "," << yEnd << "]\n";

    cout << "Quad 3:\n";
    cout << "   xe [ " << xStart << "," << xMiddle << "]\n";
    cout << "   ye [ " << yStart << "," << yMiddle << "]\n";

    cout << "Quad 4:\n";
    cout << "   xe [ " << xMiddle << "," << xEnd << "]\n";
    cout << "   ye [ " << yStart << "," << yMiddle << "]\n";
    cout << "---------------------\n";

    /*if (curr < iter)
    {
        curr++;

        cout << "SubQuad 1\n";
        splitUniverseToQuads(dim / 2, xMiddle + dim / 4, yMiddle + dim / 4);
        cout << "SubQuad 2\n";
        splitUniverseToQuads(dim / 2, xMiddle - dim / 4, yMiddle + dim / 4);
        cout << "SubQuad 3\n";
        splitUniverseToQuads(dim / 2, xMiddle - dim / 4, yMiddle - dim / 4);
        cout << "SubQuad 4\n";
        splitUniverseToQuads(dim / 2, xMiddle + dim / 4, yMiddle - dim / 4);
    }
    else
    {
        return;
    }*/
}

void printEntities(vector<PhysicalEntity> entities)
{
    for (int i = 0; i < entities.size(); i++)
    {
        cout << "#" << i << ": " << entities[i].printEntity() << "\n";
    }
}

vector<PhysicalEntity> readFile(string dataset)
{
    int totalEntities = 0, dims = 0, entitiesCounter = 0;
    vector<PhysicalEntity> entities;
    string filename = string(dataset);

    ifstream input_file(filename);

    if (!input_file.is_open())
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    if (input_file >> totalEntities)
    {
        cout << "Total entities: " << totalEntities << "\n";
    }

    if (input_file >> dims)
    {
        cout << "Dims: [" << -dims << ", " << dims << "]\n";
    }

    while (entitiesCounter < totalEntities)
    {
        double X, Y, Vx, Vy, M;
        string S;

        cout << "Entity " << entitiesCounter << ":"
             << "[X Y Vx Vy M S] = [ ";

        if (input_file >> X)
        {
            cout << X << " ";
        }

        if (input_file >> Y)
        {
            cout << Y << " ";
        }

        if (input_file >> Vx)
        {
            cout << Vx << " ";
        }

        if (input_file >> Vy)
        {
            cout << Vy << " ";
        }

        if (input_file >> M)
        {
            cout << M << " ";
        }

        if (input_file >> S)
        {
            cout << S << " ";
        }

        entitiesCounter++;
        cout << "]\n";

        entities.push_back(PhysicalEntity(X, Y, Vx, Vy, M, S));
    }

    input_file.close();

    return entities;
}
