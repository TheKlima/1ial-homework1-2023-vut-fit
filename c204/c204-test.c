#include "c204.h"
#include "c204-test-utils.h"

#include <stdio.h>
#include <stdlib.h>

// for discord tests
//#include <assert.h>

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))


TEST(test_01, "Upper and lower case characters with plus operator")
	if (convert("a+B=", &expr))
	{
		verify(expr, "aB+=");
	}
ENDTEST

TEST(test_02, "Digits with minus operator")
	if (convert("0-1=", &expr))
	{
		verify(expr, "01-=");
	}
ENDTEST

TEST(test_03, "Mixed operands with multiplication operator")
	if (convert("a*0=", &expr))
	{
		verify(expr, "a0*=");
	}
ENDTEST

TEST(test_04, "Mixed operands with division operator")
	if (convert("B/1=", &expr))
	{
		verify(expr, "B1/=");
	}
ENDTEST

TEST(test_05, "Simple parentheses support")
	if (convert("(a+b)=", &expr))
	{
		verify(expr, "ab+=");
	}
ENDTEST

TEST(test_06, "Expression evaluation from the left to the right")
	if (convert("a+b+c=", &expr))
	{
		verify(expr, "ab+c+=");
	}
ENDTEST

TEST(test_07, "Minus operator does not have higher priority than plus")
	if (convert("a+b-c=", &expr))
	{
		verify(expr, "ab+c-=");
	}
ENDTEST

TEST(test_08, "Plus operator does not have higher priority than minus")
	if (convert("A-B+C=", &expr))
	{
		verify(expr, "AB-C+=");
	}
ENDTEST

TEST(test_09, "Plus operator does not have higher priority than minus")
	if (convert("1*2/3=", &expr))
	{
		verify(expr, "12*3/=");
	}
ENDTEST

TEST(test_10, "Parentheses change operator priority")
	if (convert("A+(B-c)=", &expr))
	{
		verify(expr, "ABc-+=");
	}
ENDTEST

// my own tests

//TEST(my_test_1, "my test #1")
//    if (convert("a*(b+c)=", &expr))
//    {
//        verify(expr, "abc+*=");
//    }
//ENDTEST
//
//TEST(my_test_2, "my test #2")
//    if (convert("(a+b)*(c-d)/(e+f)*(g-h)=", &expr))
//    {
//        verify(expr, "ab+cd-*ef+/gh-*=");
//    }
//ENDTEST
//
//TEST(my_test_3, "my test #3")
//    if (convert("(a+b)*(c-d)+e=", &expr))
//    {
//        verify(expr, "ab+cd-*e+=");
//    }
//ENDTEST
//
//TEST(my_test_4, "my test #4")
//    if (convert("((A+B)–C*(D/E))+F=", &expr))
//    {
//        verify(expr, "AB+–CDE/*F+=");
//    }
//ENDTEST
//
//TEST(my_test_5, "my test #5")
//    if (convert("A+B*C/(E-F)=", &expr))
//    {
//        verify(expr, "ABC*EF-/+=");
//    }
//ENDTEST
//
//TEST(my_test_6, "my test #6")
//    if (convert("(A*0+b)*((c*(1+D))-(e/(3*f+g)))=", &expr))
//    {
//        verify(expr, "A0*b+c1D+*e3f*g+/-*=");
//    }
//ENDTEST
//
//TEST(my_test_7, "my test #7")
//    if (convert("A*(b-C)=", &expr))
//    {
//        verify(expr, "AbC-*=");
//    }
//ENDTEST
//
//TEST(my_test_8, "my test #8")
//    if (convert("A*(b/c)=", &expr))
//    {
//        verify(expr, "Abc/*=");
//    }
//ENDTEST
//
//TEST(my_test_9, "my test #9")
//    if (convert("A+(B-c)=", &expr))
//    {
//        verify(expr, "ABc-+=");
//    }
//ENDTEST

// end of my own tests

