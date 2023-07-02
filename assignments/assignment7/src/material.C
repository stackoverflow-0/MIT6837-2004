// ====================================================================
// Add this function prototype to the Material class and each of
// its subclasses.
// ====================================================================

#include "material.h"

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight
// artifact for small specular exponents (wide specular lobe)
// ====================================================================

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

// #ifdef SPECULAR_FIX
// // OPTIONAL:  global variable allows (hacky) communication
// // with glCanvas::display
// extern int SPECULAR_FIX_WHICH_PASS;
// #endif

void PhongMaterial::glSetMaterial(void) const
{

  GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
  GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
  GLfloat specular[4] = {
      getSpecularColor().r(),
      getSpecularColor().g(),
      getSpecularColor().b(),
      1.0};
  GLfloat diffuse[4] = {
      getDiffuseColor().r(),
      getDiffuseColor().g(),
      getDiffuseColor().b(),
      1.0};

  // NOTE: GL uses the Blinn Torrance version of Phong...
  float glexponent = exponent;
  if (glexponent < 0)
    glexponent = 0;
  if (glexponent > 128)
    glexponent = 128;

#if !SPECULAR_FIX

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0)
  {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  }
  else if (SPECULAR_FIX_WHICH_PASS == 1)
  {
    // Second pass, compute normal dot light
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }
  else
  {
    // Third pass, add ambient & diffuse terms
    assert(SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}

extern bool shade_back;
// ====================================================================
// ====================================================================

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f normal = hit.getNormal();
  Vec3f rd = ray.getDirection();

  if (shade_back && normal.Dot3(rd) > 0)
    normal = -1 * normal;
  Vec3f v = -1 * rd;
  v.Normalize();
  // diff
  Vec3f diffuse = diffuseColor * lightColor * std::max(normal.Dot3(l), 0.0f);
  // spec
  Vec3f h = v + l;
  h.Normalize();
  Vec3f specular = specularColor * lightColor * pow(max(normal.Dot3(h), 0.0f), exponent);
  // ans
  Vec3f color = diffuse + specular;
  return color;
}

bool PhongMaterial::reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const
{
  if (reflectiveColor.Length() < 0.0001)
    return false;
  Vec3f ray_in = ray.getDirection();
  Vec3f normal = hit.getNormal();
  if (ray_in.Dot3(normal) > 0)
    normal = -1 * normal;
  Vec3f ray_out = ray_in - 2.0f * normal.Dot3(ray_in) * normal;
  ray_out.Normalize();
  reflected = Ray(hit.getIntersectionPoint(), ray_out);
  attenuation = reflectiveColor;
  return true;
}

bool PhongMaterial::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  if (transparentColor.Length() < 0.0001)
    return false;
  Vec3f ray_in = ray.getDirection();
  Vec3f normal = hit.getNormal();
  float ni_over_nt;
  if (ray_in.Dot3(normal) > 0)
  {
    normal = -1 * normal;
    ni_over_nt = indexOfRefraction;
  }
  else
  {
    ni_over_nt = 1.0f / indexOfRefraction;
  }
  Vec3f v = ray_in * -1;
  float NoV = normal.Dot3(v);
  float t = 1 - ni_over_nt * ni_over_nt * (1 - NoV * NoV);
  if (t > 0)
  {
    Vec3f ray_out = (ni_over_nt * NoV - sqrt(t)) * normal - ni_over_nt * v;
    ray_out.Normalize();
    refracted = Ray(hit.getIntersectionPoint(), ray_out);
    attenuation = transparentColor;
    return true;
  }
  else
  {
    return false;
  }
}

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = 0;
  int t = 1;
  for (int i = 0; i < octaves; ++i)
  {
    N += PerlinNoise::noise(t * p.x(), t * p.y(), t * p.z()) / t;
    t = t << 1;
  }
  N = N < 0 ? 0 : N;
  N = N > 1 ? 1 : N;
  return mtl1->Shade(ray, hit, l, lightColor) * N + mtl2->Shade(ray, hit, l, lightColor) * (1 - N);
}

void Noise::glSetMaterial() const
{
}

bool Noise::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = 0;
  int t = 1;
  for (int i = 0; i < octaves; ++i)
  {
    N += PerlinNoise::noise(t * p.x(), t * p.y(), t * p.z()) / t;
    t = t << 1;
  }
  N = N < 0 ? 0 : N;
  N = N > 1 ? 1 : N;
  if (mtl1->refract(ray, hit, attenuation, refracted))
  {
    attenuation = mtl1->getDiffuseColor() * N + mtl2->getDiffuseColor() * (1 - N);
    // attenuation = getDiffuseColor();
    return true;
  }
  return false;
}

