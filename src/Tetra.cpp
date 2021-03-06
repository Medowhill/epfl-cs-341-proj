#include "Object.h"

#include <math.h>
#include <cmath>

#define DISTANCE_HELPER(p0, p1) proj1 = proj_to_line(p0, p1, proj0); \
                                if (in_line(p0, p1, proj1)) { \
                                    d2 = norm(proj0 - proj1); \
                                    d1 = std::min(d1, d2); \
                                }

#define TETRA_HELPER(a) d = norm(p - a); \
                        if (d < dist) { \
                            c = a; \
                            dist = d; \
                        }

/**
 * Calculate
 *
 *     ( [          ] )
 * det ( [ c0 c1 c2 ] )
 *     ( [          ] )
 *
 */
static inline double determinant(const vec3& c0, const vec3& c1, const vec3& c2)
{
    return
        c0[0] * c1[1] * c2[2] +
        c0[2] * c1[0] * c2[1] +
        c0[1] * c1[2] * c2[0] -
        c0[2] * c1[1] * c2[0] -
        c0[1] * c1[0] * c2[2] -
        c0[0] * c1[2] * c2[1];
}

/**
 * Solve
 *
 * [          ] [ x0 ] = [   ]
 * [ a0 a1 a2 ] [ x1 ] = [ b ]
 * [          ] [ x2 ] = [   ]
 *
 */
static void cramer(const vec3& a0, const vec3& a1, const vec3& a2, const vec3& b,
            double &x0, double &x1, double &x2)
{
    const double detA = determinant(a0, a1, a2);
    const double detA0 = determinant(b, a1, a2);
    const double detA1 = determinant(a0, b, a2);
    const double detA2 = determinant(a0, a1, b);
    x0 = detA0 / detA;
    x1 = detA1 / detA;
    x2 = detA2 / detA;
}

static vec3 triangle_normal(const vec3& p0, const vec3& p1, const vec3& p2) {
   return normalize(cross(p1 - p0, p2 - p0));
}

static vec3 proj_to_line(const vec3& p0, const vec3& p1, const vec3& p) {
    vec3 ab = normalize(p1 - p0);
    vec3 ap = p - p0;
    return p0 + dot(ab, ap) * ab;
}

static bool in_line(const vec3& p0, const vec3& p1, const vec3& p) {
    float l = norm(p0 - p1);
    return norm(p - p0) <= l && norm(p - p1) <= l;
}

static float distance(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p) {
    double b, c, t;
    vec3 n = triangle_normal(p0, p1, p2);
    cramer(p1 - p0, p2 - p0, n, p - p0, b, c, t);

    float d = std::abs(dot(p, n) - dot(p0, n));
    float a = 1 - b - c;
    if (a >= 0 && b >= 0 && c >= 0) // projected into the triangle
        return d;

    vec3 proj0 = a * p0 + b * p1 + c * p2;
    float d1 = norm(proj0 - p0);
    d1 = std::min(d1, float(norm(proj0 - p1)));
    d1 = std::min(d1, float(norm(proj0 - p2)));

    vec3 proj1;
    float d2;
    DISTANCE_HELPER(p0, p1);
    DISTANCE_HELPER(p0, p2);
    DISTANCE_HELPER(p1, p2);

    return sqrt(d * d + d1 * d1);
}

static float tetra_distance(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p) {
    double d0 = distance(p0, p1, p2, p);
    double d1 = distance(p0, p1, p3, p);
    double d2 = distance(p0, p2, p3, p);
    double d3 = distance(p1, p2, p3, p);
    return std::min(d0, std::min(d1, std::min(d2, d3)));
}

Tetra::Tetra(const json &_j) : Object(_j), iter(_j["iter"]) {
    double size = _j["size"];
    a1 = vec3(size, size, size);
    a2 = vec3(-size, -size, size);
    a3 = vec3(size, -size, -size);
    a4 = vec3(-size, size, -size);;
}

float Tetra::de(const vec3 &_point) const {
    vec3 p = _point;

    for (int i = 0; i < iter; i++) {
        vec3 c = a1;
        float dist = norm(p - a1), d;
        TETRA_HELPER(a2);
        TETRA_HELPER(a3);
        TETRA_HELPER(a4);
        p = 2 * p - c;
    }

    return tetra_distance(a1, a2, a3, a4, p) * pow(2, float(-iter));
}
