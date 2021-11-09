/* Program that finds the largest number in a list of integers */
                .text                           
                .global _start                  
_start:     
                movia   r8, RESULT  # r8 points to the result location
                ldw     r4, 4(r8)   # r4 holds number of elements in the list
                addi    r5, r8, 8   # r5 points to the start of the list
                call    LARGE       
                stw     r2, (r8)    # r2 holds the subroutine return value

STOP:           br      STOP   
     
LARGE:          ...
                ...           
RESULT:         .skip   4           # space for the largest number found
N:              .word   7           # number of entries in the list
NUMBERS:        .word   4, 5, 3, 6  # the data ...
                .word 	1, 8, 2    
      
                .end                            
