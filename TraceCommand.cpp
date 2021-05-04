//
// Created by chang on 5/2/2021.
//

#include "TraceCommand.h"
#include "Global.h"
#include <algorithm>
#include <iostream>
#include <chrono>

TraceCommand::TraceCommand(CLI& mainCLI):
        name("trace"), cli(mainCLI), eps(0.0006) {}

void TraceCommand::execute() {
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int row = 0; row < cli.height; row++) {
        for (int col = 0; col < cli.width; col++) {
            Ray ray = cli.cam->getRay(col, row);
            Shape* hit = nullptr;
            double final_t = -1;
            for (auto shape : cli.shapes) {
                double t = shape->hit(ray, 0);
                if (t > 0) {
                    if ((t < final_t && final_t > 0) || final_t < 0) {
                        final_t = t;
                        hit = shape;
                    }
                }
            }
            if (hit) {
                Vector3 aColor = hit->getAColor();
                Vector3 rColor = hit->getRColor();
                double r_I = aColor[0];
                double g_I = aColor[1];
                double b_I = aColor[2];

                Vector3 shadowRayStart = ray.start + ray.direction*final_t;

                for (Light& light : cli.lights) {
                    bool shadow = false;

                    Ray shadowRay(shadowRayStart, -light.direction);
                    for (auto shape : cli.shapes) {
                        double t = shape->hit(shadowRay, eps);
                        if (t > 0) {
                            shadow = true;
                        }
                    }

                    if (!shadow) {
                        double cos_theta = (-light.direction*hit->normal(ray, final_t).unit());
                        r_I += light.color[0]*rColor[0]*cos_theta;
                        g_I += light.color[1]*rColor[1]*cos_theta;
                        b_I += light.color[2]*rColor[2]*cos_theta;
                    }
                }

                if (hit->specular()) {
                    Vector3 n = hit->normal(ray, final_t).unit();
                    Vector3 r = ray.direction - n*(2*(ray.direction*n));
                    Vector3 rayColor = reflection(Ray(shadowRayStart, r), 0);
                    Vector3 sColor = hit->getSColor();

                    if (rayColor == Vector3(0, 0, 0)) {
                    } else {
                        r_I += rayColor[0]*sColor[0];
                        g_I += rayColor[1]*sColor[1];
                        b_I += rayColor[2]*sColor[2];
                    }
                }


                if (r_I > 1) {
                    r_I = 1;
                } else if (r_I < 0) {
                    r_I = 0;
                }
                if (g_I > 1) {
                    g_I = 1;
                } else if (g_I < 0) {
                    g_I = 0;
                }
                if (b_I > 1) {
                    b_I = 1;
                } else if (b_I < 0) {
                    b_I = 0;
                }

                checkImage[row][col][0] = (GLubyte) (r_I*255);
                checkImage[row][col][1] = (GLubyte) (g_I*255);
                checkImage[row][col][2] = (GLubyte) (b_I*255);
            } else {
                checkImage[row][col][0] = (GLubyte) (cli.background[0]*255);
                checkImage[row][col][1] = (GLubyte) (cli.background[1]*255);
                checkImage[row][col][2] = (GLubyte) (cli.background[2]*255);
            }
        }
    }

    cli.imageData.isGrayscale = false;
    cli.imageData.imageWidth = (uint32_t)cli.width;
    cli.imageData.imageLength = (uint32_t)cli.height;
    cli.imageData.bitsPerSample[0] = 8;
    cli.imageData.bitsPerSample[1] = 8;
    cli.imageData.bitsPerSample[2] = 8;
    cli.imageData.compression = 1;
    cli.imageData.photometric = 2;
    cli.imageData.samplesPerPixel = 3;
    cli.imageData.xResNum = 1;
    cli.imageData.xResDen = 1;
    cli.imageData.resolutionUnit = 1;

    cli.hasImage = true;

    glutPostRedisplay();

    auto current_time = std::chrono::high_resolution_clock::now();

    std::cout << "Rendering took " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << " milliseconds" << std::endl;
}

std::string TraceCommand::toString() const {
    return name;
}

Vector3 TraceCommand::reflection(Ray ray, int depth) {
    if (depth >= 4) {
        return Vector3(0, 0, 0);
    }
    Shape* hit = nullptr;
    double final_t = -1;
    for (auto shape : cli.shapes) {
        double t = shape->hit(ray, eps);
        if (t > 0) {
            if ((t < final_t && final_t > 0) || final_t < 0) {
                final_t = t;
                hit = shape;
            }
        }
    }
    if (hit) {
        Vector3 aColor = hit->getAColor();
        Vector3 rColor = hit->getRColor();
        double r_I = aColor[0];
        double g_I = aColor[1];
        double b_I = aColor[2];

        Vector3 shadowRayStart = ray.start + ray.direction*final_t;

        for (Light& light : cli.lights) {
            bool shadow = true;

            Ray shadowRay(shadowRayStart, -light.direction);
            for (auto shape : cli.shapes) {
                double t = shape->hit(shadowRay, eps);
                if (t < 0) {
                    shadow = false;
                }
            }

            if (shadow) {
            } else {
                double cos_theta = (-light.direction*hit->normal(ray, final_t).unit());
                r_I += light.color[0]*rColor[0]*cos_theta;
                g_I += light.color[1]*rColor[1]*cos_theta;
                b_I += light.color[2]*rColor[2]*cos_theta;
            }
        }
        if (r_I > 1) {
            r_I = 1;
        } else if (r_I < 0) {
            r_I = 0;
        }
        if (g_I > 1) {
            g_I = 1;
        } else if (g_I < 0) {
            g_I = 0;
        }
        if (b_I > 1) {
            b_I = 1;
        } else if (b_I < 0) {
            b_I = 0;
        }

        if (hit->specular()) {
            Vector3 n = hit->normal(ray, final_t).unit();
            Vector3 r = ray.direction - n*(2*(ray.direction*n));
            Vector3 rayColor = reflection(Ray(shadowRayStart, r), depth+1);
            Vector3 sColor = hit->getSColor();
            return Vector3(r_I + rayColor[0]*sColor[0], g_I + rayColor[1]*sColor[1], b_I + rayColor[2]*sColor[2]);
        } else {
            return Vector3(r_I, g_I, b_I);
        }
    } else {
        return Vector3(0, 0, 0);
    }
}