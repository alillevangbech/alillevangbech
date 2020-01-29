#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>

bool fexists (const std::string& name) {
    ifstream f(name.c_str());
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
	std::string a,line,iPath,oPath;
	argv[1] ? a.assign(argv[1],cStrLen(argv[1])) : a = "";
	
	// Creating main program for leetcode problem
	iPath = ExePath() + "\\template.txt";
	oPath = ExePath() + "\\" + a + "_lc.cpp";
	
	if (fexists(oPath))
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
	
	
	
	return 0;
	//const char b = argv[1];
	//std::cout << strlen(b) << std::endl;
	
}
// End


