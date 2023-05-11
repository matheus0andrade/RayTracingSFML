#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

const double PI = acos(-1);
const double DEG_TO_RAD = PI / 180.0;

sf::Vector3f normalize(sf::Vector3f v) {
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return sf::Vector3f(v.x / len, v.y / len, v.z / len);
}

sf::Vector3f cross(sf::Vector3f v1, sf::Vector3f v2) {
    return sf::Vector3f(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

float dot(sf::Vector3f v1, sf::Vector3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float len(sf::Vector3f v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
}

void rotate(sf::Vector3f &d, sf::Vector2f rot) {
    float alpha = rot.x * DEG_TO_RAD, beta = rot.y * DEG_TO_RAD;
    float sin_beta = sin(beta), cos_beta = cos(beta), sin_alpha = sin(alpha), cos_alpha = cos(alpha);
    sf::Vector3f newDir;
    newDir.x = d.x;
    newDir.y = d.y * cos_beta - d.z * sin_beta;
    newDir.z = d.y * sin_beta + d.z * cos_beta;
    d.x = newDir.x * cos_alpha - newDir.y * sin_alpha;
    d.y = newDir.x * sin_alpha + newDir.y * cos_alpha;
    d.z = newDir.z;
}


#endif