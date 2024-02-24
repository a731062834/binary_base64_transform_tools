#include "FileUtil.h"
#include <stdio.h>
#include <algorithm>

#ifdef WIN32	

#include <Windows.h>
#include <direct.h>

bool FileUtil::isDir(const char *pDir)  
{  
	char szCurPath[500];  

	ZeroMemory(szCurPath, 500);  

	sprintf_s(szCurPath, 500, "%s/*", pDir);

	WIN32_FIND_DATAA FindFileData;        
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  

	HANDLE hFile = FindFirstFile((LPCSTR)szCurPath, (WIN32_FIND_DATAA *)&FindFileData);   

	if( hFile == INVALID_HANDLE_VALUE )   
	{  
		FindClose(hFile);	
		return FALSE;   
	}
	else  
	{     
		FindClose(hFile);  
		return TRUE;  
	}  
} 

bool FileUtil::createDir(const char* pPath)
{
	WIN32_FIND_DATA		fileinfo;
	HANDLE hFile = FindFirstFile(pPath, &fileinfo);

	// if the file exists and it is a directory
	if(fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFile);
		return true;
	}

	FindClose(hFile);

	// Parse the supplied CString Directory String

	int nLength = strlen(pPath);

	std::vector<std::string> vecPath;

	int nIndex = 0;
	std::string strTemp;
	for(; nIndex < nLength; nIndex++)
	{
		// If the character is not a '\', add it to csTemp
		if(pPath[nIndex] != '/')
		{
			strTemp += pPath[nIndex];
		}
		else
		{
			vecPath.push_back(strTemp);
			strTemp += '/';
		}

		// If we reached the end of the file add the remaining string
		if(nIndex == (nLength - 1))
		{
			vecPath.push_back(strTemp);
		}
	}

	for(nIndex = 1; nIndex < vecPath.size(); nIndex++)
	{
		strTemp = vecPath[nIndex];

		BOOL bRet = CreateDirectory(strTemp.c_str(), NULL);

		if(TRUE == bRet)
		{
			SetFileAttributes(strTemp.c_str(), FILE_ATTRIBUTE_NORMAL);
		}
	}
	
	vecPath.clear();

	hFile = FindFirstFile(pPath, &fileinfo);

	FindClose(hFile);
	
	return (fileinfo.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY);
}

