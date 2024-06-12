#include <math.h>

struct Vec3{
    f32 x,y,z;

    Vec3(f32 xArg, f32 yArg, f32 zArg){
        x = xArg;
        y = yArg;
        z = zArg;
    };
    Vec3 operator+(const Vec3 &other){
        return {x + other.x, y + other.y, z + other.z};
    };
    Vec3 operator-(const Vec3 &other){
        return {x - other.x, y - other.y, z - other.z};
    };
    Vec3 operator*(u32 num){
        return {x*num, y*num, z*num};
    };
    void print(){
        printf("(%f, %f, %f)", x, y, z);
    };
};

f32 dot(const Vec3 &vec1, const Vec3 &vec2){
    return {(vec1.x*vec2.x) + (vec1.y*vec2.y) + (vec1.z*vec2.z)};
};
f32 magnitude(const Vec3 &vec){
    return sqrt(dot(vec, vec));
}
Vec3 normalize(const Vec3 &vec){
    f32 mag = magnitude(vec);
    return {vec.x/mag, vec.y/mag, vec.z/mag};
};