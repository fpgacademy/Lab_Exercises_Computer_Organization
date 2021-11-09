/* Program that converts a binary number to decimal */
          .text                   // executable code follows
          .global _start                  
_start:                             
          LDR     R4, =N          
          ADD     R5, R4, #4      // R5 points to the decimal digits
                                  // storage location
          LDR     R4, [R4]        // R4 holds N

          MOV     R0, R4          // parameter for DIVIDE goes in R0
          BL      DIVIDE                                              
          STRB    R1, [R5, #1]    // Tens digit is now in R1 
          STRB    R0, [R5]        // Ones digit is in R0

END:      B       END             

/* Subroutine to perform the integer division R0 / 10.
 * Returns: quotient in R1, and remainder in R0
 */
DIVIDE:   MOV     R2, #0          
CONT:     CMP     R0, #10         
          BLT     DIV_END         
          SUB     R0, #10         
          ADD     R2, #1          
          B       CONT            
DIV_END:  MOV     R1, R2          // return quotient in R1 (remainder in R0)
          BX      LR              

N:        .word   76              // the decimal number to be converted 
Digits:   .space  4               // storage space for the decimal digits

          .end                            

