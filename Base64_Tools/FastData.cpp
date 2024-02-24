#include "FastData.h"

const FastData FastData::Null;

FastData::FastData() :
_bytes(0),
_size(0)
{
}

#if (_MSC_VER >= 1700) //Visual Studio 2012
FastData::FastData(FastData&& other) :
_bytes(0),
_size(0)
{
    move(other);
}
#endif

FastData::FastData(const FastData& other) :
_bytes(0),
_size(0)
{
    copy(other._bytes, other._size);
}

FastData::~FastData()
{
    clear();
}

FastData& FastData::operator= (const FastData& other)
{
    copy(other._bytes, other._size);
    return *this;
}

#if (_MSC_VER >= 1700) //Visual Studio 2012
FastData& FastData::operator= (FastData&& other)
{
    move(other);
    return *this;
}
#endif

void FastData::move(FastData& other)
{
    clear();
    
    _bytes = other._bytes;
    _size = other._size;

    other._bytes = 0;
    other._size = 0;
}

bool FastData::isNull() const
{
    return (_bytes == 0 || _size == 0);
}

unsigned char* FastData::getBytes() const
{
    return _bytes;
}

size_t FastData::getSize() const
{
    return _size;
}

void FastData::copy(const unsigned char* bytes, const size_t size)
{
    clear();

    if (size > 0)
    {
        _size = size;
        _bytes = (unsigned char*)malloc(sizeof(unsigned char) * _size);
        memcpy(_bytes, bytes, _size);
    }
}

void FastData::fastSet(unsigned char* bytes, const size_t size)
{
    _bytes = bytes;
    _size = size;
}

void FastData::clear()
{
    free(_bytes);
    _bytes = 0;
    _size = 0;
}

unsigned char* FastData::takeBuffer(size_t* size)
{
    unsigned char* buffer = getBytes();
    if (size)
        *size = getSize();
    fastSet(0, 0);
    return buffer;
}