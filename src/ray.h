#ifndef RAY_H
#define RAY_H

#include "utility.h"

class Ray {
public:
    float colorx = 0, colory = 0, colorz = 0;
    float simpleColorx = 0, simpleColory = 0, simpleColorz = 0, simpleIntensity = 0;
    float intensity = 0, lightAbsorbed = 0, lightNormal = 1;
    int bounceCount = 0;
    float travelDistance = 0;
    sf::Vector3f origin, dir;
    sf::Vector3f getPoint(float t) {
        return origin + dir * t;
    }
    void bounceRandomly(sf::Vector3f normal, sf::Vector3f point) {
        float x, y, z;
        do {
            x = (float)rand() / RAND_MAX * 2 - 1, y = (float)rand() / RAND_MAX * 2 - 1, z = (float)rand() / RAND_MAX * 2 - 1;
        } while(x * x + y * y + z * z > 1);
        // add or remove normalize from random vector: 2 distributions
        sf::Vector3f newDir = normalize(normal + normalize(sf::Vector3f(x, y, z)));
        if(dot(newDir, normal) < 0)
            newDir = -newDir;
        origin = point + newDir * 0.0001f;
        dir = newDir;
        bounceCount++;
    }
    void bounce(sf::Vector3f normal, sf::Vector3f point) {
        sf::Vector3f newDir = normalize(dir - 2 * dot(dir, normal) * (normal));
        origin = point + newDir * 0.0001f;
        dir = newDir;
        bounceCount++;
    }
    void addColor(sf::Color newColor, float cInt, float addToLight = 1) {
        float r = (float) newColor.r, g = (float) newColor.g, b = (float) newColor.b;
        colorx += r * r * cInt;
        colory += g * g * cInt;
        colorz += b * b * cInt;
        intensity += addToLight;
    }
    sf::Color getColor() {
        return sf::Color(sqrt(colorx / intensity), sqrt(colory / intensity), sqrt(colorz / intensity));
    }
    void addSimpleColor(sf::Color newColor, float cInt) {
        float r = (float) newColor.r, g = (float) newColor.g, b = (float) newColor.b;
        simpleColorx += r * cInt;
        simpleColory += g * cInt;
        simpleColorz += b * cInt;
        simpleIntensity++;
    }
    sf::Color getSimpleColor() {
        return sf::Color(simpleColorx / simpleIntensity, simpleColory / simpleIntensity, simpleColorz / simpleIntensity);
    }
    Ray(sf::Vector3f origin, sf::Vector3f dir) : origin(origin), dir(dir) {}
};

#endif