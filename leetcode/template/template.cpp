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

std::string GoToParentFolder(const std::string& str)
{
	std::size_t found = str.find_last_of("/\\");
	std::string new_str = str.substr(0,found);
	return new_str;
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
	std::string id,line,iPath,oPath,iMakePath,oMakePath,name,lcname;
	argv[1] ? id.assign(argv[1],cStrLen(argv[1])) : id = "";
	name = "template";
	lcname = id + "#lc";
	
	// Creating main program for leetcode problem
	iPath = ExePath() + "\\" + name + ".txt";
	oPath = GoToParentFolder(ExePath()) + "\\" + lcname + ".cpp";
	
	if (!fexists(oPath))
	{
		std::cout << "Creating new template for ID: " + id << std::endl;
		std::ifstream iFile(iPath);
		std::ofstream oFile(oPath, std::ios::out);
		
		if (iFile.is_open() && oFile.is_open())
		{
			while (getline(iFile,line))
			{
				if (line.find("Leetcode ID:") != std::string::npos)
				{
					line.pop_back();
					line = line + " " + id + "\n";
				}
				oFile << line;
			}
			oFile.close();
			iFile.close();
		}
	}
	else
		std::cout << "template for ID: " + id + " already exists" << std::endl;
	
	
	// Change makefile to fit current id
	iMakePath = ExePath() + "\\makefile.txt";
	oMakePath = GoToParentFolder(ExePath()) + "\\makefile";
	
	std::ifstream iMakeFile(iMakePath);
	std::ofstream oMakeFile(oMakePath, std::ios::out | std::ios::trunc);
	
	std::cout << "Creating makefile" << std::endl;
	if (iMakeFile.is_open() && oMakeFile.is_open())
	{
		while (getline(iMakeFile,line))
		{
			while (line.find(name) != std::string::npos)
			{
				int len = name.length();
				int find = line.find(name);
				line.replace(find,len,lcname);
			}
			oMakeFile << line;
		}
		oMakeFile.close();
		iMakeFile.close();
	}
	std::cout << "Finished" << std::endl;
	// Optional: Create easy way to upload to github
	
	return 0;
	//const char b = argv[1];
	//std::cout << strlen(b) << std::endl;
	
}
// End


