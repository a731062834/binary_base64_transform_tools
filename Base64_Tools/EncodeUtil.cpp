#include "EncodeUtil.h"

#if defined(WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

#ifndef return_std_move
#if (_MSC_VER >= 1700) //Visual Studio 2012
#define return_std_move(value) return std::move(value)
#else
#define return_std_move(value) return value
#endif
#endif

#include <vector>

//Base64±àÂë½âÂë±í
static const std::string base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool is_base64(unsigned char c) 
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string EncodeUtil::Base64Encode(const std::string& str)
{
	int len = (int)str.length();
	if (len <= 0)
	{
		return "";
	}

	return Base64Encode(reinterpret_cast<const unsigned char*>(str.c_str()), len);
}

std::string EncodeUtil::Base64Encode(unsigned char const* bytesToEncode, unsigned int len)
{
	std::string ret;
	if (!bytesToEncode || len < 0)
	{
		return ret;
	}

	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (len--) 
	{
		char_array_3[i++] = *(bytesToEncode++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}

	return_std_move(ret);
}

std::string EncodeUtil::Base64Decode(const std::string& strSource)
{
	std::string ret;
	FastData data = Base64DecodeToBytes(strSource);
	if (!data.isNull())
	{
		ret = std::string((const char*)data.getBytes(), data.getSize());
	}
	return_std_move(ret);
}

FastData EncodeUtil::Base64DecodeToBytes(const std::string& strSource)
{
	int in_len = strSource.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::vector<char> ret;

	while (in_len-- && (strSource[in_] != '=') && is_base64(strSource[in_]))
	{
		char_array_4[i++] = strSource[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret.push_back(char_array_3[i]);
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
	}
	FastData data;
	if (ret.size() > 0)
	{
		data.copy((const unsigned char*)&ret[0], ret.size());
	}
	return_std_move(data);
}

std::string EncodeUtil::GetUTF8Bytes(std::string& str)
{
	return str;
}

std::string EncodeUtil::GB2312ToUTF8(const std::string &gb2312)
{
	std::string utf8String = "";
    
#if _WIN32 || _WIN64
	FastData utf8Data = GB2312ToUTF8(gb2312.c_str(), gb2312.length());
	if (!utf8Data.isNull())
	{
		utf8String = std::string((const char*)utf8Data.getBytes(), utf8Data.getSize());
	}
	return_std_move(utf8String);
#else
    return gb2312;
#endif
}

FastData EncodeUtil::GB2312ToUTF8(const char* gb2312Data, int gb2312len)
{
	FastData utf8Data;

	if (!gb2312Data || gb2312len <= 0)
	{
		return utf8Data; // isNull == true
	}
	
#if _WIN32 || _WIN64
	int unicodelen = ::MultiByteToWideChar(CP_ACP, 0, gb2312Data, gb2312len, 0, 0);
	wchar_t *unicode = new wchar_t[unicodelen + 1];
	memset(unicode, 0, unicodelen + 1);
	::MultiByteToWideChar(CP_ACP, 0, gb2312Data, gb2312len, unicode, unicodelen);
	int utf8len = ::WideCharToMultiByte(CP_UTF8, 0, unicode, unicodelen, 0, 0, 0, 0);
	char *utf8 = new char[utf8len + 1];
	memset(utf8, 0, utf8len + 1);
	::WideCharToMultiByte(CP_UTF8, 0, unicode, unicodelen, utf8, utf8len, 0, 0);
	if (unicode)
	{
		delete[] unicode;
		unicode = 0;
	}

	utf8Data.copy((unsigned char*)utf8, utf8len);
	delete[] utf8;
	utf8 = 0;
#endif
    
	return_std_move(utf8Data);
}

std::string EncodeUtil::UTF8ToGB2312(const std::string &utf8)
{
	std::string ansiString = "";
	FastData ansiData = UTF8ToGB2312(utf8.c_str(), utf8.length());
	if (!ansiData.isNull())
	{
		ansiString = std::string((const char*)ansiData.getBytes(), ansiData.getSize());
	}
	return_std_move(ansiString);
}

FastData EncodeUtil::UTF8ToGB2312(const char* utf8, int utf8len)
{
	FastData ansiData;
	if (!utf8 || utf8len <= 0)
	{
		return ansiData; // isNull == true
	}
#if _WIN32 || _WIN64
	int ansilen = ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[ansilen + 1];
	memset(wstr, 0, ansilen + 1);
	::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, ansilen);
	ansilen = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[ansilen];
	memset(str, 0, ansilen);
	::WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, ansilen, NULL, NULL);
	if (wstr)
	{
		delete[] wstr;
		wstr = 0;
	}
	ansiData.copy((unsigned char*)str, ansilen - 1);
	if (str)
	{
		delete[] str;
		str = 0;
	}
#endif
	return_std_move(ansiData);
}

char EncodeUtil::ToHex( unsigned char x )
{
	return  x > 9 ? x + 55 : x + 48;
}

char EncodeUtil::FromHex( unsigned char x )
{
	char y = char(0xff); // init with default data, otherwise it could crash in exception case.

	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
	else if (x >= '0' && x <= '9') y = x - '0';  
	return y;  
}

