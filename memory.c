#include<stdio.h>

#define MAX_MEMORY_SPACE 1000

int readMem(int address);
void writeMem(int address, int data);
void initialize();

int mem[MAX_MEMORY_SPACE];

void initMemory()
{
	int i;
	FILE *fp;
	fp=fopen("program.txt","r");
	int valueRead=0;
	int addressCounter=0;
	int endFlag;
	while(!feof(fp))
	{
		fscanf(fp,"%d",&valueRead);	
		mem[addressCounter]=valueRead;
		addressCounter++;
	}
	fclose(fp);	
}

int readMem(int address)
{
	return mem[address];
}
void writeMem(int address, int data)
{
	mem[address]=data;
}
