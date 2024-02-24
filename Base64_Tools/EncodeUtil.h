#ifndef __ENCODE_UTIL_H_
#define __ENCODE_UTIL_H_

#include "FastData.h"

class EncodeUtil
{
public:
	//±àÂë
	static std::string Base64Encode(const std::string& str);
	static std::string Base64Encode(unsigned char const* bytesToEncode, unsigned int len);

	//½âÂë
	static std::string Base64Decode(const std::string& str);
	static FastData Base64DecodeToBytes(const std::string& str);

	static std::string GetUTF8Bytes(std::string& str);

	static std::string GB2312ToUTF8(const std::string &gb2312);
	static FastData GB2312ToUTF8(const char* gb2312Data, int gb2312len);

	static std::string UTF8ToGB2312(const std::string &utf8);
	static FastData UTF8ToGB2312(const char* utf8, int utf8len);

	static std::string UnicodeToUTF8(const std::wstring& unicodeString);
	static std::wstring UTF8ToUnicode(const std::string& utf8String, unsigned int len = -1);

	static char ToHex(unsigned char x);

	static char FromHex(unsigned char x);

	static std::string ToHexString(const std::string& normalString);
	static std::string FromHexString(const std::string& hexString);

	static std::string ToHexString(const FastData& normalData);
	static FastData FromHexData(const FastData& hexData);

	static std::string UrlEncode(const std::string& str);
	static std::string UrlDecode(const std::string& str);
};

#ifndef Utf8Text
#define Utf8Text(text) \
	EncodeUtil::GB2312ToUTF8(text)
#endif

#endif // !__ENCODE_UTIL_H_
