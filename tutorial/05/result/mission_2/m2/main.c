#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>


uint32_t getFileSize(char *file_name);


/*
  number = strtol( argv[ 2 ], NULL, 10 );
*/


int main(int argc, char *argv[])
{


  if (argc == 5)
  {
    printf("File      : %s\n", argv[3]);
    printf("File Size : %d\n", getFileSize(argv[3]));
    printf("Port      : %s\n", argv[1]);
    printf("Baud      : %d\n", (int)strtol( argv[2], NULL, 10 ));
    printf("Mode      : %d\n", (int)strtol( argv[4], NULL, 10 ));
  }
  else
  {
    printf("error command\n");
  }


  return 0;
}


uint32_t getFileSize(char *file_name)
{
  FILE      *opencr_fp;
  uint32_t   opencr_fpsize;


  if( ( opencr_fp = fopen( file_name, "rb" ) ) == NULL )
  {
    fprintf( stderr, "Unable to open %s\n", file_name );
    exit(1);
  }
  else
  {
    fseek( opencr_fp, 0, SEEK_END );
    opencr_fpsize = ftell( opencr_fp );
    fseek( opencr_fp, 0, SEEK_SET );
    close(opencr_fp);
  }

  return opencr_fpsize;
}
