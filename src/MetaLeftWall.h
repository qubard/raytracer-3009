#pragma once

#include "MetaGeometry.h"

class MetaLeftWall : public MetaGeometry {
	public:
		MetaLeftWall(double width, double height, double offsetX, Vector3f normal, Material material);
		virtual Intersection intersects(Vector3f e, Vector3f d);
		virtual Vector3f getNormal();
		double width, height, offsetX;
		Vector3f normal;
};
