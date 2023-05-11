#ifndef PLANE_H
#define PLANE_H

#include "object.h"
#include <cmath>

class PlaneObject : public Object {
    sf::Vector3f normal;
    float d, reflectivity, emissivity;
    sf::Color color;
public:
    sf::Color getColor(sf::Vector3f point, Ray ray) {
        return color;
    }
    PlaneObject(sf::Vector3f normal, float d, sf::Color color = sf::Color::Black, float reflectivity = 0.0f, float emissivity = 0.0f) {
        this->normal = normal;
        this->d = d;
        this->color = color;
        this->reflectivity = reflectivity;
        this->emissivity = emissivity;
    }
    std::pair<bool, double> intersect(Ray ray) {
        sf::Vector3f origin = ray.origin;
        sf::Vector3f v = ray.dir;
        double a = normal.x, b = normal.y, c = normal.z;
        double x0 = origin.x, y0 = origin.y, z0 = origin.z;
        double dx = v.x, dy = v.y, dz = v.z;
        double t = -(a * x0 + b * y0 + c * z0 + d) / (a * dx + b * dy + c * dz);
        if(t < 0)
            return {false, 0};
        return {true, t};
    }
    sf::Vector3f getNormal(sf::Vector3f point) {
        return normal;
    }
    float getReflectivity() {
        return reflectivity;
    }
    float getRefractivity() {
        return 0;
    }
    void refract(Ray &ray, sf::Vector3f normal, sf::Vector3f point) {
        // not implemented
    }
};

// only for z planes now
class CheckeredPlaneObject : public PlaneObject {
    sf::Color color1, color2;
    float size;
public:
    CheckeredPlaneObject(sf::Vector3f normal, float d, sf::Color color1, sf::Color color2, float size, float reflectivity) : PlaneObject(normal, d, color1, reflectivity), color1(color1), color2(color2), size(size) {}
    sf::Color getColor(sf::Vector3f point, Ray ray) {
        if(((int)floor(point.x / size) + 10000) % 2 == ((int)floor(point.y / size) + 10000) % 2)
            return color1;
        return color2;
    }
};


#endif