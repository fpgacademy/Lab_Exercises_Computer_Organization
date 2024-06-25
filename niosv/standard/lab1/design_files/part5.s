# Program that reverses text strings

.global _start
_start:     la      s0, string0  # pointer to string
            mv      a0, s0       # pass pointer to subroutine
            jal     slen         # returns string length in a0
            add     a1, s0, a0   # a1 points to string end
            mv      a0, s0       # a0 points to string start
            jal     srev         # reverse the string

            la      s0, string1  # pointer to string
            mv      a0, s0       # pass pointer to subroutine
            jal     slen         # returns string length in a0
            add     a1, s0, a0   # a1 points to string end
            mv      a0, s0       # a0 points to string start
            jal     srev         # reverse the string

stop:       j       stop

# Subroutine that returns the length of a string
# parameter: a0 points to the string
# returns: in a0 the number of characters in the string
slen:       ...
            .
            .
            ret

# Subroutine that reverses a string
# parameters: a0 = string start, a1 = string end
srev:       ...
            .
            .
            ret

.align 4
string0:    .asciz  "gnirts a si siht"
.align 4
string1:    .asciz  "esrever dna daer"
