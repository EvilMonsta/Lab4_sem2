#include<string.h>
#include <malloc.h>
#include <stdbool.h>
#include <ctype.h>
#include "structures.h"
#include <stdio.h>
#include <limits.h>

float calculateDistance(float hospAddressX,float hospAddressY,float patAddressX,float patAddressY){
    float distance;
    distance = sqrt((hospAddressX-patAddressX)*(hospAddressX-patAddressX)+(hospAddressY-patAddressY)*(hospAddressY-patAddressY));
    return distance;
}

int compareTwoWords(char* word1, char* word2){
    if(strlen(word1)!= strlen(word2)){
        return 0;
    }
    else for(int i = 0; i < strlen(word1); i ++){
            if(word1[i]!=word2[i]){
                return 0;
            }
        }
    return 1;
}

int checkInt() {
    int number;
    while (scanf("%d", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

float checkFloat() {
    float number;
    while (scanf("%f", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

int ContinueWish(int tasknumber){
    int wish = tasknumber;
    if(tasknumber==0){
        return wish;
    }
    printf("\nWant to continue work with program?\n1 - Yes\n0 - No\nYour choice: ");
    scanf("%d",&wish);
    if(wish != 0 && wish != 1){
        printf("ERROR");
        return 0;
    }
    return wish;
}

int TaskNumCheck(int tasknum){
    if(tasknum != 1 && tasknum != 2){
        printf ("ERROR");
        return 0;
    }
    return tasknum;
}

void pushOperand(OperandNode **top, float value) {
    OperandNode *newNode = (OperandNode*)malloc(sizeof(OperandNode));
    newNode->data = value;
    newNode->next = *top;
    *top = newNode;
}

void WordCancellation(char*string, int length){
    for(int i = 0; i < length;i++ ){
        string[i]='\0';
    }
}

float popOperand(OperandNode **top) {
    if (*top == NULL)
        return 0;
    OperandNode *temp = *top;
    float value = temp->data;
    *top = (*top)->next;
    free(temp);
    return value;
}

void pushOperator(OperatorNode **top, char value) {
    OperatorNode *newNode = (OperatorNode*)malloc(sizeof(OperatorNode));
    newNode->data = value;
    newNode->next = *top;
    *top = newNode;
}

char popOperator(OperatorNode **top) {
    OperatorNode *temp = *top;
    char value = temp->data;
    *top = (*top)->next;
    free(temp);
    return value;
}

char peekOperator(OperatorNode **top) {
    return (*top)->data;
}

int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return -1;
    }
}

float ApplyOperator(char op, float operand1, float operand2) {
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            return operand1 / operand2;
        default:
            printf("Invalid operator: %c", op);
            return -1;
    }
}

bool isValidChar(char c) {
    return isdigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == ' ';
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool hasHigherPriority(char op1, char op2) {
    int priority1 = getPriority(op1);
    int priority2 = getPriority(op2);
    return priority1 >= priority2;
}

int firstCheck(char* expression, int length, int *errorPosition){
    int parenthesesCount = 0;
    if(isOperator(expression[length-1])){
        *errorPosition = length-1;
        return parenthesesCount;
    }
    for (int i = 0; i < length; i++) {
        if (!isValidChar(expression[i])) {
            *errorPosition = i;
            break;
        }
        if(isOperator(expression[i]) && isOperator(expression[i+1])){
            *errorPosition = i;
            break;
        }
        if (expression[i] == '(') {
            parenthesesCount++;
        } else if (expression[i] == ')') {
            parenthesesCount--;
            if (parenthesesCount < 0) {
                *errorPosition = i;
                break;
            }
        }
    }
    return parenthesesCount;
}

int checkInfixExpression(char *expression) {
    int length = strlen(expression)-1, errorPosition = -1, parenthesesCount = firstCheck(expression,length,&errorPosition),openParentheses = 0;

    if (parenthesesCount != 0 && errorPosition==-1) {
        for (int i = 0; i < length; i++) {
            if (expression[i] == ')') {
                openParentheses++;
            } else if (expression[i] == '(') {
                openParentheses--;
                if (openParentheses < 0) {
                    errorPosition = i;
                    break;
                }
            }
        }
    }

    if (errorPosition != -1) {
        printf("ERROR! Incorrect symbol on position %d: '%c'\n", errorPosition, expression[errorPosition]);
        return 0;
    } else {
        return 1;
    }
}

void calculateAction(OperatorNode** operators,OperandNode** operands){
    float operand2 = popOperand(operands);
    float operand1 = popOperand(operands);
    char op = popOperator(operators);
    float result = ApplyOperator(op, operand1, operand2);
    pushOperand(operands,result);
}

void actionOne(int* i, int* j, char* operandString,char* expression){
    operandString[*j] = expression[*i+1];
    *i+=1;
    *j+=1;
}


float evaluateInfixExpression(char* expression){
    OperandNode *operands = NULL;
    OperatorNode *operators = NULL;
    char operandString[20];
    float operand;
    for(int i = 0, j = 1; i < strlen(expression);i++){
        if(isdigit(expression[i]) && expression[i] != ' '){ operandString[0] = expression[i];
            while(i+1< strlen(expression) && isdigit(expression[i + 1])) actionOne(&i,&j,operandString,expression);
            operandString[j] = '\0';
            j=1;
            operand = atof(operandString);
            pushOperand(&operands,operand);
        } else if(expression[i] == '('){
            if(expression[i+1] == '-') pushOperand(&operands,0);
            pushOperator(&operators,expression[i]);
        } else if (expression[i] == ')') {
            while (peekOperator(&operators) != '(') calculateAction(&operators,&operands);
            popOperator(&operators);
        } else if (isOperator(expression[i])) {
            while (operators != NULL && peekOperator(&operators) != '(' && hasHigherPriority(peekOperator(&operators), expression[i])) calculateAction(&operators,&operands);
            pushOperator(&operators,expression[i]);
        }
    }
    while (operators != NULL) calculateAction(&operators,&operands);
    return popOperand(&operands);
}

void chooseFunctionTaskOne(){
    int currentChoice = 1;
    while(currentChoice){
        printf("Enter 1 to use test input\nEnter 2 to use your input\nEnter 0 to exit the task\nYour choice:");
        currentChoice = checkInt();
        if(currentChoice == 1){
            char test[] = "-(-(-(-3 + 4)) / (2+2) * 3)    *5";
            printf("Test is: %s\n",test);
            printf("Result: %.2f\n",evaluateInfixExpression(test));
        }
        else if(currentChoice == 2){
            char expression[100];
            printf("Enter infix:");
            fgets(expression, 100, stdin);
            if(checkInfixExpression(expression)) printf("Result: %.2f\n",evaluateInfixExpression(expression));
        }
        else currentChoice = 0;
    }
}

int findStringNumber(FILE *file, char* buffer){
    int stringCounter = 0;
    while((fgets(buffer, 100, file))!=NULL){
        stringCounter++;
    }
    rewind(file);
    return stringCounter;
}

void printData(Hospitals *hosp) {
    int i = 1;
    float distance;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        printf("\n-> %s\n",curHospital->name);
        printf("All places: %d \nFree places: %d \nAddress coordinates: %.1f %.1f\n\n", curHospital->allPlaces, curHospital->freePlaces, curHospital->addressX, curHospital->addressY);
        if(curHospital->allPlaces-curHospital->freePlaces>0) {
            for (PatientNode *curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next, i++) {
                printf("%d.%s \nAddress coordinates: %.1f %.1f\n", i, curPatient->name, curPatient->addressX,curPatient->addressY);
            }
        }
        i = 1;
    }
}

PatientNode* makeConnectionOne(HospitalNode *newHospital,PatientNode *patientTemp,PatientNode*newPatient){
    newPatient->next=NULL;
    newPatient->prev=NULL;
    patientTemp = newPatient;
    newHospital->head = patientTemp;
    return patientTemp;
}

PatientNode* makeConnectionTwo(PatientNode *patientTemp,PatientNode*newPatient){
    newPatient->next=NULL;
    newPatient->prev = patientTemp;
    patientTemp->next = newPatient;
    patientTemp = newPatient;
    return patientTemp;

}

void makeConnectionThree(Hospitals* hosp, HospitalNode*newHospital){
    newHospital->next = NULL;
    newHospital->prev = NULL;
    hosp->head = newHospital;
    hosp->tail = newHospital;
}

void makeConnectionFour(Hospitals* hosp, HospitalNode*newHospital){
    newHospital->next = NULL;
    newHospital->prev = hosp->tail;
    hosp->tail->next = newHospital;
    hosp->tail = newHospital;
}

void writeHospitalData(HospitalNode*newHospital, char*currentString, char* currentWord){
    int flag=0, IndefWordLength=0;
    for(int k = 0; k < strlen(currentString);k++) {
        if (currentString[k] == ' ' || currentString[k] == '\0' || currentString[k] == '\n') {
            if (flag == 0) newHospital->allPlaces = atoi(currentWord);
            if (flag == 1) newHospital->freePlaces = atoi(currentWord);
            if (flag == 2) newHospital->addressX = strtof(currentWord, NULL);
            if (flag == 3) newHospital->addressY = strtof(currentWord, NULL);
            flag++;
            WordCancellation(currentWord, IndefWordLength);
            IndefWordLength = 0;
        } else {
            IndefWordLength++;
            currentWord[IndefWordLength - 1] = currentString[k];
        }
    }
}

int commandOne(PatientNode *newPatient, int IndefWordLength, char*currentWord){
    strcpy(newPatient->name, currentWord);
    WordCancellation(currentWord, IndefWordLength);
    IndefWordLength = 0;
    return IndefWordLength;
}

int commandTwo(PatientNode *newPatient, int IndefWordLength, char*currentWord) {
    newPatient->addressX = strtof(currentWord, NULL);
    WordCancellation(currentWord, IndefWordLength);
    IndefWordLength = 0;
    return IndefWordLength;
}
int commandThree(PatientNode *newPatient,int currentPeople, int *IndefWordLength, char*currentWord){
    currentPeople++;
    newPatient->addressY = strtof(currentWord, NULL);
    WordCancellation(currentWord, *IndefWordLength);
    *IndefWordLength = 0;
    return currentPeople;
}
void writeHospital(HospitalNode *newHospital,FILE* file, char*currentString, char*currentWord) {
    fgets(currentString, 200, file);
    strcpy(newHospital->name, currentString);
    fgets(currentString, 200, file);
    writeHospitalData(newHospital, currentString, currentWord);
}

int writePatients(HospitalNode*newHospital,PatientNode*patientTemp,int currentPeople,char*currentString,char*currentWord){
    int k = -1,flag = 0,IndefWordLength = 0;
    if(newHospital->allPlaces-newHospital->freePlaces==0) newHospital->head=NULL;
    for(int j = 0; j < newHospital->allPlaces-newHospital->freePlaces; j ++){
        PatientNode *newPatient = malloc(sizeof(PatientNode));
        for(k=k+1; k < strlen(currentString);k++){
            if (currentString[k] == ' ' || currentString[k] == '\0' || currentString[k] == '\n') {
                flag++;
                if (flag == 1 || flag == 2) {
                    IndefWordLength++;
                    currentWord[IndefWordLength - 1] = currentString[k];
                }
                if (flag == 3) IndefWordLength = commandOne(newPatient,IndefWordLength,currentWord);
                if (flag == 4) IndefWordLength = commandTwo(newPatient,IndefWordLength,currentWord);
                if (flag == 5){
                    currentPeople = commandThree(newPatient,currentPeople,&IndefWordLength,currentWord);
                    if(currentPeople==1) patientTemp = makeConnectionOne(newHospital,patientTemp,newPatient);
                    else patientTemp = makeConnectionTwo(patientTemp,newPatient);
                    break;
                }
            } else {
                IndefWordLength++;
                currentWord[IndefWordLength - 1] = currentString[k];
            }
        }
        flag = 0;
    }
    return currentPeople;
}

Hospitals* writeDataFromFile(){
    char currentWord[100],buffer[100], currentString[200];
    FILE *file = fopen("hospitals_data.txt","rb");
    int stringLength =findStringNumber(file,buffer),currentPeople = 0;
    Hospitals * hosp = malloc(sizeof(Hospitals));
    hosp->hospitalsCount=stringLength/3;
    hosp->head=NULL;
    hosp->tail=NULL;
    PatientNode *patientTemp;
    for(int i = 0; i < hosp->hospitalsCount;i++){
        HospitalNode *newHospital = malloc(sizeof(HospitalNode));
        writeHospital(newHospital,file,currentString,currentWord);
        fgets(currentString,200,file);
        currentPeople = writePatients(newHospital,patientTemp,currentPeople,currentString,currentWord);
        if(i == 0) makeConnectionThree(hosp,newHospital);
        else makeConnectionFour(hosp,newHospital);
        currentPeople=0;
    }
    fclose(file);
    return hosp;
}

void writeDataToFile(Hospitals *hosp){
    FILE *file = fopen("hospitals_data.txt","wb");
        for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
            fprintf(file,"%s",curHospital->name);
            fprintf(file,"%d %d %.1f %.1f", curHospital->allPlaces, curHospital->freePlaces, curHospital->addressX, curHospital->addressY);
            fprintf(file,"%c",'\n');
            if(curHospital->allPlaces-curHospital->freePlaces>0){
                for(PatientNode *curPatient = curHospital->head; curPatient!=NULL;curPatient = curPatient->next){
                    fprintf(file,"%s %.1f %.1f ",curPatient->name, curPatient->addressX, curPatient->addressY);
                }
            }
            fprintf(file,"%c",'\n');
        }
    fclose(file);
}

int enterNewHospital(HospitalNode *newHospital){
    int size = 0,choice=0;
    char buf,buffer[100];
    while(1){
        printf("1 - Add to head\n2 - Add to tail\n");
        choice=checkInt();
        if(choice == 1 || choice == 2) break;
    }
    printf("Enter hospital name:\n");
    fflush(stdin);
    while((buf = getchar())!='\n'){
        size++;
        buffer[size - 1] = buf;
    }
    buffer[size] = '\n';
    strcpy(newHospital->name,buffer);
    printf("Enter number of all places in hospital:\n");
    newHospital->allPlaces = checkInt();
    printf("Enter number of free places in hospital:\n");
    newHospital->freePlaces = checkInt();
    printf("Enter hospital address(X):\n");
    newHospital->addressX = checkFloat();
    printf("Enter hospital address(Y):\n");
    newHospital->addressY = checkFloat();
    return choice;
}

void enterNewPatient(PatientNode *newPatient){
    char name[100],surname[100],lastname[100],SNL[100];
    printf("Enter patient surname, name and lastname:\n");
    scanf("%s%s%s",surname,name,lastname);
    snprintf(SNL, sizeof SNL, "%s%c%s%c%s", surname,' ', name,' ',lastname);
    strcpy(newPatient->name,SNL);
    printf("Enter patient address(X):\n");
    newPatient->addressX = checkFloat();
    printf("Enter patient address(Y):\n");
    newPatient->addressY = checkFloat();
}

void addNewHospital(Hospitals *hosp) {
    HospitalNode *newHospital = malloc(sizeof(HospitalNode));
    PatientNode *patientTemp;
    int choice = enterNewHospital(newHospital), currentPeople = 0;
    for (int i = 0; i < newHospital->allPlaces - newHospital->freePlaces; i++) {
        PatientNode *newPatient = malloc(sizeof(PatientNode));
        enterNewPatient(newPatient);
        if (currentPeople == 0) patientTemp = makeConnectionOne(newHospital, patientTemp, newPatient);
        else patientTemp = makeConnectionTwo(patientTemp, newPatient);
        currentPeople++;
    }
    if (choice == 1) {
        newHospital->next = hosp->head;
        hosp->head->prev = newHospital;
        newHospital->prev = NULL;
        hosp->head = newHospital;
    } else if (choice == 2) {
        newHospital->next = NULL;
        newHospital->prev = hosp->tail;
        hosp->tail->next = newHospital;
        hosp->tail = newHospital;
    }
}

int printListOfPatients(Hospitals*hosp){
    int i = 1;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        if(curHospital->allPlaces-curHospital->freePlaces>0) {
            for (PatientNode *curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next, i++) {
                printf("%d.%s\n", i, curPatient->name);
            }
        }
    }
    return i;
}

void findAndRemove(Hospitals*hosp,int choice){
    int i = 1;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        if(curHospital->allPlaces-curHospital->freePlaces>0) {
            bool cycleBreak = false;
            for (PatientNode *curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next, i++) {
                if(i == choice){
                    curHospital->freePlaces+=1;
                    if(curPatient->next == NULL){
                        if(curPatient == curHospital->head) curHospital->head = NULL;
                        else curPatient->prev->next = NULL;
                        free(curPatient);
                    } else if (curPatient->prev == NULL){
                        curHospital->head = curPatient->next;
                        curPatient->next->prev=NULL;
                        free(curPatient);
                    } else {
                        curPatient->next->prev = curPatient->prev;
                        curPatient->prev->next = curPatient->next;
                        free(curPatient);
                    }
                    cycleBreak=true;
                    break;
                }
            }
            if(cycleBreak == true) break;
        }
    }
}

