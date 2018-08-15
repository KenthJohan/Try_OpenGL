#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"
#include "ANSIC.h"

#define CTX_VARS_MAX 100
#define ITM_IDENTIFER "[matgen] "
#define ITM_IDENTIFER_LEN (sizeof (ITM_IDENTIFER) - 1)

struct Variable
{
	char * name;
	int type;
};


struct Varlist
{
	struct Variable array [CTX_VARS_MAX];
	uint32_t last;
};


void vars_parse (struct Varlist * vlist, char * line)
{
	char * sub;
	sub = strstr (line, ITM_IDENTIFER);
	if (sub == NULL) {return;}
	sub += ITM_IDENTIFER_LEN;
	printf ("%s", sub);
}


struct Context
{
	FILE * src;
	FILE * des;
	uint32_t line_num;
	uint32_t bracket_r;
	uint32_t bracket_l;
	struct Variable vars [CTX_VARS_MAX];
};

void update_bracket (struct Context * ctx, char * line)
{
	char * sub;
	sub = strstr (line, "{");
	if (sub) {ctx->bracket_r ++;}
	sub = strstr (line, "}");
	if (sub) {ctx->bracket_l ++;}
}

void scanington (struct Context * ctx, char * line)
{
	char * sub = strstr (line, "//[matgen]");
	if (sub == NULL) {return;}
	vars_parse (NULL, sub);
	
	if (sub == NULL) 
	{
		//fprintf (ctx->des, "%s", line);
	}
	else
	{
		//char const * col = ANSIC (ANSIC_NORMAL, ANSIC_CYAN, ANSIC_DEFAULT);
		//fprintf (ctx->des,  "%s%s%s", col, line, ANSIC_RESET);
	}
}


void scantron (struct Context * ctx)
{
	char buf [100];
	while (1)
	{
		char * r = fgets (buf, sizeof (buf), ctx->src);
		if (r == NULL) {break;}
		ctx->line_num ++;
		scanington (ctx, buf);
	}
}

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	printf ("\n\n");
	struct Context ctx;
	ctx.src = fopen ("src/test_mat/test_mat3.c", "r");
	ctx.des = stdout;
	ctx.line_num = 0;
	ctx.bracket_r = 0;
	ctx.bracket_l = 0;
	ASSERT_F (ctx.src != NULL, "%s", "");
	ASSERT_F (ctx.des != NULL, "%s", "");
	

	scantron (&ctx);
	fclose (ctx.src);
	fclose (ctx.des);
	

	
	return 0;
}
