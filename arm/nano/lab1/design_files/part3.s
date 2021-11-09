/* Program that finds the largest number in a list of integers */
            .text                           
            .global _start                   
_start:          
            LDR     R4, =RESULT     // R4 points to result location
            LDR     R0, [R4, #4]    // R0 holds the number of elements in list
            ADD     R1, R4, #8      // R1 points to the first number
            BL      LARGE           
            STR     R0, [R4]        // R0 holds the subroutine return value
END:        B       END     
        
LARGE:      ...                             
            ...   
                          
RESULT:     .word   0                       
N:          .word   7               // number of entries in the list
NUMBERS:    .word   4, 5, 3, 6      // the data
            .word   1, 8, 2                 

            .end                            
