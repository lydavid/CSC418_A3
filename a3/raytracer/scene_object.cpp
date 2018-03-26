
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

	if (t < 0 || direction[2] == 0)
	{
		return false;
	}

	Point3D p = origin + t * direction;
	Vector3D normal = Vector3D(0, 0, 1);
	
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

	// source: shirley textbook
	double a = direction.dot(direction);
	double b = (2*direction).dot(origin - center);
	double c = (origin - center).dot(origin - center) - pow(radius, 2);

	double determinant = pow(b, 2) - (4 * a * c);
	double t = 0;
	if (determinant < 0)
	{
		return false;
	}
	else
	{
		
		double t1 = (-direction.dot(origin - center) + sqrt(determinant)) / a;
		double t2 = (-direction.dot(origin - center) - sqrt(determinant)) / a;

		t = (t1 > 0) ? t1 : t2;

		if (determinant > 0)
		{
			t = (t1 > t2) ? t2 : t1;
		}

		Point3D p = origin + t * direction;
		Vector3D normal = 2 * (p - center);
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


