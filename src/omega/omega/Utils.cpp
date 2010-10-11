/********************************************************************************************************************** 
 * THE OMEGA LIB PROJECT
 *---------------------------------------------------------------------------------------------------------------------
 * Copyright 2010								Electronic Visualization Laboratory, University of Illinois at Chicago
 * Authors:										
 *  [Author]									[Mail]
 *---------------------------------------------------------------------------------------------------------------------
 * [LICENSE NOTE]
 *---------------------------------------------------------------------------------------------------------------------
 * [SUMMARY OF FILE CONTENTS]
 *********************************************************************************************************************/
#include "omega/Utils.h"

using namespace omega;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String Utils::readTextFile(const String& filename)
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	fp = fopen(filename.c_str(), "rt");

	if (fp != NULL) 
	{
		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		rewind(fp);
		if (count > 0) 
		{
			content = (char *)malloc(sizeof(char) * (count+1));
			count = fread(content,sizeof(char),count,fp);
			content[count] = '\0';
		}
		fclose(fp);
	}
	
	if (content == NULL)
	{
		fprintf(stderr, "ERROR: could not load in file %s\n", filename.c_str());
		exit(1);
	}
	String result = String(content);
	free(content);
	return result;
}                    
