# Program that finds the largest number in a list of integers
            .text
            .global _start
_start:     la      s0, result
            lw      a0, 4(s0)    # a0 holds number of list elements
            addi    a1, s0, 8    # a1 points to the list
            jal     ra, large
            sw      a0, (s0)     # store result

stop:       b       stop         # wait here

# Subroutine finds largest number in list of a0 elements starting at
# address in a1. Result is returned in a0
large:      ...
            .
            .
            ret

            .data
result:     .word   0           # result will be stored here
N:          .word   7           # number of entries in the list                             
numbers:    .word   4, 5, 3, 6  # numbers in the list
            .word   1, 8, 2     # ...
