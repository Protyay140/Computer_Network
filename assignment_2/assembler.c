#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100

int labelcnt=0;

typedef struct {
    char label[MAX_LINE_LENGTH];
    int address;
} Label;

typedef struct {
    char instruction[MAX_LINE_LENGTH];
    char operand1[MAX_LINE_LENGTH];
    char operand2[MAX_LINE_LENGTH];
} Command;

typedef struct{
    int  AX,BX,CX,DX,AH,AL,BH,BL,CH,CL,DH, DL, DS, CS, SS;
    int zf;
} Register;

void parseLine(char line[], Command *command) {
    // command->operand1 = "\0";
    // command->operand1 = '';
    sscanf(line, "%s %s %s", command->instruction, command->operand1, command->operand2);
}

int ascii_to_integer(char ascii){
    return (int)(ascii) - '0';
}


void write_registers(Register *reg,int flag){
    if(!flag) return;
    FILE* fp;
    fp = fopen("Register.txt" , "a");
    fprintf(fp,"<-------------------------------------------->\n");
    fprintf(fp,"AH->%d , AL-> %d , AX->%d\n",reg->AH,reg->AL,reg->AX);
    fprintf(fp,"BH->%d , BL-> %d , BX->%d\n",reg->BH,reg->BL,reg->BX);
    fprintf(fp,"CH->%d , CL-> %d , CX->%d\n",reg->CH,reg->CL,reg->CX);
    fprintf(fp,"DH->%d , DL-> %d , DX->%d\n",reg->DH,reg->DL,reg->DX);
    fprintf(fp,"<-------------------------------------------->\n");
    fclose(fp);
}

void print(Register *registers){
    printf("AX: %d\n",registers->AX);
    printf("BX: %d\n",registers->BX);
    printf("CX: %d\n",registers->CX);
    printf("DX: %d\n",registers->DX);
    printf("AH: %d\n",registers->AH);
    printf("AL: %d\n",registers->AL);
    printf("BH: %d\n",registers->BH);
    printf("BL: %d\n",registers->BL);
    printf("CH: %d\n",registers->CH);
    printf("CL: %d\n",registers->CL);
    printf("DH: %d\n",registers->DH);
    printf("DL: %d\n",registers->DL);
    printf("<--------------------------------->");
}

void printLabels(Label *labels){
    for(int i=0;i<labelcnt;i++){
        printf("Label: %s\t",labels[i].label);
        printf("Address: %d\n",labels[i].address);
    }
}


void setValueRegister(Register *registers, char *operand1, char *operand2) {
    int value = atoi(operand2);
    if (strcmp(operand1, "AH") == 0) {
        registers->AH = value;
    } else if (strcmp(operand1, "AL") == 0) {
        registers->AL = value;
    } else if (strcmp(operand1, "BH") == 0) {
        registers->BH = value;
    } else if (strcmp(operand1, "BL") == 0) {
        registers->BL = value;
    } else if (strcmp(operand1, "CH") == 0) {
        registers->CH = value;
    } else if (strcmp(operand1, "CL") == 0) {
        registers->CL = value;
    } else if (strcmp(operand1, "DH") == 0) {
        registers->DH = value;
    } else if (strcmp(operand1, "DL") == 0) {
        registers->DL = value;
    } else if (strcmp(operand1, "CX") == 0) {
        registers->CX = value;
    }
}

