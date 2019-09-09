#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define MAX
#define MAX_ARRAY_LEN 200
#define MAX_STRING_LEN 200
#define STRING_PARSE "%199s"

//Returns the number n raised to the power e
int power(int n, int e)
{
    if(e == 0) return 1;

    return n * power(n, e - 1);
}

//Check if string s1 and s2 are equal
int isStringEqual(char* s1, char* s2)
{
	return !strcmp(s1, s2);
}

//Returns if the current string has a digit as the first chracter
int isStringDigit(char* s)
{
	return isdigit(s[0]);
}

//Returns the next "word" from the file that is being read from (ignorning whitespace)
int getNextString(FILE* file, char* result)
{
	if(!feof(file)) 
	{
		return fscanf(file, STRING_PARSE, result);
	}

	return -1;
}

//Returns the given binary string representation of a integer back as an integer
int binStringToInt(char* s)
{
	int result = 0, len = strlen(s);

	int digit = len - 1;
	char* ptr = NULL;

	for(ptr = s; digit >= 0; ptr += 1)
	{
		if(*ptr == '1')
		{
			result += power(2, digit);
		}

		digit--;
	}

	return result;
}

//Converts the given int i into a binary string representation of that integer
void intToBin(char* result, int i, int digits)
{
	result[0] = '\0';
	
	int j = 0, k = 0, n = 0;

	for(j = digits - 1; j >= 0; j--)
	{
		k = i >> j;

		if(k & 1)
		{
			result[n] = 1 + '0';
		}
		else
		{
			result[n] = 0 + '0';
		}

		n++;
	}

	result[n] = '\0';
}

//Returns the NOT of the paramerter i
int getNOT(int i)
{
	return !i;
}

//Returns the AND of the parameters i1 and i2
int getAND(int i1, int i2)
{
	return i1 && i2;
}

//Returns the OR of the paramters i1 and i2
int getOR(int i1, int i2)
{
	return i1 || i2;
}

//Returns the NAND of the paramters i1 and i2
int getNAND(int i1, int i2)
{
	return !(i1 && i2);
}

//Returns the NOR of the paramters i1 and i2
int getNOR(int i1, int i2)
{
	return !(i1 || i2);
}

//Returns the XOR of the paramters i1 and i2
int getXOR(int i1, int i2)
{
	return (i1 && !i2) || (!i1 && i2);
}

