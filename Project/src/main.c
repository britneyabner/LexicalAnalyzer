#include <stdio.h>
#include <string.h>
#include <errno.h>

int main()
{
    return 0;
}


#define NUM_STATES 49

struct state;
enum input_type;
typedef void state_type(struct state *);

enum input_type get_type(char input);

typedef enum input_type {
    whitespace,
    uppercase,
    lowercase,
    digit,
    hyphen,
    colon,
    equals,
    period,
    lcurly,
    rcurly,
    comma,
    lparen,
    rparen,
} input_type;

// state_type points to a function that handles transitions for each state
state_type
    START, // 0
    T, TA, TAG, TAGS, // 2 ... 10
    B, BE, BEG, BEGI, BEGIN, // 6 ... 10
    S, SE, SEQ, SEQU, SEQUE, SEQUEN, SEQUENC, SEQUENCE, // 11 ... 18
    I, IN, INT, INTE, INTEG, INTEGE, INTEGER, // 19 ... 25
    D, DA, DAT, DATE, // 26 ... 29
    E, EN, END, // 30 ... 32
    TypeRef, TypeRef_one_hyphen, TypeRef_two_hyphen, // 33, 34, 35
    Identifier, Identifier_one_hyphen, Identifier_two_hyphen, // 36, 37, 38
    Number, Zero, // 39, 40
    ASSIGN_ONE, ASSIGN_TWO, ASSIGN_THREE, // 41 ... 43
    Range_Separator_ONE, Range_Separator_TWO, // 44, 45
    LCURLY, RCURLY, COMMA, LPAREN, RPAREN; // 46 ... 50

typedef struct state {
    state_type  * state_type;
    enum input_type input_type;
    char input;
    char value[];
} state;

input_type get_input_type(char input) {
    switch(input) {
        case 0x09 ... 0x0d:
        case 0x20:
                return whitespace;
                break;
        case 'A' ... 'Z':
                return uppercase;
                break;
        case 'a' ... 'z':
            {
                return lowercase;
                break;
            }
        case '0' ... '9':
            return digit;
            break;
        case ':':
            return colon;
            break;
        case '=':
            return equals;
            break;
        case '.':
            return period;
            break;
        case '{':
            return lcurly;
            break;
        case '}':
            return rcurly;
            break;
        case ',':
            return comma;
            break;
        case '(':
            return lparen;
            break;
        case ')':
            return rparen;
            break;
        default:
            {
                // TODO: error
            }
    }
}

void START(state * state) {
    switch(state->input_type) {
        case whitespace:
            {
                state->state_type = START;
            }
        case uppercase:
            {
                switch(state->input) {
                    case 'T':
                        {
                            state->state_type = T;
                            break;
                        }
                    case 'B':
                        {
                            state->state_type = B;
                            break;
                        }
                    case 'S':
                        {
                            state->state_type = S;
                            break;
                        }
                    case 'I':
                        {
                            state->state_type = I;
                            break;
                        }
                    case 'D':
                        {
                            state->state_type = D;
                            break;
                        }
                    case 'E':
                        {
                            state->state_type = E;
                            break;
                        }
                    default:
                        {
                            state->state_type = TypeRef;
                            break;
                        }
                }
            }
        case lowercase:
            {
                state->state_type = Identifier;
                break;
            }
        case digit:
            {
                if(state->input == 0) {
                    state->state_type = Zero;
                    break;
                }
                state->state_type = Number;
                break;
            }
        case lcurly:
            {
                state->state_type = LCURLY;
                break;
            }
        case rcurly:
            {
                state->state_type = RCURLY;
                break;
            }
        case comma:
            {
                state->state_type = COMMA;
                break;
            }
        case rparen:
            {
                state->state_type = RPAREN;
                break;
            }
        case lparen:
            {
                state->state_type = LPAREN;
                break;
            }
        default:
            {
                //TODO: error
            }
    }
}

void T (state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if (state->input == 'A') {
                    state->state_type = TA;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default: 
            {
                // TODO: error
            }
    }
}

