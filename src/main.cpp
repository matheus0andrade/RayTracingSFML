#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include "utility.h"
#include "ray.h"
#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "screen.h"

const int width = 1080, height = 800;
std::vector<std::vector<std::vector<float>>> trueColors(width, std::vector<std::vector<float>>(height, std::vector<float>(4, 0)));



int main() {
    Screen screen(width, height, 90, 90 * width / height);
    screen.setOrigin(sf::Vector3f(0, -3, 4));
    screen.setCameraPosition(sf::Vector2f(0, -20));
    ReflectiveSphereObject sphere3(3, -3, 7.5, 2.5, sf::Color::Red, 1.0);
    ReflectiveSphereObject sphere4(1, 4.5, 5.5, .5, sf::Color::Yellow, 0.8);
    RefractiveSphereObject sphere5(2, 1.0, 4.5, .5, sf::Color::Magenta, 0.8, 1.5);
    ReflectiveSphereObject sphere6(1, -2.5, 3.5, -.5, sf::Color::Green, 0.5);
    // ReflectiveSphereObject sphere6(1, -2.5, 4.5, -.5, sf::Color::Cyan, 0.1);
    CheckeredPlaneObject plane(sf::Vector3f(0, 0, 1), 1.5, sf::Color::Black, sf::Color::White, 1.0, 0.0);
    PlaneObject plane2(sf::Vector3f(0, 1, 0), -3.0, sf::Color::Yellow);
    screen.addObject(&sphere3);
    screen.addObject(&sphere4);
    screen.addObject(&sphere5);
    screen.addObject(&sphere6);
    // screen.addObject(&sphere7);
    screen.addObject(&plane);
    screen.run(trueColors);
    return 0;
}