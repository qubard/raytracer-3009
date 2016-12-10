#pragma once

#include "graphicsObject.h"
#include "geometry_common.h"

#include "Material.h"

struct Intersection {
	double x;
	double y;
	double z;
	bool intersects;
	double t;
};

class MetaGeometry {
	public:
		virtual Intersection intersects(Vector3f e, Vector3f d);
		virtual Material getMaterial(Intersection i);
		double x, y, z;
		virtual Vector3f getNormal();
		Material material;
};
