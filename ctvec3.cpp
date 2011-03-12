#include "ctvec3.h"
#include <cmath>
namespace ct {  
  vec3::vec3() {}
  vec3::vec3(float x,float y,float z)
    : x(x),y(y),z(z){
  }
  vec3 vec3::operator+(const vec3& r) const {
    return vec3(x+r.x, y+r.y, z+r.z);
  }
  vec3 vec3::operator-(const vec3& r) const {
    return vec3(x-r.x, y-r.y, z-r.z);
  }
  vec3 vec3::operator%(const vec3& r) const {
    return vec3(y*r.z - z*r.y, 
		z*r.x - x*r.z, 
		x*r.y - y*r.x);
  }
  float vec3::operator*(const vec3& r) const {
    return (x*r.x + y*r.y + z*r.z);
  }
  vec3 vec3::operator*(const float& r) const {
    return vec3(x*r,y*r,z*r);
  }
  vec3 vec3::operator/(const float& r) const {
    return (1.0f/r * (*this));
  }
  bool vec3::operator==(const vec3& r) const {
    return (x==r.x&&y==r.y&&z==r.z);
  }
  bool vec3::operator!=(const vec3& r) const {
    return (x!=r.x||y!=r.y||z!=r.z);
  }
  float& vec3::operator()(int i) {
    return xyz[i];
  }
  const float& vec3::operator()(int i) const {
    return xyz[i];
  }
  vec3 operator*(const float& l, const vec3& r) {
    return r*l;
  }
  float mag(const vec3& v) {
    return std::sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
  }
  float dot(const vec3&, const vec3&);
};
