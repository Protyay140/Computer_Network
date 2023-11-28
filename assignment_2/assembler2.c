#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

typedef struct
{
    char label[32];
    int address;
} Symbol;

typedef struct
{
    char instruction[MAX_LINE_LENGTH];
    int address;
} MachineCode;

Symbol symbolTable[MAX_LINES];
MachineCode machineCode[MAX_LINES];
int symbolTableSize = 0;
int machineCodeSize = 0;

void firstPass(FILE *file)
{
    char line[MAX_LINE_LENGTH];
    char *token;

    // First pass: Collect information about labels and addresses
    int address = 0;
    while (fgets(line, sizeof(line), file))
    {
        token = strtok(line, " \t\n");
        if (token && token[strlen(token) - 1] == ':')
        {
            // Label found
            token[strlen(token) - 1] = '\0'; // Remove the colon
            strcpy(symbolTable[symbolTableSize].label, token);
            symbolTable[symbolTableSize].address = address;
            symbolTableSize++;
        }
        address++;
    }
}

void secondPass(FILE *file)
{
    char line[MAX_LINE_LENGTH];
    char *token;

    // Second pass: Generate machine code
    int address = 0;
    while (fgets(line, sizeof(line), file))
    {
        token = strtok(line, " \t\n");
        if (token && token[strlen(token) - 1] == ':')
        {
            // Label found, skip
        }
        else if (token)
        {
            strcpy(machineCode[machineCodeSize].instruction, line);
            machineCode[machineCodeSize].address = address;
            machineCodeSize++;
        }
        address++;
    }
}

int main()
{
    FILE *inputFile = fopen("./progcap.asm", "r");
    if (!inputFile)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Two-pass assembler
    firstPass(inputFile);
    rewind(inputFile);
    secondPass(inputFile);

    printf("Symbol Table:\n");
    for (int i = 0; i < symbolTableSize; i++)
    {
        printf("%s: %04X\n", symbolTable[i].label, symbolTable[i].address);
    }

    // Display machine code
    printf("Generated Machine Code:\n");
    for (int i = 0; i < machineCodeSize; i++)
    {
        printf("%04X: %s ", machineCode[i].address, machineCode[i].instruction);
    }

    fclose(inputFile);
    return 0;
}
