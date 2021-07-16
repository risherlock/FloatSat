
#include "stdio.h"
#include "stdint.h"
#include "string.h"

// Reads from stdin definition of bitefield
// and generates code to serialize/deserialize
// 
// As example please read the file bitFieldsSerializer-example.bf


struct FieldDescriptor {
    char name[100];
    int numOfBits;
    char comment[200];
};

static const int MAX_FIELDS  = 200;
FieldDescriptor field[MAX_FIELDS];
int fieldCnt  = 0;
char className[100];
char nameSpaceName[100];

#define HEADER \
  "\n\n"\
  "//--- generated file, DO NOT modify\n"\
  "//--- see bitFieldsSerializer\n"\
  "#pragma once\n"\
  "\n\n"

#define SCAN_AND_SKIP_COMMENTS()\
        name[0] = comment[0] = 0; \
        sscanf(inputLine,"%s %d %[^\n]s", name, &numOfBits, comment); \
        if(name[0] ==  0 ) continue; \
        if(name[0] == '#') continue; \

int main() {

//________________________ to be read
    char inputLine[300];
    char name[100];
    int  numOfBits;
    char comment[100];
    bool nameSpaceDefined = false;

//________________________________ generate header

    printf(HEADER);
    bool nextClassName = false;
    for(bool done=false; !done; ) {
        char* s = fgets(inputLine, 300, stdin);
        if(s == 0) {
            printf("#warning  end of file and notthing to do\n");
            return 0;
        }
        SCAN_AND_SKIP_COMMENTS();
        if (!nextClassName) {
            nextClassName = true;
            if(name[0] != '-') {
                printf("namespace %s {\n", name);
                nameSpaceDefined = true;
            }
        } else {
            strncpy(className, name, 100);
            printf("struct %s {\n", name);
            done = true;
        }
    }

//________________________________ read fields
    while( fgets(inputLine, 300, stdin) ) {
        SCAN_AND_SKIP_COMMENTS();
        strncpy(field[fieldCnt].name,    name,    100);
        strncpy(field[fieldCnt].comment, comment, 200);
        field[fieldCnt].numOfBits = numOfBits;
        fieldCnt++;
        if(fieldCnt >= MAX_FIELDS) {
            fprintf(stderr, "Max num of Field %d\n", MAX_FIELDS);
            break;
        }
    }   

//______________________________ generate class

    int totalLen = 0;
    for(int i = 0; i < fieldCnt; i++) totalLen += field[i].numOfBits;
    if(totalLen / 8 > UINT16_MAX) fprintf(stderr, "uint16_t cannot store header size %d\n", totalLen / 8);
    printf("    static const uint16_t HEADER_SIZE = %d;\n", totalLen / 8);
    if(totalLen%8 != 0) printf("#warning not a multiple of 8 bits\n");

    for(int i = 0; i < fieldCnt; i++) {
        if (field[i].numOfBits <=8) {
            printf("    uint8_t  ");
        } else if (field[i].numOfBits <=16) {
            printf("    uint16_t ");
        } else {
            printf("    uint32_t ");
        }
        printf("%-20s; // %2d  bits  %s\n", 
        field[i].name, field[i].numOfBits, field[i].comment);
    }
    printf("\n");
    printf("    uint32_t serialize(uint8_t * const buf) const;\n");
    printf("    uint32_t deserialize(uint8_t const * const buf);\n");
    printf("};\n\n\n");


//_________________________ generate serializer

    int bitCnt  = 0;
    printf("inline uint32_t %s::serialize(uint8_t* const buf) const {\n", className);
    printf("    #ifndef NO_RODOS_NAMESPACE\n"
           "    using namespace RODOS;\n"
           "    #endif\n\n");
    for(int i = 0; i < fieldCnt; i++) {
        if((field[i].numOfBits == 8) && (bitCnt%8 == 0)) {
            printf("    buf[%d]      =             %s;\n", bitCnt/8, field[i].name);
        } else if((field[i].numOfBits == 16) && (bitCnt%8 == 0)) {
            printf("    uint16_tToBigEndian(buf+%d,  %s);\n", bitCnt/8, field[i].name);
        } else if((field[i].numOfBits == 32) && (bitCnt%8 == 0)) {
            printf("    uint32_tToBigEndian(buf+%d,  %s);\n", bitCnt/8, field[i].name);
        } else {
            printf("    setBitField(buf, %3d, %2d, %s);\n", bitCnt, field[i].numOfBits, field[i].name);
       }
       bitCnt += field[i].numOfBits;
    }
    printf("\n    return %d;\n", (bitCnt+7)/8);
    printf("}\n");

//_________________________ generate deserializer

    bitCnt = 0;
    printf("inline uint32_t %s::deserialize(uint8_t const * const buf) {\n", className);
    printf("    #ifndef NO_RODOS_NAMESPACE\n"
           "    using namespace RODOS;\n"
           "    #endif\n\n");
    for(int i = 0; i < fieldCnt; i++) {
        if((field[i].numOfBits == 8) && (bitCnt%8 == 0)) {
            printf("    %-16s = buf[%d];\n", field[i].name, bitCnt/8);
        } else if((field[i].numOfBits == 16) && (bitCnt%8 == 0)) {
            printf("    %-16s = bigEndianToUint16_t(buf+%d);\n", field[i].name, bitCnt/8);
        } else if((field[i].numOfBits == 32) && (bitCnt%8 == 0)) {
            printf("    %-16s = bigEndianToUint32_t(buf+%d);\n", field[i].name, bitCnt/8);
        } else {
            printf("    %-16s = ", field[i].name);
            if(field[i].numOfBits <= 8) {
                printf("static_cast<uint8_t>");
            } else if (field[i].numOfBits <= 16) {
                printf("static_cast<uint16_t>");
            }
            printf("(getBitField(buf, %3d, %2d));\n", bitCnt, field[i].numOfBits);
       }
       bitCnt += field[i].numOfBits;
    }
    printf("\n    return %d;\n", (bitCnt+7)/8);
    if(bitCnt%8 != 0) printf("#warning not alinged %d bits \n", bitCnt);
    printf("}\n");



    if(nameSpaceDefined) printf("} // namespace end\n\n");

    return 0;
}
