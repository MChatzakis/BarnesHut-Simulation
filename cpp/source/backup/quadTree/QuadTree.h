#include <iostream>
#include <vector>
#include <assert.h>

class Point2D
{
private:
    double x;
    double y;

public:
    Point2D() { x = y = 0; }

    Point2D(double _x, double _y) : x{_x}, y{_y} {}

    Point2D(const Point2D &_point)
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
};

class Region2D
{
private:
    Point2D center;
    double dimension; // start = center - dimension, end = center + dimension

public:
    Region2D(Point2D _center, double _dimension) : center{_center}, dimension{_dimension} {}

    Point2D getCenter()
    {
        return center;
    }

    double getDimension()
    {
        return dimension;
    }

    bool containsPoint(Point2D _center)
    {
        double yEnd = center.getY() + dimension, yStart = center.getY() - dimension;
        double xEnd = center.getX() + dimension, xStart = center.getX() - dimension;
        double currY = _center.getY();
        double currX = _center.getX();

        return (currY <= yEnd && currY >= yStart) && (currX <= xEnd && currX >= xStart);
    }

    void toString()
    {
        //std::cout << "Region:\nx: [" << point->get
    }
};

class QuadTree
{
private:
    Region2D region;

    std::vector<Point2D *> totalPoints;

    Point2D *point = NULL;

    QuadTree *upperRight = NULL;
    QuadTree *upperLeft = NULL;
    QuadTree *downLeft = NULL;
    QuadTree *downRight = NULL;

public:
    QuadTree(Region2D _region) : region{_region} {}

    QuadTree *getUpperRightTree()
    {
        return upperRight;
    }

    QuadTree *getUpperLeftTree()
    {
        return upperLeft;
    }

    QuadTree *getDownLeftTree()
    {
        return downLeft;
    }

    QuadTree *getDownRightTree()
    {
        return downRight;
    }

    Point2D *getPoint()
    {
        return point;
    }

    void
    createSubQuads()
    {
        double xCenter = region.getCenter().getX(), yCenter = region.getCenter().getY(), dim = region.getDimension();

        upperRight = new QuadTree(Region2D(Point2D(xCenter + dim / 2, yCenter + dim / 2), dim / 2));
        upperLeft = new QuadTree(Region2D(Point2D(xCenter - dim / 2, yCenter + dim / 2), dim / 2));
        downLeft = new QuadTree(Region2D(Point2D(xCenter - dim / 2, yCenter - dim / 2), dim / 2));
        downRight = new QuadTree(Region2D(Point2D(xCenter + dim / 2, yCenter - dim / 2), dim / 2));

        std::cout << "New subquads created\n";
    }

    bool isLeaf()
    {
        return !(upperRight || upperLeft || downLeft || downRight);
    }

    bool insertPoint(Point2D *_point)
    {

        if (!region.containsPoint(*_point))
        {
            std::cout << "Out of range!\n";
            return false;
        }

        if (isLeaf())
        {
            //empty leaf case
            if (point == NULL)
            {
                std::cout << "Insert to empty leaf\n";
                point = _point;
                return true;
            }

            createSubQuads();

            if (upperRight->insertPoint(point))
            {
                //ok
            }
            else if (upperLeft->insertPoint(point))
            {
                //ok
            }
            else if (downLeft->insertPoint(point))
            {
                //ok
            }
            else if (downRight->insertPoint(point))
            {
                //ok
            }
            else
            {
                assert(0);
            }

            totalPoints.push_back(point);
            point = NULL;
        }

        if (upperRight->insertPoint(_point))
            return true;
        if (upperLeft->insertPoint(_point))
            return true;
        if (downLeft->insertPoint(_point))
            return true;
        if (downRight->insertPoint(_point))
            return true;

        assert(0);

        return false;
    }
};

void print(QuadTree *curr)
{
    Point2D *p;

    if (curr == NULL)
    {
        return;
    }

    if (curr->isLeaf() && (p = curr->getPoint()) != NULL)
    {
        std::cout << "Point: [ " << p->getX() << "," << p->getY() << " ]\n";
    }

    print(curr->getUpperRightTree());
    print(curr->getUpperLeftTree());
    print(curr->getDownLeftTree());
    print(curr->getDownRightTree());
}