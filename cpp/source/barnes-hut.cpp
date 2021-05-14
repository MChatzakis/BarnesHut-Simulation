#include <iostream>
#include <fstream>
#include <vector>
#include "barnes-hut.h"

using namespace std;

void printEntities(vector<PhysicalEntity>);

vector<PhysicalEntity> readFile(std::string dataset);

int main(int argc, char **argv)
{
    vector<PhysicalEntity> entities = readFile("./test1.txt");
    printEntities(entities);
    return 0;
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
