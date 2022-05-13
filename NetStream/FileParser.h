#pragma once
#include "String.h"
#include "List.h"
#include <fstream>



class FileParser
{
public:
	static List<String> GetLines(const String& fileName)
	{
        List<String> lines;

        //read from file
        std::ifstream inputStream;
        inputStream.open(fileName.GetArrayPointer());

        if (!inputStream)
            throw std::exception("file could not be opened");
        
        char readBuffer[128];
        while (!inputStream.eof())
        {
            inputStream.getline(readBuffer, 128, '\n');
            lines.Add(readBuffer);
        }
        inputStream.clear();
        inputStream.close();
        return lines;
	}
};