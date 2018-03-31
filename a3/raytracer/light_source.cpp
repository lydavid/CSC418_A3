/***********************************************************
	
	Starter code for Assignment 3

	Implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"


void PointLight::shade(Ray3D& ray) {
	// TODO: implement this function to fill in values for ray.col 
	// using phong shading.  Make sure your vectors are normalized, and
	// clamp colour values to 1.0.
	//
	// It is assumed at this point that the intersection information in ray 
	// is available.  So be sure that traverseScene() is called on the ray 
	// before this function.  

	Intersection intersect = ray.intersection;
	Material* mat = intersect.mat;
	Vector3D normal = intersect.normal;
	normal.normalize();
	Vector3D viewVec = -ray.dir;
	viewVec.normalize();
	

	Point3D light_pos = get_position();
	Vector3D lightDir = ( light_pos - intersect.point );
	lightDir.normalize();
	Vector3D reflection = 2.0 * (normal.dot(lightDir)) * normal - lightDir;
	reflection.normalize();

	

	Color ambient = mat->ambient * col_ambient;
	Color diffuse = fmax(0, lightDir.dot(normal)) * (mat->diffuse * col_diffuse);
	float spec = fmax (0, viewVec.dot(reflection));
	Color specular = pow(spec, mat->specular_exp) * (mat->specular * col_specular);
	
	//https://bb-2018-01.teach.cs.toronto.edu/t/what-should-be-the-correct-image-for-the-scene-signature/878
	Color black = Color(0, 0, 0);
	Color col;
	if (HARD_SHADOW) {
		col = black; // color = black?
	}
	else {
		col = ambient + diffuse + specular;
	}
	
	col.clamp();
	ray.col = col;

}

