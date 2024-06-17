# Program that finds the largest number in a list of integers
            .text
            .global _start
_start:     la      t0, result
            lw      t1, 4(t0)    # t1 is a counter, initialized with N
            addi    t2, t0, 8    # t2 points to the first number
            lw      t3, (t2)     # t3 holds largest found so far
loop:       addi    t1, t1, -1   # decrement counter
            beqz    t1, done     # finished when counter is 0
            addi    t2, t2, 4    # increment the list pointer
            lw      t4, (t2)     # get the next number
            bge     t3, t4, loop # check if larger
            mv      t3, t4       # remember new largest
            j       loop
done:       sw      t3, (t0)     # store result

stop:       j       stop         # wait here

            .data
result:     .word   0           # result will be stored here
N:          .word   7           # number of entries in the list                             
numbers:    .word   4, 5, 3, 6  # numbers in the list
            .word   1, 8, 2     # ...