bool Noise::reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = 0;
  int t = 1;
  for (int i = 0; i < octaves; ++i)
  {
    N += PerlinNoise::noise(t * p.x(), t * p.y(), t * p.z()) / t;
    t = t << 1;
  }
  N = N < 0 ? 0 : N;
  N = N > 1 ? 1 : N;
  // 这里使用mtl1的反射光线
  if (mtl1->reflect(ray, hit, attenuation, reflected))
  {
    attenuation = mtl1->getDiffuseColor() * N + mtl2->getDiffuseColor() * (1 - N);
    return true;
  }
  return false;
}

Vec3f Noise::getDiffuseColor() const
{
  return (mtl1->getDiffuseColor() + mtl2->getDiffuseColor()) * 0.5;
}

Vec3f Marble::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = 0;
  for (int i = 0; i < octaves; ++i)
  {
    int t = pow(2, i);
    N += PerlinNoise::noise(t * p.x(), t * p.y(), t * p.z()) / t;
  }
  float M = sin(frequency * p.x() + amplitude * N);
  M = M < 0 ? 0 : M;
  M = M > 1 ? 1 : M;
  return mtl1->Shade(ray, hit, l, lightColor) * M + mtl2->Shade(ray, hit, l, lightColor) * (1 - M);
}

void Marble::glSetMaterial() const
{
}

bool Marble::reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = getN(p);
  // N = N < 0 ? 0 : N;
  // N = N > 1 ? 1 : N;
  float M = sin(frequency * p.x() + amplitude * N);
  M = M < 0 ? 0 : M;
  M = M > 1 ? 1 : M;
  if (mtl1->reflect(ray, hit, attenuation, reflected))
  {
    attenuation = mtl1->getDiffuseColor() * M + mtl2->getDiffuseColor() * (1 - M);
    // attenuation = getDiffuseColor();
    return true;
  }
  return false;
}

bool Marble::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = getN(p);

  float M = sin(frequency * p.x() + amplitude * N);

  M = M < 0 ? 0 : M;
  M = M > 1 ? 1 : M;

  // PhongMaterial mtl3(getIR(p));
  if (mtl1->refract(ray, hit, attenuation, refracted))
  {
    attenuation = mtl1->getDiffuseColor() * M + mtl2->getDiffuseColor() * (1 - M);
    // attenuation = getDiffuseColor();
    return true;
  }
  return false;
}

Vec3f Marble::getDiffuseColor() const
{
  return (mtl1->getDiffuseColor() + mtl2->getDiffuseColor()) * 0.5;
}

Vec3f Wood::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float N = getN(p);

  float M = sin(frequency * p.x() + amplitude * N);

  M = M < 0 ? 0 : M;
  M = M > 1 ? 1 : M;
  return mtl1->Shade(ray, hit, l, lightColor) * M + mtl2->Shade(ray, hit, l, lightColor) * (1 - M);
}

void Wood::glSetMaterial() const
{
}

bool Wood::reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const
{
  return false;
}

bool Wood::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  return false;
}

Vec3f Wood::getDiffuseColor() const
{
  return (mtl1->getDiffuseColor() + mtl2->getDiffuseColor()) * 0.5;
}

Vec3f Checkerboard::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float sines = sin(3 * p.x()) * sin(3 * p.y()) * sin(3 * p.z());
  if (sines > 0)
    return mtl1->Shade(ray, hit, l, lightColor);
  else
    return mtl2->Shade(ray, hit, l, lightColor);
}

void Checkerboard::glSetMaterial() const
{
  mtl1->glSetMaterial();
}

bool Checkerboard::reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float sines = sin(3 * p.x()) * sin(3 * p.y()) * sin(3 * p.z());
  if (sines > 0)
    return mtl1->reflect(ray, hit, attenuation, reflected);
  else
    return mtl2->reflect(ray, hit, attenuation, reflected);
}

bool Checkerboard::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  Vec3f p = hit.getIntersectionPoint();
  mat->Transform(p);
  float sines = sin(3 * p.x()) * sin(3 * p.y()) * sin(3 * p.z());
  if (sines > 0)
    return mtl1->refract(ray, hit, attenuation, refracted);
  else
    return mtl2->refract(ray, hit, attenuation, refracted);
}
