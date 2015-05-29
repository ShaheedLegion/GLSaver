#ifndef NOISE_H_INCLUDED
#define NOISE_H_INCLUDED

#include <cmath>

#define  PRECISION_SHIFT (int)12
#define  PRECISION_VALUE (int)(1 << PRECISION_SHIFT)
#define  PRECISION_SHIFT_TO_BYTE_RANGE (int)(PRECISION_SHIFT - 8)
#define  PRECISION_VALUE_FLOAT (float)(PRECISION_VALUE)
#define  FLOOR_VALUE (int)((int)0xFFFFFFFF ^ (PRECISION_VALUE - 1))

#define  FIXED_0_5 (int)(0.5 * PRECISION_VALUE_FLOAT)
#define  FIXED_0_6 (int)(0.6 * PRECISION_VALUE_FLOAT)
#define  FIXED_1 (int)(1 << PRECISION_SHIFT)
#define  FIXED_6 (int)(6 << PRECISION_SHIFT)
#define  FIXED_15 (int)(15 << PRECISION_SHIFT)
#define  FIXED_10 (int)(10 << PRECISION_SHIFT)

int _noise_i, _noise_j, _noise_k;
int _noise_u, _noise_v, _noise_w;

int _noise_A[] = { 0, 0, 0 };
int T[] = { 0x15, 0x38, 0x32, 0x2c, 0x0d, 0x13, 0x07, 0x2a };
unsigned char _expFilter[256];

int _fetch_k(int idx);
int shuffle(int i, int j, int k);
int b(int i, int j, int k, int B);

int I2F(int a) { return a << PRECISION_SHIFT; }
int F2I(int a) { return a >> PRECISION_SHIFT; }
int IMul(int a, int b) { return (a * b) >> PRECISION_SHIFT; }
int IDiv(int a, int b) { return (a << PRECISION_SHIFT) / b; }
int ILerp(int t, int a, int b) { return a + IMul(t, (b - a)); }
int b_2(int N, int B) { return N >> B & 1; }

int Fade(int t)
{
    int _t = IMul(FIXED_6, t);
    int _ft = IMul(_t - FIXED_15, t);
    int _tt = IMul(t, t);
    int _imultt = IMul(_tt, t);
    return IMul(_imultt, (_ft + FIXED_10));
}

int Floor(int value)
{
    return value & FLOOR_VALUE;
}

float Unfix(int value)
{
    return value / PRECISION_VALUE_FLOAT;
}

int iFix(int value)
{
    return value << PRECISION_SHIFT;
}

int fFix(float value)
{
    return (int)(value * PRECISION_VALUE);
}


/// <summary>
/// Returns _noise_A noise value between -0.5 and 0.5, fixed point
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
int noise(int x, int y, int z)
{
    int s = (x + y + z) / 3;
    _noise_i = Floor(x + s);
    _noise_j = Floor(y + s);
    _noise_k = Floor(z + s);
    s = (_noise_i + _noise_j + _noise_k) / 6;
    _noise_u = x - _noise_i + s;
    _noise_v = y - _noise_j + s; _noise_w = z - _noise_k + s;
    _noise_A[0] = _noise_A[1] = _noise_A[2] = 0;
    int hi = _noise_u >= _noise_w ? _noise_u >= _noise_v ? 0 : 1 : _noise_v >= _noise_w ? 1 : 2;
    int lo = _noise_u < _noise_w ? _noise_u < _noise_v ? 0 : 1 : _noise_v < _noise_w ? 1 : 2;
    int result = _fetch_k(hi) + _fetch_k(3 - hi - lo) + _fetch_k(lo) + _fetch_k(0);

    return result;
}

int _fetch_k(int idx)
{
    int s = (_noise_A[0] + _noise_A[1] + _noise_A[2]) / 6;
    int x = _noise_u - _noise_A[0] + s;
    int y = _noise_v - _noise_A[1] + s;
    int z = _noise_w - _noise_A[2] + s;
    int t = FIXED_0_6 - IMul(x, x) - IMul(y, y) - IMul(z, z);
    int h = shuffle(F2I(_noise_i + _noise_A[0]), F2I(_noise_j + _noise_A[1]), F2I(_noise_k + _noise_A[2]));
    _noise_A[idx] += FIXED_1;
    if (t < 0)
        return 0;
    int b5 = h >> 5 & 1, b4 = h >> 4 & 1, b3 = h >> 3 & 1, b2 = h >> 2 & 1, b = h & 3;
    int p = b == 1 ? x : b == 2 ? y : z;
    int q = b == 1 ? y : b == 2 ? z : x;
    int r = b == 1 ? z : b == 2 ? x : y;

    p = (b5 == b3 ? -p : p);
    q = (b5 == b4 ? -q : q);
    r = (b5 != (b4 ^ b3) ? -r : r);

    t = IMul(t, t);
    return IMul(IMul(t, t), (p + (b == 0 ? q + r : b2 == 0 ? q : r))) << 3;
}

