#include "lookup8.h"
#include "buffer.h"

int64_t dieselHashUTF8(const std::string str)
{
    std::vector<unsigned char> buffer(str.begin(), str.end());
    return dieselHash(buffer);
}

int64_t dieselHash(std::vector<unsigned char> buffer)
{
    return lookup8(buffer, 0);
}

#define mix64(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>43); \
  b -= c; b -= a; b ^= (a<<9); \
  c -= a; c -= b; c ^= (b>>8); \
  a -= b; a -= c; a ^= (c>>38); \
  b -= c; b -= a; b ^= (a<<23); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>35); \
  b -= c; b -= a; b ^= (a<<49); \
  c -= a; c -= b; c ^= (b>>11); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<18); \
  c -= a; c -= b; c ^= (b>>22); \
}

unsigned long long hash(unsigned char *k, unsigned long long length, unsigned long long level)
{
    unsigned long long a, b, c, len;
    len = length;
    a = b = level;
    c = 0x9e3779b97f4a7c13LL;
    while (len >= 24)
    {
        a += (k[0] + ((unsigned long long)k[1] << 8) + ((unsigned long long)k[2] << 16) + ((unsigned long long)k[3] << 24) + ((unsigned long long)k[4] << 32) + ((unsigned long long)k[5] << 40) + ((unsigned long long)k[6] << 48) + ((unsigned long long)k[7] << 56));
        b += (k[8] + ((unsigned long long)k[9] << 8) + ((unsigned long long)k[10] << 16) + ((unsigned long long)k[11] << 24) + ((unsigned long long)k[12] << 32) + ((unsigned long long)k[13] << 40) + ((unsigned long long)k[14] << 48) + ((unsigned long long)k[15] << 56));
        c += (k[16] + ((unsigned long long)k[17] << 8) + ((unsigned long long)k[18] << 16) + ((unsigned long long)k[19] << 24) + ((unsigned long long)k[20] << 32) + ((unsigned long long)k[21] << 40) + ((unsigned long long)k[22] << 48) + ((unsigned long long)k[23] << 56));
        mix64(a, b, c);
        k += 24;
        len -= 24;
    }
    c += length;
    switch (len)
    {
    case 23:
        c += ((unsigned long long)k[22] << 56);
    case 22:
        c += ((unsigned long long)k[21] << 48);
    case 21:
        c += ((unsigned long long)k[20] << 40);
    case 20:
        c += ((unsigned long long)k[19] << 32);
    case 19:
        c += ((unsigned long long)k[18] << 24);
    case 18:
        c += ((unsigned long long)k[17] << 16);
    case 17:
        c += ((unsigned long long)k[16] << 8);
    case 16:
        b += ((unsigned long long)k[15] << 56);
    case 15:
        b += ((unsigned long long)k[14] << 48);
    case 14:
        b += ((unsigned long long)k[13] << 40);
    case 13:
        b += ((unsigned long long)k[12] << 32);
    case 12:
        b += ((unsigned long long)k[11] << 24);
    case 11:
        b += ((unsigned long long)k[10] << 16);
    case 10:
        b += ((unsigned long long)k[9] << 8);
    case 9:
        b += ((unsigned long long)k[8]);
    case 8:
        a += ((unsigned long long)k[7] << 56);
    case 7:
        a += ((unsigned long long)k[6] << 48);
    case 6:
        a += ((unsigned long long)k[5] << 40);
    case 5:
        a += ((unsigned long long)k[4] << 32);
    case 4:
        a += ((unsigned long long)k[3] << 24);
    case 3:
        a += ((unsigned long long)k[2] << 16);
    case 2:
        a += ((unsigned long long)k[1] << 8);
    case 1:
        a += ((unsigned long long)k[0]);
    }
    mix64(a, b, c);
    return c;
}

int64_t lookup8(std::vector<unsigned char> buffer, int64_t level) {
    return hash(buffer.data(), buffer.size(), level);
}