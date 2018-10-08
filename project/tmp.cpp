#include<stdio.h>
#include<stdlib.h>
#include "elf32.h"
int pos=0;
Elf32_Ehdr * readheader(char * buffer)
{
	Elf32_Ehdr *header;
	 for(int i=0;i<16;i++)
        {
        	header->e_ident[i] = buffer[i];
		}
		pos += 15;
	header->e_type= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_machine=(buffer[++pos]&0x00ff)|(buffer[++pos]<<8);	/* Machine architecture. */	
	header->e_version = (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* ELF format version. */
	header->e_entry = (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Entry point. */
	header->e_phoff = (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	
	header->e_shoff = (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	
	header->e_flags = (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	
	header->e_ehsize= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_phentsize= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_phnum= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_shentsize= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_shnum= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	header->e_shstrndx= (buffer[++pos]&0x00ff)|(buffer[++pos]<<8);
	return header;
}
Elf32_Shdr * readsechd(char* buffer)
{
	Elf32_Shdr* shdr;
	Elf32_Word s;
	s= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);
	shdr->sh_name = s;
	shdr->sh_type= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Section type. */
	shdr->sh_flags= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	
	shdr->sh_addr= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Address in memory image. */
	shdr->sh_offset= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Offset in file. */
	shdr->sh_size= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Size in bytes. */
	shdr->sh_link= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Index of a related section. */
	shdr->sh_info= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Depends on section type. */
	shdr->sh_addralign= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);	/* Alignment in bytes. */
	shdr->sh_entsize= (buffer[++pos]&0x000000ff)|((buffer[++pos]<<8)&0x0000ff00)|((buffer[++pos]<<16)&0x00ff0000)|(buffer[++pos]<<24);/* Size of each entry in section. */
	return shdr;
}
void read_binary(const char* filename)
    {
        FILE *file;
        char *buffer;
        unsigned long fileLen;
        int i;
        char character;
        file = fopen(filename, "rb");
        if (!file)
        {
            fprintf(stderr, "Unable to open file %s", filename);
            return;
        }
        fseek(file, 0, SEEK_END);
        fileLen = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char *)malloc(fileLen + 1);
        if (!buffer)
        {
            fprintf(stderr, "Memory error!");
            fclose(file);
            return;
        }
        fread(buffer, fileLen, 1, file);
       Elf32_Ehdr * header = readheader(buffer);
      	if(header->e_phoff==0)
		  {
		   }
		pos=header->e_shoff -1;
	  	int secnum = header->e_shnum;
       Elf32_Shdr *sectionhd = (Elf32_Shdr*)malloc(sizeof(Elf32_Shdr)*secnum);
       int pos_ori;
       for(int i=0;i<secnum;i++)
       {
       		
    		sectionhd[i] = *readsechd(buffer);
    		if(sectionhd[i].sh_offset==0)
    			continue;
    		pos_ori = pos;
    		pos = sectionhd[i].sh_offset;
    		printf("%x ",sectionhd[i].sh_type);
    		pos = pos_ori;
	   }
		free(buffer);
       // printf("%d %x", header.ulSymbolOffset,header.ulSymbolOffset&0xffff);
        //fclose(file);
    }
int main()
{
	read_binary("hello.o");
	return 0;
}
