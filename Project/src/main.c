#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
    return 0;
}

#define NUM_STATES 46
#define NUM_INPUTS 128

struct state;
typedef void state_func(struct state *);

state_func
    START, // 0
    T, TA, TAG, TAGS, // 2 ... 10
    B, BE, BEG, BEGI, BEGIN, // 6 ... 10
    S, SE, SEQ, SEQU, SEQUE, SEQUEN, SEQUENC, SEQUENCE, // 11 ... 18
    I, IN, INT, INTE, INTEG, INEGE, INTEGER, // 19 ... 25
    D, DA, DAT, DATE, // 26 ... 29
    E, EN, END, // 30 ... 32
    TypeRef, // 33
    Identifier, // 34
    Number, // 35
    ASSIGN_ONE, ASSIGN_TWO, ASSIGN_THREE, // 36 ... 38
    Range_Separator_ONE, Range_Separator_TWO, // 39, 40
    LCURLY, RCURLY, COMMA, LPAREN, RPAREN; // 41 ... 45

struct state {
    state_func * func;
    char input;
    char value[];
};

void START(struct state * current) {
    switch(current->input) {
        // whitespace
        case 0x09 ... 0x0d:
        case 0x20:
            {

                break;
            }
        // check for possible reserved words
        case 'T':
            {
                current->func = T;
                break;
            }
        case 'B':
            {
                current->func = B;
                break;
            }
        case 'S':
            {
                current->func = S;
                break;
            }
        case 'I':
            {
                current->func = I;
                break;
            }
        case 'D':
            {
                current->func = D;
                break;
            }
        case 'E':
            {
                current->func = E;
                break;
            }
        // type references start with a capital letter
        case 'A':
        case 'C':
        case 'F' ... 'H':
        case 'J' ... 'R':
        case 'U' ... 'Z':
            {
                current->func = TypeRef;
                break;
            }
        // identifiers start with a lowercase letter
        case 'a' ... 'z':
            {
                current->func = Identifier;
                break;
            }
        // numbers start with a non 0 number character
        case '1' ... '9':
            {
                current->func = Number;
                break;
            }
        // assignment starts with a colon
        case ':':
            {
                current->func= ASSIGN_ONE;
                break;
            }
        // range separator
        case '.':
            {
                current->func = Range_Separator_ONE;
                break;
            }
        case '{':
            {
                current->func = LCURLY;
                break;
            }
        case '}':
            {
                current->func = RCURLY;
                break;
            }
        case ',':
            {
                current->func = COMMA;
                break;
            }
        case '(':
            {
                current->func = LPAREN;
                break;
            }
        case ')':
            {
                current->func = RPAREN;
                break;
            }
        default:
            {
                // TODO: error handling
            }
    }
}

void T(struct state * current) {
    switch(current->input) {
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->func = START;
                break;
            }
        case 'A':
            {
                current->func = TA;
                break;
            }
        case 'B' ... 'Z':
        case 'a' ... 'z':
        case '-':
            {
                current->func = TypeRef;
                break;
            }
        default:
            {
            // TODO: error
            }
    }
}

void TA(struct state * current) {
    switch(current->input) {
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->func = START;
                break;
            }
        case 'G':
            {
                current->func = TAG;
                break;
            }
        case 'A' ... 'F':
        case 'H' ... 'Z':
        case 'a' ... 'z':
            {
                current->func = TypeRef;
                break;
            }
        case '-':
            {
                current->func = TypeRef;
                break;
            }
        default:
            {
                // TODO: error
            }
    }
}

void TAG(struct state * current) {
    switch(current->input) {
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->func = START;
                break;
            }
        case 'S':
            {
                current->func = TAGS;
                break;
            }
        case 'A' ... 'R':
        case 'T' ... 'Z':
        case 'a' ... 'z':
            {
                current->func = TypeRef;
                break;
            }
        case '-':
            {
                current->func = TypeRef;
                break;
            }
        default:
            {
                // TODO: error
            }
    }
}

void TAGS(struct state * current) {
    switch(current->input) {
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->func = START;
                break;
            }
        case 'A' ... 'Z':
        case 'a' ... 'z':
            {
                current->func = TypeRef;
                break;
            }
        case '-':
            {
                current->func = TypeRef;
                break;
            }
        default:
            {
                // TODO: error
            }
    }
}

void B(struct state * current) {
    switch(current->input){
        case 'E':
            {
                current->func = BE;
                break;
            }
        case 'A' ... 'D':
        case 'F' ... 'Z':
            {
                current->func = TypeRef;
                break;
            }
        case '-':
            {
                current->func = TypeRef;
                break;
            }
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->func = START;
                break;
            }
        default:
            {
                // TODO: error
            }
    }
}

void BE(struct state * current) {}
void BEG(struct state * current) {}
void BEGI(struct state * current) {}
void BEGIN(struct state * current) {}
void S(struct state * current) {}
void SE(struct state * current) {}
void SEQ(struct state * current) {}
void SEQE(struct state * current) {}
void SEQUE(struct state * current) {}
void SEQUEN(struct state * current) {}
void SEQUENC(struct state * current) {}
void SEQUENCE(struct state * current) {}
void I(struct state * current) {}
void IN(struct state * current) {}
void INT(struct state * current) {}
void INTE(struct state * current) {}
void INTEG(struct state * current) {}
void INTEGE(struct state * current) {}
void INTEGER(struct state * current) {}
void D(struct state * current) {}
void DA(struct state * current) {}
void DAT(struct state * current) {}
void DATE(struct state * current) {}
void E(struct state * current) {}
void EN(struct state * current) {}
void END(struct state * current) {}
void TypeRef(struct state * current) {}
void Identifier(struct state * current) {}
void Number(struct state * current) {}
void ASSIGN_ONE(struct state * current) {}
void ASSIGN_TWO(struct state * current) {}
void ASSIGN_THREE(struct state * current) {}
void Range_Separator_ONE(struct state * current) {}
void Range_Separator_TWO(struct state * current) {}
void LCURLY(struct state * current) {}
void RCURLY(struct state * current) {}
void COMMA(struct state * current) {}
void LPAREN(struct state * current) {}
void RPAREN(struct state * current) {}
