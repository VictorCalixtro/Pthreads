/*
BMP24 code for CS 4380 / CS 5351

Copyright (c) 2019 Texas State University. All rights reserved.

Redistribution in source or binary form, with or without modification,
is *not* permitted. Use in source and binary forms, with or without
modification, is only permitted for academic use in CS 4380 or CS 5351
at Texas State University.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Author: Martin Burtscher
*/

#ifndef BMP_43805351
#define BMP_43805351

#include <cstdio>

class BMP24 {
private:
  int wo, ho;
  int w, h;
  int* bmp;

public:
  BMP24(int xmin, int ymin, int xmax, int ymax)
  {
    if ((xmin >= xmax) || (ymin >= ymax)) exit(-2);
    wo = xmin;
    ho = ymin;
    w = xmax - xmin;
    h = ymax - ymin;
    bmp = new int[w * h];
  }

  ~BMP24()
  {
    delete [] bmp;
  }

  void dot(int x, int y, const int col)
  {
    x -= wo;
    y -= ho;
    if ((0 <= x) && (0 <= y) && (x < w) && (y < h)) {
      bmp[y * w + x] = col;
    }
  }

  void save(const char* const name)
  {
    const int pad = ((w * 3 + 3) & ~3) - (w * 3);
    FILE* f = fopen(name, "wb");
    int d;

    d = 0x4d42;  fwrite(&d, 1, 2, f);
    d = 14 + 40 + h * w * 3 + pad * h;  fwrite(&d, 1, 4, f);
    d = 0;  fwrite(&d, 1, 4, f);
    d = 14 + 40;  fwrite(&d, 1, 4, f);

    d = 40;  fwrite(&d, 1, 4, f);
    d = w;  fwrite(&d, 1, 4, f);
    d = h;  fwrite(&d, 1, 4, f);
    d = 1;  fwrite(&d, 1, 2, f);
    d = 24;  fwrite(&d, 1, 2, f);
    d = 0;  fwrite(&d, 1, 4, f);
    d = h * w * 3 + pad * h;  fwrite(&d, 1, 4, f);
    d = 0;  fwrite(&d, 1, 4, f);
    d = 0;  fwrite(&d, 1, 4, f);
    d = 0;  fwrite(&d, 1, 4, f);
    d = 0;  fwrite(&d, 1, 4, f);

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        fwrite(&bmp[y * w + x], 1, 3, f);
      }
      fwrite(&d, 1, pad, f);
    }

    fclose(f);
  }
};

#endif