TEST(test_eval_01, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "(a+b)*(c-d)="; // expected result: 100
	VariableValue values[] = {
			{'a', 5},
			{'b', 5},
			{'c', 12},
			{'d', 2},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_02, "Evaluate expression")
	STACK_SIZE = 128;
	expr = "(a-b)*C="; // expected result: 10
	VariableValue values[] = {
			{'a', 3},
			{'b', 5},
			{'C', -5},
	};
	PRINT_EXPR_EVAL_CONFIG();
	if (eval(expr, values, valuesCount, &result) == false) {
		FAIL("eval() call returned false\n");
	}
	printf("eval result: %d\n", result);
ENDTEST

TEST(test_eval_03, "Evaluate expression")
    STACK_SIZE = 128;
    expr = "a*b+c/d="; // expected result: 50
    VariableValue values[] = {
            {'a', 10},
            {'b', 3},
            {'c', 100},
            {'d', 5},
    };
    PRINT_EXPR_EVAL_CONFIG();
    if (eval(expr, values, valuesCount, &result) == false) {
        FAIL("eval() call returned false\n");
    }
    printf("eval result: %d\n", result);
ENDTEST

// my own tests

//TEST(my_test_eval_01, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "a+b*c="; // expected result: 22
//    VariableValue values[] = {
//            {'a', 10},
//            {'b', 2},
//            {'c', 6},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_02, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "a*2+b="; // expected result: 212
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 12},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_03, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "a*(2+b)="; // expected result: 1400
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 12},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_04, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "a*(2+b)/c="; // expected result: 100
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 12},
//            {'c', 14},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_05, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "A*(B+C)/D="; // expected result: 7
//    VariableValue values[] = {
//            {'A', 2},
//            {'B', 4},
//            {'C', 3},
//            {'D', 2},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_06, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "2*(5*(3+6))/5-2="; // expected result: 16
//    VariableValue values[] = {
//            {'A', 2},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_07, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "a/0=";  // expected result: error (division by zero)
//    VariableValue values[] = {
//            {'a', 1},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//ENDTEST
//
//TEST(my_test_eval_08, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "(A*0+b)*(c*(1+D))-(3*f+g)="; // expected result: 153543
//    VariableValue values[] = {
//            {'A', 123456},
//            {'b', 87},
//            {'c', 52},
//            {'D', 33},
//            {'f', 76},
//            {'g', 45}
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_09, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "(A*0+b)*(c*(1+D))-(3*f+g)="; // expected result: -144495
//    VariableValue values[] = {
//            {'A', -123456},
//            {'b', -87},
//            {'c', -52},
//            {'D', -33},
//            {'f', -76},
//            {'g', -45}
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST
//
//TEST(my_test_eval_010, "Evaluate expression")
//    STACK_SIZE = 128;
//    expr = "(A*0+b)*(c*(1+D))-(3*f+g)="; // expected result: 153633
//    VariableValue values[] = {
//            {'A', -123456},
//            {'b', -87},
//            {'c', -52},
//            {'D', 33},
//            {'f', 76},
//            {'g', -45}
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    if (eval(expr, values, valuesCount, &result) == false) {
//        FAIL("eval() call returned false\n");
//    }
//    printf("eval result: %d\n", result);
//ENDTEST

// end of my own tests


// some tests from discord

//TEST(test_eval_big, "evaluate a chonker")
//    STACK_SIZE = 128;
//    expr = "((a)/(b*c-(d/(e))))=";
//    VariableValue values[] = {
//            {'a', 100}, {'b', 3}, {'c', 10}, {'d', 20}, {'e', 2},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    assert(eval(expr, values, valuesCount, &result));
//    assert(result == 5);
//ENDTEST
//
//TEST(test_eval_seq, "is the order of operations correct?")
//    STACK_SIZE = 128;
//    expr = "a/b/c=";
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 10},
//            {'c', 2},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    assert(eval(expr, values, valuesCount, &result));
//    assert(result == 5);
//ENDTEST
//
//TEST(test_eval_div0, "division by zero")
//    STACK_SIZE = 128;
//    expr = "a/(b-c)=";
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 2},
//            {'c', 2},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    assert(!eval(expr, values, valuesCount, &result));
//ENDTEST
//
//TEST(test_eval_parens, "who doesn't like parentheses")
//    STACK_SIZE = 128;
//    expr = "(((((((((a))))))))-(((((b)))))-((((((((((c)))))))))))=";
//    VariableValue values[] = {
//            {'a', 100},
//            {'b', 2},
//            {'c', 3},
//    };
//    PRINT_EXPR_EVAL_CONFIG();
//    assert(eval(expr, values, valuesCount, &result));
//    assert(result == 95);
//ENDTEST

// end of some tests from discord


void (*tests[])(void) = {
		test_01,
		test_02,
		test_03,
		test_04,
		test_05,
		test_06,
		test_07,
		test_08,
		test_09,
		test_10,
        
//        my_test_1,
//        my_test_2,
//        my_test_3,
//        my_test_4,
//        my_test_5,
//        my_test_6,
//        my_test_7,
//        my_test_8,
//        my_test_9,

		test_eval_01,
		test_eval_02,
        test_eval_03,
        
//        my_test_eval_01,
//        my_test_eval_02,
//        my_test_eval_03,
//        my_test_eval_04,
//        my_test_eval_05,
//        my_test_eval_06,
//        my_test_eval_07,
//        my_test_eval_08,
//        my_test_eval_09,
//        my_test_eval_010,
//
//        test_eval_big,
//        test_eval_seq,
//        test_eval_div0,
//        test_eval_parens,
};

int main( int argc, char *argv[] ) {
	if (argc > 2)
	{
		fprintf(stderr, "Usage: %s [test_id]\n", argv[0]);
		return 1;
	}

	char *test_id_reminder = NULL;
	long test_id;
	if (argc == 2)
	{
		test_id = strtol(argv[1], &test_id_reminder, 10);
		if (test_id_reminder[0] != 0)
		{
			fprintf(stderr, "Usage: %s {test_id}\n", test_id_reminder);
			fprintf(stderr, "Unexpected test_id: %s\n", test_id_reminder);
			return 1;
		}

		if (test_id < 0)
		{
			test_id = TEST_COUNT + test_id;
		}

		if (test_id + 1 > TEST_COUNT)
		{
			fprintf(stderr, "Unknown test: %ld (test count: %ld)\n", test_id, TEST_COUNT);
			return 1;
		}

		tests[test_id]();
	}
	else
	{
		printf("C204 - Infix to Postfix Expression Conversion, Basic Tests\n");
		printf("==========================================================\n");

		for (test_id = 0; test_id < TEST_COUNT; test_id++)
		{
			tests[test_id]();
		}
	}

	return 0;
}

/* Konec c204-test.c */