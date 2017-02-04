#ifndef dcmt_interface_h
#include <stdio.h>
#include <stdlib.h>
#include <dc.h>
#include <limits.h>
#include <randseed.h>
#include <err_printer.h>

#ifndef MT_STRUCT_PARAMS
mt_struct ** mts = NULL ;
int mt_statecount ;
int mt_alloc_stat = 1 ; //mts alloc status is assumed to be true
#define MT_STRUCT_PARAMS 1
#endif

//These functions ARE NOT THREADSAFE and ARE NOT MEANT TO BE THREAD SAFE.
int init_dcmt ( int count ) //initialize the arrays etc, returns true on success.
{
  mt_statecount = count ; //total number of states in the system
  mts = ( mt_struct ** ) malloc ( count * sizeof ( mt_struct * ) ) ;
  if ( mts == NULL )
  {
    err_print_func ( "Memory allocation for the MT state vectors failed." ) ;
    mt_alloc_stat = 0 ;
    return 0 ;
  }
  else
    return 1 ;
}

void destroy_dcmt ( void ) //must be used IF dcmt is setup using init_dcmt()
{
  if ( mt_alloc_stat )
    free_mt_struct_array ( mts , mt_statecount ) ;
  else
    err_print_func ( "Nothing to free as allocation failed." ) ;
}

int generator_dcmt ( void ) //returns true on success, false on failure.
//must be used in conjunction with the init_dcmt() call, and only upon success.
{
  if ( ! mt_alloc_stat )
  {
    err_print_func ( "init_dcmt() call failed." );
    return 0 ;
  }
  uint32_t dcmtidoffset ;
  uint32_t sgenrand_offset ;
  uint32_t sgenrand_id_coeff ;
  do {
    dcmtidoffset = randseed ( ) ;
  } while ( dcmtidoffset * mt_statecount < 1 ) ;
  do {
    sgenrand_offset = randseed ( ) ;
    sgenrand_id_coeff = randseed ( ) ;
  } while ( sgenrand_id_coeff * sgenrand_offset < 1 && sgenrand_id_coeff * mt_statecount < 1 ) ; //ensures that the coeff and the offset are not too large, and any of the state ids don't become zero.
  for ( int i = mt_statecount - 1 ; i >= 0 ; i-- )
  {
    //tries to find an MT parameter space with p = 4253, available are:
    /*
    w is the
     number of bits in one word of generated random numbers
     (w=31 or 32 only). p is the exponent of the period. The period
     should be 2^p-1, but p must be an Mersenne exponent, i.e.,
     2^p-1 should be a prime. The list of usable p are as follows.
       521   607  1279  2203
       2281  3217  4253  4423
       9689  9941 11213 19937
       21701 23209 44497
    */
    mts [ i ] = get_mt_parameter_id_st ( 32 , 4253 , dcmtidoffset * i , randseed ( ) );
    if ( mts [ i ] == NULL )
    {
      char str [ 512 ] ;
      sprintf ( str , "Error on thread %d during MT parameter space initialization." , mt_statecount - i ) ;
      err_print_func ( str ) ;
      return 0 ;
    }
    else
      sgenrand_mt ( sgenrand_offset + sgenrand_id_coeff * i , mts [ i ] ) ; //initializes the state vector
    return 1 ;
  }
}

//before using any of the below functions make sure that generator_dcmt ( ) returned true.
//THE generating functions ARE THREADSAFE.
uint32_t urand_mt ( int id ) //id must range from 0 to mt_statecount
{
  return ( uint32_t ) genrand_mt ( mts [ id ] ) ;
}

int rand_mt ( int id ) //id must range from 0 to mt_statecount
{
  return ( int ) genrand_mt ( mts [ id ] ) ;
}

double random_mt ( int id ) //id must range from 0 to mt_statecount
{
  return ( double ) genrand_mt ( mts [ id ] ) / RAND_MAX ;
}

uint32_t brand_mt ( int id , int range ) //returns random integers in [ 0 , b ), b > 0 is assumed.
{
  uint32_t r , limit = RAND_MAX - range ;
  do {
    r = urand_mt ( id ) ;
  } while ( r > limit ) ;
  return ( r % range ) ;
}

uint32_t abrand_mt ( int id , int low , int up ) //returns random integers in [ low , up ), up > low >= 0 is assumed.
{
  uint32_t r , limit = RAND_MAX - ( low + up ) ;
  do {
    r = urand_mt ( id ) ;
  } while ( r > limit ) ;
  return ( ( r % ( up - low ) ) + low ) ;
}
#define dcmt_interface_h
#endif
