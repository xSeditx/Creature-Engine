#pragma once

/*
 * ========================================================================================
 *    DEFINES USED TO CONTROL THE COMPILER AND TO A LESSER EXTENT RUNTIME BEHAVIOR 
 * ========================================================================================
 */
 
/* Defined in Minwindef however I might wish to not have that happen in the future */
#ifndef FALSE
#    define FALSE               0
#endif/* FALSE */
#ifndef TRUE
#    define TRUE                1
#endif/* TRUE */

/* Values will range 0-100 with greater values showing messages less frequently and lower more
   Errors being a value of Zero and things like Object creation and destruction 100 */

#define  ERROR_MESSAGE_DISPLAY          0
#define  USER_WARNING_MESSAGES         20
#define  TODO_MESSAGES                 50
#define  REFACTOR_MESSAGES             80
#define  OBJECT_CREATION_MESSAGE      100
#define  FUNCTION_TRACE_MESSAGES      110 // The most Pandentic Message logging to date outputting every function the user happens to place it in
#define  NOMINMAX
 





   /*      Old School Macro definition of Min Max. Works with all types that have overloaded <> Operators
	   WARNING: CAN CAUSE MAJOR ISSUES IF STD::MIX/MAX HAS BEEN USED */
#ifndef NOMINMAX
#    ifndef max
#        define max(a,b)            (((a) > (b)) ? (a) : (b))
#    endif
#    ifndef min
#        define min(a,b)            (((a) < (b)) ? (a) : (b))
#    endif
#endif  /* NOMINMAX */
