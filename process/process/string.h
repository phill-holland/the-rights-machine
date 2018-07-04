#include <time.h>
#include <string>

#if !defined(__STRING)
#define __STRING

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

	void concat(string &source) { *this += source; }
	void concat(char *source) { *this += source; }
	void concat(char *source, long len) { append(source, len); }

	long count() { return (long)this->length(); }

	bool toChar(char *destination, long length);

	void toUpper();
	void toLower();

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

	bool isIn(string &source) { return indexOf(source) != string::npos; }
	long indexOf(string &source) { return (long)find(source); }

	long split(char find, string *destination, long elements);
	string subString(long index, long len) { return std::string::substr(index, len); }

	float match(string &source);
	float match(string &source, int *offsets, long len, long scope = SCOPE);

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

	static string fromFloat(float source);
	static string fromLong(long source);
	static string fromInt(int source);
	static string fromBool(bool source);
	static string fromTime(time_t source);

	bool operator==(const string &source) { return compare(source) == 0; }

	string operator<<(string &source) { this->concat(source); return *this; }
	string operator<<(char *source) { this->concat(source); return *this; }
	string operator<<(float source) { this->concat(string::fromFloat(source)); return *this; }
	string operator<<(long source) { this->concat(string::fromLong(source)); return *this; }
	string operator<<(int source) { this->concat(string::fromInt(source)); return *this; }
	string operator<<(bool source) { this->concat(string::fromBool(source)); return *this; }
	string operator<<(time_t source) { this->concat(string::fromTime(source)); return *this; }
};

#endif