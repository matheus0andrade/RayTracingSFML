#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include "utility.h"
#include "ray.h"
#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "screen.h"

std::vector<std::vector<std::vector<float>>> trueColors(800, std::vector<std::vector<float>>(600, std::vector<float>(4, 0)));

int main() {
    Screen screen(800, 600, 90, 90 * 800 / 600);
    screen.setOrigin(sf::Vector3f(0, 0, 0));
    // SphereObject sphere(1, 3, 5, -2, sf::Color::Green, 0.0, 1.0);
    ReflectiveSphereObject sphere2(1, 0, 4.5, -.5, sf::Color::Blue, 1.0);
    ReflectiveSphereObject sphere3(1, -3, 5.5, -.5, sf::Color::Red, 0.0);
    // SphereObject sphere4(1, 3, 5.5, -.5, sf::Color::Yellow, 0.0, 0.0);
    // SphereObject spherePlane(98.5, 0, 0, -100, sf::Color::White, 0.0, 0.0);
    CheckeredPlaneObject plane(sf::Vector3f(0, 0, 1), 1.5, sf::Color::Black, sf::Color::White, 1.0, 0.0);
    // PlaneObject plane(sf::Vector3f(0, 0, 1), 1.5, sf::Color::Green, 0.4);
    PlaneObject plane2(sf::Vector3f(0, 1, 0), -3.0, sf::Color::Yellow);
    // screen.addObject(&sphere);
    screen.addObject(&sphere2);
    screen.addObject(&sphere3);
    // screen.addObject(&sphere4);
    // screen.addObject(&spherePlane);
    screen.addObject(&plane);
    // screen.addObject(&plane2);

    // SphereObject sphereCenter(1.0, 0, 3, 0, sf::Color::Red);
    // SphereObject sphereRight(2.0, 3, 6, 3, sf::Color::Blue);
    // screen.addObject(&sphereCenter);
    // screen.addObject(&sphereRight);
    screen.run(trueColors);
    return 0;
}