/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package structures;

import lombok.Data;

/**
 *
 * @author manos
 */
@Data
public class BHTree {

    private Region region;

    private BHTree quad1 = null;
    private BHTree quad2 = null;
    private BHTree quad3 = null;
    private BHTree quad4 = null;

    private Entity entity;

    private int totalEntities = 0;

    public BHTree(Region region) {
        this.region = region;
        System.out.println("Created BHTree in region: " + region.toString());
    }

    private void createSubQuads() {
        double xCenter = region.getCenter().getX(), yCenter = region.getCenter().getY(), dim = region.getDimension();

        System.out.println("---------------- Creating sub quads ----------------");

        quad1 = new BHTree(new Region(new Point(xCenter + dim / 2, yCenter + dim / 2), dim / 2));
        quad2 = new BHTree(new Region(new Point(xCenter - dim / 2, yCenter + dim / 2), dim / 2));
        quad3 = new BHTree(new Region(new Point(xCenter - dim / 2, yCenter - dim / 2), dim / 2));
        quad4 = new BHTree(new Region(new Point(xCenter + dim / 2, yCenter - dim / 2), dim / 2));

        System.out.println("----------------   Creating ended   ----------------");
    }

    public boolean isLeaf() {
        return (quad1 == null && quad2 == null && quad3 == null && quad4 == null);
    }

    public boolean insertEntity(Entity _entity) {

        if (!region.containsPoint(_entity.getPoint())) {
            return false;
        }

        if (isLeaf()) {

            //empty leaf case
            if (entity == null) {
                System.out.println("Insert entity " + _entity.getName() + " to empty leaf in the region " + region.toString());
                entity = _entity;
                totalEntities++;
                return true;
            }

            if (entity.getPoint().getX() == _entity.getPoint().getX() && entity.getPoint().getY() == _entity.getPoint().getY()) {
                System.out.println("Colision detected -- Patching the points");

                double dx = 0.01;
                double dy = 0.01;

                _entity.setPoint(new Point(_entity.getPoint().getX() + dx, _entity.getPoint().getY() + dy));
            }

            //else divide the tree
            createSubQuads();

            if (quad1.insertEntity(entity) || quad2.insertEntity(entity) || quad3.insertEntity(entity) || quad4.insertEntity(entity)) {
                //entity = null;
            } else {
                assert (false);
            }
        }

        if (quad1.insertEntity(_entity) || quad2.insertEntity(_entity) || quad3.insertEntity(_entity) || quad4.insertEntity(_entity)) {

            //calculate mass center
            entity = Entity.massCenter(entity, _entity);
            totalEntities++;

            return true;
        } else {
            assert (false);
        }

        return false;
    }

    public static void printBHTree(BHTree curr) {
        if (curr == null) {
            return;
        }

        System.out.println("-------------------------------------------");
        System.out.println("The mass center of the system : " + curr.getEntity().getPoint().toString() + " total mass: " + curr.getEntity().getMass() + " total bodies: " + curr.getTotalEntities());
        printBHTreeUtil(curr);

        System.out.println("-------------------------------------------");
    }

    public static void printBHTreeUtil(BHTree curr) {
        Entity e;

        if (curr == null) {
            return;
        }

        if (curr.isLeaf() && (e = curr.getEntity()) != null) {
            System.out.println(e.toString());
        }

        printBHTreeUtil(curr.getQuad1());
        printBHTreeUtil(curr.getQuad2());
        printBHTreeUtil(curr.getQuad3());
        printBHTreeUtil(curr.getQuad4());
    }
}
