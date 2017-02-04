#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <dcmt_interface.h>

#ifdef _OPENMP
#define OMP_PRESENT_TRUE
#endif

#ifdef _QOPENMP
#define OMP_PRESENT_TRUE
#endif


#ifdef _FOPENMP
#define OMP_PRESENT_TRUE
#endif

#ifdef OMP_PRESENT_TRUE
#include <omp.h>
#endif

int main ( int argc , char ** argv )
{
  int num_threads = 1 ;
  //setting up openmp if present
  #ifdef OMP_PRESENT_TRUE
    printf ( "Enter number of threads: " ) ;
    scanf ( "%d" , &num_threads ) ;
    omp_set_num_threads ( num_threads ) ;
    //omp_set_num_threads ( omp_get_num_threads ( ) ) ;
  #endif
  //now setting up dcmt
  int dc_ready ;
  if ( init_dcmt ( num_threads ) )
    dc_ready = generator_dcmt ( 1 ) ;
  if ( dc_ready )
  {
    #ifdef OMP_PRESENT_TRUE
      #pragma omp parallel
      {  int id = omp_get_thread_num ( ) ;
    #else
      int id = 0 ;
    #endif
    for ( int i = 10 ; i > 0 ; i-- ) {
      printf ( "Unsigned integer: %u from id %d.\n" , urand_mt ( id ) , id ) ;
      printf ( "Signed integer: %d from id %d.\n" , rand_mt ( id ) , id ) ;
      printf ( "Float from 0 to 1: %lf from id %d.\n" , random_mt ( id ) , id ) ;
      printf ( "Unsigned integer from 0 to 100 from id %d: %u.\n" , id , brand_mt ( id , 100 ) ) ;
      printf ( "Unsigned integer from 50 to 100 from id %d: %u.\n\n" , id , abrand_mt ( id , 50 , 100 ) ) ; }
    #ifdef OMP_PRESENT_TRUE
      }
    #endif
    destroy_dcmt ( ) ;
  }
  return 0 ;
}
