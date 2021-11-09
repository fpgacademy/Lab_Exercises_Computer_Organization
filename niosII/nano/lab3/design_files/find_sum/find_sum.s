            .text                   
            .global _start          
_start:                     
            # get N and pass to subroutine
            call    FINDSUM 
END:        br      END     # wait here

FINDSUM:    # ...
            # ...
N:          .word   9   
            .end                    
