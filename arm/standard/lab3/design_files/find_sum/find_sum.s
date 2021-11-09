            .text               
            .global _start       
_start:                  
            // ... get N and pass to subroutine
            BL  FINDSUM 
END:        B   END     // wait here

FINDSUM:    // ...
            // ...

N:          .word   9
            .end                
