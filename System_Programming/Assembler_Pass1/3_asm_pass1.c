#include "2_optable.c"

/* Public variables and functions */
#define	ADDR_SIMPLE		0x01
#define	ADDR_IMMEDIATE	0x02
#define	ADDR_INDIRECT	0x04
#define	ADDR_INDEX		0x08

#define	LINE_EOF		(-1)
#define	LINE_COMMENT	(-2)
#define	LINE_ERROR		(0)
#define	LINE_CORRECT	(1)

#define HASHSIZE 101

typedef struct {
    struct 		List *next;
    char 		*name;
    unsigned 	loc;
} List;

typedef struct {
	char		symbol[LEN_SYMBOL];
	char		op[LEN_SYMBOL];
	char		operand1[LEN_SYMBOL];
	char		operand2[LEN_SYMBOL];
	unsigned	code;
	unsigned	fmt;
	unsigned	addressing;
	unsigned    memloc;
} LINE;

typedef struct {
    char *name;
    struct rec *next;
} rec;

List *symtab[HASHSIZE];
rec *rec_head, **rec_ind;
unsigned pc = 0;

/* return LINE_EOF, LINE_COMMENT, LINE_ERROR, LINE_CORRECT and Instruction information in *line*/
int process_line(LINE *line);

/* Private variable and function */
void init_LINE(LINE *line) {
	line->symbol[0] = '\0';
	line->op[0] = '\0';
	line->operand1[0] = '\0';
	line->operand2[0] = '\0';
	line->code = 0x0;
	line->fmt = 0x0;
	line->addressing = ADDR_SIMPLE;
	line->memloc = 0;
}

unsigned hash(char *s) {
    unsigned hashval;
    for(hashval = 0; *s != '\0'; s++) {
    	hashval = *s + 31 * hashval;
	}
    return hashval % HASHSIZE;
}

List *lookup(char *s) {
    List *np;
    for(np = symtab[hash(s)]; np != NULL; np = np->next) {
        if(strcmp(s, np->name) == 0) {
          	return np;
		}
	}
    return NULL;
}

List *install(char *name, int loc) {
    List *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (List *) malloc(sizeof(*np));
        if(np == NULL || (np->name = strdup(name)) == NULL) {
            return NULL;
		}
        hashval = hash(name);
        np->next = symtab[hashval];
        symtab[hashval] = np;
        rec_ind = &rec_head;
        rec *new = (rec *)malloc(sizeof(rec));
        new->name = strdup(name), new->next = NULL;
        while(*rec_ind) {
            rec_ind = &((*rec_ind)->next);
		}
        *rec_ind = new;
    }
	else {
        free((void *) np->loc);
	}
    np->loc = loc;
    return np;
}

