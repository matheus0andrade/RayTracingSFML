#ifndef SCREEN_H
#define SCREEN_H

#include "ray.h"
#include "object.h"

class Screen {
    std::vector<Object*> objects;
    int width, height;
    float verticalFOV, horizontalFOV;
    sf::Vector3f origin;
    sf::Vector2f cameraPosition;
public:
    void addObject(Object* object) {
        objects.push_back(object);
    }

    Screen(int width=1080, int height=800, float verticalFOV=90.f, float horizontalFOV=90.f) : 
        width(width), height(height), verticalFOV(verticalFOV), horizontalFOV(horizontalFOV) {}

    void setOrigin(sf::Vector3f origin) {
        this->origin = origin;
    }
    void setCameraPosition(sf::Vector2f cameraPosition) {
        this->cameraPosition = cameraPosition;
    }

    void run(std::vector<std::vector<std::vector<float>>> &trueColors) {
        sf::RenderWindow window(sf::VideoMode(width, height), "Ray Tracer");

        sf::Clock clock;
        sf::Time timeSinceFrame = sf::Time::Zero;
        sf::Image image;
        image.create(width, height, sf::Color(0, 0, 0));
        int frameCnt = 1;
        while(window.isOpen()) {
            timeSinceFrame += clock.restart();
            std::cout << 1.0 / timeSinceFrame.asSeconds() << std::endl;
            timeSinceFrame = sf::Time::Zero;
            double dWidth = width, dHeight = height;
            for(int i = 0; i < width; i++) {
                for(int j = 0; j < height; j++) {
                    // Not a good implementation, might be better to store this in a global variable
                    // ok for now, but will need to change later to allow new camera directions
                    int samples = 1;
                    float finalR = 0, finalG = 0, finalB = 0;
                    for(int s = 0; s < samples; s++) {
                        auto i2 = i + (rand() % 100) / 100.0 - 0.5;
                        auto j2 = j + (rand() % 100) / 100.0 - 0.5;
                        double dx = 2. * sin(DEG_TO_RAD * horizontalFOV / 2.) * (i2 - dWidth / 2) / dWidth;
                        double dy = 1.;
                        double dz = -2. * sin(DEG_TO_RAD * verticalFOV / 2.) * (j2 - dHeight / 2) / dHeight;
                        sf::Vector3f dir = normalize(sf::Vector3f(dx, dy, dz));
                        rotate(dir, cameraPosition);
                        Ray ray(origin, dir);
                        sf::Color color(0, 0, 0);
                        float intensity = 1.;
                        while(ray.bounceCount < 20) {

                            // Another parameter to tweek.
                            double minT = 25;
                            Object* ptr = nullptr;
                            for(auto object : objects) {
                                std::pair<bool, double> check = object->intersect(ray);
                                if(!check.first)
                                    continue;
                                if(check.second < minT) {
                                    minT = check.second;
                                    ptr = object;
                                }
                            }

                            // ray.travelDistance += minT;
                            // float distance = std::min(25. / (ray.travelDistance * ray.travelDistance), 1.);
                            if(ptr) {
                                sf::Color color = ptr->getColor(ray.getPoint(minT), ray);
                                sf::Vector3f contactPoint = ray.getPoint(minT);
                                sf::Vector3f normal = ptr->getNormal(contactPoint);

                                float randomFloat = (float)std::rand() / RAND_MAX;
                                ray.bounceCount++; 
                                if(randomFloat < ptr->getReflectivity()) {
                                    ptr->bounce(ray, normal, contactPoint);
                                } else if(randomFloat < ptr->getRefractivity()) {
                                    ptr->refract(ray, normal, contactPoint);
                                } else {
                                    ray.addColor(color, intensity, 1.0);
                                    intensity /= 2;
                                    ptr->bounceRandomly(ray, normal, contactPoint);
                                }
                            } else {
                                // wandering off to the sky

                                sf::Vector3f base(255. * 0.3, 255. * 0.5, 255.);
                                float zCompNorm = (ray.dir.z + 1) / 2;
                                sf::Vector3f newColor = base * zCompNorm + sf::Vector3f(255, 255, 255) * (1 - zCompNorm);
                                ray.addColor(sf::Color(newColor.x, newColor.y, newColor.z), intensity, 1.0);

                                break;
                            }
                        }
                        sf::Color newColor = ray.getColor();
                        finalR += newColor.r * newColor.r / samples;
                        finalG += newColor.g * newColor.g / samples;
                        finalB += newColor.b * newColor.b / samples;
                    }
                    sf::Color oldColor = image.getPixel(i, j);
                    float r = oldColor.r, g = oldColor.g, b = oldColor.b;
                    r = sqrt((r * r * frameCnt + finalR) / (frameCnt + 1));
                    g = sqrt((g * g * frameCnt + finalG) / (frameCnt + 1));
                    b = sqrt((b * b * frameCnt + finalB) / (frameCnt + 1));
                    image.setPixel(i, j, sf::Color(r, g, b));
                }
            }
            sf::Texture texture;
            texture.loadFromImage(image);
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    window.close();
                }
                if(event.type == sf::Event::MouseWheelScrolled) {
                    std::cout << "delta= " << event.mouseWheelScroll.delta << std::endl;
                    std::cout << "x= " << event.mouseWheelScroll.x << std::endl;
                    std::cout << "y= " << event.mouseWheelScroll.y << std::endl;
                }
                if(event.type == sf::Event::KeyPressed) {
                    if(event.key.code == sf::Keyboard::S) {
                        std::cout << "Image saved!" << std::endl;
                        time_t t = time(NULL);
                        // struct tm *tm = localtime(&t);
                        char s[64];
                        strftime(s, sizeof(s), "%c", localtime(&t));
                        std::string s2 = s;
                        std::replace(s2.begin(), s2.end(), ':', '_');
                        std::replace(s2.begin(), s2.end(), ' ', '_');
                        std::replace(s2.begin(), s2.end(), '/', '_');
                        image.saveToFile("../images/image" + std::to_string(frameCnt) + "_at_" + s2 + ".png");
                    }
                }
            }
            window.clear();
            frameCnt++;
            window.draw(sprite);
            window.display();
            // break;
        }
    }
};

#endif