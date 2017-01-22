#include <cmath>










struct Vec3{
  float x, y, z;
};










struct Mat3{
  Vec3 x, y, z;
};










Mat3 newMat3(float xx, float xy, float xz,
             float yx, float yy, float yz,
             float zx, float zy, float zz){
  Mat3 ret;
  ret.x.x = xx;  ret.x.y = yx;  ret.x.z = zx;
  ret.y.x = xy;  ret.y.y = yy;  ret.y.z = zy;
  ret.z.x = xz;  ret.z.y = yz;  ret.z.z = zz;
  return ret;
}










float dot (Vec3 a, Vec3 b){
  a.x *= b.x;
  a.y *= b.y;
  a.z *= b.z;
  return a.x + a.y + a.z;
}










Vec3 mul(Vec3 a, Mat3 b){
  Vec3 ret;
  ret.x = dot(a, b.x);
  ret.y = dot(a, b.y);
  ret.z = dot(a, b.z);
  return ret;
}
