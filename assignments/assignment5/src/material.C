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

// ====================================================================
// ====================================================================

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &l, const Vec3f &lightColor) const
{
  Vec3f normal = hit.getNormal();

  if (normal.Dot3(ray.getDirection()) > 0)
  {
    normal = -1 * normal;
  }

  Vec3f v = -1 * ray.getDirection();
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
  if(reflectiveColor.Length() < 0.0001)
        return false;
    Vec3f ray_in_dir = ray.getDirection();
    Vec3f normal = hit.getNormal();

    Vec3f ray_out_dir = mirrorDirection(normal, ray_in_dir);

    ray_out_dir.Normalize();
    reflected = Ray(hit.getIntersectionPoint(), ray_out_dir);
    attenuation = reflectiveColor;
    return true;
}

bool PhongMaterial::refract(const Ray &ray, const Hit &hit, Vec3f &attenuation, Ray &refracted) const
{
  if(transparentColor.Length() < 0.0001)
        return false;
    Vec3f ray_in_dir = ray.getDirection();
    Vec3f normal = hit.getNormal();
    float ni_over_nt;
    if (ray_in_dir.Dot3(normal) > 0) {
        normal = -1 * normal;
        ni_over_nt = indexOfRefraction;
    } else {
        ni_over_nt = 1.0f / indexOfRefraction;
    }
    Vec3f v = ray_in_dir * -1;
    float NoV = normal.Dot3(v);
    float t = 1 - ni_over_nt * ni_over_nt * (1 - NoV * NoV);
    if (t > 0) {
        Vec3f ray_out_dir = (ni_over_nt * NoV - sqrt(t)) * normal - ni_over_nt * v;
        ray_out_dir.Normalize();
        refracted = Ray(hit.getIntersectionPoint(), ray_out_dir);
        attenuation = transparentColor;
        return true;
    }else{
        return false;
    }
}
