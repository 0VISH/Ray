#include <vector>

struct Sphere{
    Vec3 pos;
    f32 radius;

    Vec3 col;

    Sphere(){
        pos.x = 0.0;
        pos.y = 0.0;
        pos.z = 0.0;

        radius = 0.5;

        col.x = 1.0;
        col.y = 0.0;
        col.z = 1.0;
    }
};
struct Scene{
    std::vector<Sphere> spheres;
};