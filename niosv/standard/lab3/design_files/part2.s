	.include "../address_map_nios2.s"

/* ********************************************************************************
 * This program displays a decimal digit on the 7-segment display HEX0. Pressing 
 * a KEY has the following result:
 * 	KEY 0.	sets the display to be "blank"
 * 	KEY 1.	if SW0 is high		increments the displayed number
 *			else (SW0 is low)	decrements the displayed number
 *  If other KEYs exist, they will display a "0", if it was previously blank or
 *					do nothing.
 ********************************************************************************/
 	.equ	KEY0, 0b0001
 	.equ	KEY1, 0b0010
 	.equ	SW0,  0b0000000001
	.text						# executable code follows
	.global	_start
_start:		
	movi	r4, 0				# counter value to be displayed on HEX0

	movia	r5, KEY_BASE		# base address of KEY pushbuttons
	movia	r6, SW_BASE			# base address of SW slider switches
	movia	r7, HEX3_HEX0_BASE	# base address of HEX3_HEX0 7-segs
	stw		r4, (r7)			# clear the display

LOOP:
	ldw		r2, (r5)			# read KEY port
	beq		r2, zero, NO_KEY

WAIT:
	ldw		r3, (r5)			# wait for KEY to be released
	bne		r3, zero, WAIT
				
	/* check which KEY was pressed, and modify the counter as needed */
	andi	r3, r2, KEY0
	beq		r3, zero, CHK_KEY1
	br		BLANK
CHK_KEY1:
	andi	r3, r2, KEY1
	beq		r3, zero, ADJUST

CHK_SW0:
	ldw		r2, (r6)			# read SW port
	andi	r3, r2, SW0
	beq		r3, zero, DEC

INC:
	addi	r4, r4, 1
	br		ADJUST

DEC:
	subi	r4, r4, 1

ADJUST:
	movi	r3, 10
	bne		r4, r3, N1 			# check for overflow
	movi	r4, 9
	br		NO_KEY
N1:
	bge		r4, zero, NO_KEY	# check for underflow
	movi	r4, 0
	br		NO_KEY

BLANK:
	movi	r4, -1				# cause a blank display

NO_KEY:
	call	SEG7_CODE			# convert decimal number (r4) to 7-seg code
	stw		r2, (r7)			# display value on HEX display
				
	br		LOOP

/* Subroutine to convert the digits from 0 to 9 to be shown on a HEX display.
 *    Parameters: r4 = the decimal value of the digit to be displayed (a "blank"
 *       digit is represented as -1)
 *    Returns: r2 = bit patterm to be written to the HEX display
 */
SEG7_CODE:
	movia	r3, BIT_CODES		# starting address of the bit codes
	add		r3, r3, r4			# index into the bit codes
	ldb		r2, (r3)			# read the bit code needed for our digit
	ret

	.byte	0b00000000			# blank
BIT_CODES:
	.byte	0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
	.byte	0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
	.skip	2					# align to word boundary

	.end    
