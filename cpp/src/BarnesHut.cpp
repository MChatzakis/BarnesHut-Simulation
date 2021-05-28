#include <fstream>
#include <cmath>
#include <iostream>
#include <string>
#include <chrono>

#include <unistd.h>

#include "tbb/tbb.h"
#include "BarnesHut.h"

#pragma warning(disable : 588)

using namespace std::chrono;
using namespace tbb;
using namespace std;

BHTree *createBHTree(vector<Entity *> entities, double dims);

double loadEntities(string dataset, vector<Entity *> &entities);
double F(Entity e1, Entity e2, double dist);
double Fy(Entity e1, Entity e2);
double Fx(Entity e1, Entity e2);

void printEntities(vector<Entity *> entities);
void printBHTreeUtil(BHTree *curr);
void printBHTree(BHTree *curr);

void printEntitiesToFile(string filename, vector<Entity *> entities);
void appendTimeMeasurementsToFile(string filename, double time);

void BarnesHutSeq(vector<Entity *> &entities, double dims, int iterations, int dt);
void BarnesHutPar(vector<Entity *> &entities, double dims, int iterations, int threads, int dt);

void netForce(Entity *e, BHTree *bh);
void newPosition(Entity *e, double dt, double dims);

int main(int argc, char **argv)
{
    vector<Entity *> entities;
    double dims;
    int opt, iters, threads, printTime = 0;
    string filename;

    while ((opt = getopt(argc, argv, "f:i:t:mh")) != -1)
    {
        switch (opt)
        {
        case 'f':
            filename = string(optarg);
            break;
        case 'i':
            iters = stoi(optarg);
            break;
        case 't':
            threads = stoi(optarg);
            break;
        case 'm':
            printTime = 1;
            break;
        case 'h':
            break;
        }
    }

    dims = loadEntities(filename, entities);

    //printEntities(entities);

    /*This method for time measuring is accurate, giving the same results as time ./exec*/
    auto start = high_resolution_clock::now();

    /*Sequential implementation is differianted from parallel*/
    if (threads == 0)
    {
        BarnesHutSeq(entities, dims, iters, 1); //dt = 1 by default
    }
    else
    {
        BarnesHutPar(entities, dims, iters, threads, 1); //dt = 1 by default
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);

    printEntities(entities);

    /*The time measurements are appended to file to calculate statistics about the implementation*/
    if (printTime)
    {
        double seconds = duration.count() * 0.001;
        cout << "Execution time: " << seconds << " seconds.\n";
        appendTimeMeasurementsToFile("timesCPP.txt", seconds);
    }

    printEntitiesToFile("simulationCPP.txt", entities);

    return 0;
}

void BarnesHutSeq(vector<Entity *> &entities, double dims, int iterations, int dt)
{
    BHTree *bh;

    for (int i = 0; i < iterations; i++)
    {
        //Phase 1: Create a new BHTree for every iteration
        bh = createBHTree(entities, dims);

        //Phase 2: Calculate the total force acting on every body (O(nlogn))
        for (Entity *e : entities)
        {
            netForce(e, bh);
        }

        //Phase 3: For every body, calculate its new position based on the total force (O(n))
        for (Entity *e : entities)
        {
            newPosition(e, dt, dims);
        }

        //cout << "Iteration " << i + 1 << "\n";
    }
}

void BarnesHutPar(vector<Entity *> &entities, double dims, int iterations, int threads, int dt)
{
    BHTree *bh;
    size_t size = entities.size();
    size_t grainSize = size / threads;

    /*Setting the number of threads is an obsolete tactic, but it is used here for statistical reasons*/
    task_scheduler_init init(threads);

    for (int i = 0; i < iterations; i++)
    {
        /*
            Phase 1: Create a new BHTree for every simulation. 
            This action is not parallel, as it results to high overhead
        */
        bh = createBHTree(entities, dims);

        /*
            Phase 2: Calculate the total netforce acting on each body.
            The parallelism is based on the fact that each thread is assigned a chunk of bodies.
            For each body of the chunk, it changes only the his SFx and his SFy, without modifying the 
            speed or coordinates, thus there is no data race.
        */
        parallel_for(blocked_range<size_t>(0, size),
                     [&](const blocked_range<size_t> &r) -> void
                     {
                         for (size_t i = r.begin(); i != r.end(); i++)
                         {
                             netForce(entities[i], bh);
                         }
                     }); //barrier

        /*
            Phase 3: Based on Phase 2 calculations, each threads calculates the new coordinates of each chunk.
        */
        parallel_for(blocked_range<size_t>(0, size),
                     [&](const blocked_range<size_t> &r) -> void
                     {
                         for (size_t i = r.begin(); i != r.end(); i++)
                         {
                             newPosition(entities[i], dt, dims);
                         }
                     }); //barrier

        cout << "Iteration " << i + 1 << "\n";
    }
}