void TA(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if (state->input == 'G') {
                    state->state_type = TAG;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void TAG(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'S') {
                    state->state_type = TAGS;
                    break;
                }
                state->state_type = TypeRef;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void TAGS(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        default:
            {}
    }
}

void B(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'E' ) {
                    state->state_type = BE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void BE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'G' ) {
                    state->state_type = BEG;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void BEG(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'I' ) {
                    state->state_type = BEGI;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}
void BEGI(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'N' ) {
                    state->state_type = BEGIN;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void BEGIN(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void S(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'E' ) {
                    state->state_type = SE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void SE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'Q' ) {
                    state->state_type = SEQ;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void SEQ(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'U' ) {
                    state->state_type = SEQU;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void SEQU(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'E' ) {
                    state->state_type = SEQUE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void SEQUE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'N' ) {
                    state->state_type = SEQUEN;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void SEQUEN(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'C' ) {
                    state->state_type = SEQUENCE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}


void SEQUENC(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'E' ) {
                    state->state_type = SEQUENCE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}


void SEQUENCE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}


void I(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'N' ) {
                    state->state_type = IN;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void IN(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'T' ) {
                    state->state_type = INT;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void INT(struct state * state) {
switch(get_input_type(state->input)) {
    case uppercase:
        {
            if(state->input == 'E' ) {
                state->state_type = INTE;
                break;
            }
            state->state_type = TypeRef;
            break;
        }
    case lowercase:
    case digit:
        {
            state->state_type = TypeRef;
            break;
        }
    case hyphen:
        {
            state->state_type = TypeRef_one_hyphen;
            break;
        }
    case whitespace:
        {
            state->state_type = START;
            break;
        }
    default:
        {}
    }
}

void INTE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'G' ) {
                    state->state_type = INTEG;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void INTEG(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'E' ) {
                    state->state_type = INTEGE;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void INTEGE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'R' ) {
                    state->state_type = INTEGER;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void INTEGER(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void D(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'A' ) {
                    state->state_type = DA;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void DA(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'T' ) {
                    state->state_type = DAT;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void DAT(struct state * state) {
switch(get_input_type(state->input)) {
    case uppercase:
        {
            if(state->input == 'E' ) {
                state->state_type = DATE;
                break;
            }
            state->state_type = TypeRef;
            break;
        }
    case lowercase:
    case digit:
        {
            state->state_type = TypeRef;
            break;
        }
    case hyphen:
        {
            state->state_type = TypeRef_one_hyphen;
            break;
        }
    case whitespace:
        {
            state->state_type = START;
            break;
        }
    default:
        {}
    }
}

void DATE(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void E(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'N' ) {
                    state->state_type = EN;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void EN(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
            {
                if(state->input == 'D' ) {
                    state->state_type = END;
                    break;
                }
                state->state_type = TypeRef;
                break;
            }
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void END(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void TypeRef(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void TypeRef_one_hyphen(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
                state->state_type = TypeRef_two_hyphen;
                break;
            }
        case whitespace:
            {
            }
        default:
            {}
    }
}

void TypeRef_two_hyphen(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = TypeRef;
                break;
            }
        case hyphen:
            {
            }
        case whitespace:
            {
            }
        default:
            {}
    }
}

void Identifier(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = Identifier;
                break;
            }
        case hyphen:
            {
                state->state_type = Identifier_one_hyphen;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
                break;
            }
        default:
            {}
    }
}

void Identifier_one_hyphen(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = Identifier;
                break;
            }
        case hyphen:
            {
                state->state_type = Identifier_two_hyphen;
                break;
            }
        case whitespace:
            {
            }
        default:
            {}
    }
}

void Identifier_two_hyphen(struct state * state) {
    switch(get_input_type(state->input)) {
        case uppercase:
        case lowercase:
        case digit:
            {
                state->state_type = Identifier;
                break;
            }
        case hyphen:
            {
            }
        case whitespace:
            {
            }
        default:
            {}
    }
}

void Number(struct state * state) {
    switch(get_input_type(state->input)) {
        case digit:
            {
                state->state_type = Number;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
            }
        default:
            {}
    }
}

void Zero(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START;
            }
        default:
            {}
    }
}

void ASSIGN_ONE(struct state * state) {
    switch(get_input_type(state->input)) {
        case colon:
            {
                state->state_type = ASSIGN_TWO;
                break;
            }
        default:
            {}
    }
}

void ASSIGN_TWO(struct state * state) {
    switch(get_input_type(state->input)) {
        case equals:
            {
                state->state_type = ASSIGN_THREE;
                break;
            }
        case whitespace:
            {
                state->state_type = START;
            }
        default:
            {}
    }
}

void ASSIGN_THREE(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START;
            }
        default:
            {}
    }
}

void Range_Separator_ONE(struct state * state) {
    switch(get_input_type(state->input)) {
        case period:
            {
                state->state_type = Range_Separator_TWO;
                break;
            }
        default:
            {}
    }
}

void Range_Separator_TWO(struct state * state) {
    switch(get_input_type(state->input)) {
        case period:
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}

void LCURLY(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}

void RCURLY(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}

void COMMA(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}

void LPAREN(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}

void RPAREN(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START; 
            }
        default:
            {}
    }
}
