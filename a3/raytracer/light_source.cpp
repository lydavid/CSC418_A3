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

	// https://en.wikipedia.org/wiki/Phong_reflection_model
	Vector3D reflection = 2.0 * (normal.dot(lightDir)) * normal - lightDir;
	reflection.normalize();

	//Color col = ray.col; 
	Color ambient = mat->ambient * col_ambient;
	
	Color diffuse = fmax(0, lightDir.dot(normal)) * (mat->diffuse * col_diffuse);

/* float spec = max(0.0, dot(viewVec, reflection));
  vec3 specular = pow(spec, shininessVal) * specularColor * Ks;
*/
	



	float spec = fmax (0, viewVec.dot(reflection));
	Color specular = pow(spec, mat->specular_exp) * (mat->specular * col_specular);
	

	Color col = ambient + diffuse + specular;
	col.clamp();
	
	//ray.col = ambient + diffuse + specular;
	ray.col = col;

}

