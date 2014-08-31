Exploring-Multiple-Processes-and-IPC
====================================

The project will simulate a simple computer system consisting of a CPU and Memory.

The CPU and Memory will be simulated by separate processes that communicate.

CPU

 It will have these registers: PC, SP, IR, AC, X, Y.

 It will support the instructions shown on the next page of this document.

 It will run the program at address 0.

 It will run a program by fetching each instruction and any operand it may require and executing it.

 Instructions are fetched into the IR. The operand can be fetched into a local variable.

 Each instruction should be executed before the next instruction is fetched.

 The stack resides at the end of memory and grows down toward the program.

 The program ends when the End instruction is executed. The 2 processes should end at that time.

Memory

 It will consist of 1000 integer entries.

 All memory is both readable and writeable.

 There is no memory protection.

 Memory will initialize itself by reading a file named “program.txt” beginning at address 0.

 Each line in the file will hold one integer which may be an instruction or an operand.

 It will support two operations:

 read(address) returns the value at the address

 write(address, data) writes the data to the address

Instruction set

 1 = Load value 

 2 = Load addr

 3 = Store addr

 4 = AddX

 5 = AddY

 6 = SubX

 7 = SubY

 8 = Get port

 9 = Put port

 10 = CopyToX

 11 = CopyToY

 12 = CopyFromX

 13 = CopyFromY

 14 = Jump addr

 15 = JumpIfEqual addr

 16 = JumpIfNotEqual addr

 17 = Call addr

 18 = Ret 

 19 = IncX 

 20 = DecX 

 30 = End

Load the value into the AC

Load the value at the address into the AC

Store the value in the AC into the address

Add the value in X to the AC

Add the value in Y to the AC

Subtract the value in X from the AC

Subtract the value in Y from the AC

If port=1, reads an int from the keyboard, stores it in the AC

If port=2, reads a char from the keyboard, stores it in the AC

If port=1, writes an int to the screen

If port=2, writes a char to the screen

Copy the value in the AC to X

Copy the value in the AC to Y

Copy the value in X to the AC

Copy the value in Y to the AC

Jump to the address

Jump to the address only if the value in the AC is zero

Jump to the address only if the value in the AC is not zero

Push return address onto stack, jump to the address

Pop return address from the stack, jump to the address

Increment the value in X

Decrement the value in X

End execution


Sample programs

This program gets 3 integers the user types at the keyboard and sums them, then prints the result. 

Note that the program file must contain one number per line, with no comments.

 8

 1

 10

 8

 1

 11

 8

 1

 4

 5

 9

 1

 16 

This program prints HI followed by a newline to the screen. The newline is printed by calling a procedure that prints it.

 1

 72

 9

 2

 1

 73

 9

 2

 17

 11

 30 

 1

 10 

 9

 2

 18