
#include "MetaGeometry.h"
#include "MetaSphere.h"

MetaSphere::MetaSphere(double x, double y, double z, double radius, Material material)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->radius = radius;
	this->material = material;
}

Intersection MetaSphere::intersects(Vector3f e, Vector3f d) { // make sure e and d are DOUBLE vectors
	Vector3f c = Vector3f(x, y, z);

	float edotc = Vector3f().dot(d, e - c);
	float dotd = Vector3f().dot(d, d);
	float k = Vector3f().dot(e - c, e - c);
	float det = edotc*edotc - dotd*(k - radius*radius);
	if (det < 0) {
		return Intersection{ -1, -1, -1, false, -1 };
	}
	double t1 = (Vector3f().dot(-d, e - c) + sqrt(det)) / dotd;
	double t2 = (Vector3f().dot(-d, e - c) - sqrt(det)) / dotd;
	if (t2 >= 0 && t2 <= t1) {
		return Intersection{ (double)e.x+t2*(double)d.x, (double)e.y + t2*(double)d.y, (double)e.z + t2*(double)d.z, true, t2 };
	}
	else if (t1 >= 0 && t1 <= t2) {
		return Intersection{ (double)e.x + t1*(double)d.x, (double)e.y + t1*(double)d.y, (double)e.z + t1*(double)d.z, true, t1 };
	}
	return Intersection{ -1, -1, -1, false, -1 };
}

Vector3f MetaSphere::getNormal() {
	return Vector3f(-999, -999, -999);
}