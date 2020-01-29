#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

bool fexists (const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}

std::string ExePath()
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL,buffer,MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0,pos);	
}

int cStrLen(const char *s) {
        int len = 0;
        if (!s) return len;
        char c = s[0];
        while (c!='\0')
           c = s[++len];
        return len;
}


int main(int argc, const char* argv[])
{	
	std::string id,new_flag,line,iPath,oPath,iMakePath,oMakePath;
	argv[1] ? id.assign(argv[1],cStrLen(argv[1])) : id = "";
	argv[2] ? new_flag.assign(argv[2],cStrLen(argv[2])) : new_flag = "";
	
	// Creating main program for leetcode problem
	iPath = ExePath() + "template\\template.txt";
	oPath = ExePath() + "\\" + a + "_lc.cpp";
	
	if (fexists(oPath) && )
	{
	std::ifstream iFile(iPath);
	std::ofstream oFile(oPath, std::ios::out);
	
	if (iFile.is_open() && oFile.is_open())
	{
		while (getline(iFile,line))
		{
			if (line.find("Leetcode ID:") != std::string::npos)
			{
				line.pop_back();
				oFile << line + " " + a + "\n";
			}
			else
				oFile << line;
		}
		oFile.close();
		iFile.close();
	}
	}
	
	/*
	// Change makefile to fit current id
	iMakePath = ExePath() + "\\template\\makefile.txt";
	oMakePath = ExePath() + "\\makefile";
	
	std::ifstream iFile(iPath);
	std::ofstream oFile(oPath, std::ios::out | std::ios::trunc);
	
	if (iFile.is_open() && oFile.is_open())
	{
		while (getline(iFile,line))
		{
			if (line.find("template") != std::string::npos)
			{
				line.pop_back();
				oFile << line + " " + a + "\n";
			}
			else
				oFile << line;
		}
		oFile.close();
		iFile.close();
	}
	*/
	
	// Optional: Create easy way to upload to github
	
	
	
	
	return 0;
	//const char b = argv[1];
	//std::cout << strlen(b) << std::endl;
	
}
// End


