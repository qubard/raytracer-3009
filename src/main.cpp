const float PI = 3.14159265359;

const int screenWidth = 640;
const int screenHeight = 640;

unsigned char pixels[screenWidth*screenHeight][3];

Vector3f viewerPosition = Vector3f(200, 200, 200);
Vector3f lookAtPoint = Vector3f(100, 10, 100);
Vector3f upVector = Vector3f(0, 1, 0);

MetaSphere sphere1 = MetaSphere(10, 6, -20, 2, Material{ 255, 204, 153, 0});
MetaSphere sphere2 = MetaSphere(15, 4, -5, 2, Material{ 255, 204, 255, 0.4 });
MetaSphere sphere3 = MetaSphere(30, 4, -15, 2, Material{ 204, 234, 255, 0.3 });

MetaLeftWall leftWall = MetaLeftWall(45, 30, 0, Vector3f((double)1, (double)0, (double)0), Material { 255, 0, 0, 0.8 });
MetaLeftWall rightWall = MetaLeftWall(45, 30, 45, Vector3f((double)-1, (double)0, (double)0), Material { 0, 255, 0, 0.4 });
MetaBackWall backWall = MetaBackWall(45, 30, -45, Material { 0, 0, 255, 0.3 });
MetaFloor FLOOR = MetaFloor(45, 45, 0, Material { 255, 0, 255, 0.2 });

Vector3f light1 = Vector3f((double)0,(double)0,(double)50);
Vector3f light2 = Vector3f((double)30, (double)30, (double)50);

vector<MetaGeometry*> objects;

void initObjects() {
	objects.push_back(&sphere1);
	objects.push_back(&sphere2);
	objects.push_back(&sphere3);
	objects.push_back(&rightWall);
	objects.push_back(&leftWall);
	objects.push_back(&backWall);
	objects.push_back(&FLOOR);
}


void setPixel(int x, int y, char r, char g, char b) {
	int offset = y * screenWidth;
	pixels[x+offset][0] = r;
	pixels[x+offset][1] = g;
	pixels[x+offset][2] = b;
}


MetaGeometry * firstObject(Vector3f e, Vector3f d) {
	double min = -1;
	MetaGeometry * ret = NULL;
	for (std::vector<MetaGeometry*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		Intersection i = (*it)->intersects(e, d);
		if (min == -1 || (i.t >= 0 && i.t < min)) {
			if (i.intersects == true) {
				min = i.t;
				ret = *it; // i know this is bad
			}
		}
	} // find the smallest t for intersection
	return ret;
}

float max(double a, double b) {
	if (a > b) {
		return a;
	}
	return b;
}

Material computeLighting(Material original, Vector3f light, Vector3f eyepos, Vector3f pos, Vector3f normal) {
	Vector3f ray = light - pos;
	ray.normalize();
	double epsilon = (double)0.1;
	if (firstObject(pos+ray*epsilon, ray) != NULL) {
		return Material{ 0,0,0,0 };
	}
	Vector3f ka = Vector3f((double)1.0, (double)1.0, (double)1.0);
	Vector3f kd = Vector3f((double)0.75, (double)0.75, (double)0.75);
	Vector3f ks = Vector3f((double)1.0, (double)1.0, (double)1.0);
	Vector3f negRay = -ray;
	Vector3f reflected = negRay - 2 * Vector3f().dot(negRay, normal)*normal;
	reflected.normalize();
	Vector3f v = eyepos - pos;
	v.normalize();

	Vector3f Ld = Vector3f((float)original.r / (float)255, (float)original.g / (float)255, (float)original.b / (float)255);
	Vector3f La = Vector3f((float)original.r / (float)255, (float)original.g / (float)255, (float)original.b / (float)255);;
	Vector3f Ls = Vector3f(0.9, 0.9, 0.9);

	float ns = 1.0;

	float Ia = 1.0;
	float Id = max(Vector3f().dot(ray, normal),0.0);
	float Is = pow(max(Vector3f().dot(v, reflected), 0.0), ns);

	Vector3f diffuse = kd*Ld*Id;
	Vector3f ambient = ka*La*Ia;
	Vector3f specular = ks*Ls*Is;

	Vector3f ret = (ambient+diffuse+specular)/3;
	if (ret.x > 1) {
		ret.x = 1;
	}
	if (ret.y > 1) {
		ret.y = 1;
	}
	if (ret.z > 1) {
		ret.z = 1;
	}
	return Material{ (int)(255*ret.x),  (int)(255 * ret.y),  (int)(255 * ret.z), original.shininess };
}


