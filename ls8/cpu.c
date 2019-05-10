#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DATA_LEN 6
#define LR 7


// reading whats in ram
unsigned char cpu_ram_read(struct cpu *cpu, unsigned char i) {
  return cpu->ram[i];
}

// write to the ram
void cpu_ram_write(struct cpu *cpu, unsigned char i, unsigned char val)
{
  cpu->ram[i] = val;
}

    void cpu_push(struct cpu *cpu, unsigned char val)
{
  // decrement
  cpu->registers[LR] = cpu->registers[LR] - 1;
  cpu_ram_write(cpu, cpu->registers[LR], val);
}

char cpu_pop(struct cpu *cpu) {
  unsigned char myRam = cpu->ram[cpu->registers[LR]];
  cpu->registers[LR] = cpu->registers[LR] + 1;
  return myRam;
}

// /**
//  * Load the binary bytes from a .ls8 source file into a RAM array
//  */
void cpu_load(struct cpu *cpu, char *filename) {

  FILE *fp;
  char buffer[8192];
  fp = fopen(filename, "r");
  unsigned char address = 0;

  while (fgets(buffer, sizeof buffer, fp) != NULL)
  {
    char *ptr;
    unsigned char string = strtol(buffer, &ptr, 2);
    if (ptr == buffer)
    {
      continue;
    }
    cpu->ram[++address] = string;
  }
  fclose(fp);
}

// /**
//  * ALU
//  */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] *= cpu->registers[regB];
    break;
    }
}

// /**
//  * Run the CPU
//  */
void cpu_run(struct cpu *cpu)
{
  int running = 1;

  // unsigned char command, operand1, operand2;

  while (running) {
   
    unsigned char command = cpu_ram_read(cpu, cpu->PC);

    unsigned int operands = command >> 6;

    unsigned char operand1 = cpu_ram_read(cpu, (cpu->PC + 1) & 0xFF);
    unsigned char operand2 = cpu_ram_read(cpu, (cpu->PC + 2) & 0xFF);


    switch (command)
    {
      
    case LDI:
      cpu->registers[operand1] = operand2;
      // cpu->PC += 3;
      break;
    case PRN:
      printf("%d\n", cpu->registers[operand1]);
      // cpu->PC += 2;
      break;
    case MULT:
      alu(cpu, ALU_MUL, operand1, operand2);
      // cpu->registers[operand1] *= operand2;
      // cpu->PC += 3;
      break;
    // case ADD:
    //   cpu->registers[operand1] += operand2;
    //   cpu->PC += 3;
    //   break;
    case PUSH:
      cpu_push(cpu, cpu->registers[operand1]);
      // cpu->PC += 2;
      break;
    case POP:
      cpu->registers[operand1] = cpu_pop(cpu);
      // cpu->PC += 2;
      break;
    case HLT:
      running = 0;
      // cpu->PC += 1;
      break;
    default:
      // printf("Cannot complete this command\n");
      // exit(1);
      break;
    }
    cpu->PC = cpu->PC + operands + 1;
  }
}

// /**
//  * Initialize a CPU struct
//  */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  // 
//   for (int i = 0; i < 6; i++)
//   {
//     cpu->registers[i] = 0;
//   }
//   cpu->registers[7] = 0xF4;
    cpu->PC = 0;
    memset(cpu->registers, 0, sizeof cpu->registers);

    // RAM is set to 0
    memset(cpu->ram, 0, sizeof(cpu->ram));
    cpu->equal = 0;
}