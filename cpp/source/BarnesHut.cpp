#include <fstream>
#include <cmath>
#include <iostream>

#include "BarnesHut.h"

using namespace std;

BHTree *createBHTree(vector<Entity *> entities, double dims);

double loadEntities(string dataset, vector<Entity *> &entities);
double F(Entity e1, Entity e2);
double Fy(Entity e1, Entity e2);
double Fx(Entity e1, Entity e2);

void printEntities(vector<Entity *> entities);
void printBHTreeUtil(BHTree *curr);
void printBHTree(BHTree *curr);
void BarnesHut(vector<Entity *> &entities, double dims, int iterations, int dt);
void updateNetForceData(Entity *e, BHTree *bh);
void calcNewPos(Entity *e, double dt);

bool containsPoint(vector<Entity *> entities, Entity *en);

int main(int argc, char **argv)
{
    vector<Entity *> entities;
    double dims;

    dims = loadEntities("./../../datasets/input1.txt", entities);
    printEntities(entities);

    BarnesHut(entities, dims, 1, 1);

    return 0;
}

void BarnesHut(vector<Entity *> &entities, double dims, int iterations, int dt)
{
    BHTree *bh;

    for (int i = 0; i < iterations; i++)
    {
        bh = createBHTree(entities, dims);

        for (Entity *e : entities)
        {
            updateNetForceData(e, bh);
        }

        // -- Barrier --

        for (Entity *e : entities)
        {
            calcNewPos(e, dt);
        }
    }

    printEntities(entities);
    //bh = createBHTree(entities, dims);
    //printBHTree(bh);
}

void updateNetForceData(Entity *e, BHTree *bh)
{
    bool pathFound = false;

    if (bh == NULL)
    {
        return;
    }

    if (bh->isLeaf())
    {
        assert(bh->getEntity() == e);
        return;
    }

    BHTree *quads[4] = {bh->getQuad1(), bh->getQuad2(), bh->getQuad3(), bh->getQuad4()};
    BHTree *quadToGo = NULL;

    for (int i = 0; i < 4; i++)
    {
        if (quads[i] != NULL)
        {
            Entity *cent = quads[i]->getEntity();
            Region reg = quads[i]->getRegion();

            if (cent == NULL)
            {
                continue;
            }

            //vector<Entity *> entitiesOfRegion = quads[i]->getTotalEntities();

            //ContainsPoint is really slow.. =>optimize?
            //if (containsPoint(entitiesOfRegion, e))
            if (!pathFound && reg.containsPoint(e->getPoint()))
            {
                cout << "Found the region that body " << e->getName() << " belongs. Going to squad[ " << i << " ]\n";
                quadToGo = quads[i];

                pathFound = true;
            }
            else
            {
                cout << "Calculating net force for body " << e->toString() << " by body " << cent->toString() << "\n";

                double fx = Fx(*cent, *e);
                double fy = Fy(*cent, *e);

                e->addToSFx(fx);
                e->addToSFy(fy);
            }
        }
    }

    //fix;
    assert(quadToGo);
    updateNetForceData(e, quadToGo);
}

void calcNewPos(Entity *e, double dt)
{
    double SFy = e->getSFy();
    double SFx = e->getSFx();
    double m = e->getMass();

    double oldVx = e->getVx();
    double oldVy = e->getVy();
    double oldX = e->getPoint().getX();
    double oldY = e->getPoint().getY();

    double Ax = SFx / m;
    double Ay = SFy / m;

    double newVx = oldVx + Ax * dt;
    double newVy = oldVy + Ay * dt;

    double newX = oldX + newVx * dt;
    double newY = oldY + newVy * dt;

    e->setPoint(Point(newX, newY));

    e->setSFx(0);
    e->setSFy(0);

    e->setVx(newVx);
    e->setVy(newVy);

    //return isInsideUniverse(e, )
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

double Fx(Entity e1, Entity e2)
{
    double x1 = e1.getPoint().getX(), x2 = e2.getPoint().getX();
    double r = distance(e1, e2);
    double f = F(e1, e2);

    return f * (x2 - x1) / r;
}

double Fy(Entity e1, Entity e2)
{
    double y1 = e1.getPoint().getY(), y2 = e2.getPoint().getY();
    double r = distance(e1, e2);
    double f = F(e1, e2);

    return f * (y2 - y1) / r;
}

void printBHTreeUtil(BHTree *curr)
{
    Entity *e;

    if (curr == NULL)
    {
        return;
    }

    if (curr->isLeaf() && (e = curr->getEntity()) != NULL)
    {
        std::cout << e->toString() << "\n";
    }

    printBHTreeUtil(curr->getQuad1());
    printBHTreeUtil(curr->getQuad2());
    printBHTreeUtil(curr->getQuad3());
    printBHTreeUtil(curr->getQuad4());
}

void printBHTree(BHTree *curr)
{
    if (curr == NULL)
    {
        return;
    }
    std::cout << "-------------------------------------------\n";
    //std::cout << "Printing BHTree containing " << curr->getTotalEntities().size() << " entities\n";
    std::cout << "The mass center of the system is at: " << curr->getEntity()->getPoint().toString() << " with total mass " << curr->getEntity()->getMass() << "\n";
    printBHTreeUtil(curr);
    std::cout << "-------------------------------------------\n";
}

bool isInsideUniverse(Entity *en, double dim);

bool containsPoint(vector<Entity *> entities, Entity *en)
{
    for (Entity *e : entities)
    {
        if (e == en)
        {
            return true;
        }
    }

    return false;
}