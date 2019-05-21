---
title: Rendering 3-D Fractal Images via Ray Tracing
---

##### Group 7: Jaemin Hong and Hyojin Kook

# Abstract
Ray tracing is a rendering technique, which traces paths of lights for pixels in an image plane.
Despite its expensive computational cost, it is widely used because it can produce realistic images.
In this project, we render various 3-d fractal images via ray tracing.
Since directly calculating an intersection between a ray and a fractal object is a nontrivial problem, we use distance estimation and adaptive ray marching to find intersections.
Normal vectors at intersection points are estimated via the finite difference method rather than be calculated geometrically.
From calculated intersections and normal vectors, we can apply multiple lighting techniques to make images more realistic.
In this project, we adopt the Phong lighting, reflection, refraction, soft shadows, and ambient occlusion.
In addition, we calculate multiple camera positions based on Bezier curves and create videos by combining multiple rendered images.

# Technical Approach
## Core Goals
### Adaptive Ray Marching
Hart et al.
suggested the adaptive ray marching technique in 1989.
It is useful for rendering objects, whose boundary is not directly known, like 3-d fractals.
Instead of computing an intersection at once by solving equations, a ray advances multiple times until the ray becomes close enough to an object.
For this purpose, we should define a distance estimation function.
The function calculates the minimum distance to the object from a given point.
By advancing no more than the return value of the function, the ray avoids passing through the object rather than stopping at an intersection.

The below `intersect` function judges whether a given ray intersects with an object in a scene.
The `de` function is a distance estimator.
```c++
bool intersect(Ray ray) {
    float distance = 0;
    int steps;
    for (steps = 0; steps < max_ray_steps; steps++) {
        float d = de(ray(distance));
        distance += d;
        if (d < min_distance) break;
    }
    return steps != max_ray_steps;
}
```
The code is simplified for conciseness and one can find real implementation in the `Scene.cpp` file.

### Finite Difference Normal Vectors
As Hart et al.
noted, multiple methods to calculate normal vectors at an intersection point exist.
We use the finite difference method, which is easy to implement and precise enough.
Since a normal vector at an intersection point has a direction, which increases the distance to the object most fast, we can estimate the normal vector as the gradient vector of a distance estimator function at the intersection.
We cannot compute the derivative of each component so that we use the finite difference method to estimate the gradient vector numerically.

The below `estimate_normal` function estimate the normal vector at a given point.
```c++
vec3 estimate_normal(vec3 point) {
    vec3 x(normal_distance, 0, 0), y(0, normal_distance, 0), z(0, 0, normal_distance);
    return normalize(vec3(
        de(point + x) - de(point - x),
        de(point + y) - de(point - y),
        de(point + z) - de(point - z)));
}
```
The code is simplified for conciseness and one can find real implementation in the `Scene.cpp` file.

### Julia Quaternion Fractals
A Julia quaternion fractal contains every quaternion $z$ such that $f^n(z)$ does not diverges as $n$ goes to the infinity where $f(z)=z^2+d$, superscript $n$ denotes $n$-fold application of the function, and $d$ is an arbitrary fixed quaternion.
Hart et al.
noted that we can use $d(z)=\frac{|f^n(z)|}{2|f'^n(z)|}log|f^n(z)|$ where $n$ is large enough as a distance estimator for Julia fractals.
We can find both $f^n$ and $f’^n$ iteratively.

* $f^0(z)=z$
* $f^{(n+1)}(z)=f^n(z)^2+d$
* $f'^0(z)=1$
* $f'^{(n+1)}(z)=2f^n(z)f'^n(z)$

Since Julia fractals exist at a 4-d hyperspace but we can render objects in a 3-d space, we should project the fractals into some 3-d space.
In this project, we simply project them into the $k=k’$ space for some real number $k’$.

The below `de` function is the distance estimator for Julia fractals.
```c++
float de(vec3 point) {
    quat q = quat(point, k);
    quat dq = quat(1, 0, 0, 0);
    double r;
    for (int i = 0; i < iter; i++) {
        dq = 2 * q * dq;
        q = q * q + d;
        r = size(q);
    }
    return 0.5 * log(r) * r / size(dq);
}
```
The code is simplified for conciseness and one can find real implementation in the `Objects.cpp` file.

### Comparison with the Reference

## Extensions

### Shadows

#### Soft Shadows
$~~~~\frac{1}{\pi R^2}\int\int_{C}Intersect(P,L(r,\theta))dA$

$=\frac{1}{\pi R^2}\int_{0}^{2\pi}\int_{0}^{R}Intersect(P,L(r,\theta))rdrd\theta$

$=\frac{2\pi R}{\pi R^2}\int_{0}^{2\pi}\int_{0}^{R}Intersect(P,L(r,\theta))r\frac{1}{R}\frac{1}{2\pi}drd\theta$

$=\frac{2}{R}E[Intersect(P,L(r,\theta))\times r]$ where $r\sim U[0,R]$ and $\theta\sim U[0,2\pi]$

$\approx\frac{2}{RN}\sum_{i=1}^{N}Intersect(P,L(r_i,\theta_i))\times r_i$

We can implement it as the following function:
```c++
double soft_shadow(vec3 light, vec3 point) {
    double sum = 0;

    for (int i = 0; i < N; i++) {
        double r = rand_uniform(0, R);
        double theta = rand_uniform(0, 2 * M_PI);

        vec3 rand_light = adjust(light, point, r, theta);
        double distance = norm(rand_light - point);
        Ray ray(point, rand_light - point);

        float t;
        if (intersect(ray, t) && t < distance) sum += r;
    }

    return 2 * sum / N / R;
}
```
The code is simplified for conciseness and one can find real implementation in the `Scene.cpp` file.

### Ambient Occlusion
$~~~~\frac{1}{2\pi}\int\int_{H}Intersect(\phi,\theta)dS$

$=\frac{1}{2\pi}\int_{0}^{\pi/2}\int_{0}^{2\pi}Intersect(\phi,\theta)sin\theta d\phi d\theta$

$=\frac{2\pi\times\pi/2}{2\pi}\int_{0}^{\pi/2}\int_{0}^{2\pi}Intersect(\phi,\theta)sin\theta\frac{1}{2\pi}\frac{1}{\pi/2}d\phi d\theta$

$=\frac{\pi}{2}E[Intersect(\phi,\theta)\times sin\theta]$ where $\phi\sim U[0,2\pi]$ and $\theta\sim U[0,\pi/2]$

$\approx\frac{\pi}{2N}\sum_{i=1}^{N}Intersect(\phi_i,\theta_i)\times sin\theta_i$

We can implement it as the following function:
```c++
double occlusion(vec3 point, vec3 normal) {
    double sum = 0;

    for (int i = 0; i < N; i++) {
        double phi = rand_uniform(0, M_PI * 2.0);
        double theta = rand_uniform(0, M_PI * 0.5);

        vec3 dir = calculate_dir(normal, phi, theta);
        Ray ray(point, dir);

        if (intersect(ray)) sum += sin(theta);
    }

    return sum * M_PI / 2 / N;
}
```
The code is simplified for conciseness and one can find real implementation in the `Scene.cpp` file.

### Refraction

### Other Fractals

### Bezier-curve-based Camera Paths
