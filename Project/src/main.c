#include <stdio.h>
#include <string.h>

int main()
{
    return 0;
}

struct state;
typedef void state_function(struct state *);

struct state
{
    state_function * next;
    char input;
};

state_function state_start, state_T, state_TA, state_TAG, state_TAGS,
                state_B, state_BE, state_BEG, state_BEGI, state_BEGIN,
                state_S,
                state_I,
                state_D,
                state_E,
                state_type_reference,
                state_identifier;

void state_start(struct state * current)
{
    switch(current->input)
    {
        // check for whitespace character
        case 0x09 ... 0x0d:
        case 0x20:
            {
                current->next = state_start;
            }
        // check for possible reserved words
        case 'T':
            {
                current->next = state_T;
                break;
            }
        case 'B':
            {
                current->next = state_B;
                break;
            }
        case 'S':
            {
                current->next = state_S;
                break;
            }
        case 'I':
            {
                current->next = state_I;
                break;
            }
        case 'D':
            {
                current->next = state_D;
                break;
            }
        case 'E':
            {
                current->next = state_E;
                break;
            }
        // the GCC compiler allows for checking ranges using '...'
        // type references start with a capital letter
        case 'A':
        case 'C':
        case 'F' ... 'H':
        case 'J' ... 'R':
        case 'U' ... 'Z':
            {
                current->next = state_type_reference;
                break;
            }
        // identifiers start with a lowercase letter
        case 'a' ... 'z':
            {
                current->next = state_identifier;
                break;
            }
    }
}

void state_T(struct state * current)
{}
void state_B(struct state * current)
{}
void state_S(struct state * current)
{}
void state_I(struct state * current)
{}
void state_D(struct state * current)
{}
void state_E(struct state * current)
{}
void state_type_reference(struct state * current)
{}
void state_identifier(struct state * current)
{}
