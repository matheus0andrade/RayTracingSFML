
#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

class Object {
public:
    float reflectivity = 0, refractivity = 0;
    virtual std::pair<bool, double> intersect(Ray ray) = 0;
    virtual sf::Color getColor(sf::Vector3f point, Ray ray) = 0;
    virtual sf::Vector3f getNormal(sf::Vector3f point) = 0;
    virtual float getReflectivity() = 0;
    virtual float getRefractivity() = 0;
    virtual void refract(Ray &ray, sf::Vector3f normal, sf::Vector3f point) = 0;
    // virtual Ray bounce(Ray ray, sf::Vector3f contactPoint) = 0;

    void bounce(Ray &ray, sf::Vector3f normal, sf::Vector3f point) {
        // if(dot(ray.dir, normal) > 0)
        //     normal = -normal;
        sf::Vector3f newDir = normalize(ray.dir - 2 * dot(ray.dir, normal) * (normal));
        ray.origin = point + newDir * 0.0001f;
        ray.dir = newDir;
    }
    void bounceRandomly(Ray &ray, sf::Vector3f normal, sf::Vector3f point) {
        float x, y, z;
        do {
            x = (float)rand() / RAND_MAX * 2 - 1, y = (float)rand() / RAND_MAX * 2 - 1, z = (float)rand() / RAND_MAX * 2 - 1;
        } while(x * x + y * y + z * z > 1);
        // add or remove normalize from random vector: 2 distributions
        sf::Vector3f newDir = normalize(normal + normalize(sf::Vector3f(x, y, z)));
        if(dot(newDir, normal) < 0)
            newDir = -newDir;
        ray.origin = point + newDir * 0.0001f;
        ray.dir = newDir;
    }


};

#endif