#include<math.h>
typedef struct operandNode {
    float data;
    struct operandNode *next;
}OperandNode;


typedef struct operatorNode {
    char data;
    struct operatorNode *next;
}OperatorNode;


typedef struct patientNode{
    char name[100];
    float addressX;
    float addressY;
    struct patientNode* prev;
    struct patientNode* next;
}PatientNode;


typedef struct hospitalNode{
    char name[100];
    int allPlaces;
    int freePlaces;
    PatientNode *head;
    float addressX;
    float addressY;
    float(*distanceFunc)(float hospAddressX,float hospAddressY,float patAddressX,float patAddressY);
    struct hospitalNode *next;
    struct hospitalNode *prev;
}HospitalNode;

typedef struct {
    HospitalNode *head;
    HospitalNode *tail;
    int hospitalsCount;
}Hospitals;
