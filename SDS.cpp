#include "SDS.h"
#include <cstring>

namespace std
{
	SDS::SDS() : len(0), free(0)
	{
		buf = new char[1]{'\0'};
	}

	SDS::SDS(const string &s) : free(0)
	{
		len = s.length();
		buf = new char[len + 1]{};
		strcpy(buf, str);
	}
	
	SDS::SDS(const SDS &sds)
	{
		buf = new char[sds.len + sds.free + 1]{};
		memmove(buf, sds.buf, sds.len + 1);		
		len = sds.len;
		free = sds.free;
	}

	SDS::~SDS()
	{
		delete[] buf;
	}
	
	void SDS::makeRoomFor(size_t newLen)
	{
		size_t bufSize = 0;
		if (newLen > SDS_MAX_PREALLOC)
		{
			bufSize = newLen + SDS_MAX_PREALLOC + 1
		}
		else
		{
			bufSize = newLen * 2 + 1;
		}
		
		char *newBuf = new char[bufSize]{};
		
		memcpy(newBuf, buf, len);
		delete buf;

		buf = newBuf;	 
		free = bufSize - len - 1;	
	}

	size_t SDS::len() const
	{
		return len;
	}

	size_t SDS::avail() const
	{
		return free;
	}

	void SDS::clear()
	{
		free += len + 1;
		len = 0;
		buf[0] = '\0';
	}	
	
	SDS& SDS::cat(const string &s)
	{
		if (s.empty())
		{
			return *this;
		}
		size_t strLen = s.length();
		size_t newLen = len + strLen;
		if (strLen > free)
		{
			makeRoomFor(newLen);
		}
		strcat(buf + len, s.c_str());
		len = newLen;
		free -= strLen;
		return *this;
	}
	
	SDS& SDS::catSDS(const SDS &sds)
	{
		size_t strLen = sds.len();
		if (strLen == 0)
		{
			return *this;
		}

		size_t newLen = len + strLen;
		if (strLen > free)
		{
			makeRoomFor(newLen);
		}	

		memcpy(buf + len, sds.buf, strLen);
		len = newLen;
		free -= strLen;	
		return *this;					
	}

	SDS& SDS::cpy(const SDS &sds)
	{
		size_t strLen = sds.len();
		if (strLen > free + len)		
		{
			 makeRoomFor(strLen); 			
		}
		
		memcpy(buf, sds.buf, strLen);
		free = free + len - strLen;		
		len = strLen;		
		return *this;
	}
	
	bool SDS::cmp(const SDS &sds) const
	{
		if (len != sds.len)
		{
			return false;
		}
		return memcmp(buf, sds.buf, len) == 0;		
	}
	
	SDS& SDS::trim(const char *str)
	{
		size_t left = 0;
		size_t right = len - 1;
		while (left <= right && strchr(str, buf[left])) left++;
		while (right >= left && strchr(str, buf[right])) right--;

		size_t newLen = left > right ? 0 : right - left + 1;
		memmove(buf, buf + left, newLen);
		free += len - newLen;
		return *this;
	}
	
	void SDS::range(size_t start, size_t end)
	{
		if (start < 0) start = len + start;
		if (end < 0) end = len + end;

		memmove(buf, buf + start, end - start);				
		free = len - (end - start);
		len = end - start;
		return *this;
	}

	SDS& SDS::growZero(size_t newLen)
	{
		if (newLen <= len)
		{
			return *this;
		}	
		
		if (newLen - len > free)
		{
			makeRoomFor(newLen);
		}
		
		for (int i = len; i < newLen; i++)
		{
			buf[i] = '\0';
		}
		free = free - (newLen - len);
		len = newLen;
		return *this;		
	}
	
	SDS &dup(const SDS &sds)
	{
		return *(new SDS(sds));
	}
