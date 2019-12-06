#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int padString(char*, char**);

int main(int argc, char* argv[argc + 1]) {
  char* paddedstr;
  int n;
  if (argc < 2) {
    printf("usage: md5 string\n");
    exit(1);
  }

  u_int32_t inputLength = strlen(argv[1]);
  char* input = (char*) malloc(sizeof(char) * (inputLength + 1));
  if (inputLength > 0) {
    strncpy(input, argv[1], inputLength);
    input[inputLength] = '\0';
  }


  n = padString(input, &paddedstr);
  printf("padded to become %d\n", n);
  printf("%p\n", paddedstr);
  for (int i = 0; i < n ; i++)
    printf("%x\n", paddedstr[i]);


  return 0;
}


int padString(char* s, char** targetstr) {
  u_int16_t i;
  u_int64_t length = strlen(s);
  char* newstr = NULL;
  int n = strlen(s);
  for (i = 0; ((n * 8) + i + 64) % 512; i++);
  if (i > 0) {
    newstr = (char*) malloc(sizeof(char) * (n  + 9 + (i  / 8)));
    strncpy(newstr, s, n);
    newstr[n] = 128;
    for (int j = 0; j < (i / 8); j++)
      newstr[n + j] = 0;
    newstr[n + (i / 8) + 7] = length;
  }
  *targetstr = newstr;
  targetstr[n + 9 + (i / 8)] = '\0';
  return (n + (i / 8) + 8);
}
