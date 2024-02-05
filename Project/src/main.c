#include <stdio.h>
#include <string.h>
#include <errno.h>

#define NUM_STATES 49

struct state;
enum input_type;
struct token;
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
    vline,
} input_type;

typedef struct token {
    int flag;
    char str[20];
} token;

/*
typedef enum token {
    Range_Separator_token,
    ASSIGN_token,
    LCURLY_token,
    RCURLY_token,
    COMMA_token,
    LPAREN_token,
    RPAREN_token,
    TypeRef_token,
    Identifier_token,
    Number_token,
    TAGS_token,
    BEGIN_token,
    SEQUENCE_token,
    INTEGER_token,
    DATE_token,
    END_token,
} token;

void print_token(enum token token) {
    char str[100];
    switch(token) {
        case Range_Separator_token:
            strcpy(str, "Range_Separator");
        case ASSIGN_token:
            strcpy(str, "ASSIGN");
        case LCURLY_token:
            strcpy(str, "LCURLY");
        case RCURLY_token:
            strcpy(str, "RCURLY");
        case COMMA_token:
            strcpy(str, "COMMA");
        case LPAREN_token:
            strcpy(str, "LPAREN");
        case RPAREN_token:
            strcpy(str, "RPAREN");
        case TypeRef_token:
            strcpy(str, "TypeRef");
        case Identifier_token:
            strcpy(str, "Identifier");
        case Number_token:
            strcpy(str, "Number");
        case TAGS_token:
            strcpy(str, "TAGS");
        case BEGIN_token:
            strcpy(str, "BEGIN"):
        case SEQUENCE_token:
            strcpy(str, "SEQUENCE");
        case INTEGER_token:
            strcpy(str, "INTEGER");
    }
}
*/

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
    LCURLY, RCURLY, COMMA, LPAREN, RPAREN, // 46 ... 50
    VLINE; // 51q

// stores various data regarding a particular state
typedef struct state {
    state_type  * state_type;
    input_type input_type;
    token token;
    char input;
    char value[100];
} state;

input_type get_input_type(char input) {
    switch(input) {
        case 0x09 ... 0x0d:
            return whitespace;
            break;
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
        case '|':
            return vline;
            break;
        default:
            {
                // TODO: error
            }
    }
}

// The following functions represent each state in the FSM.
// Each function handles the transitions based on the input using
// switch statements
// Valid final state funcitions will change the state token when a whitespace
// is reached.
void START(state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START;
                break;
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
                    break;
                }
            break;
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
        case vline:
            {
                state->state_type = VLINE;
                break;
            }
        case period:
            {
                state->state_type = Range_Separator_ONE;
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
                strcpy(state->token.str, "TAGS");
                state->token.flag = 1;
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
                strcpy(state->token.str, "BEGIN");
                state->token.flag = 1;
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
                strcpy(state->token.str, "SEQUENCE");
                state->token.flag = 1;
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
                strcpy(state->token.str, "INTEGER");
                state->token.flag = 1;
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
                strcpy(state->token.str, "DATE");
                state->token.flag = 1;
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
                strcpy(state->token.str, "END");
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
                strcpy(state->token.str, "TypeRef");
                state->token.flag = 1;
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
                strcpy(state->token.str, "Identifier");
                state->token.flag = 1;
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
                strcpy(state->token.str, "Number");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "Number");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "ASSIGN");
                state->token.flag = 1;
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
        case whitespace:
            {
                state->state_type = START; 
                strcpy(state->token.str, "Range_Separator");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "LCURLY");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "RCURLY");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "COMMA");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "LPAREN");
                state->token.flag = 1;
                break;
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
                strcpy(state->token.str, "RPARN");
                state->token.flag = 1;
                break;
            }
        default:
            {}
    }
}

void VLINE(struct state * state) {
    switch(get_input_type(state->input)) {
        case whitespace:
            {
                state->state_type = START;
                strcpy(state->token.str, "VLINE");
                state->token.flag = 1;
                break;
            }
        default:
            {}
    }
}

void test_successful() {
    // load the content of the source code file into a string
    FILE * code;
    code = fopen("./TestSuccessful.txt", "r");
    fseek(code ,0 , SEEK_END);
    int length = ftell(code);
    rewind(code);
    char str[length];
    for(int i = 0; i < length; i++) {
        str[i] = fgetc(code);
    }
    fclose(code);

    struct state current_state;
    current_state.state_type = START;
    for(int i = 0; i < length; i++) {
        current_state.input = str[i];
        char input_str[] = {current_state.input};
        current_state.state_type(&current_state);
        if(get_input_type(current_state.input) != whitespace) {
            strcat(current_state.value, input_str);
        }
        if(current_state.token.flag == 1) {
            printf("Token: %s\n Name: %s\n", current_state.token.str,
                   current_state.value);
            current_state.token.flag = 0;
            current_state.token.str[0] = 0;
            current_state.value[0] = 0;
        }
    }
}

int main() {
    test_successful();
    return 0;
}
