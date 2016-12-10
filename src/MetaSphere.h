#pragma once

#include "MetaGeometry.h"

class MetaSphere : public MetaGeometry {
	public:
		MetaSphere(double x, double y, double z, double radius, Material material);
		virtual Intersection intersects(Vector3f e, Vector3f d);
		virtual Vector3f getNormal();
	private:
		double radius;
};