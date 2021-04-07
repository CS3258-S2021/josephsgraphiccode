#include "LookatCommand.h"
#include "Matrix44.h"
#include "global.h"
#include "Vector3.h"
#include "Matrix44.h"

#include <iostream>
#include <cmath>

LookatCommand::LookatCommand(float fX, float fY, float fZ, float atX, float atY, float atZ, float upX, float upY, float upZ, CLI& mainCLI):
        name("lookat"), fx(fX), fy(fY), fz(fZ), atx(atX), aty(atY), atz(atZ), upx(upX), upy(upY), upz(upZ), cli(mainCLI) {}

void LookatCommand::execute() {
    float dx, dy, dz ;
    Vector3 slnv, rx, ry, rz, up;
    Matrix44 ltrans, tmpsln, rfin, lookat;
    //Vertex_unit v, sv;
    //int i, j;

    /*   translation */
    // ltrans default is I
    // ltrans = I;
    ltrans.set(0, 3, -fx);
    ltrans.set(1, 3, -fy);
    ltrans.set(2, 3, -fz);

    up[0] = upx; up[1] = upy; up[3] = upz;
    up = up.unit();

    /*  make P1P2  (rz) vector */
    dx = atx - fx;   dy = aty - fy;  dz = atz - fz;
    rz[0] = dx; rz[1] = dy; rz[2] = dz;
    rz = rz.unit();

    // default is I
    // rfin = I;
    /* make rx */
    rx = rz.cross(up);
    rx = rx.unit();

    /* make ry */
    ry = rx.cross(rz);

    rfin.set(0, 0, rx[0]); rfin.set(0, 1, rx[1]); rfin.set(0, 2, rx[2]);
    rfin.set(1, 0, ry[0]); rfin.set(1, 1, ry[1]); rfin.set(1, 2, ry[2]);
    rfin.set(2, 0, -rz[0]); rfin.set(2, 1, -rz[1]); rfin.set(2, 2, -rz[2]);
    rfin.set(3, 3, 0);

    lookat = rfin * ltrans;

    /* Multiply the lookat matrix by the matrix currently on the top
       of the stack.  This then becomes the new top of the stack. */

    cli.currentMatrix *= lookat;

}

std::string LookatCommand::toString() const {
    return name;
}