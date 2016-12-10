#include "MetaBackWall.h"

MetaBackWall::MetaBackWall(double width, double height, double offsetZ, Material material) {
	this->width = width;
	this->height = height;
	this->offsetZ = offsetZ;
	this->material = material;
}

Intersection MetaBackWall::intersects(Vector3f e, Vector3f d) {
	double t = (offsetZ - e.z) / d.z;

	if (t < 0) {
		return Intersection{ -1, -1, -1, false, -1 };
	}

	double intersectY = e.y + t*d.y;
	double intersectX = e.x + t*d.x;

	if (intersectX >= 0 && intersectX <= width) {
		if (intersectY >= 0 && intersectY <= height) {
			return Intersection{ intersectX, intersectY, offsetZ, true, t };
		}
	}


	return Intersection{ -1, -1, -1, false, -1 };
}

Vector3f MetaBackWall::getNormal() {
	return Vector3f((double)0,(double)0,(double)1);
}