void removePatient(Hospitals*hosp){
    int i = printListOfPatients(hosp),choice;
    printf("Select patient to delete:");
    choice = checkInt();
    if(choice> 0 && choice <= i){
        findAndRemove(hosp,choice);
    }
    else printf("Patient not found!");
}

int findNearestHospital(Hospitals *hosp, int *i, PatientNode *newPatient) {
    float minDistance = (float) INT_MAX, distance;
    int chosenHospitalIndex;
    for (HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next) {
        if (curHospital->freePlaces > 0) {
            *i += 1;
            curHospital->distanceFunc = calculateDistance;
            distance = curHospital->distanceFunc(curHospital->addressX, curHospital->addressY, newPatient->addressX, newPatient->addressY);
            if (distance < minDistance) {
                minDistance = distance;
                chosenHospitalIndex = *i;
            }
        }
    }
    return chosenHospitalIndex;
}

void noSpace(PatientNode*newPatient){
    printf("No space for patient found!");
    free(newPatient);
}

void makeConnections(Hospitals*hosp, int chosenHospitalIndex, PatientNode*curPatient, PatientNode*newPatient){
    int i = 0;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        if(curHospital->freePlaces>0) {
            i++;
            if(i == chosenHospitalIndex){
                if(curHospital->head!=NULL){
                    for (curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next) {
                        if(curPatient->next == NULL){
                            curPatient->next = newPatient;
                            curPatient->next->next = NULL;
                            newPatient->prev = curPatient;
                            break;
                        }
                    }
                }
                else {
                    curHospital->head = newPatient;
                    curHospital->head->next=NULL;
                    curHospital->head->prev = NULL;
                }
                curHospital->freePlaces-=1;
                break;
            }
        }
    }
}