int shuffle(int i, int j, int k)
{
    int a = b(i, j, k, 0);
    a += b(j, k, i, 1);
    a += b(k, i, j, 2);
    a += b(i, j, k, 3);
    a += b(j, k, i, 4);
    a += b(k, i, j, 5);
    a += b(i, j, k, 6);
    a += b(j, k, i, 7);
    return  a;
}

int b(int i, int j, int k, int B)
{
    return T[b_2(i, B) << 2 | b_2(j, B) << 1 | b_2(k, B)];
}


/// <summary>
/// Returns _noise_A noise value between 0 and 255
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="alpha"></param>
/// <param name="beta"></param>
/// <param name="n"></param>
/// <returns></returns>
unsigned char PerlinNoise3D(int x, int y, int z, int alpha, int beta, int n)
{
    int i;
    int val, sum = 0;
    int scale = FIXED_1;
    for (i = 0; i < n; i++)
    {
        val = noise(x, y, z);
        sum += IDiv(val, scale);
        scale = IMul(scale, alpha);
        x = IMul(x, beta);
        y = IMul(y, beta);
        z = IMul(z, beta);
    }
    return (unsigned char)((sum + FIXED_0_5) >> PRECISION_SHIFT_TO_BYTE_RANGE);
}


/************generate the perlin noise********/

unsigned char ExpFilter(unsigned char value, float cover, float sharpness)
{
    float c = (float)(value - (255 - cover));  //reduce coverage
    if (c < 0) c = 0;            // prevent from going negative

    unsigned char val = (unsigned  char)(255 - (std::pow(sharpness, c) * 255));
    return val;
}

void PrecompExpFilter(float cover, float sharpness)
{
    int i;
    for (i = 0; i < 256; i++)
    {
        _expFilter[i] = ExpFilter((unsigned char)i, cover, sharpness);
    }
}

/// <summary>
/// Creates a nebula starry image
/// </summary>
/// <param name="width">Width of the image in pixels</param>
/// <param name="height">Height of the image in pixels</param>
/// <param name="cover">Nebula cover from 0 (no nebula) to 255 (all nebula). Default is 150</param>
/// <param name="sharpness">Nebula sharpness (color gradient) from 0 to 1; Default 0.985</param>
/// <param name="time">Nebula age, default is 9; Use different values for different nebulas</param>
/// <returns>An ARGB pixel bitmap array (same format as Silverlight's WriteableBitmap)</returns>
void CreateNebula(unsigned char * tex, int width, int height, float cover, float sharpness, float time)
{
    int sizeToUseForDivision = width > height ? width : height;

    int freq = iFix(2);
    int mulX = freq / sizeToUseForDivision;
    int mulY = freq / sizeToUseForDivision;

    int z = fFix(time);
    int z3 = fFix(time+3);
    int z4 = fFix(time+4);
    int z5 = fFix(time+5);
    int alpha = iFix(2);
    int beta = iFix(2);
    PrecompExpFilter(cover, sharpness);

    int offset = 0;
    int * pixelsARGB = (int *)tex;

    int x, y;
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            int xMul = x * mulX;
            int yMul = y * mulY;
            float unfixedXMul = Unfix(xMul);
            float unfixedYMul = Unfix(yMul);
            unsigned char value = PerlinNoise3D(xMul, yMul, z, alpha, beta, 7);

            value = _expFilter[value];
            pixelsARGB[offset] = 0;
            if (value == 0)
            {
                //pixelsARGB[offset] = 0;
                offset++;
                continue;
            }

            unsigned char r = PerlinNoise3D(xMul, yMul, z3, alpha, beta, 2);
            unsigned char g;
            unsigned char b;
            //g = b = r;
            g = PerlinNoise3D(xMul, yMul, z4, alpha, beta, 2);
            b = PerlinNoise3D(xMul, yMul, z5, alpha, beta, 2);

            r = _expFilter[r];
            g = _expFilter[g];
            b = _expFilter[b];

            unsigned char * pixdata = (unsigned char *)&(pixelsARGB[offset]);
            pixdata[0] = (value * r)>>8;
            pixdata[1] = (value * g)>>8;
            pixdata[2] = (value * b)>>8;
            pixdata[3] = 255;
            //pixelsARGB[offset] = (((value * r) >> 8) << 24) |
            //    ((value * g) & 0xFF00) |
            //    ((value * b) >> 16) | (255 << 8);

            offset++;
        } // for (int x = 0; x < width; x++)
    } // for (int y = 0; y < height; y++)
}

#endif // NOISE_H_INCLUDED
