#include <time.h>
#include <string>

#ifndef _PSTRING
#define _PSTRING

class string : public std::string
{
	const static long SCOPE = 64L;

public:
	string() : std::string() { reset(); }
	string(const string &source) : std::string(source) { }
	string(const char *source) : std::string(source) { }
	string(const std::string &source) : std::string(source) { }
	string(long size) : std::string() { resize(size); }
	string(const char *source, long count) : std::string(source, count) { }

	void reset() { (*this) = std::string(); }

	void concat(string source) { *this += source; }
	void concat(char *source) { *this += source; }
	void concat(char *source, long len) { append(source, len); }

	long count() const { return (long)this->length(); }

	bool toChar(char *destination, long length);

	string upper();
	string lower();

	long hash();

	string ltrim() { return ltrim(' '); }
	string rtrim() { return rtrim(' '); }
	string trim() { return ltrim(' ').rtrim(' '); }

	string ltrim(char character);
	string rtrim(char character);
	string trim(char character) { return ltrim(character).rtrim(character); }

	string replace(char find, char swap);
	string replace(char find, string &swap);

	void copy(const string &source) { (*this) = string(source); }
	void copy(const char *source) { (*this) = string(source); }
	bool copy(const char *source, long count) { (*this) = string(source, count); return true; }

	static string randBase64();

	bool isIn(string source) { return indexOf(source) != string::npos; }
	long indexOf(string source) { return (long)find(source); }

	long split(char find, string *destination, long elements) const;
	string subString(long index, long len) { return std::string::substr(index, len); }

	bool icompare(string a);

	float match(string source);
	float match(string source, int *offsets, long len, long scope = SCOPE);

	float toFloat();
	double toDouble();
	long toLong();
	long toLongFromHex();
	int toInteger() { return (int)toLong(); }
	bool toBoolean();
	string toBase64();
	string fromBase64();

	bool isFloat();
	bool isInteger();
	bool isHex();

public:
	static string fromFloat(float source);
	static string fromLong(long source);
	static string fromInt(int source);
	static string fromBool(bool source);
	static string fromTime(time_t source);

public:
	bool operator==(const string &source);

public:
	string operator<<(string &source);
	string operator<<(char *source);
	string operator<<(float source);
	string operator<<(long source);
	string operator<<(int source);
	string operator<<(bool source);
};

#endif