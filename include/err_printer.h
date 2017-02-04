#ifndef err_printer_h
char * err_out_buff = "stderr" ;
void err_print_func ( char * message )
{
  //printf ( "%s\n" , err_out_buff ) ;
  if ( strcmp ( err_out_buff , "stderr" ) )
    fprintf ( stderr , "%s\n" , message ) ;
  else if ( strcmp ( err_out_buff , "stdout" ) )
    fprintf ( stdout , "%s\n" , message ) ;
  else if ( strcmp ( err_out_buff , "stdin" ) )
    fprintf ( stderr , "%s\nThe program tried to write to stdin. Error.\n" , message ) ;
  else {
    FILE * fp = NULL ;
    fp = fopen ( err_out_buff , "w+" ) ;
    if ( fp != NULL )
    {
      fprintf ( fp , "%s\n" , message ) ;
      fclose ( fp ) ;
    }
    else
    {
      fprintf ( stderr , "%s\nThe specified output buffer/file not found.\n" , message ) ;
    }
  }
}
#define err_printer_h
#endif
