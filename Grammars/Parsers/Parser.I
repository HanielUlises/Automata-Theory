%{
    // C-Support code declarations/functions
    // Supportive data structures BEGIN


    // Supportive data structures END

    // Global variables for the use of the parser BEGIN
    extern "C" int yylex();
    extern lex_buffer[512];

    char lex_buffer[512];
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
    // Non-parsable character
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