void executeCommand(FILE *file,Command *command, Register *registers, Label *labels) {
    // printf("%s %s %s\n",command->instruction,command->operand1,command->operand2);
    // print(registers);
    // printLabels(labels);
    int flag = 1;
    if (strcmp(command->instruction, "MOV") == 0) {
        registers->zf = 0;
        if (isdigit(command->operand2[0]) || (command->operand2[0] == '-' && isdigit(command->operand2[1]))) {
            setValueRegister(registers, command->operand1, command->operand2);
        } else {
            int labelIndex = atoi(command->operand2 + 1);
            setValueRegister(registers, command->operand1, labels[labelIndex].label);
        }
    } else if (strcmp(command->instruction, "ADD") == 0) {
    // Assuming registers is a struct with members AH, AL, BH, BL, CH, CL, DH, DL
    registers->zf = 0;
    switch (command->operand1[0]) {
        case 'A': registers->AH += registers->AL; break;
        case 'B': registers->BH += registers->BL; break;
        case 'C': registers->CH += registers->CL; break;
        case 'D': registers->DH += registers->DL; break;
        default:
            printf("Unknown register: %s\n", command->operand1);
    }
} 
else if (strcmp(command->instruction, "SUB") == 0) {
    switch (command->operand1[0]) {
        case 'A': {registers->AH -= registers->AL; if(registers->AH==0)registers->zf = 1; break;}
        case 'B': {registers->BH -= registers->BL; if(registers->BH==0)registers->zf = 1;break;}
        case 'C': {registers->CH -= registers->CL; if(registers->CH==0)registers->zf = 1;break;}
        case 'D': {registers->DH -= registers->DL; if(registers->DH==0)registers->zf = 1;break;}
        default:
            printf("Unknown register: %s\n", command->operand1);
    }
} 
else if (strcmp(command->instruction, "INC") == 0) {
     registers->zf = 0;
    switch (command->operand1[0]) {
        case 'A': registers->AL++; break;
        case 'B': registers->BL++; break;
        case 'C': registers->CL++; break;
        case 'D': registers->DL++; break;
        default:
            printf("Unknown register: %s\n", command->operand1);
    }
} 
else if (strcmp(command->instruction, "DEC") == 0) {
    switch (command->operand1[0]) {
        case 'A': {registers->AX --; if(registers->AX==0)registers->zf = 1; break;}
        case 'B': {registers->BX --; if(registers->BX==0)registers->zf = 1;break;}
        case 'C': {registers->CX --; if(registers->CX==0)registers->zf = 1;break;}
        case 'D': {registers->DX --; if(registers->DX==0)registers->zf = 1;break;}
        default:
            printf("Unknown register: %s\n", command->operand1);
    }
} 
else if (strcmp(command->instruction, "INT") == 0) {
    if (strcmp(command->operand1, "21H") == 0 && registers->AH==2) {       
        printf("Output: %d\n", ascii_to_integer(registers->DL));
        
    }
} else if (strcmp(command->instruction, "JNZ") == 0) {
//    int regIndex = command->operand1[1] - '0';
    // printf("abc");
    if(registers->zf==0)
        
    {
        int labelIndex = atoi(command->operand1 + 1);

        // Assuming registers is a struct with members AH, AL, BH, BL, CH, CL, DH, DL
        int registerValue = 0;

        // switch (regIndex) {
        //     case 0: registerValue = registers->AH; break;
        //     case 1: registerValue = registers->AL; break;
        //     case 2: registerValue = registers->BH; break;
        //     case 3: registerValue = registers->BL; break;
        //     case 4: registerValue = registers->CH; break;
        //     case 5: registerValue = registers->CL; break;
        //     case 6: registerValue = registers->DH; break;
        //     case 7: registerValue = registers->DL; break;
        //     default:
        //         printf("Unknown register index: %d\n", regIndex);
        // }

        // if (registerValue != 0) {
            printf("Jumping to label %s because %s is not zero\n", labels[labelIndex].label, command->operand1);

            // Set the file pointer to the line after the specified label
            fseek(file, labels[labelIndex].address, SEEK_SET);
            // printf("Output: %d\n", registers->DH);
    }

        // Note: Depending on your program structure, you might need to handle the first pass again.
        // If the labels are required in the second pass, you may need to adjust the firstPass and labels structures accordingly.
    // }
    // if (registers->registers[regIndex] != 0) {
    //     printf("Jumping to label %s because %s is not zero\n", labels[labelIndex].label, command->operand1);
    //     // You may need additional logic to handle the jump to the specified label
    // }
}
else if (strcmp(command->instruction, "LEA") == 0) {
    int regIndex = atoi(command->operand2 + 2);
    int regIndex2 = command->operand1[1] - '0';
    // Assuming registers is a struct with members AH, AL, BH, BL, CH, CL, DH, DL
    switch (regIndex) {
        case 0: registers->AH = registers->AL; break;
        case 1: registers->BH = registers->BL; break;
        case 2: registers->CH = registers->CL; break;
        case 3: registers->DH = registers->DL; break;
        default:
            printf("Unknown register index: %d\n", regIndex);
    }
    printf("Performing LEA %s, %s\n", command->operand1, command->operand2);
}
else if (strcmp(command->instruction, "PROMPT") == 0) {
    int regIndex = command->operand1[1] - '0';
    // printf("PROMPT command received. Value in register %c: %d\n", command->operand1[0], registers->registers[regIndex]);
    // You may want to implement logic to handle the PROMPT command
} else {
        printf("Unknown instruction: %s\n", command->instruction);
        flag = 0;
    }

    write_registers(registers,flag);
}

void firstPass(FILE *file, Label *labels) {
    char line[MAX_LINE_LENGTH];
    int address = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == ';') {
            continue; // Skip comments
        }

        Command command;
        parseLine(line, &command);

        if (command.instruction[strlen(command.instruction) - 1] == ':') {   
            command.instruction[strlen(command.instruction) - 1] = '\0'; // Remove the colon
            printf("%s\n",command.instruction);
            strcpy(labels[labelcnt].label, command.instruction);
            labels[labelcnt].address = ftell(file);
            labelcnt++;
             FILE *fp;
             fp = fopen("SymbolTable.txt","a");
        fprintf(fp,"Label-> %s\t address->%d\n",labels[labelcnt-1].label,labels[labelcnt-1].address);
    fclose(fp);
        }

        address++;
    }
    // printLabels(labels);
}

void secondPass(FILE *file, Label *labels, Register *registers) {
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == ';') {
            continue; // Skip comments
        }

        Command command;
        parseLine(line, &command);

        if (command.instruction[strlen(command.instruction) - 1] == ':') {
            continue; // Skip labels during the second pass
        }

        executeCommand(file, &command, registers, labels);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.asm>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Label labels[MAX_LINE_LENGTH];
    Register registers = {0}; // Assuming there are 10 registers R0, R1, ..., R9

    // First pass to collect labels and addresses
    firstPass(file, labels);

    // Reset file to the beginning for the second pass
    fseek(file, 0, SEEK_SET);

    // Second pass to generate machine code
    secondPass(file, labels, &registers);

    // write_symbol_table(labels);
    fclose(file);

    return 0;
}
