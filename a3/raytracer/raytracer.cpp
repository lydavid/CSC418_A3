/***********************************************************
	
	Starter code for Assignment 3

	Implementations of functions in raytracer.h, 
	and the main function which specifies the scene to be rendered.	

***********************************************************/


#include "raytracer.h"
#include <cmath>
#include <iostream>
#include <cstdlib>



void Raytracer::traverseScene(Scene& scene, Ray3D& ray)  {
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
			ray.intersection.mat = node->mat;
		}
	}
}

void Raytracer::computeTransforms(Scene& scene) {
	// right now this method might seem redundant. But if you decide to implement 
	// scene graph this is where you would propagate transformations to child nodes
		
	for (size_t i = 0; i < scene.size(); ++i) {
		SceneNode* node = scene[i];

		node->modelToWorld = node->trans;
		node->worldToModel = node->invtrans; 
	}
}

void Raytracer::computeShading(Ray3D& ray, LightList& light_list) {
	for (size_t  i = 0; i < light_list.size(); ++i) {
		LightSource* light = light_list[i];
		
		// Each lightSource provides its own shading function.
		// Implement shadows here if needed.


		light->shade(ray);
		
		     
	}
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {

		/*
		// iterate through lights to send ray to them
		for (size_t i = 0; i < light_list.size(); i++) {

			LightSource* light = light_list[i];
			Point3D origin = ray.intersection.point;
			Vector3D direction = ((light->get_position()) - origin);
			Ray3D shadowRay = Ray3D(origin, direction);
			
			
			if (shadowRay.intersection.none) {
				computeShading(shadowRay, light_list);
				col = ray.col;
			}
			else {
				Point3D origin = shadowRay.intersection.point;
			}

		}*/
		computeShading(ray, light_list); 
		col = ray.col;  
	}

	// You'll want to call shadeRay recursively (with a different ray, 
	// of course) here to implement reflection/refraction effects.  

	return col; 
}	

void Raytracer::render(Camera& camera, Scene& scene, LightList& light_list, Image& image) {
	computeTransforms(scene);

	Matrix4x4 viewToWorld;
	double factor = (double(image.height)/2)/tan(camera.fov*M_PI/360.0);

	viewToWorld = camera.initInvViewMatrix();
	
	// Construct a ray for each pixel.
	for (int i = 0; i < image.height; i++) {
		for (int j = 0; j < image.width; j++) {

			if (ANTIALIAS) {

				Color col;
				// Antialiasing from textbook - jittering 
				int n = 4;
				for (int p = 0; p < n; p++) {
					for (int q = 0; q < n; q++) {

						Point3D origin(0, 0, 0);
						Point3D imagePlane;

						double random = (double) std::rand() / ((double)RAND_MAX + 1);

						imagePlane[0] = (-double(image.width) / 2 + i + ((q + random) / n)) / factor;
						imagePlane[1] = (-double(image.height) / 2 + j + ((p + random) / n)) / factor;
						imagePlane[2] = -1;

						Ray3D ray; 
						// TODO: Convert ray to world space  
						Vector3D direction = imagePlane - origin;
						direction = viewToWorld * direction;
						direction.normalize();
						origin = viewToWorld * origin;
						ray = Ray3D(origin, direction);
						col = col + shadeRay(ray, scene, light_list);
					}
				}
				col[0] = col[0] / pow(n, 2);
				col[1] = col[1] / pow(n, 2);
				col[2] = col[2] / pow(n, 2);
				image.setColorAtPixel(i, j, col);


			}
			else {


				// Sets up ray origin and direction in view space, 
				// image plane is at z = -1.
				Point3D origin(0, 0, 0);
				Point3D imagePlane;
				imagePlane[0] = (-double(image.width) / 2 + 0.5 + j) / factor;
				imagePlane[1] = (-double(image.height) / 2 + 0.5 + i) / factor;
				imagePlane[2] = -1;

				Ray3D ray;
				// TODO: Convert ray to world space  
				Vector3D direction = imagePlane - origin;
				direction = viewToWorld * direction;
				direction.normalize();
				origin = viewToWorld * origin;
				ray = Ray3D(origin, direction);

				Color col = shadeRay(ray, scene, light_list);
				image.setColorAtPixel(i, j, col);

			}
		}
	}
}

