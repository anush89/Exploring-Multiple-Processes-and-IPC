/* I have 2 created files CPU.c and memory.c for this project.
 * memory.c contains the memory array and a routine for its initialization.
 * I have included the memory.c and memory.h file so that when CPU.c is compiled it will automatically compile memory.c.*/

#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>

#include "memory.h"
#include "memory.c"

//Maximum array size is 1000 as per project description
#define MAX_ADDRESS 999

/* Main contains a switch case to run child and parent processes. Memory is the child and the processor is the parent.*/
int main()
{
    // creating pipes, one from parent to child and another from child to parent
    pid_t pid;
    int pipeChildParent[2],pipeParentChild[2];
    if(pipe(pipeChildParent)==-1 || pipe(pipeParentChild)==-1)
    {
            printf("pipe call error");
            exit(1);
    }
    
    // switch case for separating child and parent processes.
    switch (pid=fork())
    {
    	   // Error handling.
           case -1:
	   {
	       printf("fork call error");
               exit(2);
           }

	   // Child process (memory)
           case 0:
           {
	   	// initMemory() reads a program.txt file and fills the read value in the memory array. Code is in memory.c file.
                initMemory();
                char readBuf;
                int temp,stackTemp,temp1;

		// Closing the unused sides of the pipe.
                close(pipeParentChild[1]);
		close(pipeChildParent[0]);

		// Child ends only when parent ends. 
		while(1)
                {  
		    // Read the type of instruction written to pipe by parent.
		    read(pipeParentChild[0], &readBuf,sizeof(char)); 

		    // I-Instruction, O-Operand
		    // read the address and send the value at the address to parent.
		    if(readBuf=='I' || readBuf=='O')
		    {
		        read(pipeParentChild[0],&temp, sizeof(int));
			temp=readMem(temp);
			write(pipeChildParent[1],&temp,sizeof(int));
		    }
		    
		    // L-Load instruction
		    // For loading the value from an address.
		    else if(readBuf=='L')
		    {
		    	read(pipeParentChild[0],&temp,sizeof(int));
			temp=readMem(temp);
			write(pipeChildParent[1],&temp,sizeof(int));
			read(pipeParentChild[0],&temp,sizeof(int));
			temp=readMem(temp);
			write(pipeChildParent[1],&temp,sizeof(int));
		    }

		    // S-Store instruction
		    // Storing the value to an address.
		    else if(readBuf=='S')
		    {
		    	read(pipeParentChild[0],&temp,sizeof(int));
			temp=readMem(temp);
			read(pipeParentChild[0],&temp1,sizeof(int));
			writeMem(temp,temp1);
		    }

		    // Call addr
		    // jumps to the parent specified address by storing the return address in stack.
		    else if(readBuf=='C')
		    {
		    	read(pipeParentChild[0],&temp,sizeof(int));
			read(pipeParentChild[0],&stackTemp,sizeof(int));
			writeMem(stackTemp,temp);
			read(pipeParentChild[0],&temp,sizeof(int));
			temp=readMem(temp);
			write(pipeChildParent[1],&temp,sizeof(int));
		    }

		    // R-Return
		    // Returns to the next instruction of call addr.
		    else if(readBuf=='R')
		    {
		        read(pipeParentChild[0],&stackTemp,sizeof(int));
		        temp=readMem(stackTemp);
			write(pipeChildParent[1],&temp,sizeof(int));
		    }

		    // E-End
		    // Program end reached.
		    else if(readBuf=='E')
		    {
		        break;
		    }
                }
		break;
           }
                
	   // Parent process(processor)
           default:
           {
	   	//Processor variables.
                int PC,SP,IR,AC,X,Y,tempRegister;
		char tempRegister2;
                PC=0;X=0;Y=0;
		SP=MAX_ADDRESS;

		// Close unused pipe ends.
                close(pipeParentChild[0]);
		close(pipeChildParent[1]);

		// Parent ends when it reads instruction 30(end).
		while(1)
		{
		// Fetching instruction from memory by sending PC and type of instruction(I,O,L,S,C,R,E).
                write(pipeParentChild[1],"I",sizeof(char));
                write(pipeParentChild[1],&PC,sizeof(int));
                read(pipeChildParent[0],&IR,sizeof(int));
		
		// Depending on IR different instructions are executed.
		switch(IR)
		{
		    case 1:
		    {
		        PC=PC+1;
			write(pipeParentChild[1],"O",sizeof(char));
		        write(pipeParentChild[1],&PC,sizeof(int));
		        read(pipeChildParent[0], &AC , sizeof(int));
			break;
		    }

		   case 2:
		    {
		    	PC=PC+1;
			write(pipeParentChild[1],"L",sizeof(char));
			write(pipeParentChild[1],&PC,sizeof(int));
			read(pipeChildParent[0],&tempRegister,sizeof(int));
			write(pipeParentChild[1],&tempRegister,sizeof(int));
			read(pipeChildParent[0],&AC,sizeof(int));
			break;
		    }

		   
		    case 3:
		    {
		    	PC=PC+1;
			write(pipeParentChild[1],"S",sizeof(char));
			write(pipeParentChild[1],&PC,sizeof(int));
			write(pipeParentChild[1],&AC,sizeof(int));
			break;
		    }

		    case 4:
		    {
		    	AC=AC+X;
			break;
		    }

		    case 5:
		    {
		    	AC=AC+Y;
			break;
		    }

		    case 6:
		    {
		    	AC=AC-X;
			break;
		    }

		    case 7:
		    {
		    	AC=AC-Y;
			break;
		    }

		    case 8:
		    {
		    	PC=PC+1;
		    	write(pipeParentChild[1],"O",sizeof(char));
			write(pipeParentChild[1],&PC,sizeof(int));
			read(pipeChildParent[0],&tempRegister,sizeof(int));
			if(tempRegister==1)
			{
			    printf("Enter an integer: ");
			    scanf("%d",&tempRegister);
			    AC=tempRegister;
			}
			else if(tempRegister==2)
			{
			    printf("Enter a character: ");
			    tempRegister2=getchar();
			    AC=tempRegister2;
			}
			else
			{ 
			    printf("Invalid operand for instruction get port(8)... terminating");
			    exit(2);
			}
			break;
		    }

		    case 9:
		    {
		    	PC=PC+1;
			write(pipeParentChild[1],"O",sizeof(char));	
			write(pipeParentChild[1],&PC,sizeof(int));
			read(pipeChildParent[0],&tempRegister,sizeof(int));
			if(tempRegister==1)
			{
			    printf("%d\n",AC);
			}
			else if(tempRegister==2)
			{
			    tempRegister2=(char)AC;
			    putchar(tempRegister2);
			    printf("\n");
			}
			else
			{
			    printf("Invalid operand for instruction put port(9)... terminating");
			    exit(2);
			}
			break;
		    }

		    case 10:
		    {
		        X=AC;
			break;
		    }

		    case 11:
		    {
		    	Y=AC;
			break;
		    }

		    case 12:
		    {
		    	AC=X;
			break;
		    }

		    case 13:
		    {
		        AC=Y;
			break;
		    }

		    case 14:
		    {
		        PC=PC+1;
			write(pipeParentChild[1],"O",sizeof(char));
			write(pipeParentChild[1],&PC,sizeof(int));
			read(pipeChildParent[0],&tempRegister,sizeof(int));
			PC=tempRegister-1;
		    	break;
		    }

		    case 15:
		    {
		        if(AC==0)
			{
			    PC=PC+1;
			    write(pipeParentChild[1],"O",sizeof(char));
			    write(pipeParentChild[1],&PC,sizeof(int));
			    read(pipeChildParent[0],&tempRegister,sizeof(int));
			    PC=tempRegister-1;
			}
			else
			{
			    PC=PC+1; //skip the operand
			}
			break;
		    }

		    case 16:
		    {
		        if(AC!=0)
			{
			    PC=PC+1;
			    write(pipeParentChild[1],"O",sizeof(char));
			    write(pipeParentChild[1],&PC,sizeof(int));
			    read(pipeChildParent[0],&tempRegister,sizeof(int));
			    PC=tempRegister-1;
			}
			else
			{
			    PC=PC+1;	//skip the operand
			}
			break;
		    }

		    case 17:
		    {
		    	write(pipeParentChild[1],"C",sizeof(char));
			tempRegister=PC+2;
			write(pipeParentChild[1],&tempRegister,sizeof(int));
			write(pipeParentChild[1],&SP,sizeof(int));
			SP=SP-1;
		    	PC=PC+1;	
			write(pipeParentChild[1],&PC,sizeof(int));
			read(pipeChildParent[0],&tempRegister,sizeof(int));
			PC=tempRegister-1;
		    	break;
		    }

		    case 18:
		    {
		    	if(SP!=MAX_ADDRESS)
			{
		    	    write(pipeParentChild[1],"R",sizeof(char));
			    SP=SP+1;
			    write(pipeParentChild[1],&SP,sizeof(int));
			    read(pipeChildParent[0],&tempRegister,sizeof(int));
			    PC=tempRegister-1;
			}
			else
			{
			    printf("Nothing in stack to retrieve... Terminating\n");
			    exit(2);
			}
		        break;
		    }

		    case 19:
		    {
		    	X=X+1;
			break;
		    }

		    case 20:
		    {
		    	X=X-1;
			break;
		    }

		    case 30:
		    {
		    	printf("\nProgram end reached... exiting\n");
		        write(pipeParentChild[1],"E",sizeof(char));
			break;
		    }
		    
		    // Not in the instruction set.
		    default:
		    {
		    	printf("\nInvalid instruction code %d... exiting",PC);
			exit(1);
		    }
		}

		if(IR==30)
		{
		    break;
	        }
		else
		{
		    PC=PC+1;
		}

		}// end switch
           }// end while
    }// end parent process
}
