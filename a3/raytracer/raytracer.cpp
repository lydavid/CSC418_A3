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

		//Point3D origin = ray.intersection.point;
		//Vector3D direction = ((light->get_position()) - origin);
		//Ray3D shadowRay = Ray3D(origin, direction);

		//if (!shadowRay.intersection.none) {
		//	ray.shadow = true;
		//}
<<<<<<< HEAD


		/*if (ray.intersection.t_value > 0) {
			ray.intersection.inShadow = false;
		} else {
			ray.intersection.inShadow = true;
		}*/
		
		

=======
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b

		light->shade(ray);
		
	}
}

Color Raytracer::shadeRay(Ray3D& ray, Scene& scene, LightList& light_list) {
	Color col(0.0, 0.0, 0.0); 
	traverseScene(scene, ray); 

	// Don't bother shading if the ray didn't hit 
	// anything.
	if (!ray.intersection.none) {

<<<<<<< HEAD
<<<<<<< HEAD
		
=======
=======
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
		if (ray.intersection.mat->image) {
			int width = ray.intersection.mat->texture_width;
			int height = ray.intersection.mat->texture_height;
			double u = ray.tex_u;
			double v = ray.tex_v;
			int x = u * width;
			int y = v * height;
			
			//https://stackoverflow.com/questions/22420778/texture-mapping-in-a-ray-tracing-for-sphere-in-c
			int px = x * width + y;

			double r = ray.intersection.mat->rarray[px];
			double g = ray.intersection.mat->garray[px];
			double b = ray.intersection.mat->barray[px];
			col = Color(r, g, b);
			return col;
		}

		computeShading(ray, light_list);
		col = ray.col;
		/*
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
		// iterate through lights to send ray to them
		/*for (size_t i = 0; i < light_list.size(); i++) {
			LightSource* light = light_list[i];
			Point3D origin = ray.intersection.point;
			Vector3D direction = ((light->get_position()) - origin);
			Ray3D shadowRay = Ray3D(origin, direction);
			
			
<<<<<<< HEAD
<<<<<<< HEAD
			if (shadowRay.intersection.none) {
				computeShading(shadowRay, light_list);
				//col = ray.col;
=======
=======
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
			if (!shadowRay.intersection.none) {
				ray.shadow = true;
				computeShading(ray, light_list);
				col = ray.col;
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
			}
			else {
				computeShading(ray, light_list);
				col = ray.col;
			}
<<<<<<< HEAD
<<<<<<< HEAD
			

		}*/


		/*for (light in scene) {
			shadowRay= create ray from intersection to light;
			findIntersections(shadowRay);
			if(shadowRay.intersection.none) {
				color += phongIllumination(light, ray);
			}
		}
		if (depth > 0) {
			reflectRay= create a reflected ray at the intersection;
			color += shadeRay(reflectRay, --depth);
		}*/

		// for each light in scene
		for (size_t i = 0; i < light_list.size(); i++) {
			LightSource* light = light_list[i];

			//Check N.L < 0
			if (ray.intersection.normal.dot((light->get_position() - ray.intersection.point)) < 0) {
				ray.intersection.inShadow = true;
			} else {
				ray.intersection.inShadow = false;

				float epsilon = 0.000001;
				Vector3D shadowRayDirection = (light->get_position() - ray.intersection.point);
				shadowRayDirection.normalize();
				Point3D shadowRayStart = ray.intersection.point + epsilon * shadowRayDirection;

				//Vector3D shadowRayDirection = light->get_position() - ray.intersection.point;
				//Point3D shadowRayStart = ray.intersection.point + epsilon * shadowRayDirection;// * shadowRayDirection;
				Ray3D shadowRay = Ray3D(shadowRayStart, shadowRayDirection);

				//Point3D origin = ray.intersection.point + epsilon * light->get_position();
				//Vector3D direction = ((light->get_position()) - origin);
				//Ray3D shadowRay = Ray3D(origin, direction);

				//Ray3D shadowRay = Ray3D(ray.intersection.point, light->get_position());

				traverseScene(scene, shadowRay);  // find intersections
				//printf("%f\n", shadowRay.intersection.t_value);
				//std::cout <<  + "\n";

				/*if (shadowRay.intersection.t_value > epsilon) {
					ray.intersection.inShadow = false;
				} else {
					ray.intersection.inShadow = true;
				}*/

				if (shadowRay.intersection.none) {
					//computeShading(ray, light_list);
					ray.intersection.inShadow = false;

				} else {
					ray.intersection.inShadow = true;
					
					

				}
			}
		}
		



		computeShading(ray, light_list); 
		col = ray.col;  
=======
		}*/
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
=======
		}*/
>>>>>>> 0e88abc8f73ac5b13df4ec5146ad243ee5d0753b
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

						imagePlane[0] = (-double(image.width) / 2 + (i + ((q + random) / n))) / factor;
						imagePlane[1] = (-double(image.height) / 2 + (j + ((p + random) / n))) / factor;
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