void putPatientIntoHospital(Hospitals*hosp){
    PatientNode *newPatient = malloc(sizeof(PatientNode)),*curPatient;
    enterNewPatient(newPatient);
    int i = 0, chosenHospitalIndex = findNearestHospital(hosp,&i,newPatient);
    if(i == 0) noSpace(newPatient);
    else {
        makeConnections(hosp,chosenHospitalIndex,curPatient,newPatient);
    }
}

void findHospital(Hospitals*hosp){
    char SNL[100],name[100],surname[100],lastname[100];
    float addressX, addressY;
    int flag = 0;
    printf("Enter patient surname, name and lastname:\n");
    scanf("%s%s%s",surname,name,lastname);
    snprintf(SNL, sizeof SNL, "%s%c%s%c%s", surname,' ', name,' ',lastname);
    printf("Enter address(X):\n");
    scanf("%f",&addressX);
    printf("Enter address(Y):\n");
    scanf("%f",&addressY);
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        if(curHospital->allPlaces-curHospital->freePlaces>0) {
            for (PatientNode *curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next) {
                if(curPatient->addressX == addressX && curPatient->addressY == addressY){
                    if(compareTwoWords(SNL, curPatient->name)) {
                        printf("Patient now located in %s",curHospital->name);
                        flag=1;
                    }
                }
            }
        }
    }
    if(flag == 0) printf("Patient not found!");
}

