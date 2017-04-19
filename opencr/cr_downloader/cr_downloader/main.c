#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "uart.h"
#include "boot.h"
#include "download.h"




int main(int argc, char *argv[])
{
  long baud;


  printf("cr_bootloaer \r\n");
  printf("ver : 1.0    \r\n");



  if( argc < 4 )
  {
    fprintf( stderr, "Usage: cr_downloader <port> <baud> <binary image name> [<0|1 to send Go command to new flashed app>]\n" );
    exit( 1 );
  }

  errno = 0;
  baud = strtol( argv[ 2 ], NULL, 10 );
  if( ( errno == ERANGE && ( baud == LONG_MAX || baud == LONG_MIN ) ) || ( errno != 0 && baud == 0 ) || ( baud < 0 ) )
  {
    fprintf( stderr, "Invalid baud '%s'\n", argv[ 2 ] );
    exit( 1 );
  }

  download(argc, argv);


  return 0;
}
