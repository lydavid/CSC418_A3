
/***********************************************************
	
	Starter code for Assignment 3
	
	Implements scene_object.h

***********************************************************/

#include <cmath>
#include "scene_object.h"

bool UnitSquare::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices (0.5, 0.5, 0), 
	// (-0.5, 0.5, 0), (-0.5, -0.5, 0), (0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.

	// Transforming to object space
	Point3D origin = worldToModel * ray.origin;
	Vector3D direction = worldToModel * ray.dir;
	
	// t value used in point = ray.origin + t_value * ray.dir
	double t = -origin[2] / direction[2];

	// from raytracing part1 slides
	if (t < 0 || direction[2] == 0)
	{
		return false;
	}

	if (!ray.intersection.none && ray.intersection.t_value < t)
	{
		return false;
	}

	Point3D p = origin + t * direction;
	Vector3D normal = Vector3D(0, 0, 1);


	
	if (TEXTURE) 
	{
		double u, v;

		ray.tex_u = u;
		ray.tex_v = v;

	}
	
	// check boundaries
	if (p[0] >= -0.5 && p[0] <= 0.5 && p[1] >= -0.5 && p[1]<=0.5)
	{
		ray.intersection.point = modelToWorld * p;
		ray.intersection.normal = transNorm(worldToModel, normal);
		ray.intersection.t_value = t;
		ray.intersection.none = false;
		return true;
	}
	
	return false;
}

bool UnitSphere::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.


	// transform to object space
	Point3D origin = worldToModel * ray.origin;
	Vector3D direction = worldToModel * ray.dir;

	double radius = 1; // radius of sphere
	Point3D center = Point3D(0, 0, 0); // center of sphere

	// from textbook
	double a = direction.dot(direction);
	double b = (2*direction).dot(origin - center);
	double c = (origin - center).dot(origin - center) - pow(radius, 2);

	// det from quadratic formula
	double determinant = pow(b, 2) - (4 * a * c);
	double t = 0;
	if (determinant < 0)
	{
		return false;
	}
	else
	{
		// possible solutions up to 2
		double t1 = (-direction.dot(origin - center) + sqrt(determinant)) / a;
		double t2 = (-direction.dot(origin - center) - sqrt(determinant)) / a;


		// this part may be wrong quick check
		if ((!ray.intersection.none && ray.intersection.t_value < t1) || (!ray.intersection.none && ray.intersection.t_value < t2) )
		{
			return false;
		}

		t = (t1 > 0) ? t1 : t2;

		if (determinant > 0)
		{
			t = (t1 > t2) ? t2 : t1;
		}

		Point3D p = origin + t * direction;
		Vector3D normal = (p - center);
		normal.normalize();


		// for texturing a sphere
		if (TEXTURE) 
		{
			// source: https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
			
			ray.intersection.mat->image = true;

			double u, v;

			//Vector3D d_x = Vector3D(1, 0, 0);
			//Vector3D d_y = Vector3D(0, 1, 0);
			//Vector3D d_z = Vector3D(0, 0, 1);


			//Calculating the texture coordinate for a sphere
			//u = 0.5 + atan2(normal[2], normal[0]) / (2 * M_PI);
			//v = 0.5 - asin(normal[1]) / M_PI;

			
			Vector3D v_n = Vector3D(0, 1, 0);
			Vector3D v_e = Vector3D(1, 0, 0);
			Vector3D v_p = normal;

			double phi = acos(-v_n.dot(v_p));
			v = phi / M_PI;

			double theta =  (acos(v_p.dot(v_e) / sin(phi))) / (2 * M_PI);

			if (v_p.dot(v_n.cross(v_e)) > 0) {
				u = 0.5 + theta;
			}
			else {
				u = 0.5 - theta;
			}

			ray.tex_u = u;
			ray.tex_v = v;

		}


		ray.intersection.point = modelToWorld * p;
		ray.intersection.normal = transNorm(worldToModel, normal);
		ray.intersection.t_value = t;
		ray.intersection.none = false;
		return true;
	}

	return false;
}

