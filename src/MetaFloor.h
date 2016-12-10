#pragma once
#include "MetaGeometry.h"

class MetaFloor : public MetaGeometry {
public:
	MetaFloor(double width, double length, double offsetY, Material material);
	virtual Intersection intersects(Vector3f e, Vector3f d);
	virtual Vector3f getNormal();
	Material getMaterial(Intersection i);
	double width, length, offsetY;
};