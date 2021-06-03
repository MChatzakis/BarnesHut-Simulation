#include <iostream>
#include <string>
#include <vector>

#include <assert.h>

class Point
{
private:
    double x;
    double y;

public:
    Point() { x = y = 0; }

    Point(double _x, double _y) : x{_x}, y{_y} {}

    Point(const Point &_point)
    {
        x = _point.x;
        y = _point.y;
    }

    void setX(double x)
    {
        this->x = x;
    }

    void setY(double y)
    {
        this->y = y;
    }

    double getX()
    {
        return x;
    }

    double getY()
    {
        return y;
    }

    std::string toString()
    {
        return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
};

class Region
{
private:
    Point center;
    double dimension; // start = center - dimension, end = center + dimension

public:
    Region(Point _center, double _dimension) : center{_center}, dimension{_dimension} {}
    Region() : center{Point()}, dimension{0} {}

    Point getCenter()
    {
        return center;
    }

    double getDimension()
    {
        return dimension;
    }

    bool containsPoint(Point _center)
    {
        double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
        double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;

        double currY = _center.getY();
        double currX = _center.getX();

        return (currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
    }

    bool containsPoint(Point _center, int quadNo)
    {
        double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
        double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;

        double currY = _center.getY();
        double currX = _center.getX();

        switch (quadNo)
        {
        case 1:
            //std::cout << "Point contained in quad no " << 1 << "\n";
            return (currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
        case 2:
            //std::cout << "Point contained in quad no " << 2 << "\n";
            return (currY <= yEnd && currY >= yStart) && (currX < xEnd && currX >= xStart);
        case 3:
            //std::cout << "Point contained in quad no " << quadNo << "\n";
            return (currY < yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
        case 4:
            //std::cout << "Point contained in quad no " << quadNo << "\n";
            return (currY < yEnd && currY >= yStart) && (currX <= xEnd && currX > xStart);
        default:
            assert(0);
        }

        assert(0);
        return false;
    }

    std::string toString()
    {
        std::string str;
        double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
        double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;
        str = "x:[" + std::to_string(xStart) + "," + std::to_string(xEnd) + "] y:[" + std::to_string(yStart) + "," + std::to_string(yEnd) + "]";
        return str;
    }
};

class Entity
{
private:
    double Vy;
    double Vx;

    double mass;

    double SFx = 0;
    double SFy = 0;

    bool isMassCenter = false;

    std::string name;
    Point point;
    Region currentRegion;

public:
    Entity(Point _point, std::string _name, double _Vx, double _Vy, double _mass) : point{_point}, name{_name}, Vx{_Vx}, Vy{_Vy}, mass{_mass} {}

    Region getCurrentRegion()
    {
        return currentRegion;
    }

    void setCurrentRegion(Region _reg)
    {
        currentRegion = _reg;
    }

    Point getPoint()
    {
        return point;
    }

    bool getIsMassCenter()
    {
        return isMassCenter;
    }

    void setIsMassCenter(bool mc)
    {
        this->isMassCenter = mc;
    }

    void setPoint(Point dPoint)
    {
        point = dPoint;
    }

    double getMass()
    {
        return mass;
    }

    double getVy()
    {
        return Vy;
    }

    double getVx()
    {
        return Vx;
    }

    void setVy(double _Vy)
    {
        Vy = _Vy;
    }

    void setVx(double _Vx)
    {
        Vx = _Vx;
    }

    double getSFy()
    {
        return SFy;
    }

    void setSFy(double SFy)
    {
        this->SFy = SFy;
    }

    double getSFx()
    {
        return SFx;
    }

    void setSFx(double SFx)
    {
        this->SFx = SFx;
    }

    void addToSFy(double dfy)
    {
        SFy += dfy;
    }

    void addToSFx(double dfx)
    {
        SFx += dfx;
    }

    std::string getName()
    {
        return name;
    }

    std::string toString()
    {
        std::string output;
        output += "";
        output += name + ": " + point.toString() +
                  " " + std::to_string(Vx) + " " + std::to_string(Vy) +
                  " " + std::to_string(mass);
        output += " ";
        return output;
    }
};

Entity *massCenter(Entity *e1, Entity *e2)
{
    double centerX, centerY, mass = e1->getMass() + e2->getMass();

    centerX = ((e1->getPoint().getX() * e1->getMass()) + (e2->getPoint().getX() * e2->getMass())) / mass;
    centerY = ((e1->getPoint().getY() * e1->getMass()) + (e2->getPoint().getY() * e2->getMass())) / mass;

    return new Entity(Point(centerX, centerY), "MassCenter", 0, 0, mass);
}

class BHTree
{
private:
    Region region;

    Entity *entity = NULL; //When the current node is not leaf, this entity represents the mass center of the region

    BHTree *quad1 = NULL;
    BHTree *quad2 = NULL;
    BHTree *quad3 = NULL;
    BHTree *quad4 = NULL;

public:
    BHTree(Region _region) : region{_region}
    {
        //std::cout << "Created BHTree in region: " << region.toString() << "\n";
    }

    BHTree *getQuad1()
    {
        return quad1;
    }

    BHTree *getQuad2()
    {
        return quad2;
    }

    BHTree *getQuad3()
    {
        return quad3;
    }

    BHTree *getQuad4()
    {
        return quad4;
    }

    Entity *getEntity()
    {
        return entity;
    }

    Region getRegion()
    {
        return region;
    }

    void createSubQuads()
    {
        double xCenter = region.getCenter().getX(), yCenter = region.getCenter().getY(), dim = region.getDimension();

        //std::cout << "---------------- Creating sub quads ----------------\n";
        try
        {
            quad1 = new BHTree(Region(Point(xCenter + dim / 2, yCenter + dim / 2), dim / 2));
            quad2 = new BHTree(Region(Point(xCenter - dim / 2, yCenter + dim / 2), dim / 2));
            quad3 = new BHTree(Region(Point(xCenter - dim / 2, yCenter - dim / 2), dim / 2));
            quad4 = new BHTree(Region(Point(xCenter + dim / 2, yCenter - dim / 2), dim / 2));
        }
        catch (std::bad_alloc &)
        {

            std::cout << "Memory failed!!\n";
            exit(-1);
        }

        //std::cout << "----------------   Creating ended   ----------------\n";
    }

    bool isLeaf()
    {
        return !(quad1 || quad2 || quad3 || quad4);
    }

    bool insertEntity(Entity *_entity)
    {
        if (!region.containsPoint(_entity->getPoint()))
        {
            return false;
        }

        if (isLeaf())
        {
            //empty leaf case
            if (entity == NULL)
            {
                //std::cout << "Insert entity " << _entity->getName() << " to empty leaf in the region " << region.toString() << ".\n";
                entity = _entity;
                _entity->setCurrentRegion(region);
                return true;
            }

            if (entity->getPoint().getX() == _entity->getPoint().getX() &&
                entity->getPoint().getY() == _entity->getPoint().getY())
            {
                //std::cout << "Colision detected -- Patching the points\n";

                double dx = 0.01;
                double dy = 0.01;

                _entity->setPoint(Point(_entity->getPoint().getX() + dx, _entity->getPoint().getY() + dy));
            }

            //else divide the tree
            createSubQuads();

            if (quad1->insertEntity(entity) ||
                quad2->insertEntity(entity) ||
                quad3->insertEntity(entity) ||
                quad4->insertEntity(entity))
            {
            }
            else
            {
                assert(0);
            }
        }

        if (quad1->insertEntity(_entity) ||
            quad2->insertEntity(_entity) ||
            quad3->insertEntity(_entity) ||
            quad4->insertEntity(_entity))
        {
            Entity *cmass = massCenter(entity, _entity);
            cmass->setIsMassCenter(true);
            /*
                If this deallocation is not done, it could actually
                crash the simulation because of the high memory requirements
                Deallocation is done only for the BHTree - created nodes, and not for the actual bodies.    
            */
            //if(entity->getName() == "MassCenter"){ //Removing nodes from their name is not a good practice.
            if (entity->getIsMassCenter())
            {
                //Getting here means that the memory pointed to entity is allocated by the BHTree
                delete entity;
                entity = NULL;
            }

            entity = cmass;
            return true;
        }
        else
        {
            assert(0);
        }

        return false;
    }
};
