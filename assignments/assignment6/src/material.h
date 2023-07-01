#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "glCanvas.h"
#include "ray.h"
#include "hit.h"
#include "vectors.h"
#include "perlin_noise.h"
#include "matrix.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class Material
{

public:
  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }

  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                      const Vec3f &lightColor) const = 0;

  virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const = 0;

  virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const = 0;

  virtual void glSetMaterial(void) const = 0;

protected:
  // REPRESENTATION
  Vec3f diffuseColor;
};

class PhongMaterial : public Material
{
public:

  PhongMaterial(const Vec3f &d_color) : Material(d_color), exponent(0), indexOfRefraction(0) {}

  PhongMaterial(const Vec3f &diffuseColor,
                const Vec3f &specularColor,
                float exponent,
                const Vec3f &reflectiveColor,
                const Vec3f &transparentColor,
                float indexOfRefraction) : Material(diffuseColor), specularColor(specularColor), exponent(exponent),
                                           reflectiveColor(reflectiveColor), transparentColor(transparentColor), indexOfRefraction(indexOfRefraction){};

  ~PhongMaterial(){};

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                      const Vec3f &lightColor) const;

  static Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming)
  {
    return incoming - 2.0f * normal.Dot3(incoming) * normal;
  };

  virtual bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const;

  virtual bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const;

  virtual void glSetMaterial(void) const;

  Vec3f getSpecularColor() const { return specularColor; };

private:
  Vec3f specularColor;
  float exponent;
  Vec3f reflectiveColor;
  Vec3f transparentColor;
  float indexOfRefraction;
};

// ====================================================================
// ====================================================================

class Noise : public Material
{
public:
  Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) : Material(Vec3f()), mat(m), mtl1(mat1), mtl2(mat2), octaves(octaves)
  {
  }

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

  void glSetMaterial() const override;

  bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

  bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

  Vec3f getDiffuseColor() const override;

  float getN(Vec3f p) const
  {
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
    return N;
  }
private:
  Material *mtl1;
  Material *mtl2;
  Matrix *mat;
  int octaves;
};

class Marble : public Material
{
public:
  Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Material(Vec3f()), mat(m), mtl1(mat1), mtl2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude) {}

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

  void glSetMaterial() const override;

  bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

  bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

  Vec3f getDiffuseColor() const override;

  float getN(Vec3f p) const
  {
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
    return N;
  }

private:
  Material *mtl1;
  Material *mtl2;
  Matrix *mat;

  int octaves;
  float frequency;
  float amplitude;
};

class Wood : public Material
{
public:
  Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Material(Vec3f()), mat(m), mtl1(mat1), mtl2(mat2), octaves(octaves), frequency(frequency), amplitude(amplitude) {}

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

  void glSetMaterial() const override;

  bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

  bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;

  Vec3f getDiffuseColor() const override;

  float getN(Vec3f p) const
  {
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
    return N;
  }
private:
  Material *mtl1;
  Material *mtl2;
  Matrix *mat;
  int octaves;
  float frequency;
  float amplitude;
};


class Checkerboard : public Material
{
public:
  Checkerboard(Matrix *m, Material *mat1, Material *mat2) : Material(Vec3f()), mat(m), mtl1(mat1), mtl2(mat2) {}

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const override;

  void glSetMaterial() const override;

  bool reflect(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &reflected) const override;

  bool refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const override;


private:
  Material *mtl1;
  Material *mtl2;
  Matrix *mat;
};

#endif
