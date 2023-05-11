#ifndef RAY_H
#define RAY_H

#include "utility.h"

class Ray {
// too lazy to make getters and setters
public:
    float colorx = 0, colory = 0, colorz = 0;
    float simpleColorx = 0, simpleColory = 0, simpleColorz = 0, simpleIntensity = 0;
    float intensity = 0, lightAbsorbed = 0, lightNormal = 1;
    int bounceCount = 0;
    float travelDistance = 0, currentRefractiveIndex = 1;
    sf::Vector3f origin, dir;
    sf::Vector3f getPoint(float t) {
        return origin + dir * t;
    }
    void addColor(sf::Color newColor, float cInt, float addToLight = 1.0) {
        float r = (float) newColor.r, g = (float) newColor.g, b = (float) newColor.b;
        colorx += r * r * cInt;
        colory += g * g * cInt;
        colorz += b * b * cInt;
        intensity += addToLight;
    }
    sf::Color getColor() {
        return sf::Color(sqrt(colorx / intensity), sqrt(colory / intensity), sqrt(colorz / intensity));
    }
    Ray(sf::Vector3f origin, sf::Vector3f dir, float refractiveIndex = 1) : 
        origin(origin), dir(dir), currentRefractiveIndex(refractiveIndex) {}
};

#endif