// e,d make up the rays
Material computeColour(Vector3f e, Vector3f d, int depth) {
	MetaGeometry * first = firstObject(e, d);
	Material colour = Material { 0, 0, 0, 0 };
	if (first != NULL && depth > 0) {
		Intersection i = first->intersects(e, d);
		e = Vector3f(i.x, i.y, i.z);
		//colour = first->getMaterial(i); // ambient colour is by default
		Vector3f normal = first->getNormal();
		if (normal.x == -999) {
			normal = Vector3f(i.x, i.y, i.z) - Vector3f(first->x, first->y, first->z); // move the coordinate of the sphere to the origin
		}
		normal.normalize();
		Material original = first->getMaterial(i);
		Material c1 = computeLighting(original, light1, Vector3f(5, 8, 0), e, normal);
		Material c2 = computeLighting(original, light2, Vector3f(5, 8, 0), e, normal);
		colour = Material{ (c1.r + c2.r)/2, (c1.g + c2.g) / 2, (c1.b + c2.b) / 2, c1.shininess };
		// to disable lighting do colour = orignial;
		double shininess = colour.shininess;
		// shininess = 0.25 -> 25% reflective, keeps (1-shininess) of its colour, absorbs shininess from other colours
		double epsilon = (double)0.1;
		Vector3f reflected = d - 2 * Vector3f().dot(d, normal)*normal; // newD
		reflected.normalize();
		Vector3f newE = e + reflected*epsilon;  // offset by epsilon in direction of reflected vector;
		Material newColour = computeColour(newE, reflected, depth - 1);
		colour = Material{
			(int)(colour.r*(1 - shininess) + newColour.r*shininess),
			(int)(colour.g*(1 - shininess) + newColour.g*shininess),
			(int)(colour.b*(1 - shininess) + newColour.b*shininess),
			shininess // not used
		};
	}
	else if (first == NULL || depth <= 0) {
		return Material{ 0, 0, 0, 0 };
	}
	return colour;
}

Vector3f rayAt(double x, double y, double fov) {
	double relativeX = 2 * (double)x / (double)screenWidth - 1;
	double relativeY = 2 * (double)y / (double)screenHeight - 1;
	Vector3f ray = Vector3f((double)relativeX*(double)sin(fov), (double)relativeY*(double)sin(fov), -(double)1);
	ray.normalize();
	return ray;
}

void render()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH);
	glRasterPos2i(-1, -1); // screen is in range of [-1,1]
	double fov = PI / 4;
	double aspect = screenHeight / screenWidth;
	double viewerOffsetX = (double)4.2; 
	double viewerOffsetY = (double)0;

	for (int y = 0; y < screenHeight; y++) {
		for (int x = 0; x < screenWidth; x++) {
			Vector3f ray = rayAt(x, y, fov);
			double worldZ = (double)6;
			double worldX = viewerOffsetX + (double)15*(double)x / (double)screenWidth;
			double worldY = viewerOffsetY + (double)15*aspect*(double)y / (double)screenHeight;
			Vector3f e = Vector3f(worldX, worldY, worldZ);
			Material colour = computeColour(e, ray, 3);
			if (y > 0 && y < screenHeight) {
				if (x > 0 && x < screenWidth) {
					// ensure we always have 4 pixels
					Material colour1 = computeColour(e, rayAt(x-1,y,fov), 3);
					Material colour2 = computeColour(e, rayAt(x + 1, y, fov), 3);
					Material colour3 = computeColour(e, rayAt(x, y+1, fov), 3);
					Material colour4 = computeColour(e, rayAt(x, y-1, fov), 3);
					setPixel(x, y, 
						(colour1.r+colour2.r+colour3.r+colour4.r+colour.r)/5, 
						(colour1.g + colour2.g + colour3.g + colour4.g + colour.g) / 5,
						(colour1.b + colour2.b + colour3.b + colour4.b + colour.b) / 5);
				}
			}
			else {
				setPixel(x, y, colour.r, colour.g, colour.b);
			}
		}
	}
	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
}