#pragma once
#include "defines.h"
#include <iostream>
#include <cmath>
#define PI 3.1415926535
#define PI2 PI/2
#define PI3 3*PI/2
//!DO NOT ADD ANY MORE MEMBER VARIABLES SINCE MEMORY IS SIZED AND LAID OUT EXACTLY////////////////////////////////////////////////////////////////
//TODO:Possibly make the pos data of the vector be independent of what is sent to opengl

template< ui8 size, typename T> 
struct vec2
{
public:
    T x,y;
    vec2() : x(0), y(0) {}
    vec2(T x, T y): x(x), y(y){}

    vec2 operator+(vec2 other) const {return vec2(x+other.x,y+other.y);}
    vec2 operator-(vec2 other) const {return vec2(x-other.x,y-other.y);}
    T operator*(vec2 other) const {return (x*other.x)+(y*other.y);}//dot product
    vec2& operator*(T const scalar){x*=scalar; y*=scalar; return *this;}//scalar ops
    friend vec2& operator*(T const scalar, vec2&v){v.x*=scalar; v.y*=scalar; return v;}
    T len(){return std::sqrt(x*x+y*y);}

    vec2& operator=(const vec2& C)
    {
        // Check for self-assignment
        if (this != &C) {
            x = C.x;
            y = C.y;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const vec2& v) {os<<"["<<v.x<<" "<<v.y<<"]"; return os;}
    
};
template< ui8 size, typename T> 
struct vec3
{
public:
    T x,y,z;
    vec3() : x(0), y(0),z(0) {}
    vec3(T x, T y,T z): x(x), y(y),z(z){}

    vec3 operator+(vec3 other) const {return vec3(x+other.x,y+other.y,z+other.z);}
    vec3 operator-(vec3 other) const {return vec3(x-other.x,y-other.y,z-other.y);}
    T operator*(vec3 other) const {return (x*other.x)+(y*other.y)+(z*other.z);}//dot product
    vec3& operator*(T const scalar){x*=scalar; y*=scalar; z*=scalar; return *this;}//scalar ops
    friend vec3& operator*(T const scalar, vec3&v){v.x*=scalar; v.y*=scalar; v.z*=scalar; return v;}
    T len(){return std::sqrt(x*x+y*y+z*z);}

    vec3& operator=(const vec3& C)
    {
        // Check for self-assignment
        if (this != &C) {
            x = C.x;
            y = C.y;
            z = C.z;
            
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const vec3& v) {os<<"["<<v.x<<" "<<v.y<<" "<<v.z<<"]"; return os;}
    void clipSpace()
    {
        x=(2*x/WIDTH)-1;
        y=1-(2*y/HEIGHT);
        z=0;
    }

};
template< ui8 size, typename T> 
struct vec4
{
public:
    T x,y,z,w;
    vec4() : x(0), y(0),z(0),w(0) {}
    vec4(T x, T y,T z, T w): x(x), y(y),z(z),w(w){}

    vec4 operator+(vec4 other) const {return vec4(x+other.x,y+other.y,z+other.z,w+other.w);}
    vec4 operator-(vec4 other) const {return vec4(x-other.x,y-other.y,z-other.y,w-other.y);}
    T operator*(vec4 other) const {return (x*other.x)+(y*other.y)+(z*other.z)+(w*other.w);}//dot product
    vec4& operator*(T const scalar){x*=scalar; y*=scalar; z*=scalar; w*=scalar; return *this;}//scalar ops
    friend vec4& operator*(T const scalar, vec4&v){v.x*=scalar; v.y*=scalar; v.z*=scalar; v.w*=scalar; return v;}
    T len(){return std::sqrt(x*x+y*y+z*z+w*w);}

    vec4& operator=(const vec4& C)
    {
        // Check for self-assignment
        if (this != &C) {
            x = C.x;
            y = C.y;
            z = C.z;
            w = C.w;
        }
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const vec4& v) {os<<"["<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w<<"]"; return os;}
    
};
typedef vec2<2, i32> vec2i;
typedef vec2<2, float> vec2f;
typedef vec3<3, i32> vec3i;
typedef vec3<3, float> vec3f;
typedef vec4<4, i32> vec4i;
typedef vec4<4, float> vec4f;

template<typename T>
void clipSpace(T& px, T& py)
{
    px=(2*px/WIDTH)-1;
    py=1-(2*py/HEIGHT);
}