int printHospitalsAndChoose(Hospitals*hosp){
    int i = 1,choice;
    for (HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next,i++) {
        printf("%d.%s",i,curHospital->name);
    }
    printf("Choose hospital to delete:\n");
    choice = checkInt();
    return choice;
}

void freePatients(HospitalNode*curHospital, PatientNode*patientTemp){
    for (PatientNode *curPatient = curHospital->head; curPatient != NULL; curPatient = curPatient->next) {
        patientTemp = curPatient->next;
        free(curPatient);
        curHospital->head = patientTemp;
    }
}

void deleteHospital(Hospitals*hosp) {
    PatientNode *patientTemp;
    int i = 1,choice = printHospitalsAndChoose(hosp);
    i = 1;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next,i++){
        if(i == choice){
            freePatients(curHospital,patientTemp);
            if(curHospital->prev!=NULL){
                if(curHospital->next == NULL) curHospital->prev->next = NULL;
                else{
                    curHospital->prev->next = curHospital->next;
                    curHospital->next->prev = curHospital->prev;
                }
            }
            else {
                if(hosp->head->next!=NULL){
                    curHospital->next->prev = NULL;
                    hosp->head = curHospital->next;
                }
                else hosp->head = NULL;
            }
            free(curHospital);
            break;
        }
    }
}

