#include <stdio.h>   /* foundational lib */
#include <stdlib.h>  /* foundational lib */
#include <string.h>  /* foundational lib */
#include <stdbool.h> /* bool type */
#include <stdint.h>  /* std size int types */
typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;
//~ #define NDEBUG
#include <assert.h>

static u8*
loadFile(u8 *file_name)
{
	FILE *pFile;
	u8 *buffer;
	u64 fileSize, result;
	
	pFile = fopen(file_name, "rb");
	if (pFile==NULL) {
		fputs ("File error, cannot open source file\n",stderr);
		exit(1);
	}
	// obtain file size:
	fseek(pFile , 0 , SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);
	// allocate memory to contain the whole file:
	buffer = malloc(fileSize+1);
	if (buffer == NULL) {
		fputs ("Memory error\n",stderr);
		exit(2);
	}
	// copy the file into the buffer:
	result = fread (buffer,1,fileSize,pFile);
	if (result != fileSize) {
		fputs ("Reading error\n",stderr);
		exit(3);
	}
	/* 0x00 terminate buffer, leave return in sub file */
	buffer[fileSize]=0;
	fclose(pFile);
	return buffer;
}

static u8*
getNextLine(u8 *doc)
{
	while(*doc!='\n'&&*doc!=0){doc++;}
	return doc;
}

static u8*
outputLine(u8 *doc, u8 *out)
{
	if (*doc=='\\'||*doc=='\n'||*doc==0) { return out; }
	if (doc[0]=='/'&&doc[1]=='/') { return out; }
	*out++ = '\"';
	while(*doc!='\n'&&*doc!=0)
	{
		switch (*doc)
		{
		case 0 ... 31:	*out++ = ' '; break;
		case '\\':	*out++ = '\\'; *out++ = '\\'; break;
		case '\"':	*out++ = '\\'; *out++ = '\"'; break;
		default: *out++ = *doc; break;
		}
		doc++;
	}
	*out++ = '\\';
	*out++ = 'n';
	*out++ = '\"';
	*out++ = '\n';
	return out;
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	if (argc < 2)
	{
		printf("Please provide name of source file.\n");
		return 1;
	}
	printf("opening file %s.\n", argv[1]);
	u8 *sourceCode = loadFile(argv[1]);
	argv[1][strlen(argv[1]) - 5] = 0;
	u8 *outputData = calloc(128*1024,1);
	u8 *end;
	u8 *start = sourceCode;
	
	strcpy(outputData, "char *G");
	strcat(outputData, (argv[1]));
	strcat(outputData, "= ");
	u8 *cursor = outputData + strlen(outputData);
	printf("Starting the copy.\n");
	while (1)
	{
		end = getNextLine(start);
		cursor = outputLine(start, cursor);
		start = end + 1;
		if (*end == 0) { break; }
	}
	*cursor++ = ';';
	*cursor = 0;
	printf("Copy finished.\n");
	
	// make new file locations
	u8 outFileName[256];
	strcpy(outFileName, "src/");
	strcat(outFileName, (argv[1]));
	strcat(outFileName, ".c");
	// write out results
	FILE *outputFile = fopen ( outFileName, "w" );
	if (outputFile==NULL) {fputs ("File error",stderr); exit (1);}
	fwrite (outputData , sizeof(char), (cursor-outputData), outputFile);
	fflush (outputFile); 
	fclose (outputFile);
	return 0;
}

