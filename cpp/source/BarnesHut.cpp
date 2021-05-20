#include <fstream>
#include <cmath>
#include "BarnesHut.h"

using namespace std;

double loadEntities(string dataset, vector<Entity *> &entities);
void printEntities(vector<Entity *> entities);
BHTree *createBHTree(vector<Entity *> entities, double dims);

int main(int argc, char **argv)
{
    BHTree *bh;
    vector<Entity *> entities;
    double dims;

    dims = loadEntities("./../../datasets/input5.txt", entities);
    printEntities(entities);

    bh = createBHTree(entities, dims);
    printBHTree(bh);

    return 0;
}

BHTree *createBHTree(vector<Entity *> entities, double dims)
{
    BHTree *tree = new BHTree(Region(Point(0, 0), dims));
    for (Entity *e : entities)
    {
        tree->insertEntity(e);
    }

    return tree;
}

void printEntities(vector<Entity *> entities)
{
    for (Entity *e : entities)
    {
        cout << e->toString() << "\n";
    }
}

double loadEntities(string dataset, vector<Entity *> &entities)
{
    int totalEntities = 0, entitiesCounter = 0;
    double dims = 0;

    string filename = string(dataset);

    ifstream input_file(filename);

    if (!input_file.is_open())
    {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    cout << "--- Starting parsing file\n";

    if (input_file >> totalEntities)
    {
        cout << "Total entities: " << totalEntities << "\n";
    }

    if (input_file >> dims)
    {
        cout << "Dims: [" << -dims << ", " << dims << "]\n";
    }

    //bh = new BHTree(Region(Point(0, 0), dims)); //total dimen = 2dim

    while (entitiesCounter < totalEntities)
    {
        double X, Y, Vx, Vy, M;
        string S;

        input_file >> X;
        input_file >> Y;
        input_file >> Vx;
        input_file >> Vy;
        input_file >> M;
        input_file >> S;

        //cout << "Entity " << entitiesCounter << ": [X Y Vx Vy M S] = [ " << X << " " << Y << " " << Vx << " " << Vy << " " << M << " " << S << " ]\n";

        entitiesCounter++;

        //bh->insertEntity(new Entity(Point(X, Y), S, Vx, Vy, M));
        entities.push_back(new Entity(Point(X, Y), S, Vx, Vy, M));
    }

    input_file.close();

    cout << "--- Done parsing file\n";
    return dims;
}

Point massCenter(Entity e1, Entity e2)
{
    double centerX, centerY, mass = e1.getMass() + e2.getMass();

    centerX = ((e1.getPoint().getX() * e1.getMass()) + (e2.getPoint().getX() * e2.getMass())) / mass;
    centerY = ((e1.getPoint().getY() * e1.getMass()) + (e2.getPoint().getY() * e2.getMass())) / mass;

    return Point(centerX, centerY);
}

double distance(Entity e1, Entity e2)
{
    double x1 = e1.getPoint().getX();
    double x2 = e2.getPoint().getX();

    double y1 = e1.getPoint().getY();
    double y2 = e2.getPoint().getY();

    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double F(Entity e1, Entity e2)
{
    double G = 6.67 / pow(10, 11); //6.67 × 10−11
    double dist = distance(e1, e2);
    double m1 = e1.getMass();
    double m2 = e2.getMass();

    return G * (m1 * m2) / pow(dist, 2);
}
