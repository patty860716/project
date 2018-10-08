#include "opcode.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "elf32.h"
extern struct opcodetable optab;
int iindex = 0;
char result[100];
char reg_8[8][4] = {"al","cl","dl","bl","ah","ch","dh","bh"};
char reg_16[8][4] = {"ax","cx","dx","bx","sp","bp","si","di"}; 
char reg_32[8][4] = {"eax","ecx","edx","ebx","esp","ebp","esi","edi"};
char group1[8][4] = {"ADD","OR","ADC","SBB","AND","SUB","XOR","CMP"};
char group2[8][4] = {"ROL","ROR","RCL","RCR","SHL","SHR","SAL","SAR"};
struct optabentry* newentry(char s[100])
{//printf("%s",s);
	char *pch;
	pch = strtok(s," ");
	Elf32_Half num0 = (Elf32_Half)strtol(pch, NULL, 16);
	optab.entries[iindex].po = num0;
	pch = strtok(NULL," ");
	//if(pch!=NULL)
	strncpy(optab.entries[iindex].ins,pch,strlen(pch));
	//printf("%s\n",optab.entries[iindex].ins);
	//optab.entries[iindex].opmap = (struct opelement *)malloc(3*sizeof(struct opelement));
	pch = strtok(NULL," ");
	optab.entries[iindex].opnum = 0;
	optab.entries[iindex].undefined = 0;
	while(pch!=NULL&&pch[0]!=13)
	{
		strncpy(optab.entries[iindex].opmap[optab.entries[iindex].opnum].op,pch,strlen(pch)); 
		if(pch[0]=='E'||pch[0]=='M')
		optab.entries[iindex].undefined = 1;
		if(pch[1]=='b')
		optab.entries[iindex].opmap[optab.entries[iindex].opnum].bytes = 1;
		else if(pch[1]=='v')
		optab.entries[iindex].opmap[optab.entries[iindex].opnum].bytes = 4; 
		else if(pch[1]=='w')
		optab.entries[iindex].opmap[optab.entries[iindex].opnum].bytes = 2;
		else if(pch[1]=='a')
		optab.entries[iindex].opmap[optab.entries[iindex].opnum].bytes = 4; //bound
		else
		optab.entries[iindex].opmap[optab.entries[iindex].opnum].bytes = 0;
		optab.entries[iindex].opnum++;
		pch = strtok(NULL," ");
		
	}
	iindex ++;	
	//return 0;
	return &optab.entries[iindex-1];
}
void constructtable()
{
	 FILE *file;
       char str[100];
char s[2];
        file = fopen("newtab.txt", "rb");
	//fgets (str, 100, file);
	//printf("%d",strlen(str));
    while( fgets (str, 100, file)!=NULL ) {
newentry(str);
      /* writing content to stdout */
//  strncpy(s,str,2)  ;
     
   }
	//optab.size = iindex;
	//printf("%s %x",optab.entries[0].ins[0].mnemonic,optab.entries[0].po);
        //return 0;
}
struct modebyte rbyte;
struct modebyte *readmodbyte(char modbyte)
{
	
