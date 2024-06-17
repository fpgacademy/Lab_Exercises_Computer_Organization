# Program that counts consecutive 1's in a list of data words

            .text
            .global _start
_start:     li      s0, 0        	# final result will be in s0
            la      s1, test_num 	# s1 = address of data word

loop:       lw      a0, (s1)     	# data word to be passed to subroutine
            ... code not shown

            .
            .
            j       loop

done:       la      t0, result
            sw      s0, (t0)
end:        j       end          	# wait here

# Count the longest successive string of 1s
# Input is in a0, result is returned in a0
ones:       ... code not shown
            .
            .
            ret

            .data
test_num:   .word	0x0000e000   	# the data ...
            .word	0x3fabedef
            .word	0x00000001 							
            .word	0x00000002 							
            .word	0x75a5a5a5 							
            .word	0x01ffC000 							
            .word	0x03ffC000 							
            .word	0x55555555 							
            .word	0x77777777 							
            .word	0x08888888 							
            .word	0x11111111 							
            .word	0 			

result:     .space  4               # reserve space for the result
