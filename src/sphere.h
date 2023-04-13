#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class SphereObject : public Object {
    float radius, x, y, z;
    float reflectivity, transparency, emissivity;
    sf::Color color;
public:
    sf::Color getColor(sf::Vector3f point, Ray ray) {
        return color;
    }
    SphereObject(float radius, float x = 0, float y = 0, float z = 0, sf::Color color = sf::Color::Black, 
                    float reflectivity = 0.0f, float emissivity = 0.0f) {
        this->radius = radius;
        this->x = x;
        this->y = y;
        this->z = z;
        this->color = color;
        this->reflectivity = reflectivity;
        this->emissivity = emissivity;
    }
    std::pair<bool, double> intersect(Ray ray) {
        sf::Vector3f origin = ray.origin;
        sf::Vector3f v = ray.dir;
        double dx = origin.x - x, dy = origin.y - y, dz = origin.z - z;
        double a = v.x * v.x + v.y * v.y + v.z * v.z;
        double b = 2 * (v.x * dx + v.y * dy + v.z * dz);
        double c = dx * dx + dy * dy + dz * dz - radius * radius;
        double delta = b * b - 4 * a * c;
        if(delta < 0)
            return {false, 0};
        double t1 = (-b - sqrt(delta)) / (2 * a);
        double t2 = (-b + sqrt(delta)) / (2 * a);
        if(t1 < 0 && t2 < 0)
            return {false, 0};
        if(t1 < 0)
            return {true, t2};
        if(t2 < 0)
            return {true, t1};
        return {true, std::min(t1, t2)};
    }
    sf::Vector3f getNormal(sf::Vector3f point) {
        return normalize(sf::Vector3f(point.x - x, point.y - y, point.z - z));
    }
    float getReflectivity() {
        return reflectivity;
    }
    float getEmissivity() {
        return emissivity;
    }
};

#endif