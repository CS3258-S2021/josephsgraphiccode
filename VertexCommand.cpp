#include "VertexCommand.h"
#include "Matrix44.h"
#include "Global.h"

#include <iostream>
#include <cmath>

VertexCommand::VertexCommand(float xIn, float yIn, float zIn, CLI& mainCLI):
        name("vertex"), x(xIn), y(yIn), z(zIn), cli(mainCLI) {}

void VertexCommand::execute() {
    float x0, y0, z0, x1, y1, z1;

    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = abs(viewport[2]-viewport[0]);
    int height = abs(viewport[3]-viewport[1]);

    /* set up 4x1 matrix of the vertex */
    Vector4 printvec(x, y, z, 1);

    Vector4 tmp = cli.currentMatrix * printvec;
    tmp[3] = 1;

    cli.printFlag++;       /* increase counter */

    if(cli.printFlag==1) { /* if the first vertex, save the points */
        cli.savemat = tmp;
    }
    if(cli.printFlag==2) { /* if the second vertex, test clipping */
        x1=(float)tmp[0];
        y1=(float)tmp[1];
        z1=(float)tmp[2];

        x0=(float)cli.savemat[0];
        y0=(float)cli.savemat[1];
        z0=(float)cli.savemat[2];

        /* if clipping occurs and points are within view volume, draw line */
        /* from v1 to v2 */
        if(near_far_clip((float)cli.nearer, (float)cli.farther, &x0, &y0, &z0, &x1, &y1, &z1) == 1) {
            Vector4 pvert1(x0, y0, z0, 1);
            Vector4 pvert2(x1, y1, z1, 1);

            if(cli.orthFlag && !cli.perspFlag) {  /* if not a perpective projection, use ortho */

                /* calculate 2d coordinates */
                Vector4 vertex1 = cli.orthMatrix * pvert1;
                Vector4 vertex2 = cli.orthMatrix * pvert2;

                vertex1 += 1; vertex1[0] *= width/2.0; vertex1[1] *= height/2.0;
                vertex2 += 1; vertex2[0] *= width/2.0; vertex2[1] *= height/2.0;

                draw_line((float)vertex1[0], (float)vertex1[1],
                          (float)vertex2[0], (float)vertex2[1], cli);

                //draw_line(0, 0, 200, 200);

                cli.printFlag=0;
            }
            else if (cli.perspFlag && !cli.orthFlag){   /* if a perspective projection, use persp */
                /* divide by abs(z) to account for */
                /* z=1 assumption in gtPerspective */
                Vector4 permat1(x0/fabs(z0), y0/fabs(z0), 1.0, (float)(1/fabs(z0)));

                Vector4 permat2(x1/fabs(z1), y1/fabs(z1), 1.0, (float)(1/fabs(z1)));

                Vector4 vertex1 = cli.perspMatrix * permat1;
                Vector4 vertex2 = cli.perspMatrix * permat2;

                vertex1 += 1; vertex1[0] *= width/2.0; vertex1[1] *= height/2.0;
                vertex2 += 1; vertex2[0] *= width/2.0; vertex2[1] *= height/2.0;

                //BOBBY
                // printf(" Vertex 1: %f %f\n Vertex 2: %f %f\n", vertex1[0], vertex1[1], vertex2[0], vertex2[1]);

                draw_line((float)vertex1[0],
                          (float)vertex1[1],
                          (float)vertex2[0],
                          (float)vertex2[1], cli);


                cli.printFlag = 0; /* set counter=0 to look for another set of vertices */
            } else {
                std::cout << "Must set either perspective or ortho before drawing" << std::endl;
            }
        }

        else
            cli.printFlag = 0;  /* if vertices beyond clipping plane, reset counter */

    }
}

std::string VertexCommand::toString() const {
    return name + ' ' + std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z);
}