#pragma once

#include "MetaGeometry.h"

class MetaBackWall : public MetaGeometry {
	public:
		MetaBackWall(double width, double height, double offsetZ, Material material);
		virtual Intersection intersects(Vector3f e, Vector3f d);
		virtual Vector3f getNormal();
		double width, height, offsetZ;
};
