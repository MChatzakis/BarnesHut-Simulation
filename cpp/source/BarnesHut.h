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

class Entity
{
private:
    double Vy;
    double Vx;
    double mass;

    std::string name;
    Point point;

public:
    Entity(Point _point, std::string _name, double _Vx, double _Vy, double _mass) : point{_point}, name{_name}, Vx{_Vx}, Vy{_Vy}, mass{_mass} {}

    Point getPoint()
    {
        return point;
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

class Region
{
private:
    Point center;
    double dimension; // start = center - dimension, end = center + dimension

public:
    Region(Point _center, double _dimension) : center{_center}, dimension{_dimension} {}

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

    std::string toString()
    {
        std::string str = "[";
        str = center.toString() + " " + std::to_string(dimension) + "]";
        return str;
    }
};

class BHTree
{
private:
    Region region;

    std::vector<Entity *> totalEntities;

    Entity *entity = NULL;

    BHTree *quad1 = NULL;
    BHTree *quad2 = NULL;
    BHTree *quad3 = NULL;
    BHTree *quad4 = NULL;

public:
    BHTree(Region _region) : region{_region} {}

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

    void createSubQuads()
    {
        double xCenter = region.getCenter().getX(), yCenter = region.getCenter().getY(), dim = region.getDimension();

        quad1 = new BHTree(Region(Point(xCenter + dim / 2, yCenter + dim / 2), dim / 2));
        quad2 = new BHTree(Region(Point(xCenter - dim / 2, yCenter + dim / 2), dim / 2));
        quad3 = new BHTree(Region(Point(xCenter - dim / 2, yCenter - dim / 2), dim / 2));
        quad4 = new BHTree(Region(Point(xCenter + dim / 2, yCenter - dim / 2), dim / 2));

        std::cout << "New subquads created\n";
    }

    bool isLeaf()
    {
        return !(quad1 || quad2 || quad3 || quad4);
    }

    bool insertEntity(Entity *_entity)
    {
        if (!region.containsPoint(_entity->getPoint()))
        {
            std::cout << "Entity out of range!\n";
            return false;
        }

        if (isLeaf())
        {
            //empty leaf case
            if (entity == NULL)
            {
                std::cout << "Insert to empty leaf\n";
                entity = _entity;
                return true;
            }

            //else divide the tree
            createSubQuads();

            if (quad1->insertEntity(entity) ||
                quad2->insertEntity(entity) ||
                quad3->insertEntity(entity) ||
                quad4->insertEntity(entity))
            {
                totalEntities.push_back(entity);
                entity = NULL;
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
            totalEntities.push_back(_entity);
            return true;
        }
        else
        {
            assert(0);
        }

        return false;
    }
};

void printBHTree(BHTree *curr)
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

    printBHTree(curr->getQuad1());
    printBHTree(curr->getQuad2());
    printBHTree(curr->getQuad3());
    printBHTree(curr->getQuad4());
}
