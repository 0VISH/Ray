#include <vector>

struct Material{
    Vec3 col;
    f32 roughness;
    f32 metalic;

    Material(){
        col.x = 1.0;
        col.y = 0.0;
        col.z = 1.0;

        roughness = 0;
        metalic = 1;
    };
};
struct Sphere{
    Material mat;
    Vec3 pos;
    f32 radius;

    Sphere() : mat(){
        pos.x = 0.0;
        pos.y = 0.0;
        pos.z = 0.0;

        radius = 0.5;
    }
};
struct Scene{
    std::vector<Sphere> spheres;
};
struct HitPayload{
    Vec3 pos;
    Vec3 normal;
    f32 hitDistance;
    s32 objectIndex;
};
struct Ray{
    Vec3 origin;
    Vec3 direction;
};