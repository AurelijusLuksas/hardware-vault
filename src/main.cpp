#include <iostream>

EXEC SQL BEGIN DECLARE SECTION ;
int result ;
EXEC SQL END DECLARE SECTION ;

int main()
{
  printf("Programa pradeda darba\n") ;

  EXEC SQL CONNECT TO studento@localhost;

  printf("CONNECT TO biblio. SQLCODE=%ld\n", SQLCODE ) ;
  if( 0 == SQLCODE )
    EXEC SQL SELECT COUNT(*) INTO :result FROM stud.knyga ;
  if( 0 == SQLCODE )
    printf("Programa veikia! Knygu skaicius=%d\n", result) ;
  else
    printf("SQLCODE=%ld\n", SQLCODE ) ;

  EXEC SQL DISCONNECT ;

  return 0 ;
}