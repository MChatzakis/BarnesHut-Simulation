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

    double SFx = 0;
    double SFy = 0;

    std::string name;
    Point point;

    std::vector<Entity> interactingEntities;

public:
    Entity(Point _point, std::string _name, double _Vx, double _Vy, double _mass) : point{_point}, name{_name}, Vx{_Vx}, Vy{_Vy}, mass{_mass} {}

    Point getPoint()
    {
        return point;
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

    std::vector<Entity> getInteractingEntities()
    {
        return interactingEntities;
    }

    void clearInteractionEntities()
    {
        interactingEntities.clear();
    }

    void addInteractionEntity(Entity en)
    {
        interactingEntities.push_back(en);
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
        std::string str;
        double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
        double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;
        //str += center.toString() + " " + std::to_string(dimension) + "]";
        str = "x:[" + std::to_string(xStart) + "," + std::to_string(xEnd) + "] y:[" + std::to_string(yStart) + "," + std::to_string(yEnd) + "]";
        return str;
    }
};

class BHTree
{
private:
    Region region;

    std::vector<Entity *> totalEntities;

    Entity *entity = NULL; //When the current node is not leaf, this entity represents the mass center of the region

    BHTree *quad1 = NULL;
    BHTree *quad2 = NULL;
    BHTree *quad3 = NULL;
    BHTree *quad4 = NULL;

public:
    BHTree(Region _region) : region{_region} { std::cout << "Created BHTree in region: " << region.toString() << "\n"; }

    std::vector<Entity *> getTotalEntities()
    {
        return totalEntities;
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

        std::cout << "---------------- Creating sub quads ----------------\n";

        quad1 = new BHTree(Region(Point(xCenter + dim / 2, yCenter + dim / 2), dim / 2));
        quad2 = new BHTree(Region(Point(xCenter - dim / 2, yCenter + dim / 2), dim / 2));
        quad3 = new BHTree(Region(Point(xCenter - dim / 2, yCenter - dim / 2), dim / 2));
        quad4 = new BHTree(Region(Point(xCenter + dim / 2, yCenter - dim / 2), dim / 2));

        std::cout << "----------------   Creating ended   ----------------\n";
    }

    bool isLeaf()
    {
        return !(quad1 || quad2 || quad3 || quad4);
    }

    bool insertEntity(Entity *_entity)
    {

        if (!region.containsPoint(_entity->getPoint()))
        {
            //std::cout << "Entity out of range!\n";
            return false;
        }

        if (isLeaf())
        {
            //empty leaf case
            if (entity == NULL)
            {
                //std::cout << "Inserted to empty leaf\n";
                std::cout << "Insert entity " << _entity->getName() << " to empty leaf in the region " << region.toString() << ".\n";
                entity = _entity;
                totalEntities.push_back(_entity);
                return true;
            }

            if (entity->getPoint().getX() == _entity->getPoint().getX() &&
                entity->getPoint().getY() == _entity->getPoint().getY())
            {
                std::cout << "Colision detected -- Patching the points\n";

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
                //totalEntities.push_back(entity);
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
            //calculate mass center
            entity = calcRegionMassCenterEntity();
            return true;
        }
        else
        {
            assert(0);
        }

        return false;
    }

    double calcTotalMass()
    {
        double m = 0;
        for (Entity *e : totalEntities)
        {
            m += e->getMass();
        }
        return m;
    }

    Entity *calcRegionMassCenterEntity()
    {
        double totMass = calcTotalMass();
        double totCenterX = 0;
        double totCenterY = 0;

        for (Entity *e : totalEntities)
        {
            totCenterX += e->getMass() * e->getPoint().getX();
            totCenterY += e->getMass() * e->getPoint().getY();
        }

        totCenterX = totCenterX / totMass;
        totCenterY = totCenterY / totMass;

        return new Entity(Point(totCenterX, totCenterY), "MassCR", 0, 0, totMass);
    }
};