	Elf32_Half tmp = modbyte&0x00ff;
	int mod = tmp/64;
	int rm = tmp&0x0007;
	int modrm = tmp&0x0038;
	modrm/=8;
	rbyte.mod = mod;
	rbyte.rm = rm;
	rbyte.modrm = modrm;
	//printf("%d %d %d",mod,modrm,rm);
	return &rbyte;
}
int i;
void readimmbyte(char *buffer,int b,int base)
{
	char tmp1[8];
	Elf32_Word p,jp ;
	int i_tmp = i;
	int neg_flag = 0;
	switch(b)
	{
		/*case 0:
		strcat(result,optab.entries[k].opmap[j].op);
		break;*/
		case 1:
		p = (buffer[++i]&0x000000ff);
		if((p&0x00000080)!=0)
		jp = (p-1)^0x000000ff;
		else jp = p;
		break;
		case 4:
		p = (buffer[++i]&0x000000ff)|((buffer[++i]<<8)&0x0000ff00)|((buffer[++i]<<16)&0x00ff0000)|(buffer[++i]<<24);
		if((p&0x80000000)!=0)
		jp = (p-1)^0xffffffff;
		else jp = p;
		break;
		case 2:
		p = (buffer[++i]&0x000000ff)|((buffer[++i]<<8)&0x0000ff00);
		if((p&0x00008000)!=0)
		jp = (p-1)^0x0000ffff;
		else jp = p;
		break;
		default:
		break;
	}
	if(base>=0)
	{
	p=base+i-i_tmp-jp+1;
	}
	sprintf(tmp1,"%x",p);
		strcat(result,"0x");
		strcat(result,tmp1);
	return;
}
void checkmodE(struct modebyte* m,char* buffer,int b)
{
	//int i_tmp = i;
	//char *tmp = (char*)malloc(sizeof(char)*30);
	if(m->mod==0)
	{
		int flag = 0;
		if(m->rm == 5)
		flag = 1;
		strcat(result,"[");
		if(b==1)
		{
			//strcat(result,"byte ptr ds:[");
			if(flag == 0)
			strcat(result,reg_8[m->rm]);
			
		}
		else if(b==2)
		{
			//strcat(result,"word ptr ds:[");
			if(flag == 0)
			strcat(result,reg_8[m->rm]);
		}
		else if(b==4)
		{
			//strcat(result,"dword ptr ds:[");
			if(flag == 0)
			strcat(result,reg_32[m->rm]);
		}
		if(flag==1)
		readimmbyte(buffer,4,-1);
		strcat(result,"]");
	}
	else if(m->mod==1)
	{strcat(result,"[");
		if(b==1)
		{
			//strcat(result,"byte ptr ds:[");
			strcat(result,reg_8[m->rm]);
		}
		else if(b==2)
		{
			//strcat(result,"word ptr ds:[");
			strcat(result,reg_16[m->rm]);
		}
		else if(b==4)
		{
			//strcat(result,"dword ptr ds:[");
			strcat(result,reg_32[m->rm]);
		}
		strcat(result,"+");
		readimmbyte(buffer,1,-1);
		strcat(result,"]");
	}
	else if(m->mod==2)
	{strcat(result,"[");
		if(b==1)
		{
			//strcat(result,"byte ptr ds:[");
			strcat(result,reg_8[m->rm]);
		}
		else if(b==2)
		{
			//strcat(result,"word ptr ds:[");
			strcat(result,reg_16[m->rm]);
		}
		else if(b==4)
		{
			//strcat(result,"dword ptr ds:[");
			strcat(result,reg_32[m->rm]);
		}
		strcat(result,"+");
		readimmbyte(buffer,4,-1);
		strcat(result,"]");
	}
	else
	{
		if(b==1)
		{
			strcat(result,reg_8[m->rm]);
		}
		else if(b==2)
		{
			strcat(result,reg_16[m->rm]);
		}
		else if(b==4)
		{
			strcat(result,reg_32[m->rm]);
		}
	}
	return ;
}
void checkmodG(struct modebyte* m,int b)
{
	if(b==1)
	strcat(result,reg_8[m->modrm]);
	else if(b==2)
	strcat(result,reg_16[m->modrm]);
	else if(b==4)
	strcat(result,reg_32[m->modrm]);
	return;
}
void codesection(int ii,int size,char* buffer)
{
	i = ii;
	printf("\n");
	while(i<size+ii)
	{
		printf("%-6x:",i-ii);
		//char *result = (char*)malloc(sizeof(char)*100);
		Elf32_Half tmp = buffer[i]&0x00ff;
		//printf("%x\n",tmp);
		int k = tmp;
		if(optab.entries[k].undefined==1)
		{
			struct modebyte* m = readmodbyte(buffer[++i]);
			if(optab.entries[k].ins[0]=='#')
			{
				switch(optab.entries[k].ins[1])
				{
					case '1':
					strcpy(result,group1[m->modrm]);
					break;
					case '2':
					strcpy(result,group2[m->modrm]);
					break;
				}
			}
			else
			strcpy(result,optab.entries[k].ins);
			strcat(result,"	");
			for(int j=0;j<optab.entries[k].opnum;j++)
			{
				if(j!=0)
				strcat(result,",");
				if(optab.entries[k].opmap[j].op[0]=='E'||optab.entries[k].opmap[j].op[0]=='M')
				{
					checkmodE(m,buffer,optab.entries[k].opmap[j].bytes);
				}
				else if(optab.entries[k].opmap[j].op[0]=='G'){checkmodG(m,optab.entries[k].opmap[j].bytes);}
				else
				{
					if(optab.entries[k].opmap[j].bytes==0)
						strcat(result,optab.entries[k].opmap[j].op);
					else
					readimmbyte(buffer,optab.entries[k].opmap[j].bytes,-1);
				}
			}
		}
		else
		{
			strcpy(result,optab.entries[k].ins);
			strcat(result,"	");
			for(int j=0;j<optab.entries[k].opnum;j++)
			{	
				if(j!=0)
				strcat(result,",");
				if(optab.entries[k].opmap[j].bytes==0)
				strcat(result,optab.entries[k].opmap[j].op);
				else
				{
				int base =-1;
				if(optab.entries[k].opmap[j].op[0]=='J')
				base = i-ii;
				readimmbyte(buffer,optab.entries[k].opmap[j].bytes,base);
				}
			}
		}
		printf("%s\n",result);
		i++;
		//free(result);
	}
	return;
}
