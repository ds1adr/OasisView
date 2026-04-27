#ifndef OASISELEMENT_H
#define OASISELEMENT_H

#include <vector>

namespace OASISParser {

typedef unsigned char byte_t;

enum Direction {
    east = 0, north, west, south
};

struct Delta2 {
    Direction direction;
    unsigned int value;
};

struct Delta {
    int dx, dy;
};

struct OPoint {
    int x, y;
};

struct PointList {

};

struct Delta3 {
    int dx;
    int dy;

    Delta3() {
        dx = 0;
        dy = 0;
    }

    Delta3(int _dx, int _dy) {
        dx = _dx;
        dy = _dy;
    }
};

// Single element
struct NoRepetition {

};

struct Repetition {
    unsigned int nx, ny;
    unsigned int dx, dy;

    Repetition() {
        nx = 0;
        ny = 0;
        dx = 0;
        dy = 0;
    }

    Repetition(unsigned _nx, unsigned _ny, unsigned _dx, unsigned _dy) {
        nx = _nx;
        ny = _ny;
        dx = _dx;
        dy = _dy;
    }
};

struct NSpaceRepetition {
    unsigned int nx, ny;
    unsigned int grid;
    std::vector<unsigned int> v_dx, v_dy;
};

struct DiagonalRepetition {
    unsigned int nDim, mDim;
    Delta3 nDisplacement, mDisplacement;
};

struct DisplacementRepetition {
    unsigned int n;
    Delta3 d;
};

struct NDisplacementRepetition {
    unsigned int n;
    unsigned int grid;
    std::vector<Delta3> v_d;
};

}

#endif // OASISELEMENT_H