//Reads the file given and formats the data into a more usable form
void parseFile(FILE* file, char** input, int* inputCount, char** output, int* outputCount, char** circuit, int* circuitCount)
{
	char buff[MAX_STRING_LEN];

    	//Reads all directives
	while(getNextString(file, buff) > 0)
	{
		//printf("DIRECTIVE: %s\n", buff);

		if(isStringEqual(buff, "INPUT"))
		{
			//printf("INPUT DIRECTIVE\n");

			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}

			int i, n = atoi(buff);

			//printf("%d INPUT VARS!\n", n);

			for(i = 0; i < n; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
				
				strcpy(input[*inputCount + i], buff);
			}

			*inputCount += n;

		}
		else if(isStringEqual(buff, "OUTPUT"))
		{
			//printf("OUTPUT DIRECTIVE\n");

			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}

			int i, n = atoi(buff);

			//printf("%d OUTPUT VARS!\n", n);

			for(i = 0; i < n; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
				
				strcpy(output[*outputCount + i], buff);
			}

			*outputCount += n;
		}
		else if(isStringEqual(buff, "NOT") || isStringEqual(buff, "PASS"))
		{
			//printf("%s DIRECTIVE\n", buff);

			//TODO: RENAME VAR
			char logicLine[MAX_STRING_LEN];
			
			logicLine[0] = '\0';

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get first param
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}
			

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get second param
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}

			strcat(logicLine, buff);

			//printf("Test: %s\n", logicLine);

			strcpy(circuit[*circuitCount], logicLine);
			(*circuitCount)++;
		}
		else if(isStringEqual(buff, "AND") || isStringEqual(buff, "OR") || isStringEqual(buff, "NAND") || isStringEqual(buff, "NOR") || isStringEqual(buff, "XOR"))
		{
			//printf("%s DIRECTIVE\n", buff);

			//TODO: RENAME VAR
			char logicLine[MAX_STRING_LEN];
			
			logicLine[0] = '\0';

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get first param
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}
			
			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get second param
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}
			strcat(logicLine, buff);			
			strcat(logicLine, " ");

			//Get third param
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}
			strcat(logicLine, buff);

			//printf("Test: %s\n", logicLine);

			strcpy(circuit[*circuitCount], logicLine);
			(*circuitCount)++;
		}
			
		else if(isStringEqual(buff, "DECODER"))
		{
			//printf("%s DIRECTIVE\n", buff);

			//TODO: RENAME VAR
			char logicLine[MAX_STRING_LEN];
			
			logicLine[0] = '\0';

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}

			int i, inputVars = atoi(buff), outputVars = power(2, inputVars);

			//printf("%d INPUT VARS!\n", inputVars);

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get inputVars
			for(i = 0; i < inputVars; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
			
				strcat(logicLine, buff);
				strcat(logicLine, " ");
			}

			for(i = 0; i < outputVars; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
			
				strcat(logicLine, buff);

				if(i < outputVars - 1)
				{
					strcat(logicLine, " ");
				}
			}

			strcpy(circuit[*circuitCount], logicLine);
			(*circuitCount)++;
		}
		else if(isStringEqual(buff, "MULTIPLEXER"))
		{
			//printf("%s DIRECTIVE\n", buff);

			//TODO: RENAME VAR
			char logicLine[MAX_STRING_LEN];
			
			logicLine[0] = '\0';

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}

			int i, selectorVars = atoi(buff), inputVars = power(2, selectorVars);

			//printf("%d INPUT VARS!\n", inputVars);

			strcat(logicLine, buff);
			strcat(logicLine, " ");

			//Get inputVars
			for(i = 0; i < inputVars; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
			
				strcat(logicLine, buff);
				strcat(logicLine, " ");
			}

			for(i = 0; i < selectorVars; i++)
			{
				getNextString(file, buff);
			
				if(isStringEqual(buff, ":"))
				{
					getNextString(file, buff);
				}
			
				strcat(logicLine, buff);
				strcat(logicLine, " ");
			}
			
			getNextString(file, buff);
			
			if(isStringEqual(buff, ":"))
			{
				getNextString(file, buff);
			}
			
			strcat(logicLine, buff);

			strcpy(circuit[*circuitCount], logicLine);
			(*circuitCount)++;
		}
		else
		{
			//printf("Oops! Invalid directive recieved!!!\n");
		}
	}
}

//Returns if the var given is in the output of the gate given
int isInOutput(char** circuit, int circuitCount, int gateIndex, char* var)
{
	char* line = "";

	int i = 0, j = 0, k = 0;

	line = circuit[gateIndex];

	char buff[MAX_STRING_LEN][MAX_STRING_LEN];

	for(i = 0; i < strlen(line); i++)
	{
		if(line[i] == ' ')
		{
			buff[k][j] = '\0';
			j = 0;
			k++;
		}
		else
		{
			buff[k][j] = line[i];
			j++;
		}
	}

	buff[k][j] = '\0';

	char* directive = buff[0];

	//printf("DIRECTIVE PASSED <O>: %s\n", directive);

	if(isStringEqual(directive, "NOT") || isStringEqual(directive, "PASS"))
	{
		if(isStringEqual(buff[2], var))
		{
		    return 1;
		}
	}
	else if(isStringEqual(directive, "AND") || isStringEqual(directive, "OR") || isStringEqual(directive, "NAND") || isStringEqual(directive, "NOR") || isStringEqual(directive, "XOR"))
	{
	    	//printf("%sTOHis is working:%s\n", var,  buff[2]);

	    	if(isStringEqual(buff[3], var))
		{
		    return 1;
		}
	}
	else if(isStringEqual(directive, "DECODER"))
	{
	    	int inputVars = atoi(buff[1]);
		int outputVars = power(2, inputVars);

		//printf("DECODER OUTPUT VARS: %d\n", inputVars);

		for(i = 2 + inputVars; i < 2 + inputVars + outputVars; i++)
		{
		    if(isStringEqual(buff[i], var))
		    {
			//printf("LOOKING AT1: %s\n", buff[i]);
			return 1;
		    }
		}
	}
	else if(isStringEqual(directive, "MULTIPLEXER"))
	{
		int selectorVars = atoi(buff[1]);
		int inputVars = power(2, selectorVars);
		//printf("MULTIPLEXER INPUT VARS: %d\n", inputVars);
		
		if(isStringEqual(buff[2 + inputVars + selectorVars], var))
		{
			return 1;
		}
	}

	return 0;	
}

