#ifndef __FILEUITL_H__
#define __FILEUTIL_H__

#include <string>
#include <vector>

class FileUtil
{
public:
	static bool isDir(const char* pDir);

	static bool createDir(const char* pPath);

	static bool isExistPath(const char* pPath);

	static bool deleteDir(const char* pDir);

	static void moveFile(const char* from, const char* to);

	static bool ListAllFileInDirectory(const char* szPath, std::vector<std::string>& vecFiles);

	static bool saveFile(const char* path, const char* data, int len);

	static bool appendFile(const char* path, char* data, int len);

	static bool readFile(const char* path, std::vector<char>& vecData);
	
	static bool readFile(const char* path, std::vector<unsigned char>& vecData);

	static bool checkAndCreateDir(const char* path);

	static std::string  getAppPath();

private:
	FileUtil();
	~FileUtil();

	FileUtil(const FileUtil&);
	FileUtil& operator=(const FileUtil&);
};

#endif // !__FILEUITL_H__