void netForce(Entity *e, BHTree *bh)
{
    bool pathFound = false;

    if (bh == NULL)
    {
        return;
    }

    /*
        Reaching a leaf means that the traverse has come to an end.
        It is clear that this leaf can only be the leaf containing body e, 
        else there has ocurred an error through the iterative traverse of the tree
    */
    if (bh->isLeaf())
    {
        assert(bh->getEntity() == e);
        return;
    }

    BHTree *quads[4] = {bh->getQuad1(), bh->getQuad2(), bh->getQuad3(), bh->getQuad4()};
    BHTree *quadToGo = NULL;

    /*
        Through this iteration we have two major aims:
        1. Find the subquad that entity e is located, to proceed there recursively
        2. Calculate the force acting on body e from all other subquads, with each subquad 
        represented totally as the mass center of all the bodies it contains.
    */
    for (int i = 0; i < 4; i++)
    {
        /*This operation is done only for quads that contain entities*/
        if (quads[i] != NULL)
        {
            Entity *cent = quads[i]->getEntity();
            Region reg = quads[i]->getRegion();

            if (cent == NULL)
            {
                continue;
            }

            /*
                Warning: pathFound flag is a patch for the cases that bodies are on the border of different subquads.
                e.g. If an entity is located at (0,0), we will (by default) assign in to subquad 1, but containgPoint 
                method will return true for every subquad of this region. 
                This problem is solved with the help of this flag, which is set when we find the first matching region.  
            */
            if (!pathFound && reg.containsPoint(e->getPoint()))
            {
                //cout << "Found the region that body " << e->getName() << " belongs. Going to s_quad[ " << i << " ]\n";
                quadToGo = quads[i]; //saving the destination quad
                pathFound = true;
            }
            else
            {
                /*In this case, entity cent represents the mass center of the current adjacent subquad*/

                //cout << "Calculating net force for body " << e->toString() << " by body " << cent->toString() << "\n";

                /*The force of two entities always points to one another (gravitational law)*/
                double fx = Fx(*e, *cent);
                double fy = Fy(*e, *cent);

                /*As SFx and SFy are only modifies by the thread assigned this chunk, there is no data race*/
                e->addToSFx(fx);
                e->addToSFy(fy);
            }
        }
    }

    /*quadToGo should never be NULL*/
    assert(quadToGo);
    netForce(e, quadToGo);
}

void newPosition(Entity *e, double dt, double dims)
{
    double SFy = e->getSFy();
    double SFx = e->getSFx();
    double m = e->getMass();

    double oldVx = e->getVx();
    double oldVy = e->getVy();
    double oldX = e->getPoint().getX();
    double oldY = e->getPoint().getY();

    /*Just running the Newtonian equations*/
    double Ax = SFx / m;
    double Ay = SFy / m;

    double newVx = oldVx + Ax * dt;
    double newVy = oldVy + Ay * dt;

    double newX = oldX + newVx * dt;
    double newY = oldY + newVy * dt;

    /*Pathing the coordinates in case a body leaves the universe*/
    if (newX > dims)
    {
        newX = dims;
    }

    if (newX < -dims)
    {
        newX = -dims;
    }

    if (newY > dims)
    {
        newY = dims;
    }

    if (newY < -dims)
    {
        newY = -dims;
    }

    /*Updating the bodies data*/
    e->setPoint(Point(newX, newY));

    /*This is added here because for every new simulation we may consider that we run again from the start*/
    e->setSFx(0);
    e->setSFy(0);

    e->setVx(newVx);
    e->setVy(newVy);
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

    //cout << "--- Starting parsing file\n";

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

    //cout << "--- Done parsing file\n";
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

double F(Entity e1, Entity e2, double dist)
{
    double G = 6.67 / pow(10, 11); //6.67 × 10−11
    double m1 = e1.getMass();
    double m2 = e2.getMass();

    return G * (m1 * m2) / pow(dist, 2);
}

double Fx(Entity e1, Entity e2)
{
    double x1 = e1.getPoint().getX(), x2 = e2.getPoint().getX();
    double r = distance(e1, e2);
    double f = F(e1, e2, r);

    return f * (x2 - x1) / r; //We prefer x2-x1 and not |x2-x1| to also calculate the force route (<- or ->)
}

double Fy(Entity e1, Entity e2)
{
    double y1 = e1.getPoint().getY(), y2 = e2.getPoint().getY();
    double r = distance(e1, e2);
    double f = F(e1, e2, r);

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

void printEntitiesToFile(string filename, vector<Entity *> entities)
{
    ofstream outfile;

    outfile.open(filename); //, std::ios_base::app); // append instead of overwrite

    for (Entity *e : entities)
    {
        double X = e->getPoint().getX();
        double Y = e->getPoint().getY();
        double M = e->getMass();
        double Vy = e->getVy();
        double Vx = e->getVx();
        string S = e->getName();

        outfile << X << " " << Y << " " << Vx << " " << Vy << " " << M << " " << S << endl;
    }

    outfile.close();
}

void appendTimeMeasurementsToFile(string filename, double time)
{
    ofstream outfile;

    outfile.open(filename, std::ios_base::app); // append instead of overwrite

    outfile << time << endl;

    outfile.close();
}