//Swaps the two given string pointers
void swap(char** s1, char** s2)
{
	char* buff = *s1;
	*s1 = *s2;
	*s2 = buff;
}

//Sorts the circuit
void sortCircuit(char** input, int inputCount, char** temp, int tempCount, char** output, int outputCount, char** circuit, int circuitCount)
{
	int swaps = 0;

	do
	{
		int i = 0, j = 0, k = 0, m = 0;
		swaps = 0;

		i = 0;
		j = 0;
		k = 0;
		
		for(m = 0; m < circuitCount - 1; m++)
		{
			char* gate = circuit[m];
			char** circuitDecomp;

			//printf("LOGIC GATE: %s\n", circuit[m]);

			circuitDecomp = (char**) malloc(sizeof(char*) * MAX_ARRAY_LEN);

			i = 0;
			j = 0;
			k = 0;

			for(i = 0; i < MAX_STRING_LEN; i++)
			{
				circuitDecomp[i] = (char*) malloc(sizeof(char) * MAX_STRING_LEN);
			}

			for(i = 0; i < strlen(gate); i++)
			{
				if(gate[i] == ' ')
				{
					circuitDecomp[k][j] = '\0';
					j = 0;
					k++;
				}
				else
				{
					circuitDecomp[k][j] = gate[i];
					j++;
				}
			}

			circuitDecomp[k][j] = '\0';

			char* directive = circuitDecomp[0];
			
			//PASTE HERE

			int inputStartIndex = 1;
			int inputEndIndex = 1;

			//printf("Directive: %s \n", directive);

			if(isStringEqual(directive, "NOT") || isStringEqual(directive, "PASS"))
			{    
				inputStartIndex = 1;
				inputEndIndex = 2;
			}
			else if(isStringEqual(directive, "AND") || isStringEqual(directive, "OR") || isStringEqual(directive, "NAND") || isStringEqual(directive, "NOR") || isStringEqual(directive, "XOR"))
			{
				inputStartIndex = 1;
	    			inputEndIndex = 3;
			}
			else if(isStringEqual(directive, "DECODER"))
			{
				int inputVars = atoi(circuitDecomp[1]);
				inputStartIndex = 2;
				inputEndIndex = 3 + inputVars;
			}
			else if(isStringEqual(directive, "MULTIPLEXER"))
			{
				int selectorVars = atoi(circuitDecomp[1]);
				int inputVars = power(2, selectorVars);
				inputStartIndex = 2;
				inputEndIndex = 3 + inputVars + selectorVars;
			}	

			//Current circuit gate has input
			for(j = m + 1; j < circuitCount; j++)
			{
				//printf("INDEXS: %d -> %d\n", inputStartIndex, inputEndIndex);
				for(k = inputStartIndex; k < inputEndIndex; k++)
				{
					//printf("INPUTS CHECKED: %s\n", circuitDecomp[k]);
					if(isInOutput(circuit, circuitCount, j, circuitDecomp[k]))
					{
						//printf("ELEMENT1: %s\n", circuit[m]);
						//printf("ELEMENT2: %s\n", circuit[j]);
						//Needs swap
						swap(&circuit[m], &circuit[j]);
							
						swaps++;

						//printf("SWAPPING!!!!\n");
					}
				}	
			}

			for(i = 0; i < MAX_STRING_LEN; i++)
			{
				free(circuitDecomp[i]);
			}


			free(circuitDecomp);
		}
	} while(swaps != 0);
}

//Returns the index of the element in the given list
int getIndex(char** list, int listCount, char* var)
{
	int i;

	for(i = 0; i < listCount; i++)
	{
		if(isStringEqual(list[i], var))
		{
			return i;
		}
	}

	return -1;
}

