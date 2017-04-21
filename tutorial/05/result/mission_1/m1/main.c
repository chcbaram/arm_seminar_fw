#include <stdio.h>

int main(int argc, char *argv[])
{
  int baud;
  int mode;

  if (argc == 4)
  {
    baud = strtol( argv[ 2 ], NULL, 10 );
    mode = strtol( argv[ 3 ], NULL, 10 );

    printf("Port : %s\n", argv[1]);
    printf("Baud : %d\n", baud);
    printf("Mode : %d\n", mode);
  }
  else
  {
    printf("error command\n");
  }

  return 0;
}
