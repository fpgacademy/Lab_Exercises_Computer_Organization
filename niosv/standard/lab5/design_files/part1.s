# Program that displays SW switch settings on LEDR. When a KEY is pressed,
# an interrupt is generated and the handler displays SW on HEX0
.equ LEDR_BASE, 0xff200000
.equ HEX3_HEX0_BASE, 0xff200020
.equ SW_BASE, 0xff200040
.equ KEY_BASE, 0xff200050
.equ KEY_INT, 0x40000                   # KEY port is IRQ bit 18
            .text
            .global _start
_start:     li      sp, 0x20000         # set up the stack
            la      t0, handler         # get trap handler address
            csrw    mtvec, t0           # set trap address
            li      t0, KEY_INT         # KEY port interrupt bit
            csrs    mie, t0             # enable KEY interrupts
            csrsi   mstatus, 0b1000     # enable Nios V interrupts

            jal     setup_KEY           # initialize KEY port

            li      t0, LEDR_BASE       # pointer to LEDR port
            li      t1, SW_BASE         # pointer to SW port
loop:       lw      t3, (t1)            # read from SW
            sw      t3, (t0)            # write to LEDR
            j       loop

# Trap handler
handler:    addi    sp, sp, -8          # save regs
            sw      t1, 4(sp)
            sw      t0, (sp)
            
                # check for cause of exception
            csrr    t0, mcause          # read mcause register
            li      t1, 0x80000012      # 0x8... is the interrupt bit
            xor     t1, t1, t0          # 0x12 = 18 is the KEY int #
stay:       bnez    t1, stay            # unexpected exception
            
            li      t0, KEY_BASE
            lw      t1, 12(t0)
            sw      t1, 12(t0)          # clear the interrupt

            # show SW on KEY port
            li      t0, SW_BASE
            lw      t1, (t0)            # read from SW
            li      t0, HEX3_HEX0_BASE
            sw      t1, (t0)            # write to HEX0

            lw      t0, (sp)            # restore regs
            lw      t1, 4(sp)
            addi    sp, sp, 8 
            mret

setup_KEY:  li      t0, KEY_BASE
            li      t1, 0b1111          # all four KEYs
            sw      t1, 8(t0)           # write to InterruptMask
            ret
