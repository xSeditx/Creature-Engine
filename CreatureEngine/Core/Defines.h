/*
 * ========================================================================================
 *    DEFINES USED TO CONTROL THE COMPILER AND TO A LESSER EXTENT RUNTIME BEHAVIOR 
 * ========================================================================================
 */
 

#define FALSE  0
#define TRUE   !FALSE




/* Values will range 0-100 with greater values showing messages less frequently and lower more
   Errors being a value of Zero and things like Object creation and destruction 100 */

#define  ERROR_MESSAGE_DISPLAY          0
#define  USER_WARNING_MESSAGES         20
#define  TODO_MESSAGES                 50
#define  REFACTOR_MESSAGES             80
#define  OBJECT_CREATION_MESSAGE      100