#ifndef KB_H
#define KB_H

#define CTRL	0x1D
#define RSHIFT	0x36
#define LSHIFT	0x2A
#define ALT		0x38
#define OPT		0x5C

#define CTRLM	0x1
#define RSHIFTM	0x2
#define LSHIFTM	0x4
#define ALTM	0x8
#define OPTM	0x10

#define NEWLINE	'\n'
#define BACKSPC	'\b'

#define IS_MOD(c)	(c == CTRL || c == RSHIFT || c == LSHIFT || \
					c == ALT || c == OPT) 

#define RELEASED(c)	(c & 0b10000000)


void	kb_install(void);

#endif