//Evaluates the given directive
void evaluateDirective(char** input, int* inputValues, int inputCount, char** temp, int* tempValues, int* tempCount, char** output, int* outputValues, int outputCount, char* gate)
{
    	int i = 0, j = 0, k = 0;

	char** circuitDecomp;

	circuitDecomp = (char**) malloc(sizeof(char*) * MAX_ARRAY_LEN);

	for(i = 0; i < MAX_STRING_LEN; i++)
	{
		circuitDecomp[i] = (char*) malloc(sizeof(char) * MAX_STRING_LEN);
	}

	for(i = 0; i < strlen(gate); i++)
	{
		if(gate[i] == ' ')
		{
			circuitDecomp[k][j] = '\0';
			j = 0;
			k++;
		}
		else
		{
			circuitDecomp[k][j] = gate[i];
			j++;
		}
	}

	circuitDecomp[k][j] = '\0';

	//Process directive
	
	//printf("CUR GATE: %s\n", gate);

	char* directive = circuitDecomp[0];

	//printf("CUR DIR: %s\n", directive);

	if(isStringEqual(directive, "NOT"))
	{
		int inVarIndex = getIndex(input, inputCount, circuitDecomp[1]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[2]);

		int inVar = 0;

		if(inVarIndex < 0)
		{
			inVarIndex = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVarIndex < 0)
			{
			    	strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVarIndex = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar = tempValues[inVarIndex];
		}
		else
		{
			inVar = inputValues[inVarIndex];
		}


		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(outVarIndex < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			tempValues[outVarIndex] = getNOT(inVar);
		}
		else
		{
			outputValues[outVarIndex] = getNOT(inVar);
		}

	}
	else if(isStringEqual(directive, "AND"))
	{
		int inVar1Index = getIndex(input, inputCount, circuitDecomp[1]);
		int inVar2Index = getIndex(input, inputCount, circuitDecomp[2]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[3]);

		int inVar1 = 0;
		int inVar2 = 0;

		if(inVar1Index < 0)
		{
			inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVar1Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar1 = tempValues[inVar1Index];
		}
		else
		{
			inVar1 = inputValues[inVar1Index];
		}

		if(inVar2Index < 0)
		{
			inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(inVar2Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			inVar2 = tempValues[inVar2Index];
		}
		else
		{
			inVar2 = inputValues[inVar2Index];
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);

			if(outVarIndex < 0)
			{
			    	

				//printf("TESTER: %s\n", circuitDecomp[3]);

				strcpy(temp[*tempCount], circuitDecomp[3]);
				
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);
			}

			tempValues[outVarIndex] = getAND(inVar1, inVar2);
		}
		else
		{
			outputValues[outVarIndex] = getAND(inVar1, inVar2);
		}

		//printf("AND: %d && %d -> %d\n", inVar1, inVar2, outVar);
	}
	else if(isStringEqual(directive, "OR"))
	{
		int inVar1Index = getIndex(input, inputCount, circuitDecomp[1]);
		int inVar2Index = getIndex(input, inputCount, circuitDecomp[2]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[3]);

		int inVar1 = 0;
		int inVar2 = 0;

		if(inVar1Index < 0)
		{
			inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVar1Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar1 = tempValues[inVar1Index];
		}
		else
		{
			inVar1 = inputValues[inVar1Index];
		}

		if(inVar2Index < 0)
		{
			inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(inVar2Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			inVar2 = tempValues[inVar2Index];
		}
		else
		{
			inVar2 = inputValues[inVar2Index];
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);

			if(outVarIndex < 0)
			{
			    	

				//printf("TESTER: %s\n", circuitDecomp[3]);

				strcpy(temp[*tempCount], circuitDecomp[3]);
				
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);
			}

			tempValues[outVarIndex] = getOR(inVar1, inVar2);
		}
		else
		{
			outputValues[outVarIndex] = getOR(inVar1, inVar2);
		}	
	}
	else if(isStringEqual(directive, "NAND"))
	{
		int inVar1Index = getIndex(input, inputCount, circuitDecomp[1]);
		int inVar2Index = getIndex(input, inputCount, circuitDecomp[2]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[3]);

		int inVar1 = 0;
		int inVar2 = 0;

		if(inVar1Index < 0)
		{
			inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVar1Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar1 = tempValues[inVar1Index];
		}
		else
		{
			inVar1 = inputValues[inVar1Index];
		}

		if(inVar2Index < 0)
		{
			inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(inVar2Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			inVar2 = tempValues[inVar2Index];
		}
		else
		{
			inVar2 = inputValues[inVar2Index];
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);

			if(outVarIndex < 0)
			{
			    	

				//printf("TESTER: %s\n", circuitDecomp[3]);

				strcpy(temp[*tempCount], circuitDecomp[3]);
				
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);
			}

			tempValues[outVarIndex] = getNAND(inVar1, inVar2);
		}
		else
		{
			outputValues[outVarIndex] = getNAND(inVar1, inVar2);
		}	
	}
	else if(isStringEqual(directive, "NOR"))
	{
		int inVar1Index = getIndex(input, inputCount, circuitDecomp[1]);
		int inVar2Index = getIndex(input, inputCount, circuitDecomp[2]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[3]);

		int inVar1 = 0;
		int inVar2 = 0;

		if(inVar1Index < 0)
		{
			inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVar1Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar1 = tempValues[inVar1Index];
		}
		else
		{
			inVar1 = inputValues[inVar1Index];
		}

		if(inVar2Index < 0)
		{
			inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(inVar2Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			inVar2 = tempValues[inVar2Index];
		}
		else
		{
			inVar2 = inputValues[inVar2Index];
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);

			if(outVarIndex < 0)
			{
			    	

				//printf("TESTER: %s\n", circuitDecomp[3]);

				strcpy(temp[*tempCount], circuitDecomp[3]);
				
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);
			}

			tempValues[outVarIndex] = getNOR(inVar1, inVar2);
		}
		else
		{
			outputValues[outVarIndex] = getNOR(inVar1, inVar2);
		}	
	}
	else if(isStringEqual(directive, "XOR"))
	{
		int inVar1Index = getIndex(input, inputCount, circuitDecomp[1]);
		int inVar2Index = getIndex(input, inputCount, circuitDecomp[2]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[3]);

		int inVar1 = 0;
		int inVar2 = 0;

		if(inVar1Index < 0)
		{
			inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVar1Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar1Index = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			inVar1 = tempValues[inVar1Index];
		}
		else
		{
			inVar1 = inputValues[inVar1Index];
		}

		if(inVar2Index < 0)
		{
			inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(inVar2Index < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVar2Index = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			inVar2 = tempValues[inVar2Index];
		}
		else
		{
			inVar2 = inputValues[inVar2Index];
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);

			if(outVarIndex < 0)
			{
			    	

				//printf("TESTER: %s\n", circuitDecomp[3]);

				strcpy(temp[*tempCount], circuitDecomp[3]);
				
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[3]);
			}

			tempValues[outVarIndex] = getXOR(inVar1, inVar2);
		}
		else
		{
			outputValues[outVarIndex] = getXOR(inVar1, inVar2);
		}	
	}
	else if(isStringEqual(directive, "DECODER"))
	{
		int inputVars = atoi(circuitDecomp[1]);
		
		char* inputStr = (char*) calloc(MAX_STRING_LEN, sizeof(char));

		for(i = 0; i < inputVars; i++)
		{
		    	char* buff = (char*) calloc(MAX_STRING_LEN, sizeof(char));

			int inVarIndex = getIndex(input, inputCount, circuitDecomp[2 + i]);
			int inVar = 0;
			int isInputTemp = 0;

			if(inVarIndex < 0)
			{
				inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i]);

				if(inVarIndex < 0)
				{
					strcpy(temp[*tempCount], circuitDecomp[2 + i]);
					tempValues[*tempCount] = 0;

					(*tempCount)++;

					inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i]);
				}
	
				isInputTemp = 1;
			}

			if(isInputTemp)
			{
				inVar = tempValues[inVarIndex];
			}
			else
			{
				inVar = inputValues[inVarIndex];
			}
	
			sprintf(buff, "%d", inVar);
			strcat(inputStr, buff);

			free(buff);
		}

		int outputOffset = binStringToInt(inputStr);

		int outVarIndex = getIndex(output, outputCount, circuitDecomp[2 + i + outputOffset]);

		int isOutputTemp = 0;


		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i + outputOffset]);

			if(outVarIndex < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2 + i + outputOffset]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i + outputOffset]);
			}
	
			isOutputTemp = 1;
		}

		if(isOutputTemp)
		{
			tempValues[outVarIndex] = 1;
		}
		else
		{
			outputValues[outVarIndex] = 1;
		}

		free(inputStr);

	}
	else if(isStringEqual(directive, "MULTIPLEXER"))
	{
	    	int selectVars = atoi(circuitDecomp[1]);
		int inputVars = power(2, selectVars);
		
		char* inputStr = (char*) calloc(MAX_STRING_LEN, sizeof(char));

		for(i = inputVars; i < inputVars + selectVars; i++)
		{
		    	char* buff = (char*) calloc(MAX_STRING_LEN, sizeof(char));

			int inVarIndex = getIndex(input, inputCount, circuitDecomp[2 + i]);
			int inVar = 0;

			if(inVarIndex < 0)
			{
				inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i]);

				if(inVarIndex < 0)
				{
					strcpy(temp[*tempCount], circuitDecomp[2 + i]);
					tempValues[*tempCount] = 0;

					(*tempCount)++;

					inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + i]);
				}

				inVar = tempValues[inVarIndex];
			}
			else
			{
				inVar = inputValues[inVarIndex];
			}

			sprintf(buff, "%d", inVar);

			strcat(inputStr, buff);
			free(buff);
		}

		int inputOffset = binStringToInt(inputStr);
		
		//printf("\nResult: %s    %d\n\n", inputStr,  inputOffset);
		
		int inVarIndex = getIndex(input, inputCount, circuitDecomp[2 + inputOffset]);
		int inVar = 0;

		if(inVarIndex < 0)
		{
			if(isStringDigit(circuitDecomp[2 + inputOffset]))
			{	
				inVar = atoi(circuitDecomp[2 + inputOffset]);
			}
			else
			{
				inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + inputOffset]);

				if(inVarIndex < 0)
				{
					strcpy(temp[*tempCount], circuitDecomp[2 + inputOffset]);
					tempValues[*tempCount] = 0;

					(*tempCount)++;

					inVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + inputOffset]);
				}
					
				inVar = tempValues[inVarIndex];
			}
			
		}
		else
		{
			inVar = inputValues[inVarIndex];
		}
		
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[2 + inputVars + selectVars]);

		//printf("TEST: %s\n", circuitDecomp[2 + inputVars + selectVars]);

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + inputVars + selectVars]);

			if(outVarIndex < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2 + inputVars + selectVars]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2 + inputVars + selectVars]);
			}

			tempValues[outVarIndex] = inVar;
		}
		else
		{
		    	outputValues[outVarIndex] = inVar;
		}

		free(inputStr);
	}
	else if(isStringEqual(directive, "PASS"))
	{
		int inVarIndex = getIndex(input, inputCount, circuitDecomp[1]);
		int outVarIndex = getIndex(output, outputCount, circuitDecomp[2]);

		int inVar = 0;

		int isInputTemp = 0;
		int isOutputTemp = 0;

		if(inVarIndex < 0)
		{
			inVarIndex = getIndex(temp, *tempCount, circuitDecomp[1]);

			if(inVarIndex < 0)
			{
			    	strcpy(temp[*tempCount], circuitDecomp[1]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				inVarIndex = getIndex(temp, *tempCount, circuitDecomp[1]);
			}

			isInputTemp = 1;
		}

		if(outVarIndex < 0)
		{
			outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2]);

			if(outVarIndex < 0)
			{
				strcpy(temp[*tempCount], circuitDecomp[2]);
				tempValues[*tempCount] = 0;

				(*tempCount)++;

				outVarIndex = getIndex(temp, *tempCount, circuitDecomp[2]);
			}

			isOutputTemp = 1;
		}
		
		if(isInputTemp)
		{
			inVar = tempValues[inVarIndex];
		}
		else
		{
			inVar = inputValues[inVarIndex];
		}
			//INPUT & OUTPUT LIST VAR
		
		if(isOutputTemp)
		{
			tempValues[outVarIndex] = inVar;
		}
		else
		{
			outputValues[outVarIndex] = inVar;
		}	
	}

	for(i = 0; i < MAX_STRING_LEN; i++)
	{
		free(circuitDecomp[i]);
	}

	free(circuitDecomp);
}

