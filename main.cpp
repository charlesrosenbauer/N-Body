#include "SDL/SDL.h"
#include "stdio.h"
#include <cmath>
#include "matrix.cpp"










struct Particle{
  float x, y, z;
  float dx, dy, dz;
};










void moveParticle(Particle* p, float t){
  p->x += p->dx * t;
  p->y += p->dy * t;
  p->z += p->dz * t;
  if((p->x > 4) || (p->x < -4))
    p->dx *= -1;
  if((p->y > 4) || (p->y < -4))
    p->dy *= -1;
  if((p->z > 4) || (p->z < -4))
    p->dz *= -1;
}










float distSqr(Particle* a, Particle* b, float mn){
  float dx = a->x - b->x;
  float dy = a->y - b->y;
  float dz = a->z - b->z;
  float dst = (dx * dx) + (dy * dy) + (dz * dz);
  return (dst < mn)? mn : dst;
}










void force(Particle* a, Particle* b, float c0, float c1){
  float dst = distSqr(a, b, 0.01) * c0 * c1 * -100;
  a->dx += (b->x / dst);
  a->dy += (b->y / dst);
  a->dz += (b->z / dst);
  b->dx += (a->x / dst);
  b->dy += (a->y / dst);
  b->dz += (a->z / dst);
}










void drawPixel(SDL_Surface* screen, int x, int y, uint32_t color){
  if((x < 0) || (x > 1023) || (y < 0) || (y > 1023)) return;
  uint32_t* pixels = (uint32_t*) screen->pixels;
  pixels[(y * 1024) + x] = color;
}










void bp(int x){
  printf("%i \n", x);
}










void bp(int x, int y){
  printf("%i %i\n", x, y);
}










int rState = 837596917;
void randUpdate(){
  int x = (rState & (1 <<  4)) >>  4;
  x    ^= (rState & (1 <<  6)) >>  6;
  x    ^= (rState & (1 << 12)) >> 12;
  x    ^= (rState & (1 << 19)) >> 19;
  x    ^= (rState & (1 << 23)) >> 23;
  rState <<= 1;
  rState |= x;
}










float randFn(){
  for(int it = 0; it < 10; it++) randUpdate();
  float ret = ((float)(rState % 1024)) / 512.0f;
  return ret - 1;
}










void projection(Particle* p, float rx, float ry, float rz, int* x, int* y){
  float px = p->x;
  float py = p->y;
  float pz = p->z;

  Vec3 pv = {px, py, pz};
  Mat3 rmx = newMat3( 1,          0,           0,
                      0,          (  cos(rx)), (-sin(rx)),
                      0,          (  sin(rx)), ( cos(rx)));

  Mat3 rmy = newMat3( ( cos(ry)), 0,          ( sin(ry)),
                      0,          1,          0,
                      (-sin(ry)), 0,          ( cos(ry)));

  Mat3 rmz = newMat3( ( cos(rz)), (-sin(rz)), 0,
                      ( sin(rz)), ( cos(rz)), 0,
                      0,          0,          1         );

  Vec3 t = mul(mul(mul(pv, rmx), rmy), rmz);

  t.z += 10;
  t.x /= t.z;
  t.y /= t.z;

  t.x += 1;
  t.y += 1;

  (*x) = (512 * t.x);
  (*y) = (512 * t.y);
}










int main(){

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface* screen = SDL_SetVideoMode(1024, 1024, 32, 0);
  //bp(0);

  const int PNUM = 2048;
  const int CDIV = 1024;
  Particle* ps = new Particle[PNUM];
  //bp(1);

  for(int i = 0; i < PNUM; i++){
    ps[i].x = randFn();
    ps[i].y = randFn();
    ps[i].z = randFn();
    ps[i].dx = 0;
    ps[i].dy = 0;
    ps[i].dz = 0;
  }
  //bp(2);

  bool cont = true;
  float iterate = 3.4;
  while(cont){
    SDL_FillRect(screen, 0, 0);
    for(int i = 0; i < PNUM; i++){
      for(int j = i+1; j < PNUM; j++){
        float c0 = (i < CDIV)?  1 :  1;
        float c1 = (j < CDIV)?  1 :  1;
        force(&(ps[i]), &(ps[j]), c0, c1);
      }
      //bp(3, iterate);
      moveParticle(&(ps[i]), 0.00001);
      //bp(4, iterate);
      //float posX = (ps[i].x * 256) + 256;
      //float posY = (ps[i].y * 256) + 256;
      int px, py;
      projection(&(ps[i]), 0, iterate, 0, &px, &py);
      drawPixel(screen, px, py, ((i < CDIV)? 0xFF0000 : 0x0000FF));
      //bp(5, iterate);
    }
    iterate += 0.001;
    //printf("%f \n", iterate);
    //printf("%f, %f, %f, %f \n", ps[0].x, ps[0].y, ps[0].dx, ps[0].dy);
    //bp(3);
    SDL_Flip(screen);
    //bp(4);
    SDL_Delay(8);
  }

}
