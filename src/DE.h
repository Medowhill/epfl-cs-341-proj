#ifndef DE_H
#define DE_H

#include "vec3.h"

#include "json.hpp"
#include <vector>

using json = nlohmann::json;

class DE {
    public:
        virtual ~DE() {}
        virtual float operator()(const vec3 &_point) const { return 0; }
        virtual void set_params(const json &_j) {}
};

class SpheresDE : public DE {
    private:
        struct sphere {
            vec3 center;
            float radius;
        };
        std::vector<sphere> spheres;
        float plane_z;
        vec3 plane_normal;
    public:
        virtual float operator()(const vec3 &_point) const;
        virtual void set_params(const json &_j);
};

class InfiniteSpheresDE : public DE {
    private:
        float radius;
        float distance;
    public:
        virtual float operator()(const vec3 &_point) const;
        virtual void set_params(const json &_j);
};

class TetraDE : public DE {
    private:
        int iter;
        vec3 a1, a2, a3, a4;
    public:
        virtual float operator()(const vec3 &_point) const;
        virtual void set_params(const json &_j);
};

class MandelbulbDE : public DE {
    private:
        int iter, power;
        double bail_out, min_distance;
    public:
        virtual float operator()(const vec3 &_point) const;
        virtual void set_params(const json &_j);
};

class JuliaDE : public DE {
    private:
        int iter, power;
        double bail_out, min_distance;
        vec3 d;
    public:
        virtual float operator()(const vec3 &_point) const;
        virtual void set_params(const json &_j);
};

inline DE *get_DE(int id) {
    switch(id) {
        case 0: return new SpheresDE();
        case 1: return new InfiniteSpheresDE();
        case 2: return new TetraDE();
        case 3: return new MandelbulbDE();
        case 4: return new JuliaDE();
        default:
            std::cerr << "Unknown DE id\n" << std::flush;
            exit(1);
    }
}

#endif
