#ifndef TLB
#define TLB

#include <cstddef>    
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <atomic>
#include <math.h>

#include <benchmark/benchmark.h>

#define LC_MAX_PS 16
#define BT_MAX_PS 16

#define MAX_THREAD 8



inline void CustomArguments_LC(benchmark::internal::Benchmark* b) {
  for (int i = 2; i <= LC_MAX_PS; i+=2)
    for (int j = 1; j <= MAX_THREAD; j *= 2)
      b->Args({i, j});
}

inline void CustomArguments_BT(benchmark::internal::Benchmark* b) {
  for (int i = 2; i <= BT_MAX_PS; i+=2)
    for (int j = 1; j <= MAX_THREAD; j *= 2)
      b->Args({i, j});
}


// The BELOW code is taken from the Marl Repository for fractal creation
// Copyright 2019 The Marl Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");

// A color formed from a red, green and blue component.
template <typename T>
struct Color {
  T r, g, b;

  inline Color<T>& operator+=(const Color<T>& rhs) {
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    return *this;
  }

  inline Color<T>& operator/=(T rhs) {
    r /= rhs;
    g /= rhs;
    b /= rhs;
    return *this;
  }
};

// colorize returns a 'rainbow-color' for the scalar v.
inline Color<float> colorize(float v) {
  constexpr float PI = 3.141592653589793f;
  constexpr float PI_2_THIRDS = 2.0f * PI / 3.0f;
  return Color<float>{
      0.5f + 0.5f * cosf(v + 0 * PI_2_THIRDS),
      0.5f + 0.5f * cosf(v + 1 * PI_2_THIRDS),
      0.5f + 0.5f * cosf(v + 2 * PI_2_THIRDS),
  };
}

// lerp returns the linear interpolation between min and max using the weight x.
inline float lerp(float x, float min, float max) {
  return min + x * (max - min);
}

// julia calculates the Julia-set fractal value for the given coordinate and
// constant. See https://en.wikipedia.org/wiki/Julia_set for more information.
inline Color<float> julia(float x, float y, float cx, float cy) {
  for (int i = 0; i < 1000; i++) {
    if (x * x + y * y > 4) {
      return colorize(sqrtf(static_cast<float>(i)));
    }

    auto xtemp = x * x - y * y;
    y = 2 * x * y + cy;
    x = xtemp + cx;
  }

  return {};
}

// writeBMP writes the given image as a bitmap to the given file, returning
// true on success and false on error.
inline bool writeBMP(const Color<uint8_t>* texels,
              int width,
              int height,
              const char* path) {
  auto file = fopen(path, "wb");
  if (!file) {
    fprintf(stderr, "Could not open file '%s'\n", path);
    return false;
  }
//   defer(fclose(file));

  bool ok = true;
  auto put4 = [&](uint32_t val) { ok = ok && fwrite(&val, 1, 4, file) == 4; };
  auto put2 = [&](uint16_t val) { ok = ok && fwrite(&val, 1, 2, file) == 2; };
  auto put1 = [&](uint8_t val) { ok = ok && fwrite(&val, 1, 1, file) == 1; };

  const uint32_t padding = -(3 * width) & 3U;   // in bytes
  const uint32_t stride = 3 * width + padding;  // in bytes
  const uint32_t offset = 54;

  // Bitmap file header
  put1('B');  // header field
  put1('M');
  put4(offset + stride * height * 3);  // size in bytes
  put4(0);                             // reserved
  put4(offset);

  // BITMAPINFOHEADER
  put4(40);      // size of header in bytes
  put4(width);   // width in pixels
  put4(height);  // height in pixels
  put2(1);       // number of color planes
  put2(24);      // bits per pixel
  put4(0);       // compression scheme (none)
  put4(0);       // size
  put4(72);      // horizontal resolution
  put4(72);      // vertical resolution
  put4(0);       // color pallete size
  put4(0);       // 'important colors' count

  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      auto& texel = texels[x + y * width];
      put1(texel.b);
      put1(texel.g);
      put1(texel.r);
    }
    for (uint32_t i = 0; i < padding; i++) {
      put1(0);
    }
  }

  fclose(file);
  return ok;
}
// End Of Marl Repository Code //

#endif