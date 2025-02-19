%{
    // C-Support code declarations/functions
    // Supportive data structures BEGIN
    typedef struct lex_data_{
        int token_code;
        int token_len;
        char *token_val;
    } lex_data_t;

    #define MAX_MEXPR_LEN 512

    typedef struct stack_{
        int top;
        lex_data_t data[MAX_MEXPR_LEN];
    }stack_t;

    // Supportive data structures END

    // Global variables for the use of the parser BEGIN
    extern "C" int yylex();
    extern lex_buffer[512];
    stack_t undo_stack = {-1,{0,0,0}};
    char lex_buffer[512];

    // Basic stack functions 
    static void lex_push(lex_data_t lex_data){
        assert(undo_stack.top < MAX_MEXPR_LEN - 1);
        undo_stack.data[++undo_stack.top] = lex_data;
    }

    static lex_data_t lex_pop (){
        assert(undo_stack.top > -1);
        // Obtained from
        lex_data_t res = undo_stack.data[undo_stack.top];
        return res;
    }
    // Global variables for the use of the parser END

    #include "expr_enums.h"
%}

%%
// Regular expressions to generate tokens

\n{
    return PARSER_EOL;
}

// Integer numbers
0|-?[1-9][0-9]* {
    return MATH_CPP_INT;
}

// Decimal values
-?[0-9]*\.[0-9]+ {
    return MATH_CPP_DOUBLE;
}

// Alphanumeric
[a-zA-Z0-9_]+ {
    return MATH_CPP_VARIABLE;
}

// IPv4 address
^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)(\.(?!$)|$)){4}$ {
    return MATH_CPP_IPV4;
}

.{
    // Non-parseable character
}

%%

int main(int argc, char *argv[]) {
    while (1) {
        fgets(lex_buffer, sizeof(lex_buffer), stdin);

        if (lex_buffer[0] == '\n') {
            lex_buffer[0] = 0;
            continue;
        }
        yy_can_string(lex_buffer);
        int toekn_code = yylex();

        while(token_code != PARSERL_EOL){
            printf("Token code =  %d\nToken: %s\nToken Length = %d", token_code, yytext, yyleng);
            token_code = yylex();
        }
    }
}