//Displays the given row of the truth table
void printTruthTableRow(int* inputValues, int inputCount, int* tempValues, int tempCount, int* outputValues, int outputCount)
{
	int i;

	for(i = 0; i < inputCount - 1; i++)
	{
		printf("%d ", inputValues[i]);
	}

	printf("%d | ", inputValues[i]);

	for(i = 0; i < outputCount - 1; i++)
	{
		printf("%d ", outputValues[i]);
	}

	printf("%d\n", outputValues[i]);
}

int main(int argc, char **argv) 
{
	//Load file or setup input file pointer
	FILE* file = NULL;
	if(argc == 1) 
	{
	    	//No input file specified
		file = stdin;
	}
	else if(argc > 1) 
	{
		//Input file specified

		char *inputFileDir = argv[1];
		file = fopen(inputFileDir, "r");

		//Terminates if the input file is not able to be opened
		if(!file)
		{
			printf("Error!!!!");
			return 0;
		}
	}
	
	//Allocate memory for input names, output names and circuit
	char** input;
	char** temp;
	char** output;
	char** circuit;

	input = (char**) calloc(MAX_ARRAY_LEN, sizeof(char*));
	temp = (char**) calloc(MAX_ARRAY_LEN, sizeof(char*));
	output = (char**) calloc(MAX_ARRAY_LEN, sizeof(char*));
	circuit = (char**) calloc(MAX_ARRAY_LEN, sizeof(char*));

	int i, j;

	for(i = 0; i < MAX_ARRAY_LEN; i++)
	{
	    input[i] = (char*) calloc(MAX_STRING_LEN, sizeof(char));
	    temp[i] = (char*) calloc(MAX_STRING_LEN, sizeof(char));
	    output[i] = (char*) calloc(MAX_STRING_LEN, sizeof(char));
	    circuit[i] = (char*) calloc(MAX_STRING_LEN, sizeof(char));
	}

	int inputCount = 0;
	int tempCount = 0;
	int outputCount = 0;
	int circuitCount = 0;

	parseFile(file, input, &inputCount, output, &outputCount, circuit, &circuitCount);
/*
	printf("\n\nCIRCUIT SCANNED:");
	printf("\n--------------------------------------------------\n");

	for(i = 0; i < inputCount; i++)
	{
		printf("INPUT[%d]: %s\n", i, input[i]);
	}

	for(i = 0; i < outputCount; i++)
	{
		printf("OUTPUT[%d]: %s\n", i, output[i]);
	}

	for(i = 0; i < circuitCount; i++)
	{
		printf("CIRCUIT[%d]: %s\n", i, circuit[i]);
	}

	printf("--------------------------------------------------\n");
*/
	//Process and print truthtable

	sortCircuit(input, inputCount, temp, tempCount, output, outputCount, circuit, circuitCount);
/*
	printf("\n\nSORTED CIRCUIT:");
	printf("\n--------------------------------------------------\n");

	for(i = 0; i < circuitCount; i++)
	{
		printf("CIRCUIT[%d]: %s\n", i, circuit[i]);
	}

	printf("--------------------------------------------------\n");

*/
	//Allocate memory for input, output values
	int* inputValues;
	int* tempValues;
	int* outputValues;

	inputValues = (int*) calloc(inputCount, sizeof(int));
	tempValues = (int*) calloc(MAX_ARRAY_LEN, sizeof(int));
	outputValues = (int*) calloc(outputCount, sizeof(int));
	
	int curInputValuesLen = inputCount;
	char* curInputValues = malloc(sizeof(char*) * curInputValuesLen);

	//Circuit processing and truth table fabrication
	for(i = 0; i < power(2, inputCount); i++)
	{

		intToBin(curInputValues, i, curInputValuesLen);

		//printf("Int Rep: %d\n", i);
		//printf("Bin Rep: %s\n", curInputValues);

		for(j = 0; j < inputCount; j++)
		{
			inputValues[j] = (curInputValues[j] == '1' ? 1 : 0);
		}

		for(j = 0; j < circuitCount; j++)
		{
			evaluateDirective(input, inputValues, inputCount, temp, tempValues, &tempCount, output, outputValues, outputCount, circuit[j]);
		}

		printTruthTableRow(inputValues, inputCount, tempValues, tempCount, outputValues, outputCount);

		for(j = 0; j < tempCount; j++)
		{
			tempValues[j] = 0;
		}

		for(j = 0; j < outputCount; j++)
		{
			outputValues[j] = 0;
		}
	}

	free(curInputValues);

	//Freeing memory
	for(i = 0; i < MAX_ARRAY_LEN; i++)
	{
		free(input[i]);
		free(temp[i]);
		free(output[i]);
		free(circuit[i]);
	}

	free(input);
	free(temp);
	free(output);
	free(circuit);

	free(inputValues);
	free(tempValues);
	free(outputValues);

	//printf("Finished Running!!!\n");
	fclose(file);
	return 0;
}