/* return LINE_EOF, LINE_COMMENT, LINE_ERROR, LINE_CORRECT */
int process_line(LINE *line) {
	char buf[LEN_SYMBOL];
	int	c, state, ret;
	Instruction	*op;
	
	c = ASM_token(buf);	/* get the first token of a line */
	if(c == EOF) {
		return LINE_EOF;
	}
	else if((c == 1) && (buf[0] == '\n')) {	/* blank line */
		return LINE_COMMENT;
	}
	else if((c == 1) && (buf[0] == '.')) {	/* a comment line */
		do {
			c = ASM_token(buf);
		} while((c != EOF) && (buf[0] != '\n'));
		return LINE_COMMENT;
	}
	else {
		init_LINE(line);
		ret = LINE_ERROR;
		state = 0;
		while(state < 8) {
			switch(state) {
				case 0:
				case 1:
				case 2:
					op = is_opcode(buf);
					if((state < 2) && (buf[0] == '+')) {	/* + */
						line->fmt = FMT4;
						state = 2;
					}
					else if(op != NULL)	{	/* INSTRUCTION */
						strcpy(line->op, op->op);
						line->code = op->code;
						state = 3;
						if(line->fmt != FMT4) {
							line->fmt = op->fmt & (FMT1 | FMT2 | FMT3);
						}
						else if((line->fmt == FMT4) && ((op->fmt & FMT4) == 0)) {	/* INSTRUCTION is FMT1 or FMT 2*/
							/* ERROR 20210326 added */
							printf("ERROR at token %s, %s cannot use format 4 \n", buf, buf);
							ret = LINE_ERROR;
							state = 7;		/* skip following tokens in the line */
						}
					}
					else if(state == 0) {	/* SYMBOL */
						strcpy(line->symbol, buf);
						state = 1;
					}
					else {	/* ERROR */
						printf("ERROR at token %s\n", buf);
						ret = LINE_ERROR;
						state = 7;		/* skip following tokens in the line */
					}
					break;
				case 3:
					if(line->fmt == FMT1 || line->code == 0x4C)	{	/* no operand needed */
						if(c == EOF || buf[0] == '\n') {
							ret = LINE_CORRECT;
							state = 8;
						}
						else {	/* COMMENT */
							ret = LINE_CORRECT;
							state = 7;
						}
					}
					else {
						if(c == EOF || buf[0] == '\n') {
							ret = LINE_ERROR;
							state = 8;
						}
						else if(buf[0] == '@' || buf[0] == '#') {
							line->addressing = (buf[0] == '#') ? ADDR_IMMEDIATE : ADDR_INDIRECT;
							state = 4;
						}
						else {	/* get a symbol */
							op = is_opcode(buf);
							if(op != NULL) {
								printf("Operand1 cannot be a reserved word\n");
								ret = LINE_ERROR;
								state = 7; 		/* skip following tokens in the line */
							}
							else {
								strcpy(line->operand1, buf);
								state = 5;
							}
						}
					}			
					break;		
				case 4:
					op = is_opcode(buf);
					if(op != NULL) {
						printf("Operand1 cannot be a reserved word\n");
						ret = LINE_ERROR;
						state = 7;		/* skip following tokens in the line */
					}
					else {
						strcpy(line->operand1, buf);
						state = 5;
					}
					break;
				case 5:
					if(c == EOF || buf[0] == '\n') {
						ret = LINE_CORRECT;
						state = 8;
					}
					else if(buf[0] == ',') {
						state = 6;
					}
					else {	/* COMMENT */
						ret = LINE_CORRECT;
						state = 7;		/* skip following tokens in the line */
					}
					break;
				case 6:
					if(c == EOF || buf[0] == '\n') {
						ret = LINE_ERROR;
						state = 8;
					}
					else {	/* get a symbol */
						op = is_opcode(buf);
						if(op != NULL) {
							printf("Operand2 cannot be a reserved word\n");
							ret = LINE_ERROR;
							state = 7;	/* skip following tokens in the line */
						}
						else {
							if(line->fmt == FMT2) {
								strcpy(line->operand2, buf);
								ret = LINE_CORRECT;
								state = 7;
							}
							else if((c == 1) && (buf[0] == 'x' || buf[0] == 'X')) {
								line->addressing = line->addressing | ADDR_INDEX;
								ret = LINE_CORRECT;
								state = 7;	/* skip following tokens in the line */
							}
							else {
								printf("Operand2 exists only if format 2  is used\n");
								ret = LINE_ERROR;
								state = 7;	/* skip following tokens in the line */
							}
						}
					}
					break;
				case 7:	/* skip tokens until '\n' || EOF */
					if(c == EOF || buf[0] =='\n') {
						state = 8;
					}
					break;										
			}
			if(state < 8) {
				c = ASM_token(buf);	/* get the next token */
			}
		}
		if (line->code == OP_START) {
            pc = atoi(line->operand1);
            line->memloc = pc;
        }
		if (line->fmt != 0 || (line->code >= OP_BYTE && line->code <= OP_RESW)) {
            line->memloc = pc;
		}
		if (*(line->symbol) != '\0') {
            install(line->symbol, pc);
		}
		if (line->fmt != 0) {
            for (int i=line->fmt; i!=0; i>>=1) {
                pc++;
			}
		}
		else if (line->code == OP_BYTE) {
            pc += 1;
		}
        else if (line->code == OP_WORD) {
            pc += 3;
		}
        else if (line->code == OP_RESB) {
            pc += atoi(line->operand1);
		}
        else if (line->code == OP_RESW) {
            pc += atoi(line->operand1) * 3;
		}
		return ret;
	}
}

int main(int argc, char *argv[]) {
	int		i, c, line_count;
	char	buf[LEN_SYMBOL];
	LINE	line;

	if(argc < 2) {
		printf("Usage: %s fname.asm\n", argv[0]);
	}
	else {
		if(ASM_open(argv[1]) == NULL) {
			printf("File not found!!\n");
		}
		else {
			for(line_count = 1 ; (c = process_line(&line)) != LINE_EOF; line_count++) {
				char line_memloc[LEN_SYMBOL], line_op[LEN_SYMBOL], line_operand1[LEN_SYMBOL], line_operand2[LEN_SYMBOL];
                sprintf(line_memloc, "%06X", line.memloc);
                sprintf(line_op, "%s%s", (line.fmt==FMT4)?"+":" ", line.op);
                sprintf(line_operand1, "%s%s", (line.addressing==ADDR_IMMEDIATE)?"#":(line.addressing==ADDR_INDIRECT?"@":""), line.operand1);
                sprintf(line_operand2, "%s%5s", (*line.operand2) ? "," : "", line.operand2);

				if(c == LINE_ERROR)
					printf("%03d : Error\n", line_count);
				else if(c == LINE_COMMENT)
					printf("%03d : ------ . Comment line \n", line_count);
				else
					printf("%03d : %6s %12s %12s %12s %s\n", line_count, (line.memloc || line.fmt) ? line_memloc : "------", line.symbol, line_op, line_operand1, line_operand2);
			}
			printf(".\n.\n");
            printf("Program length = %06X\n.\n", pc);
            rec_ind = &rec_head;
            while(*rec_ind) {
                printf("%12s:\t %06X\n", (*rec_ind)->name, lookup((*rec_ind)->name)->loc);
                rec_ind = &((*rec_ind)->next);
			}
			ASM_close();
		}
	}
}