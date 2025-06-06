#include <stdio.h>

#include "dac/file.h"
#include "dac/string.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Please provide at least 1 file.\n");
    return 1;
  }

  File* srcFile = File_Open(argv[1], FileMode_Read);
  String src = File_ReadText(srcFile);
  printf("DAC CODE:\n\n%s\n", src);


  // TODO START THE INTERPRETATION


  return 0;
}