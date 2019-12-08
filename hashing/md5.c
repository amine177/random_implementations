#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define LOR(X, N) (((X) << (N)) | ((X) >> (32  - N)))
#define F(X, Y, Z) (((X) & (Y)) | ((~X) & (Z)))
#define G(X, Y, Z) (((X) & (Z)) | ((Y) & (~Z)))
#define H(X, Y, Z) ((X) ^ (Y) ^ (Z))
#define I(X, Y, Z) ((Y) ^ ((X) | (~Z)))

int padString(u_int8_t*, u_int8_t**);
int md5(u_int8_t*, u_int8_t**);

int main(int argc, u_int8_t* argv[argc + 1]) {
  u_int8_t* hash;
  int n;
  if (argc < 2) {
    printf("usage: md5 string\n");
    exit(1);
  }


  u_int32_t inputLength = strlen(argv[1]);
  u_int8_t* input = (u_int8_t*) malloc(sizeof(u_int8_t) * (inputLength + 1));
  if (inputLength > 0) {
    strncpy(input, argv[1], inputLength);
    input[inputLength] = '\0';
  }



  if (!md5(input, &hash)) {
    for (int j = 0; j < 16; j++)
      printf("%2.2x", hash[j]);
    printf("\n");
    free(hash);
    free(input);
  } else {
    printf("err\n");
  }

  return 0;
}

int md5(u_int8_t* s, u_int8_t** hash) {
  u_int8_t* padded;
  u_int32_t* X;
  u_int64_t n;
  u_int32_t T[64];
  u_int32_t temp;
  u_int32_t A = 0x67452301;
  u_int32_t B = 0xefcdab89;
  u_int32_t C = 0x98badcfe;
  u_int32_t D = 0x10325476;
  u_int32_t AA, BB, CC, DD;
  u_int32_t S[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};


  for (int i = 0; i < 64; i++) {
    T[i] = floor(4294967296 * fabs(sin(i + 1)));
  }

  n = padString(s, &padded);
  for (int i = 0; i < n/64; i++) {
      X = (u_int32_t*) (padded+i*64);
    AA = A;
    BB = B;
    CC = C;
    DD = D;
    int w;

    for (int k = 0; k < 64; k++) {
      if (k < 16) {
        w = k;
        A = B + LOR((A + F(B, C, D) + X[w] + T[k]), S[k]);
      } else if (k < 32) {
        w = (5*k + 1) % 16;
        A = B + LOR((A + G(B, C, D) + X[w] + T[k]), S[k]);
      } else if (k < 48) {
        w = (3*k + 5) % 16;
        A = B + LOR((A + H(B, C, D) + X[w] + T[k]), S[k]);
      } else {
        w = (7*k) % 16;
        A = B + LOR((A + I(B, C, D) + X[w] + T[k]), S[k]);
      }


      temp = D;
      D =  C;
      C = B;
      B = A;
      A = temp;
    }

    A = A + AA;
    B = B + BB;
    C = C + CC;
    D = D + DD;
  }


  *hash = (u_int8_t*)malloc(sizeof(u_int8_t)*16);
  u_int8_t* p;

  p = (u_int8_t*) &A;
  (*hash)[0] = p[0];
  (*hash)[1] = p[1];
  (*hash)[2] = p[2];
  (*hash)[3] = p[3];
  p = (u_int8_t*) &B;
  (*hash)[4] = p[0];
  (*hash)[5] = p[1];
  (*hash)[6] = p[2];
  (*hash)[7] = p[3];
  p = (u_int8_t*) &C;
  (*hash)[8] = p[0];
  (*hash)[9] = p[1];
  (*hash)[10] = p[2];
  (*hash)[11] = p[3];
  p = (u_int8_t*) &D;
  (*hash)[12] = p[0];
  (*hash)[13] = p[1];
  (*hash)[14] = p[2];
  (*hash)[15] = p[3];
  return 0;
}

int padString(u_int8_t* s, u_int8_t** targetstr) {
  u_int16_t pad;
  u_int64_t length = strlen(s);
  u_int8_t* newstr = NULL;
  u_int64_t n = strlen(s);
  if (n % 64 < 56)
    pad = 56 - n % 64;
  else
    pad =  120 - n % 64;
  if (pad > 0) {
    newstr = (u_int8_t*) malloc(sizeof(u_int8_t) * (n  + 9 + pad));
    strncpy(newstr, s, n);
    newstr[n] = 128;
    for (int j = 1; j < pad; j++)
      newstr[n + j] = 0;
    *(u_int64_t*)(newstr + n + pad) = (u_int64_t) length * 8;
  }
  *targetstr = newstr;
  (*targetstr)[n + 9 + pad] = 0x0;
  return (n + pad + 8);
}