std::string EncodeUtil::UnicodeToUTF8(const std::wstring& unicodeString)
{
	std::string ret;

#if _WIN32 || _WIN64
	do
	{
		if (unicodeString.empty())
			break;

		size_t len = unicodeString.length();
		size_t convertedChars = 0;

		char * pszUtf8Str = new char[len * 3 + 1];
		if (!pszUtf8Str)
			break;

		memset(pszUtf8Str, 0, len * 3 + 1);

		WideCharToMultiByte(CP_UTF8, 0, unicodeString.c_str(), len + 1, pszUtf8Str, len * 3 + 1, 0, 0);
		ret = pszUtf8Str;

		delete[] pszUtf8Str;
		pszUtf8Str = 0;
	} while (false);
#endif

	return ret;
}

std::wstring EncodeUtil::UTF8ToUnicode(const std::string& utf8String, unsigned int len /*= -1*/)
{
	std::wstring ret;
    
#if _WIN32 || _WIN64

	do
	{
		if (utf8String.empty())
		{
			break;
		}
		// get UTF8 string length
		if (len <= -1)
		{
			len = utf8String.length();
		}
		else if (len > utf8String.length())
		{
			len = utf8String.length();
		}

		// get UTF16 string length
		int wLen = MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), len, 0, 0);
		if (0 == wLen || 0xFFFD == wLen) 
			break;

		// convert string  
		wchar_t * pwszStr = new wchar_t[wLen + 1];
		if (!pwszStr) 
			break;

		wmemset(pwszStr, 0, wLen + 1);
		
		MultiByteToWideChar(CP_UTF8, 0, utf8String.c_str(), len, pwszStr, wLen + 1);
		ret = pwszStr;

		delete[] pwszStr;
		pwszStr = 0;

	} while (0);
#endif

	return_std_move(ret);
}

std::string EncodeUtil::ToHexString(const std::string& normalString)
{
	std::string hexString;
	for (size_t index = 0; index < normalString.length(); ++index)
	{
		unsigned char c = (unsigned char)normalString[index];
		hexString += ToHex((unsigned char)c >> 4);
		hexString += ToHex((unsigned char)c % 16);
	}
	return_std_move(hexString);
}

std::string EncodeUtil::FromHexString(const std::string& hexString)
{
	std::string normalString;
	int length = (int)hexString.length();
	if (length % 2 != 0)
	{
		length -= 1;
	}
	for (int i = 0; i < length; i += 2)
	{
		unsigned char high = FromHex(hexString[i]);
		unsigned char low = FromHex(hexString[i + 1]);
		normalString += high * 16 + low;
	}
	return_std_move(normalString);
}

std::string EncodeUtil::ToHexString(const FastData& normalData)
{
	std::string hexString = "";
	if (normalData.isNull())
	{
		return hexString;
	}
	std::vector<char> hexVec;
	for (size_t i = 0; i < normalData.getSize(); ++i)
	{
		unsigned char c = normalData.getBytes()[i];
		hexString += ToHex(c >> 4);
		hexString += ToHex(c & 0x0F);
	}
	return_std_move(hexString);
}

FastData EncodeUtil::FromHexData(const FastData& hexData)
{
	FastData normalData;
	if (hexData.isNull())
	{
		return normalData;
	}

	int length = (int)hexData.getSize();
	if (length % 2 != 0)
	{
		length -= 1;
	}

	std::vector<unsigned char> normalVec;
	for (int i = 0; i < length; i += 2)
	{
		unsigned char high = FromHex(hexData.getBytes()[i]);
		unsigned char low = FromHex(hexData.getBytes()[i + 1]);
		normalVec.push_back(high * 16 + low);
	}

	if ((int)normalVec.size() > 0)
	{
		normalData.copy(&normalVec[0], normalVec.size());
	}
	return_std_move(normalData);
}

std::string EncodeUtil::UrlEncode( const std::string& str )
{
	std::string encodedString = "";
	size_t length = str.length();  
	for (size_t i = 0; i < length; i++)  
	{  
		if (isalnum((unsigned char)str[i]) ||   
			(str[i] == '-') ||  
			(str[i] == '_') ||   
			(str[i] == '.') ||   
			(str[i] == '~'))  
			encodedString += str[i];  
		else if (str[i] == ' ')  
			encodedString += "+";  
		else  
		{  
			encodedString += '%';  
			encodedString += ToHex((unsigned char)str[i] >> 4);  
			encodedString += ToHex((unsigned char)str[i] % 16);  
		}  
	}  
	return_std_move(encodedString);
}

std::string EncodeUtil::UrlDecode( const std::string& str )
{
	std::string decodedString = "";
	size_t length = str.length();  
	try
	{
		for (size_t i = 0; i < length; i++)
		{
			if (str[i] == '+') decodedString += ' ';
			else if (str[i] == '%')
			{
				unsigned char high = FromHex((unsigned char)str[++i]);
				unsigned char low = FromHex((unsigned char)str[++i]);
				decodedString += high * 16 + low;
			}
			else decodedString += str[i];
		}
	}
	catch (...)
	{
		decodedString = "";
	}	
	return_std_move(decodedString);
}
