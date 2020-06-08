// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(KBD0)
	@SCREEN
	D = A
	@addr
	M = D
	
	@8191
	D = A
	@n
	M = D
	
	@i
	M = 0
	
	(LOOP0)
		@i
		D = M
		@n
		D = D - M
		@CHECK_STATE
		D;JGT
		
		@addr
		A = M
		M = 0
		
		@i
		M = M + 1
		@addr
		M = M + 1
		@LOOP0
		0;JMP

(KBD1)
	@SCREEN
	D = A
	@addr
	M = D
	
	@8191
	D = A
	@n
	M = D
	
	@i
	M = 0
	
	(LOOP1)
		@i
		D = M
		@n
		D = D - M
		@CHECK_STATE
		D;JGT
		
		@addr
		A = M
		M = -1
		
		@i
		M = M + 1
		@addr
		M = M + 1
		@LOOP1
		0;JMP

(CHECK_STATE)		
	@KBD
	D = M
	@KBD0
	D;JEQ
	@KBD1
	D;JNE

		
(END)
	@END
	0;JMP