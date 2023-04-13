#ifndef SCREEN_H
#define SCREEN_H

#include "ray.h"
#include "object.h"

class Screen {
    std::vector<Object*> objects;
    int width, height;
    float verticalFOV, horizontalFOV;
    sf::Vector3f origin;
public:
    void addObject(Object* object) {
        objects.push_back(object);
    }

    Screen(int width=1080, int height=800, float verticalFOV=90.f, float horizontalFOV=90.f) : 
        width(width), height(height), verticalFOV(verticalFOV), horizontalFOV(horizontalFOV) {}

    void setOrigin(sf::Vector3f origin) {
        this->origin = origin;
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
                        Ray ray(origin, normalize(sf::Vector3f(dx, dy, dz)));
                        sf::Color color(0, 0, 0);
                        float intensity = 1.;
                        while(ray.bounceCount < 20) {

                            // Another parameter to tweek.
                            double minT = 20;
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
                            // float intensity = 1. / (ray.travelDistance * ray.travelDistance + 0.5);
                            if(ptr) {
                                sf::Color color = ptr->getColor(ray.getPoint(minT), ray);
                                sf::Vector3f contactPoint = ray.getPoint(minT);
                                sf::Vector3f normal = ptr->getNormal(contactPoint);

                                // In case we reach a light source, we should add this total to the light absorbed
                                // In any case, we adjust the intensity according to the normal of the bounces up to this point
                                // Probably gonna get too dark...
                                // ray.lightNormal *= abs(dot(normal, normalize(ray.origin - contactPoint)));
                                // ray.lightAbsorbed += ptr->getEmissivity() * ray.lightNormal;

                                // ray.addSimpleColor(color, intensity);
                                // ray.addColor(sf::Color(ray.lightNormal * color.r, ray.lightNormal * color.g, ray.lightNormal * color.b),
                                //     (.3 + ptr->getEmissivity()) * intensity * ray.lightNormal);

                                float randomFloat = (float)std::rand() / RAND_MAX;                            
                                if(randomFloat < ptr->getReflectivity()) {
                                    ray.bounce(normal, contactPoint);
                                } else {
                                    ray.addColor(color, intensity, 1.0);
                                    intensity /= 2;
                                    ray.bounceRandomly(normal, contactPoint);
                                }
                            } else {
                                // wandering off to the sky
                                // ray.addColor(sf::Color(0, 0, 0), 0.1);
                                sf::Vector3f base(255. * 0.3, 255. * 0.5, 255.);
                                float zCompNorm = (ray.dir.z + 1) / 2;
                                sf::Vector3f newColor = base * zCompNorm + sf::Vector3f(255, 255, 255) * (1 - zCompNorm);
                                ray.addColor(sf::Color(newColor.x, newColor.y, newColor.z), intensity, 1.0);

                                break;
                            }
                        }
                        sf::Color newColor = ray.getColor();
                        finalR += newColor.r * newColor.r;
                        finalG += newColor.g * newColor.g;
                        finalB += newColor.b * newColor.b;
                        // float oldR = trueColors[i][j][0], oldG = trueColors[i][j][1], oldB = trueColors[i][j][2];
                        // float totalLight = trueColors[i][j][3];
                        // float newR = newColor.r, newG = newColor.g, newB = newColor.b;
                        
                        // Not sure if these values make sense, might change later
                        // float thisLight = std::max(std::min(2.f, ray.lightAbsorbed), .1f);
                        // float addLight = std::max(1.f, ray.lightAbsorbed);

                        // Not getting light from a source should be penalized.
                        // float setR = sqrt((oldR * oldR * totalLight + newR * newR * thisLight) / (totalLight + addLight));
                        // float setG = sqrt((oldG * oldG * totalLight + newG * newG * thisLight) / (totalLight + addLight));
                        // float setB = sqrt((oldB * oldB * totalLight + newB * newB * thisLight) / (totalLight + addLight));
                        // std::cout << setR << " " << setG << " " << setB << std::endl;
                        // trueColors[i][j] = {setR, setG, setB, totalLight + addLight};
                        // image.setPixel(i, j, sf::Color(setR, setG, setB));
                        // image.setPixel(i, j, newColor);
                    }
                    sf::Color oldColor = image.getPixel(i, j);
                    float r = oldColor.r, g = oldColor.g, b = oldColor.b;
                    r = sqrt((r * r * frameCnt + finalR) / (frameCnt + 1));
                    g = sqrt((g * g * frameCnt + finalG) / (frameCnt + 1));
                    b = sqrt((b * b * frameCnt + finalB) / (frameCnt + 1));
                    image.setPixel(i, j, sf::Color(r, g, b));
                    // image.setPixel(i, j, sf::Color(sq * sqrt(finalR / samples), sq * sqrt(finalG / samples), sq * sqrt(finalB / samples)));
                    // image.setPixel(i, j, sf::Color(sq * sqrt(finalR / 255), sq * sqrt(finalG / 255), sq * sqrt(finalB / 255)));
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