bool FileUtil::isExistPath(const char* pPath)
{
	DWORD dwAttrib = GetFileAttributes(pPath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib;
}

bool FileUtil::deleteDir(const char * DirName)  
{
	char szCurPath[MAX_PATH];       
	_snprintf(szCurPath, MAX_PATH, "%s/*.*", DirName); 

	WIN32_FIND_DATAA FindFileData;        
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  

	HANDLE hFile = FindFirstFile((LPCSTR)szCurPath, (LPWIN32_FIND_DATAA)&FindFileData);  
    bool IsFinded = true;

	while(IsFinded)  
	{
		IsFinded = FindNextFile(hFile, (LPWIN32_FIND_DATAA)&FindFileData); 

		if( strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..") ) 
		{  
			std::string strFileName = "";  
			strFileName = strFileName + DirName + "/" + FindFileData.cFileName;  

			std::string strTemp;  
			strTemp = strFileName;  

			if( isDir(strFileName.c_str()) ) 
			{     

				deleteDir(strTemp.c_str());  
			}  
			else  
			{  

				DeleteFile((LPCSTR)strTemp.c_str());  
			}  
		}  
	}  
	FindClose(hFile);

	RemoveDirectory((LPCSTR)DirName);

	return TRUE;  
}

void FileUtil::moveFile(const char* from, const char* to)
{
	::MoveFile((LPCSTR)from,(LPCSTR)to);
}

bool FileUtil::ListAllFileInDirectory(const char* szPath, std::vector<std::string>& vecFiles)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	TCHAR szFilePath[MAX_PATH];
	TCHAR szFullPath[MAX_PATH];

	lstrcpy(szFilePath, szPath);
	lstrcat(szFilePath,TEXT("/*"));
	int dwTotalFileNum = 0;
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	if(hListFile == INVALID_HANDLE_VALUE)
	{
		printf("错误: %d\n", GetLastError());
		return false;
	}
	else
	{
		do
		{
			if(lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

			sprintf(szFullPath, "%s/%s", szPath, FindFileData.cFileName);

			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				printf("<DIR>");
				ListAllFileInDirectory(szFullPath, vecFiles);
			}
			else
			{
				//文件  
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
				{
					//系统文件
				}
				else
				{
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					{
						//隐藏文件  
					}
					else
					{
						//普通文件  
						std::string strFile(szFullPath);
						if (strFile.size() > 4 && strFile.substr(strFile.size()-4) == TEXT(".bmp"))
						{
							dwTotalFileNum++;
							vecFiles.push_back(std::string(szFullPath));
						}
					}
				}
			}
		}while(FindNextFile(hListFile,&FindFileData));
	}

	return true;
}

bool FileUtil::saveFile(const char* path, const char* data, int len)
{
	if (!path || !data || len <= 0)
	{
		return false;
	}

	FILE* fp = fopen (path, "wb");
	if (!fp)
	{
		return false;
	}

	fwrite(data, sizeof(char), len, fp);
	
	fclose(fp);

	return true;
}

bool FileUtil::appendFile(const char* path, char* data, int len)
{
	FILE* fp = fopen (path, "ab+");
	if (!fp)
	{
		return false;
	}

	fwrite(data, sizeof(char), len, fp);
	fclose(fp);

	return true;
}

bool FileUtil::readFile(const char* path, std::vector<char>& vecData)
{
	FILE* fpFile = fopen(path, "rb");
	if (!fpFile)
	{
		return false;
	}

	fseek(fpFile, 0, SEEK_END);
	int length = ftell(fpFile);

	fseek(fpFile, 0, SEEK_SET);

	vecData.resize(length);
	fread(vecData.data(), length, 1, fpFile);

	fclose(fpFile);

	return true;
}

bool FileUtil::readFile(const char* path, std::vector<unsigned char>& vecData)
{
	std::vector<char> vecTmpData;
 	bool bRet = readFile(path, vecTmpData);
	if (!bRet)
	{
		return false;
	}

	vecData.resize(vecTmpData.size());
	vecData.assign(vecTmpData.begin(), vecTmpData.end());

	return true;
}

bool FileUtil::checkAndCreateDir(const char* path)
{
	if (!isExistPath(path))
	{
		createDir(path);
	}

	return true;
}

std::string FileUtil::getAppPath()
{
	char exeFullPath[MAX_PATH]; 

	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	_splitpath(exeFullPath, drive, dir, NULL,NULL);
	std::string  strAppPath(drive);
	strAppPath += dir;

	std::replace(strAppPath.begin(), strAppPath.end(), '\\', '/');
	return strAppPath;
}

#else	

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <limits.h>

bool isDir(const char *path)
{
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)
	{
		return S_ISDIR(statbuf.st_mode) != 0;
	}
	return false;
}

bool is_file(const char *path)
{
	struct stat statbuf;
	if(lstat(path, &statbuf) ==0)
		return S_ISREG(statbuf.st_mode) != 0;
	return false;
}

bool is_special_dir(const char *path)
{
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

void get_file_path(const char *path, const char *file_name,  char *file_path)
{
	strcpy(file_path, path);
	if(file_path[strlen(path) - 1] != '\\' && file_path[strlen(path) - 1] != '/')
	{
		strcat(file_path, "/");
	}
	strcat(file_path, file_name);
}

bool deleteDir(const char *path)
{
	DIR *dir;
	dirent *dir_info;
	char file_path[PATH_MAX];
	if(is_file(path))
	{
		remove(path);

        return true;
	}

	if(isDir(path))
	{
		if((dir = opendir(path)) == NULL)
        {
            return false;
        }

		while((dir_info = readdir(dir)) != NULL)
		{
			get_file_path(path, dir_info->d_name, file_path);
			if(is_special_dir(dir_info->d_name))
				continue;
			deleteDir(file_path);
			rmdir(file_path);
		}
	}

    return true;
}
void move_file_(char *from,char *to)
{
	std::string cmd = std::string("mv ")+std::string(from)+std::string(" ")+std::string(to);
	system(cmd.c_str());
}
#endif
