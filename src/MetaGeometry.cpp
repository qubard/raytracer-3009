#include "MetaGeometry.h"


Intersection MetaGeometry::intersects(Vector3f e, Vector3f d) {
	return{ -1, -1, -1, false, -1 };
}

Vector3f MetaGeometry::getNormal() {
	return Vector3f(0, 0, 0);
}

Material MetaGeometry::getMaterial(Intersection i) {
	return material;
}