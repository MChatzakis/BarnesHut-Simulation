#include <fstream>
#include "BarnesHut.h"

using namespace std;

BHTree *bh = NULL;

void loadEntities(string dataset);

int main()
{
    loadEntities("test2.txt");

    printBHTree(bh);

    return 0;
}

void loadEntities(string dataset)
{
    int totalEntities = 0, dims = 0, entitiesCounter = 0;

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

    bh = new BHTree(Region(Point(0, 0), dims / 2));

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

        bh->insertEntity(new Entity(Point(X, Y), S, Vx, Vy, M));

        //entities.push_back(PhysicalEntity(X, Y, Vx, Vy, M, S));
    }

    input_file.close();
}