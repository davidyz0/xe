#pragma once
#include "common.h"
#include "container/vector.h"
#include "container/map.h"

char xe_string_tolower(char c);

size_t xe_string_length(xe_cstr str);

xe_cstr xe_string_find(xe_cstr ptr, char c, size_t n);

int xe_string_compareCase(xe_cstr s1, xe_cstr s2, size_t n);
int xe_string_compareCasez(xe_cstr s1, xe_cstr s2, size_t n);
int xe_string_compare(xe_cstr s1, xe_cstr s2, size_t n);
int xe_string_comparez(xe_cstr s1, xe_cstr s2, size_t n);

static inline char xe_ctoi(char c){
	return c - '0';
}

static inline bool xe_cisi(char c){
	return c >= '0' && c <= '9';
}

static inline int xe_hex(char c){
	if(xe_cisi(c))
		return xe_ctoi(c);
	if(c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	if(c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	return -1;
}

class xe_string : public xe_array<char>{
protected:
	using xe_array<char>::_data;
	using xe_array<char>::_size;
public:
	using xe_array<char>::data;
	using xe_array<char>::max_size;
	using xe_array<char>::at;
	using xe_array<char>::free;

	xe_string();
	xe_string(xe_string&& src);
	xe_string& operator=(xe_string&& other);

	xe_string(const xe_string& other) = default;
	xe_string& operator=(const xe_string& other) = default;

	xe_string(xe_cptr string);
	xe_string(xe_cptr string, size_t len);

	xe_string(xe_vector<char>&& vec);

	xe_string& operator=(xe_cptr string);

	constexpr inline xe_cstr data() const{
		return _data;
	}

	constexpr inline xe_cstr c_str() const{
		return _data;
	}

	constexpr inline size_t length() const{
		return _size;
	}

	size_t indexOf(char c) const;
	size_t indexOf(char c, size_t off) const;

	bool operator==(const xe_string& o) const;
	bool operator==(xe_cptr o) const;

	bool equal(const xe_string& o) const;
	bool equalCase(const xe_string& o) const;

	bool equal(xe_cptr o) const;
	bool equalCase(xe_cptr o) const;

	bool copy(xe_cptr src, size_t n);
	bool copy(xe_cptr src);
	bool copy(const xe_string& src);

	void clear();

	constexpr inline size_t hash() const{
		return xe_hash::hash_bytes(c_str(), length());
	}

	xe_string substring(size_t start);
	xe_string substring(size_t start, size_t end);

	static size_t length(xe_cptr str);

	static bool equal(const xe_string& a, const xe_string& b);
	static bool equalCase(const xe_string& a, const xe_string& b);
};