#include "Texture.h"
#include "vec3.h"

#include <algorithm>
#include <math.h>
#include <cmath>

static int hash_poly(int x) { return ((x * 34) + 1) * x % 289; }
static int hash_func(const vec3 &v) { return hash_poly(hash_poly(hash_poly(int(v[0])) + int(v[1])) + int(v[2])); }
static float blending_weight_poly(float t) { return t * t * t * (t * (t * 6.0 - 15.0) + 10.0); }
static float mix(float x, float y, float a) { return x * (1 - a) + y * a; }
static vec3 mix(const vec3 &x, const vec3 &y, float a) { return x * (1 - a) + y * a; }

PerlinTexture::PerlinTexture(float _freq_multiplier, float _ampl_multiplier, int _num_octaves) :
    freq_multiplier(_freq_multiplier), ampl_multiplier(_ampl_multiplier), num_octaves(_num_octaves) {
    gradients.push_back(vec3( 1,  1,  1));
    gradients.push_back(vec3( 1,  1, -1));
    gradients.push_back(vec3( 1, -1,  1));
    gradients.push_back(vec3( 1, -1, -1));
    gradients.push_back(vec3(-1,  1,  1));
    gradients.push_back(vec3(-1,  1, -1));
    gradients.push_back(vec3(-1, -1,  1));
    gradients.push_back(vec3(-1, -1, -1));
    gradients.push_back(vec3( 0,  1,  1));
    gradients.push_back(vec3( 0,  1, -1));
    gradients.push_back(vec3( 0, -1,  1));
    gradients.push_back(vec3( 0, -1, -1));
    gradients.push_back(vec3( 0,  1,  1));
    gradients.push_back(vec3( 0,  1, -1));
    gradients.push_back(vec3( 0, -1,  1));
    gradients.push_back(vec3( 0, -1, -1));
    gradients.push_back(vec3( 1,  0,  1));
    gradients.push_back(vec3( 1,  0, -1));
    gradients.push_back(vec3( 1,  0,  1));
    gradients.push_back(vec3( 1,  0, -1));
    gradients.push_back(vec3(-1,  0,  1));
    gradients.push_back(vec3(-1,  0, -1));
    gradients.push_back(vec3(-1,  0,  1));
    gradients.push_back(vec3(-1,  0, -1));
    gradients.push_back(vec3( 1,  1,  0));
    gradients.push_back(vec3( 1,  1,  0));
    gradients.push_back(vec3( 1, -1,  0));
    gradients.push_back(vec3( 1, -1,  0));
    gradients.push_back(vec3(-1,  1,  0));
    gradients.push_back(vec3(-1,  1,  0));
    gradients.push_back(vec3(-1, -1,  0));
    gradients.push_back(vec3(-1, -1,  0));
}

float PerlinTexture::noise(const vec3 &point) const {
    const int N = 8;
    vec3 c000(floor(point[0]), floor(point[1]), floor(point[2]));
    vec3 c[N], g[N], d[N];
    float p[N];

    for (int i = 0; i < N; i++) {
        c[i] = c000 + vec3(i / 4, (i - i / 4 * 4) / 2, i % 2);
        g[i] = gradients[hash_func(c[i]) % gradients.size()];
        d[i] = point - c[i];
        p[i] = dot(g[i], d[i]);
    }

    float alpha_x = blending_weight_poly(d[0][0]);
    float ix0 = mix(p[0], p[4], alpha_x);
    float ix1 = mix(p[1], p[5], alpha_x);
    float ix2 = mix(p[2], p[6], alpha_x);
    float ix3 = mix(p[3], p[7], alpha_x);

    float alpha_y = blending_weight_poly(d[0][1]);
    float iy0 = mix(ix0, ix2, alpha_y);
    float iy1 = mix(ix1, ix3, alpha_y);

    float alpha_z = blending_weight_poly(d[0][2]);
    float iz0 = mix(iy0, iy1, alpha_z);
    return iz0;
}

float PerlinTexture::fbm(const vec3 &point) const {
    float res = 0, freq = 1, ampl = 1;
    for (int i = 0; i < num_octaves; i++, freq *= freq_multiplier, ampl *= ampl_multiplier)
        res += ampl * noise(point * freq);
    return res;
}

float PerlinTexture::turbulence(const vec3 &point) const {
    float res = 0, freq = 1, ampl = 1;
    for (int i = 0; i < num_octaves; i++, freq *= freq_multiplier, ampl *= ampl_multiplier)
        res += ampl * std::abs(noise(point * freq));
    return res;
}

MarbleTexture::MarbleTexture(const json &_j) :
    PerlinTexture(_j["freq_multiplier"], _j["ampl_multiplier"], _j["num_octaves"]),
    dark(_j["dark"]), light(_j["light"]) {}

Material MarbleTexture::get_material(const vec3 &point) const {
    vec3 q(fbm(point), fbm(point + vec3(1.7, 4.6, 2.3)), fbm(point + vec3(4.7, 2.6, 1.3)));
    float alpha = (1 + fbm(point + 4 * q)) / 2;
    alpha = std::max(std::min(alpha, 1.0f), 0.0f);
    const vec3 color = mix(light, dark, alpha);
    return Material(color, color, vec3(1), 100, 0, false, 1.5);
}
