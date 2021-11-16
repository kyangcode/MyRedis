#pragma once
#include <cstddef>

#define SDS_MAX_PREALLOC 1024 * 1024 // 预分配最大的字节数

namespace std
{
	class SDS
	{
	public:
		SDS();
		SDS(const string &s);
		~SDS();

		size_t len() const; // 返回sds长度
		size_t avail() const; // 返回sds未使用空间字节数
		
		void clear(); // 清空sds

		SDS &cat(const char *str); // 将c字符串拼接到sds后面
		SDS &catSDS(const SDS &sds); // 将指定sds拼接到当前sds后面
		
		SDS &cpy(const SDS &sds); // 用指定sds覆盖当前sds
		
		bool cmp(const SDS &sds) const; // 判断两个sds是否相等
		
		SDS &trim(const char *str); // sds修剪两边在str出现过的字符 
		
		void range(size_t start, size_t end); // 除区间中其他字符清空
		
		SDS &growZero(size_t len); // 将字符串用空字符填充到指定长度
		
	private:
		void makeRoomFor(size_t len);
	private:
		size_t len; // 字符串长度
		size_t free; // 空闲字节数
		char* buf; // 字节数组 
	};
	SDS &dup(const SDS &sds);// 创建一个sds的副本
}

