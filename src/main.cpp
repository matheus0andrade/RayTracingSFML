#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include "utility.h"
#include "ray.h"
#include "object.h"
#include "plane.h"
#include "sphere.h"
#include "screen.h"

const int width = 800, height = 600;
std::vector<std::vector<std::vector<float>>> trueColors(width, std::vector<std::vector<float>>(height, std::vector<float>(4, 0)));



int main() {
    Screen screen(width, height, 90, 90 * width / height);
    screen.setOrigin(sf::Vector3f(0, 0, 0));
    ReflectiveSphereObject sphere3(1, -1, 7.5, -.5, sf::Color::Red, 1.0);
    // RefractiveSphereObject sphere4(1, 0, 5.5, -.5, sf::Color::Yellow, 1.0, 1.5);
    RefractiveSphereObject sphere5(1, 3, 5.5, -.5, sf::Color::Yellow, 1.0, 2.0);
    // RefractiveSphereObject sphere6(1, -3, 5.5, -.5, sf::Color::Yellow, 1.0, 2.0);
    // ReflectiveSphereObject sphere7(1, 0, 5.5, -.5, sf::Color::Yellow, 0.0);
    CheckeredPlaneObject plane(sf::Vector3f(0, 0, 1), 1.5, sf::Color::Black, sf::Color::White, 1.0, 0.0);
    PlaneObject plane2(sf::Vector3f(0, 1, 0), -3.0, sf::Color::Yellow);
    screen.addObject(&sphere3);
    // screen.addObject(&sphere4);
    screen.addObject(&sphere5);
    // screen.addObject(&sphere6);
    // screen.addObject(&sphere7);
    screen.addObject(&plane);
    screen.run(trueColors);
    return 0;
}