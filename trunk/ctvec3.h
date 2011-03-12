#ifndef CT_VEC3_H_
#define CT_VEC3_H_

namespace ct {
  struct vec3 {
    union {
      float xyz[3];
      struct {
	float x, y, z;    
      };
    };
    vec3();
    vec3(float,float,float);
    vec3 operator+(const vec3&) const;
    vec3 operator-(const vec3&) const;
    vec3 operator%(const vec3& r) const;
    float operator*(const vec3& r) const;
    float& operator()(int i);
    const float& operator()(int i) const;
    vec3 operator*(const float&) const;
    vec3 operator/(const float&) const;
    bool operator==(const vec3&) const;
    bool operator!=(const vec3&) const;
  };
  vec3 operator*(const float&, const vec3&);
  float mag(const vec3&);
  float dot(const vec3&, const vec3&);
};

#endif
