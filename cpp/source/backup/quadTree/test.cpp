#include "QuadTree.h"

int main()
{
    QuadTree *qt = new QuadTree(Region2D(Point2D(0, 0), 4));

    qt->insertPoint(new Point2D(1, 1));
    qt->insertPoint(new Point2D(1, 2));
    qt->insertPoint(new Point2D(1, 4));
    qt->insertPoint(new Point2D(1, 3));

    qt->insertPoint(new Point2D(2, 0));
    qt->insertPoint(new Point2D(2, 1));
    qt->insertPoint(new Point2D(2, 3));

    qt->insertPoint(new Point2D(3, 0));
    qt->insertPoint(new Point2D(3, 1));
    qt->insertPoint(new Point2D(3, 2));
    qt->insertPoint(new Point2D(3, 3));
    qt->insertPoint(new Point2D(3, 4));

    qt->insertPoint(new Point2D(4, 1));
    qt->insertPoint(new Point2D(4, 2));
    qt->insertPoint(new Point2D(4, 3));
    
    qt->insertPoint(new Point2D(0, 1));
    qt->insertPoint(new Point2D(0, 2));
    qt->insertPoint(new Point2D(0, 3));

    print(qt);

    return 0;
}