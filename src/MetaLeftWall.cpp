#include "MetaLeftWall.h"

MetaLeftWall::MetaLeftWall(double width, double height, double offsetX, Vector3f normal, Material material) {
	this->width = width;
	this->height = height;
	this->offsetX = offsetX;
	this->normal = normal;
	this->material = material;
}

Intersection MetaLeftWall::intersects(Vector3f e, Vector3f d) {
	// check if e+td reaches x = xOffset;
	// y is bounded by height 0 <= y <= height
	// z is bounded by width (15)
	// 0 <= z <= -15
	// we're looking down the positive z
	double t = (offsetX - e.x)/d.x;
	
	if (t < 0) {
		return Intersection{ -1, -1, -1, false, -1 };
	}

	double intersectZ = e.z + t*d.z;
	double intersectY = e.y + t*d.y;

	if (intersectZ >= -width && intersectZ <= 0) {
		if (intersectY >= 0 && intersectY <= height) {
			return Intersection{ offsetX, intersectY, intersectZ, true, t };
		}
	}

	return Intersection{ -1, -1, -1, false, -1 };
}

Vector3f MetaLeftWall::getNormal() {
	return this->normal;
}