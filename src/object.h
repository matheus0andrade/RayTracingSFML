
#ifndef OBJECT_H
#define OBJECT_H

#include "ray.h"

class Object {
public:
    virtual std::pair<bool, double> intersect(Ray ray) = 0;
    virtual sf::Color getColor(sf::Vector3f point, Ray ray) = 0;
    virtual sf::Vector3f getNormal(sf::Vector3f point) = 0;
    virtual float getReflectivity() = 0;
    virtual float getEmissivity() = 0;
};

#endif