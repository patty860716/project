#ifndef _OPCODE_H_
#define _OPCODE_H_
#include"elf32.h"
struct opelement
{
	char op[5];
	int bytes;
};
struct optabentry {
   Elf32_Half po;
	char ins[10];
	struct opelement opmap[3];
	int opnum;
	int undefined;
	
};
struct modebyte{
	int mod;
	int modrm;
	int rm;
};
struct opcodetable {
    int size;
    struct optabentry entries[560];
};
void constructtable();
void codesection(int iindex,int size,char* buffer);
#endif
