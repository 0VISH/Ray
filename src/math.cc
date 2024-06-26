#include <math.h>

struct Vec3{
    f32 x,y,z;

    Vec3(){
        x = 0;
        y = 0;
        z = 0;
    };
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
    Vec3 operator*(f32 num){
        return {x*num, y*num, z*num};
    };
    Vec3 operator*(s32 num){
        return {x*num, y*num, z*num};
    };
    Vec3 operator/(f32 num){
        return {x/num, y/num, z/num};
    };
    Vec3 operator/(s32 num){
        return {x/num, y/num, z/num};
    };
    void operator=(Vec3 &other){
        x = other.x;
        y = other.y;
        z = other.z;
    }
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
Vec3 normalize(Vec3 &vec){
    f32 mag = magnitude(vec);
    return vec/mag;
};
Vec3 reflect(Vec3 &vec, Vec3 &normal){
    return vec - (normal * 2 * dot(vec, normal));
};