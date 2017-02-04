#ifndef randseed_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err_printer.h>


uint32_t randseed ( void ) //the random seed generator function
{
    uint32_t r ;
    FILE * fp = NULL ;
    fp = fopen ( "/dev/urandom" , "r" ) ;
    if ( fp != NULL ) {
      fread ( &r , 1 , sizeof ( uint32_t ) , fp ) ;
      fclose ( fp ) ;
    }
    else {
      r = ( uint32_t ) rand ( ) ;
      err_print_func ( "Could not open /dev/urandom and hence using the PRNG rand()." ) ;
    }
    return r ;
}

/*int randseedtest ( void ) //the random seed generator tester
//exit codes: 1, working; 0, not working, also prints error to stderr
{
  FILE * fp ;
  fp = fopen ( "/dev/urandom" , "r" ) ;
  if ( fp == NULL ) {
    fprintf ( stderr , "Could not open the device /dev/urandom.\n" ) ;
    fclose ( fp ) ;
    return 0 ;
  }
  else {
    fclose ( fp ) ;
    return 1 ;
  }
}*/
#define randseed_h
#endif
