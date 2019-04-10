#include "shell.h"
#include "debug.h"
#include "common.h"
#include "string.h"
#include "heap.h"

int echo(char * str)
{
    printk("%s\n", str);
    return 0;
}

int clear(char * str)
{
    console_clear();
    return 0;
}

int shutdown(char * str)
{
    outw(0x604, 0x2000);

    return 0; // unreachable
}

// calc begin

int atoi(const char *str) 
{ 
    int sign = 1, base = 0, i = 0; 
    // if whitespaces then ignore. 
    while (str[i] == ' ') {   
        i++; 
    } 
    // sign of number 
    if (str[i] == '-' || str[i] == '+') { 
        sign = 1 - 2 * (str[i++] == '-');  
    } 
    // checking for valid input 
    while (str[i] >= '0' && str[i] <= '9') { 
        //handling overflow test case   
        if (base >  2147483647 / 10 || (base == 2147483647 / 10 && str[i] - '0' > 7)) { 
            if (sign == 1) return 2147483647; 
            else return -2147483648; 
        } 
        base  = 10 * base + (str[i++] - '0'); 
    } 
    return base * sign; 
} 

int isLeftParantheses(char p) {
    if (p == '(') return 1;
    else return 0;
}

int isRightParantheses(char p) {
    if (p == ')') return 1;
    else return 0;
}

int isOperator(char p) {
    if (p == '+' || p == '-' || p == '*' || p == '/') return p;
    else return 0;
}

int performOperator(int a, int b, char p) {
    switch(p) {
        case '+': return a+b;
        case '-': return a-b;
        case '*': return a*b;
        case '/':
            if (b == 0) { printk("Can't divide by 0, aborting...\n"); return 0; }  // now we dont want the world to expload here do we.
            return a/b;
        default:
        printk("Bad value in switch.\n"); // A replacement which was mentioned in the thread- better have a default response just in case something goes wrong.
        break;
    }
    return 0;
}


char isDigit(char p) {
    if (p >= '0' && p <= '9') return 1;
    else return 0;
}

int charToDigit(char p) {
    if (p >= '0' && p <= '9') return p - '0';
    else return 0;
}

int isNumber(char * p) {
    while(*p) {
        if (!isDigit(*p)) return 0;
        p++;
    }
    return 1;
}

int len(char * p)
{
    return (int) strlen(p); // This was bugged in the source, so I fixed it like the thread advised.
}

int numOfOperands(char * p) {
    int total = 0;
    while(*p) {
        if (isOperator(*p)) total++;
        p++;
    }
    return total+1;
}

int isMDGRoup(char *p)
{

    for(; *p; p++) // used to be a while loop in the source, but this is better imho. more readable, also mentioned on the thread itself.
    {
        if (!isDigit(*p) && *p != '/' && *p != '*') return 0;
    }
    return 1;
    }
    int getLeftOperand(char * p, char * l) {
    // Grab the left operand in p, put it in l,
    //and return the index where it ends.
    int i = 0;

    // Operand is part of multi-*/ group
    if (isMDGRoup(p)) {
        while(1) {
            if (*p == '*' || *p == '/') break;
            l[i++] = *p++;
        }
        return i;
    }

    // Operand is in parantheses (so that's how you write it! sorry for my bad english :)
    if(isLeftParantheses(*p)) {
        int LeftParantheses = 1;
        int RightParantheses= 0;
        p++;
        while(1) {
            if (isLeftParantheses(*p))  LeftParantheses++;
            if (isRightParantheses(*p)) RightParantheses++;

            if (isRightParantheses(*p) && LeftParantheses == RightParantheses)
                break;
            l[i++] = *p++;
        }
        // while (!isRightParantheses(*p)) {
        //  l[i++] = *p++;
        // }
        l[i] = '\0';
        return i+2;
    }

    // Operand is a number
    while (1) {
        if (!isDigit(*p)) break;
        l[i++] = *p++;
    }
    l[i] = '\0';
    return i;
}

int getOperator(char * p, int index, char * op) {
    *op = p[index];
    return index + 1;
}

int getRightOperand(char * p, char * l) {
    // Grab the left operand in p, put it in l,
    //and return the index where it ends.
    while(*p && (isDigit(*p) || isOperator(*p) ||
                isLeftParantheses(*p) || isRightParantheses(*p))) {
        *l++ = *p++;
    }
    *l = '\0';

    return 0;
}

int isEmpty(char * p) {
    // Check if string/char is empty
    if (len(p) == 0) return 1;
    else return 0;
}

int calcExpression(char * p) {
    // if p = #: return atoi(p)
    //
    // else:
    //  L = P.LeftSide
    //  O = P.Op
    //  R = P.RightSide
    //  return PerformOp(calcExpression(L), calcExpression(R), O)

    // ACTUAL FUNCTION

    // if p is a number, return it
    if (isNumber(p)) return atoi(p);

    // Get Left, Right and Op from p.
    char leftOperand[256], rightOperand[256];
    memset(leftOperand, 0, sizeof leftOperand);
    memset(rightOperand, 0, sizeof leftOperand);

    char op;

    int leftOpIndex   = getLeftOperand(p, leftOperand);
    int operatorIndex = getOperator(p, leftOpIndex, &op);
    int rightOpIndex  = getRightOperand(p+operatorIndex, rightOperand);

    if (isEmpty(rightOperand)) return calcExpression(leftOperand);

    return performOperator(
        calcExpression(leftOperand),
        calcExpression(rightOperand),
        op
    );
}

int calc(char * str)
{
    int result = calcExpression(str);
    printk("%d\n", result);

    return 0;
}
// calc end

struct core_command core_commands[] = {
        { "echo", echo },
        { "clear", clear },
        { "shutdown", shutdown },
        { "calc", calc },
};

struct command_list commands = { 
    .size = sizeof core_commands / sizeof (struct core_command), 
    .list = core_commands
};
