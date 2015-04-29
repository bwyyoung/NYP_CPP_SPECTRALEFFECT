//bryo file parser

#ifndef FileParser_H
#define FileParser_H

#include <String>
#include <sstream>
#include <fstream>

using namespace std;

class BryoFileParser
{
private:
	string curFile;
public:
	void TargetFile(string filename)
	{
		curFile = filename;		
	}

	int GetInt(string header, string var)
	{
		ifstream file;
		string Line;
		int i;
		file.open(curFile.c_str());

		while (!file.eof())
		{
			stringstream LineReader;
			getline(file, Line);
			
			string Head, Varname, Result;

			LineReader << Line;
			LineReader >> Head;

			if (Head == header)
			{
				//found our line, check for the variable
				while (!LineReader.eof())
				{
					LineReader >> Head;
					i = Head.find('=');
					Varname = Head.substr(0, i);

					if (Varname == var)
					{
						Result = Head.substr(i + 1);
						
						stringstream Convertor;
						int retVal;
						
						Convertor << Result;
						Convertor >> retVal;

						return retVal;
					}
				}
			}
		}

		return -1;
	}
};
#endif