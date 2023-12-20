// File: c204.c
// Subject: IAL
// Homework: #1
// Author: Andrii Klymenko, FIT VUT
// Login: xklyme00
// Date: 20.12.2023

/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

    char stackTopOperator = '\0';         // temporary variable
    Stack_Top(stack, &stackTopOperator);

    while(stackTopOperator != '(')
    {
        // append operator that now is at the top of stack
        postfixExpression[*postfixExpressionLength] = stackTopOperator;

        (*postfixExpressionLength)++;
        Stack_Pop(stack);                       // the operator on the top of the stack is no longer needed
        Stack_Top(stack, &stackTopOperator);    // get next operator from the stack
    }

    Stack_Pop(stack); // remove '(' from stack
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    char stackTopOperator = '\0';

    while(!Stack_IsEmpty(stack))
    {
        Stack_Top(stack, &stackTopOperator);

        // if there is '(' on the top of the stack or if the priority
        // of the stack top operator is less than priority of the processing operator
        if(stackTopOperator == '(' || ((stackTopOperator == '+' || stackTopOperator == '-') && (c == '*' || c == '/')))
        {
            break;
        }

        // append operator that now is at the top of stack
        postfixExpression[*postfixExpressionLength] = stackTopOperator;

        (*postfixExpressionLength)++;
        Stack_Pop(stack); // the operator on the top of the stack is no longer needed
    }

    Stack_Push(stack, c); // push processing operator to stack
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

    // string that will contain postfix expr.
    char* postfixExpression = (char*) malloc(sizeof(char) * MAX_LEN);

    if(postfixExpression == NULL)
    {
        return NULL;
    }

    unsigned postfixExpressionLength = 0; // actual number of characters in postfix expr.

    // allocate memory for stack ptr
    // stack will hold operators from expression
    Stack* operatorStack = (Stack*) malloc(sizeof(Stack));

    if(operatorStack == NULL)
    {
        free(postfixExpression);
        postfixExpression = NULL;

        return NULL;
    }

    Stack_Init(operatorStack);

    if(operatorStack->array == NULL) // check if stack initialization was successful
    {
        free(postfixExpression);
        postfixExpression = NULL;

        free(operatorStack);
        operatorStack = NULL;

        return NULL;
    }

    char current_char = infixExpression[0]; // it will hold current character of the infix epr.

    for(int i = 0; current_char != '='; i++)
    {
        switch(current_char)
        {
            case '(':
                Stack_Push(operatorStack, '(');
                break;

            case ')':
                untilLeftPar(operatorStack, postfixExpression, &postfixExpressionLength);
                break;

            case '+':
            case '-':
            case '*':
            case '/':
                doOperation(operatorStack, current_char, postfixExpression, &postfixExpressionLength);
                break;

            default: // alphanumeric character
                postfixExpression[postfixExpressionLength] = current_char;
                postfixExpressionLength++;
                break;
        }

        current_char = infixExpression[i + 1]; // update current char
    }

    // removing operators from the stack (making it empty) and adding them to postfix expr.
    while(!Stack_IsEmpty(operatorStack))
    {
        Stack_Top(operatorStack, &current_char); // current_char will hold stack top operator
        postfixExpression[postfixExpressionLength] = current_char;
        postfixExpressionLength++;
        Stack_Pop(operatorStack);
    }

    postfixExpression[postfixExpressionLength] = '=';      // postfix expr. must end with '='
    postfixExpression[postfixExpressionLength + 1] = '\0'; // terminate string

    Stack_Dispose(operatorStack); // remove and free stack
    free(operatorStack);          // free stack ptr
    operatorStack = NULL;

    return postfixExpression;
}

/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {

    if(!Stack_IsFull(stack))
    {
        unsigned bits_in_byte = 8;                           // number of bits in one byte
        unsigned bits_in_char = sizeof(char) * bits_in_byte; // number of bits in one character
        unsigned bits_in_int = sizeof(int) * bits_in_byte;   // number of bits in one integer

        for(unsigned i = 0; i < bits_in_int / bits_in_char; i++) // loop until we push a whole integer to the stack
        {
            Stack_Push(stack, value); // push 8 less significant bits of the integer to the stack

            // shift integer by 8 bits to the right
            // we need to push remaining bits of the integer
            value >>= bits_in_char;
        }
    }
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {

    if(!Stack_IsEmpty(stack))
    {
        unsigned bits_in_byte = 8;                           // number of bits in one byte
        unsigned bits_in_char = sizeof(char) * bits_in_byte; // number of bits in one character
        unsigned bits_in_int = sizeof(int) * bits_in_byte;   // number of bits in one integer
        unsigned char tmp_byte = '\0';

        for(unsigned i = 0; i < bits_in_int / bits_in_char; i++) // loop until we get a whole integer from the stack
        {
            (*value) <<= bits_in_char;           // shift integer by 8 bits to the left in order to get its next 8 bits
            Stack_Top(stack, (char*) &tmp_byte); // get next 8 bits of the integer from the stack
            (*value) |= tmp_byte;                // 'load' 8 bits that we got from the stack to the integer

            // pop 8 used bits of the integer from the stack because we don't need them now
            // and we need to get remaining bits of the integer
            Stack_Pop(stack);
        }
    }
}

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {

    char *postfixExpression = infix2postfix(infixExpression); // get postfix expression

    if(postfixExpression == NULL)
    {
        return false;
    }

    // allocate memory for stack ptr
    // stack will hold both operators and operands
    Stack* stack = (Stack*) malloc(sizeof(Stack));

    if(stack == NULL)
    {
        free(postfixExpression);
        postfixExpression = NULL;

        return NULL;
    }

    Stack_Init(stack);

    if(stack->array == NULL)
    {
        free(postfixExpression);
        postfixExpression = NULL;

        free(stack);
        stack = NULL;

        return NULL;
    }

    char current_char = postfixExpression[0]; // it will hold current character of the postfix epr.

    for(int i = 0; current_char != '='; i++)
    {
        if(current_char >= '0' && current_char <= '9')
        {
            expr_value_push(stack, postfixExpression[i] - '0'); // push integer (constant) to the stack
        }
        else if((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z'))
        {
            for(int j = 0; j < variableValueCount; j++)
            {
                if(current_char == variableValues[j].name)
                {
                    expr_value_push(stack, variableValues[j].value);
                    break;
                }
            }
        }
        else // if current char is an operator '+', '-', '*' or '/'
        {
            int operand1 = 0;
            int operand2 = 0;

            // get two operands from the stack
            expr_value_pop(stack, &operand1);
            expr_value_pop(stack, &operand2);

            // push result of the operation to the stack
            switch (current_char)
            {
                case '+':
                    expr_value_push(stack, operand2 + operand1);
                    break;

                case '-':
                    expr_value_push(stack, operand2 - operand1);
                    break;

                case '*':
                    expr_value_push(stack, operand2 * operand1);
                    break;

                case '/':

                    if(operand1 != 0)
                    {
                        expr_value_push(stack, operand2 / operand1);
                        break;
                    }

                    // else
                    // we can't divide by 0
                    free(postfixExpression);
                    postfixExpression = NULL;

                    Stack_Dispose(stack);
                    free(stack);
                    stack = NULL;

                    return false;
            }
        }

        current_char = postfixExpression[i + 1]; // update current char
    }

    expr_value_pop(stack, value); // the result of the expression is at the top of the stack

    Stack_Dispose(stack);       // remove and free stack
    free(stack);                // free stack ptr
    stack = NULL;

    free(postfixExpression);    // free postfix expr.
    postfixExpression = NULL;

    return true;
}

/* Konec c204.c */
