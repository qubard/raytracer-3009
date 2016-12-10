#include "MetaFloor.h"

// width along x, length along z
MetaFloor::MetaFloor(double width, double length, double offsetY, Material material) {
	this->width = width;
	this->length = length;
	this->offsetY = offsetY;
	this->material = material;
}

Intersection MetaFloor::intersects(Vector3f e, Vector3f d) {
	double t = (offsetY - e.y) / d.y;

	if (t < 0) {
		return Intersection{ -1, -1, -1, false, -1 };
	}

	double intersectX = e.x + t*d.x;
	double intersectZ = e.z + t*d.z;

	if (intersectZ <= 0 && intersectZ >= -length) {
		if (intersectX >= 0 && intersectX <= width) {
			return Intersection{ intersectX, offsetY, intersectZ, true, t };
		}
	}


	return Intersection{ -1, -1, -1, false, -1 };
}

Vector3f MetaFloor::getNormal() {
	return Vector3f((double)0, (double)1, (double)0);
}

Material MetaFloor::getMaterial(Intersection i) {
	int x = i.x/5+10;
	int z = i.z/5+1;
	if ((x + z) % 2 == 1) {
		return Material{ 255, 255, 255, material.shininess };
	}
	else {
		return Material{ 0, 0, 0, material.shininess };
	}
}