bool UnitCylinder::intersect(Ray3D& ray, const Matrix4x4& worldToModel,
	const Matrix4x4& modelToWorld) {

	//source :http://www.cl.cam.ac.uk/teaching/1999/AGraphHCI/SMAG/node2.html#SECTION00023200000000000000

	// Transforming to object space
	Point3D origin = worldToModel * ray.origin;
	Vector3D direction = worldToModel * ray.dir;
	Point3D center = Point3D(0, 0, 0);

	double a = pow(direction[0], 2) + pow(direction[1], 2);
	double b = (2 * origin[0] * direction[0]) + (2 * origin[1] * direction[1]);
	double c = pow(origin[0], 2) + pow(origin[1], 2) - 1;

	double determinant = pow(b, 2) - (4 * a * c);
	double t;

	if (determinant < 0)
	{
		return false;
	}

	// intersections with wall vals 
	double t0 = (-b + sqrt(determinant)) / a;
	double t1 = (-b - sqrt(determinant)) / a;

	float z0 = origin[2] + t0 * direction[2];
	float z1 = origin[2] + t1 * direction[2];


	bool z0_check = (-1 < z0) && (z0 < 1);
	bool z1_check = (-1 < z1) && (z1 < 1);

	if (z0_check || z1_check) {
		t = fmin(z0, z1);
	}

	//min cap
	double t2 = (-1 - origin[2]) / direction[2];
	//max cap
	double t3 = (1 - origin[2]) / direction[2];

	Point3D p = origin + t * direction;
	Vector3D normal = center-p;
	if (z0 > 1) {
		if (z1 > 1) {
			return false;
		}
		else
		{
			p = origin + t3 * direction;
			normal = Vector3D(0, 1, 0);
		}
	}
	else if (z0 < -1) {
		if (z1 < -1) {
			return false;
		}
		else {
			p = origin + t2 * direction;
			normal = Vector3D(0, -1, 0);
		}
	}

	if (ray.intersection.none || ray.intersection.t_value > t) {
		//p = origin + t * direction;
		//normal = p - center;
		normal.normalize();
		ray.intersection.point = modelToWorld * p;
		ray.intersection.normal = transNorm(worldToModel, normal);
		ray.intersection.t_value = t;
		ray.intersection.none = false;
		return true;
	}

	return false;


}

void SceneNode::rotate(char axis, double angle) {
	Matrix4x4 rotation;
	double toRadian = 2*M_PI/360.0;
	int i;
	
	for (i = 0; i < 2; i++) {
		switch(axis) {
			case 'x':
				rotation[0][0] = 1;
				rotation[1][1] = cos(angle*toRadian);
				rotation[1][2] = -sin(angle*toRadian);
				rotation[2][1] = sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'y':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][2] = sin(angle*toRadian);
				rotation[1][1] = 1;
				rotation[2][0] = -sin(angle*toRadian);
				rotation[2][2] = cos(angle*toRadian);
				rotation[3][3] = 1;
			break;
			case 'z':
				rotation[0][0] = cos(angle*toRadian);
				rotation[0][1] = -sin(angle*toRadian);
				rotation[1][0] = sin(angle*toRadian);
				rotation[1][1] = cos(angle*toRadian);
				rotation[2][2] = 1;
				rotation[3][3] = 1;
			break;
		}
		if (i == 0) {
			this->trans = this->trans*rotation; 	
			angle = -angle;
		} 
		else {
			this->invtrans = rotation*this->invtrans; 
		}	
	}
}

void SceneNode::translate(Vector3D trans) {
	Matrix4x4 translation;
	
	translation[0][3] = trans[0];
	translation[1][3] = trans[1];
	translation[2][3] = trans[2];
	this->trans = this->trans*translation; 	
	translation[0][3] = -trans[0];
	translation[1][3] = -trans[1];
	translation[2][3] = -trans[2];
	this->invtrans = translation*this->invtrans; 
}

void SceneNode::scale(Point3D origin, double factor[3] ) {
	Matrix4x4 scale;
	
	scale[0][0] = factor[0];
	scale[0][3] = origin[0] - factor[0] * origin[0];
	scale[1][1] = factor[1];
	scale[1][3] = origin[1] - factor[1] * origin[1];
	scale[2][2] = factor[2];
	scale[2][3] = origin[2] - factor[2] * origin[2];
	this->trans = this->trans*scale; 	
	scale[0][0] = 1/factor[0];
	scale[0][3] = origin[0] - 1/factor[0] * origin[0];
	scale[1][1] = 1/factor[1];
	scale[1][3] = origin[1] - 1/factor[1] * origin[1];
	scale[2][2] = 1/factor[2];
	scale[2][3] = origin[2] - 1/factor[2] * origin[2];
	this->invtrans = scale*this->invtrans; 
} 


