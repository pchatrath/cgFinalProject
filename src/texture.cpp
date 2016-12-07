#include "texture.h"

#include <assert.h>
#include <iostream>
#include <algorithm>

using namespace std;

namespace CMU462 {

inline void uint8_to_float( float dst[4], unsigned char* src ) {
  uint8_t* src_uint8 = (uint8_t *)src;
  dst[0] = src_uint8[0] / 255.f;
  dst[1] = src_uint8[1] / 255.f;
  dst[2] = src_uint8[2] / 255.f;
  dst[3] = src_uint8[3] / 255.f;
}

inline void float_to_uint8( unsigned char* dst, float src[4] ) {
  uint8_t* dst_uint8 = (uint8_t *)dst;
  dst_uint8[0] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[0])));
  dst_uint8[1] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[1])));
  dst_uint8[2] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[2])));
  dst_uint8[3] = (uint8_t) ( 255.f * max( 0.0f, min( 1.0f, src[3])));
}

void Sampler2DImp::generate_mips(Texture& tex, int startLevel) {

  // NOTE(sky): 
  // The starter code allocates the mip levels and generates a level 
  // map simply fills each level with a color that differs from its
  // neighbours'. The reference solution uses trilinear filtering
  // and it will only work when you have mipmaps.

  // Task 7: Implement this

  // check start level
  if ( startLevel >= tex.mipmap.size() ) {
    std::cerr << "Invalid start level"; 
  }

  // allocate sublevels
  int baseWidth  = tex.mipmap[startLevel].width;
  int baseHeight = tex.mipmap[startLevel].height;
  int numSubLevels = (int)(log2f( (float)max(baseWidth, baseHeight)));

  numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
  tex.mipmap.resize(startLevel + numSubLevels + 1);

  int width  = baseWidth;
  int height = baseHeight;
  for (int i = 1; i <= numSubLevels; i++) {

    MipLevel& level = tex.mipmap[startLevel + i];

    // handle odd size texture by rounding down
    width  = max( 1, width  / 2); assert(width  > 0);
    height = max( 1, height / 2); assert(height > 0);

    level.width = width;
    level.height = height;
    level.texels = vector<unsigned char>(4 * width * height);

  }

  // fill all 0 sub levels with interchanging colors
  Color colors[3] = { Color(1,0,0,1), Color(0,1,0,1), Color(0,0,1,1) };
  for(size_t i = 1; i < tex.mipmap.size(); ++i) {

    //cout << "Here\n";

    //Color c = colors[i % 3];
    MipLevel& mipOld = tex.mipmap[i-1];
    MipLevel& mip = tex.mipmap[i];

    float row = 1;
    float col = 1;

    // See writeup for convolution details
    for(size_t i = 0; i < mip.width * mip.height; i ++) {
      // Caculate filtered color
      float r = 0;
      float g = 0;
      float b = 0;
      float a = 0;

      // Apply convolution to pixel
      for (int j = -1; j <= 1; j++) 
      {
        for (int k = -1; k <= 1; k++)
        {
          int pixel = ( row + j) * mipOld.width + k + col;

          Color c;
          uint8_to_float ( &c.r, &mipOld.texels[4*pixel] );
          if (j == 0 && k == 0) {
            r = r + .4 * c.r;
            g = g + .4 * c.g;
            b = b + .4 * c.b;
            a = a + .4 * c.a;
        }
        else {
            r = r + .075 * c.r;
            g = g + .075 * c.g;
            b = b + .075 * c.b;
            a = a + .075 * c.a;          
        }

        }
      }

      Color c;
      c.r = r / 1.0;
      c.b = b / 1.0;
      c.g = g / 1.0;
      c.a = a / 1.0;

      // Save convolution details to mipmap
      float_to_uint8( &mip.texels[4*i], &c.r );

      col = col + 2;
      if (col >= mipOld.width) {
        col = 0;
        row = row + 2;

      }
    }
  }

}

Color Sampler2DImp::sample_nearest(Texture& tex, 
                                   float u, float v, 
                                   int level) {

  // Task 6: Implement nearest neighbour interpolation
  
  // allocate sublevels
  int baseWidth  = tex.mipmap[level].width;
  int baseHeight = tex.mipmap[level].height;

  Color c;

  // Find nearest textron
  int x = round(baseWidth * u);
  int y = round(baseHeight * v);

  int i = 4*(y * baseWidth + x);


  MipLevel& mip = tex.mipmap[level];


  uint8_to_float( &c.r, &mip.texels[i+0]);

  return c;


}

Color Sampler2DImp::sample_bilinear(Texture& tex, 
                                    float u, float v, 
                                    int level) {
  
  // Task 6: Implement bilinear filtering

  // allocate sublevels
  int baseWidth  = tex.mipmap[level].width;
  int baseHeight = tex.mipmap[level].height;

  // Find closest points
  int x = (int)floor(baseWidth * u);
  int y = (int)floor(baseHeight * v);
  vector<int> xPts = {x,x+1};
  vector<int> yPts = {y,y+1};

  Color c;
  MipLevel& mip = tex.mipmap[level];

  Color c0, c1, c2, c3;

  // Find four neighboring pixels
  int index = 4*(yPts[0] * baseWidth + xPts[0]);
  uint8_to_float( &c2.r, &mip.texels[index+0]);

  index = 4*(yPts[0] * baseWidth + xPts[1]);
  uint8_to_float( &c3.r, &mip.texels[index+0]);
  
  index = 4*(yPts[1] * baseWidth + xPts[1]);
  uint8_to_float( &c1.r, &mip.texels[index+0]);
  
  index = 4*(yPts[1] * baseWidth + xPts[0]);
  uint8_to_float( &c0.r, &mip.texels[index+0]);

  float t = x - xPts[0];
  float s = y - yPts[0];

  // Generate average color
  c = (1-t) * ((1-s) * c0 + s * c1) + (t) * ((1-s) * c2 + s * c3);

  return c;


}

Color Sampler2DImp::sample_trilinear(Texture& tex, 
                                     float u, float v, 
                                     float u_scale, float v_scale) {

  // Task 7: Implement trilinear filtering
  // Scale factor of 0.75 used to improve image clarity
  float dudx = 0.75/u_scale;
  float dvdy = 0.75/v_scale;  
  float L = max(dudx,dvdy);
  float D = log(L) * 1.442695040888963; // Scale factor

  // Offest of 0.5 used to improve image sharpmess
  int Dmin = (int)floor(D-0.5);
  if (Dmin < 0) {Dmin = 0;}
  int Dmax = Dmin + 1;

  // Sample colors using bilinear filter at two nearest neighbors.
  Color cmin = sample_bilinear(tex,u,v,Dmin);
  Color cmax = sample_bilinear(tex,u,v,Dmax);

  // Sample between cmin and cmax
  float s = floor(D-0.5);
  if (s < 0) {s = 0;}
  float dist = s - Dmin;
  Color cOut = (1-dist) * cmin + dist * cmax;

  // return output color
  return cOut;

}

} // namespace CMU462
