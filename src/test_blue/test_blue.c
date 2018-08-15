#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "ANSIC.h"
#include "misc.h"

#define NODE_VARIABLE 1
#define NODE_IS 2
#define NODE_TYPE 3
#define NODE_ASSIGNMENT 4
#define NODE_UNKNOWN 5

struct Info_Pos
{
	uint32_t row;
	uint32_t col;
};

struct Node_Expression
{
	struct Info_Pos pos;
	char * name;
};

struct Node_Unknown
{
	struct Info_Pos pos;
	char * name;
};

struct Node_Declaration
{
	struct Info_Pos pos;
	char * name;
};

struct Node_Type
{
	struct Info_Pos pos;
	char * name;
};

struct Node
{
	int ntype;
	union
	{
		struct Node_Expression exp;
		struct Node_Declaration decl;
		struct Node_Type type;
		struct Node_Unknown unknown;
	};
};

struct Context
{
	FILE * src;
	FILE * des;
	char * buffer;
};


char const * skip_blank (char const * p)
{
	while (1)
	{
		char const c = p [0];
		if (c == '\0') {return p;};
		else if (c == ' ' || c == '\t') {p ++;}
		else {return p;}
	}
}


void parse (struct Node * node, char const * src)
{
	char const * p = src;
	p = skip_blank (p);
	
	
	while (1)
	{
		char const c = p [0];
		if (c == '\0') {return;};
		if (c == ' ' || c == '\t')
		{
			p ++;
		}
		if (isalpha (c))
		{
			p ++;
		}
		else
		{
			printf ("%c", c);
			p ++;
		}
	}
}





void scantron (struct Context * ctx)
{
	struct Node node;
	node.type = NODE_UNKNOWN;
	node_parse_unknown (&node, ctx->buffer);
	//printf ("%s", ctx->buffer);
}

int main (int argc, char *argv[])
{
	ASSERT_F (argc == 1, "No arg allowed");
	ASSERT_F (argv != NULL, "NULL error");
	printf ("\n\n");
	struct Context ctx;
	ctx.src = fopen ("src/test_blue/test.b", "r");
	ctx.des = stdout;
	ctx.buffer = app_malloc_from_file (ctx.src);
	ASSERT_F (ctx.src != NULL, "%s", "");
	ASSERT_F (ctx.des != NULL, "%s", "");
	

	scantron (&ctx);
	fclose (ctx.src);
	fclose (ctx.des);
	

	
	return 0;
}
