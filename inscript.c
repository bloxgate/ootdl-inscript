#define _GNU_SOURCE

#include <memory.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

char16_t mappings[28] = {L'α', L'β', L'χ', L'δ', L'ε', L'φ', L'γ', L'η', L'Ι', 
	L'Ι', L'κ', L'λ', L'μ', L'ν', L'ο', L'π', L'Ϙ', L'ρ', L'σ', L'τ', L'υ', L'υ', 
	L'ω', L'ξ', L'υ', L'ζ', L'ψ', L'θ'};

#if defined(__WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__MINGW32__) || defined (__MINGW64__) || defined(_MSC_VER)
size_t getline(char** buf, size_t* n, FILE* file)
{
	if(*buf == NULL && *n == 0)
	{
		size_t i = 0;
		char tmp[1];
		while(fread(tmp, sizeof(char), 1, file) > 0)
		{
			i++;
			*buf = realloc(*buf, sizeof(char) * i);
			(*buf)[i-1] = tmp[0];
			*n = i;
			if(tmp[0] == '\n')
			{
				break;
			}
		}
		if(i > 0)
		{
			*buf = realloc(*buf, sizeof(char) * i+1);
			(*buf)[i] = '\0';
			*n = i+1;
		}
		return *n;
	} else {
		char tmp[*n];
		size_t i = 0;
		while(i < *n && fread(tmp, sizeof(char), 1, file) > 0)
		{
			i++;
			if(tmp[i-1] == '\n')
			{
				break;
			}
		}
		if(i < *n && i > 0)
		{
			*buf = realloc(*buf, sizeof(char) * (i + 1));
			memcpy(*buf, tmp, sizeof(char) * (i+1));
			(*buf)[i] = '\0';
			*n = i + 1;
		} else {
			*buf = realloc(*buf, sizeof(char) * *n);
			memcpy(*buf, tmp, sizeof(char) * *n);
			(*buf)[i - 1] = '\0';
		}
		return *n;
	}
}
#endif

int main(int argc, char* argv[])
{
	if (argc < 2 || argc > 3)
	{
		fprintf(stderr, "Usage: ./inscript [input] (output)\n");
		return EXIT_FAILURE;
	}
	setlocale(LC_ALL, "C.UTF-8");

	FILE* input = fopen(argv[1], "r");
	if(input == NULL)
	{
		perror("Unable to open input");
		return EXIT_FAILURE;
	}

	FILE* output;
	if(argc == 3)
	{
		output = fopen(argv[2], "w");
	} else {
		output = fopen("output.txt", "w");
	}
	if(input == NULL)
	{
		perror("Unable to open output");
		return EXIT_FAILURE;
	}

	char* line;
	size_t lineLen = 0;
	while(getline(&line, &lineLen, input) > 0)
	{
		int len = strlen(line);
		for(int i = 0; i < len; i++)
		{
			int c = line[i] - 'a';
			if(c == 15 && i < len - 1)
			{
				if(line[i+1] - 'a' == 18)
				{
					fprintf(output, "%lc", mappings[26]);
					i++;
					continue;
				}
				fprintf(output, "%lc", mappings[c]);
			}
			else if(c == 19 && i < len - 1)
			{
				if(line[i+1] - 'a' == 7)
				{
					fprintf(output, "%lc", mappings[27]);
					i++;
					continue;
				}
				fprintf(output, "%lc", mappings[c]);
			}
			else if (c < 0 || c >= 26)
			{
				fprintf(output, "%c", line[i]);
			}
			else 
			{
				fprintf(output, "%lc", mappings[c]);				
			}
		}
		lineLen = 0;
		free(line);
		line = NULL;
	}

	free(line);
	fclose(input);
	fclose(output);

	return EXIT_SUCCESS;
}