void clear(Hospitals*hosp){
    PatientNode *patientTemp;
    for(HospitalNode *curHospital = hosp->head; curHospital != NULL; curHospital = curHospital->next){
        freePatients(curHospital,patientTemp);
        if(curHospital->prev!=NULL){
            if(curHospital->next == NULL) curHospital->prev->next = NULL;
            else{
                curHospital->prev->next = curHospital->next;
                curHospital->next->prev = curHospital->prev;
            }
        }
        else {
            if(hosp->head->next!=NULL){
                curHospital->next->prev = NULL;
                hosp->head = curHospital->next;
            }
            else hosp->head = NULL;
        }
        free(curHospital);
    }
}

void chooseFunctionTaskTwo(){
    int choice = -1;
    Hospitals *hosp = writeDataFromFile();
    while(choice!=0){
        printf("\nEnter 1 to add new hospital\nEnter 2 to delete hospital\nEnter 3 to print data\nEnter 4 to remove selected patient\nEnter 5 to put patient into nearest hospital\nEnter 6 to find hospital\nEnter 0 to exit task\nYour choice:");
        choice = checkInt();
        if(choice == 1){
            addNewHospital(hosp);
            writeDataToFile(hosp);
        }else if(choice == 2){
            deleteHospital(hosp);
            writeDataToFile(hosp);
        }else if(choice == 3){
            printData(hosp);
        }else if(choice == 4){
            removePatient(hosp);
            writeDataToFile(hosp);
        }else if(choice == 5) {
            putPatientIntoHospital(hosp);
            writeDataToFile(hosp);
        }else if(choice == 6){
            findHospital(hosp);
        }
    }
    clear(hosp);
}

