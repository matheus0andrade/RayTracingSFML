#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

class BaseSphereObject : public Object {
    float radius, x, y, z;
    sf::Color color;
public:
    sf::Color getColor(sf::Vector3f point, Ray ray) {
        return color;
    }
    BaseSphereObject(float radius, float x = 0, float y = 0, float z = 0, sf::Color color = sf::Color::Black) {
        this->radius = radius;
        this->x = x;
        this->y = y;
        this->z = z;
        this->color = color;
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
};

class ReflectiveSphereObject : public BaseSphereObject {
    float reflectivity;
public:
    ReflectiveSphereObject(float radius, float x = 0, float y = 0, float z = 0, sf::Color color = sf::Color::Black, float reflectivity = 0) :
        BaseSphereObject(radius, x, y, z, color), reflectivity(reflectivity) {}
    float getReflectivity() {
        return reflectivity;
    }
    float getRefractivity() {
        return 0;
    }
    void refract(Ray &ray, sf::Vector3f normal, sf::Vector3f point) {
        // do nothing
    }
};

class RefractiveSphereObject : public BaseSphereObject {
    float refractiveIndex, refractivity;
public:
    RefractiveSphereObject(float radius, float x = 0, float y = 0, float z = 0, 
                        sf::Color color = sf::Color::Black, float refractivity = 1.0, 
                        float refractiveIndex = 1.0) : 
        BaseSphereObject(radius, x, y, z, color), refractivity(refractivity), refractiveIndex(refractiveIndex) {}

    float getReflectivity() {
        return 0;
    }
    float getRefractivity() {
        return refractivity;
    }
    float getRefractiveIndex() {
        return refractiveIndex;
    }

    void refract(Ray &ray, sf::Vector3f normal, sf::Vector3f point) {
        // ray.origin = point + 0.001f * ray.dir;
        // get angle between ray and normal
        if(dot(ray.dir, normal) > 0) {
            normal *= -1.f;
        }
        double cos_theta = dot(ray.dir, -normal) / (len(ray.dir) * len(normal));
        double theta = acos(cos_theta);
        double sin_theta = sin(theta);
        double sin_alpha = ray.currentRefractiveIndex * sin_theta / refractiveIndex;
        if(sin_alpha >= 0.999) {
            // normal reflection
            bounce(ray, normal, point);
        } else {
            double alpha = asin(sin_alpha);
            sf::Vector3f dir = normalize(ray.dir);
            sf::Vector3f refractedDir(0, 0, 0);
            float a = cos(alpha) / (cos_theta);
            float b = sin(alpha) - a * sin(theta);
            refractedDir = a * dir + b * normal;
            ray.currentRefractiveIndex = refractiveIndex;
            ray.dir = normalize(refractedDir);
            ray.origin = point + 0.001f * ray.dir;
        }
    }
};

#endif