#include "custom/string.h"
#include <stdio.h>
#include <memory.h>
#include <random>

char base64[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

bool string::toChar(char *destination, long length)
{
	bool result = true;

	long temp = count();
	if (temp > length - 1L) { temp = length - 1L; result = false; }

	memcpy(destination, c_str(), temp);

	return result;
}

string string::upper()
{
	string result;

	for (long index = 0L; index < count(); ++index)
	{
		if (((*this)[index] >= 'a') && ((*this)[index] <= 'z'))
			result += (*this)[index] - 32;
		else result += (*this)[index];
	}

	return result;
}

string string::lower()
{
	string result;

	for (long index = 0L; index < count(); ++index)
	{
		if (((*this)[index] >= 'A') && ((*this)[index] <= 'Z'))
			result += (*this)[index] + 32;
		else result += (*this)[index];
	}

	return result;
}

long string::hash()
{
	return (long)std::hash<std::string>()(*this);
}

string string::ltrim(char character)
{
	if (character == 0) return *this;
	if (count() == 0L) return *this;

	bool begin = false;
	long i = 0L, j = 0L;
	string result;

	while ((i < count()) && (!begin))
	{
		char temp = (*this)[i];
		if (temp != character) { j = i; begin = true; }
		++i;
	}

	result = (*this).substr(j, count() - j);

	return result;
}

string string::rtrim(char character)
{
	if (character == 0) return *this;
	if (count() == 0L) return *this;

	char temp;
	long reverse = 0L;
	string result;

	do
	{
		temp = (*this)[count() - 1L - reverse];
		if (temp == character) ++reverse;
	} while (temp == character);

	result = (*this).substr(0, count() - reverse);

	return result;
}

string string::replace(char find, char swap)
{
	if (find == 0) return string();
	string result;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (temp == find) result.concat(((char*)&swap), 1L);
		else result += temp;
	}

	return result;
}

string string::replace(char find, string &swap)
{
	if (find == 0) return string();
	string result;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (temp == find) result.concat(swap);
		else result += temp;
	}

	return result;
}

string string::randBase64()
{
	std::random_device rd;
	std::mt19937_64 generator{ rd() };
	std::uniform_int_distribution<int> dist{ 0, 0x7fff };

	return string::fromInt(dist(generator)).toBase64();
}

long string::split(char find, string *destination, long elements) const
{
	long element = 0L;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (temp != find)
		{
			if (element < elements) destination[element] += temp;
		}
		else if (destination[element].count() > 0L)
		{
			++element;
			if (element == elements) return elements;
		}

	}

	if (destination[element].count() > 0L) ++element;

	return element;
}

bool string::icompare(string a)
{
	auto upper = [](char source)
	{
		char result = source;
		if ((source >= 'a') && (source <= 'z'))
			result = source - 32;

		return result;
	};

	if ((count() == 0L) || (a.count() == 0L)) return false;
	if (count() != a.count()) return false;

	for (long i = 0L; i < count(); ++i)
	{
		if (upper((*this)[i]) != upper(a[i])) return false;
	}

	return true;
}

float string::match(string source)
{
	auto upper = [](char source)
	{
		char result = source;
		if ((source >= 'a') && (source <= 'z'))
			result = source - 32;

		return result;
	};

	if ((count() == 0L) || (source.count() == 0L)) return 0.0f;

	float score = 0.0f;
	long result = 0L;
	long j = 0L;

	const long TOTAL = 256L;
	long hashes[TOTAL];

	for (long i = 0L; i < source.count(); ++i)
	{
		if (source[i] != ' ')
		{
			result = result << 1L ^ (long)upper(source[i]);
		}
		else
		{
			if (j < TOTAL) hashes[j++] = result;
			result = 0L;
		}

	}

	if (j < TOTAL) hashes[j++] = result;
	long max = j;

	long position = 0L;
	result = 0L;

	for (long i = 0L; i < count(); ++i)
	{
		if ((*this)[i] != ' ')
		{
			result = result << 1L ^ (long)upper((*this)[i]);
		}
		else
		{
			long x = 0L;
			while ((x < j) && (result > 0))
			{
				if ((hashes[x] == result) && (hashes[x] != 0))
				{
					score += 1.0f;
					hashes[x] = 0;
					result = 0;
					if (x == position) score += 1.0f;
				}
				++x;
			};

			result = 0L;
			++position;
		}
	}

	long x = 0L;
	while ((x < j) && (result > 0))
	{
		if ((hashes[x] == result) && (hashes[x] != 0))
		{
			score += 1.0f;
			result = 0;
			if (x == position) score += 1.0f;
		}
		++x;
	};

	++position;

	if (position > max) max = position;
	return score / (float)(max * 2L);
}

float string::match(string source, int *offsets, long len, long scope)
{
	auto upper = [](char source)
	{
		char result = source;
		if ((source >= 'a') && (source <= 'z'))
			result = source - 32;

		return result;
	};

	if ((offsets == NULL) || (len <= 0L)) return 0.0f;
	if ((count() == 0L) || (source.count() == 0L)) return 0.0f;

	if ((count() == 1L) && (source.count() == 1L))
	{
		if ((*this)[0] == source[0]) return 1.0f / (float)source.count();
		return  0.0f;
	}

	string *a = &source, *b = this;

	memset(offsets, 0, sizeof(int) * len);

	if (this->count() < source.count())
	{
		a = this;
		b = &source;
	}

	long a_count = a->count();
	long b_count = b->count();

	if (a_count > len) a_count = len;
	if (b_count > len) b_count = len;

	for (long i = 0L; i < a_count; ++i)
	{
		if (upper((*a)[i]) == upper((*b)[i]))
		{
			offsets[i] = i + 1;
		}
		else
		{
			bool match = false;
			int j = i - 1;
			int index = 0;
			while ((!match) && (j >= 0) && (index < scope))
			{
				if (upper((*a)[j]) == upper((*b)[i]))
				{
					offsets[i] = j + 1;
					match = true;
				}
				--j;
				++index;
			}

			if (!match)
			{
				int j = i + 1;
				index = 0;
				while ((!match) && (j < a_count) && (index < scope))
				{
					if (upper((*a)[j]) == upper((*b)[i]))
					{
						offsets[i] = j + 1;
						match = true;
					}
					++j;
					++index;
				}
			}
		}
	}

	long runs = 0L, length = 0L;
	long total = 0L;
	float result = 0.0f;

	for (long i = 1L; i < a_count; ++i)
	{
		if (offsets[i - 1L] != offsets[i] - 1L)
		{
			if (length > 0L)
			{
				total += length + 1L;
			}
			length = 0L;
			++runs;
		}
		else ++length;
	}

	total += length;
	result = (float)total / (b_count - 1L);

	return result;
}

float string::toFloat()
{
	float result = 0.0f, divisor = 10.0f;
	bool decimal = false;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (((temp >= '0') && (temp <= '9')))
		{
			float f = (float)(temp)-48.0f;
			if (!decimal)
			{
				result = (result * 10.0f) + f;
			}
			else
			{
				result += f / divisor;
				divisor *= 10.0f;
			}
		}
		else if (temp == '.')
		{
			decimal = true;
		}
	}

	if ((*this)[0] == '-') result *= -1.0f;

	return result;
}

double string::toDouble()
{
	double result = 0.0, divisor = 10.0;
	bool decimal = false;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (((temp >= '0') && (temp <= '9')))
		{
			double f = (double)(temp)-48.0;
			if (!decimal)
			{
				result = (result * 10.0f) + f;
			}
			else
			{
				result += f / divisor;
				divisor *= 10.0;
			}
		}
		else if (temp == '.')
		{
			decimal = true;
		}
	}

	if ((*this)[0] == '-') result *= -1.0;

	return result;
}

long string::toLong()
{
	long result = 0L;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (((temp >= '0') && (temp <= '9')))
		{
			long l = (long)(temp)-48L;
			result = (result * 10L) + l;
		}
	}

	if ((*this)[0] == '-') result *= -1L;

	return result;
}

long string::toLongFromHex()
{
	long result = 0L;

	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		long l = -1L;

		if (((temp >= '0') && (temp <= '9'))) l = (long)(temp)-48L;
		else if (((temp >= 'A') && (temp <= 'F'))) l = (long)(temp)-55L;
		else if (((temp >= 'a') && (temp <= 'f'))) l = (long)(temp)-87L;

		if (l >= 0L) result = (result * 16L) + l;
	}

	return result;
}

bool string::toBoolean()
{
	if (upper().trim().icompare(string("false"))) return false;
	else if (upper().trim().icompare(string("true"))) return true;

	return toLong() <= 0L ? false : true;
}

string string::toBase64()
{
	string result;

	long index = 0L;
	unsigned char a, b, c;

	for (long i = 2L; i < count(); i += 3L)
	{
		a = 0; b = 0; c = 0;

		a = (*this)[i - 2];
		b = (*this)[i - 1];
		c = (*this)[i];

		long temp = 0L;

		temp = temp | (((int)a) << 16);
		temp = temp | (((int)b) << 8);
		temp = temp | ((int)c);

		long idx1 = (temp & 0xFC0000) >> 18;
		long idx2 = (temp & 0x03F000) >> 12;
		long idx3 = (temp & 0x000FC0) >> 6;
		long idx4 = (temp & 0x00003F);

		char lu[] = { base64[idx1], base64[idx2], base64[idx3], base64[idx4] };
		result.concat((char*)&lu, 4L);
	}

	long remainder = (count() % 3);
	if (remainder > 0L)
	{
		if (remainder == 1L)
		{
			a = (*this)[count() - 1L];
			b = 0;
		}

		if (remainder == 2L)
		{
			a = (*this)[count() - 2L];
			b = (*this)[count() - 1L];
		}

		long temp = 0L;

		temp = temp | (((int)a) << 16);
		temp = temp | (((int)b) << 8);

		long idx1 = (temp & 0xFC0000) >> 18;
		long idx2 = (temp & 0x03F000) >> 12;
		long idx3 = (temp & 0x000FC0) >> 6;
		long idx4 = (temp & 0x00003F);

		char lu[] = { base64[idx1], base64[idx2] };
		result.concat(lu, 2L);

		if (remainder == 1L)
		{
			result.concat(string("=="));
		}
		else
		{
			result.concat(&base64[idx3], 1L);
			if (remainder == 2) result.concat(string("="));
			else result.concat(&base64[idx4], 1L);
		}
	}

	return result;
}

string string::fromBase64()
{
	auto address = [](unsigned char source)
	{
		if ((source >= 'A') && (source <= 'Z')) return ((long)source) - 65L;
		if ((source >= 'a') && (source <= 'z')) return ((long)source) - 71L;
		if ((source >= '0') && (source <= '9')) return ((long)source) + 4L;
		if (source == '+') return 62L;
		if (source == '/') return 63L;

		return 0L;
	};

	long length = count();
	long i = 0L, j = 0L, index = 0L;
	unsigned char temp[4], output[3];

	string result;

	while ((length > 0L) && ((*this)[index] != '='))
	{
		temp[i++] = (*this)[index];
		index++;

		if (i == 4L)
		{
			for (i = 0L; i < 4L; i++) temp[i] = (unsigned char)address(temp[i]);

			output[0] = (temp[0] << 2) + ((temp[1] & 0x30) >> 4);
			output[1] = ((temp[1] & 0xf) << 4) + ((temp[2] & 0x3c) >> 2);
			output[2] = ((temp[2] & 0x3) << 6) + temp[3];

			result += string((const char*)&output, 3L);
			i = 0L;
		}
		--length;
	};

	if (i > 0L)
	{
		for (j = 0L; j < 4L; j++) temp[j] = (unsigned char)address(temp[j]);

		output[0] = (temp[0] << 2) + ((temp[1] & 0x30) >> 4);
		output[1] = ((temp[1] & 0xf) << 4) + ((temp[2] & 0x3c) >> 2);
		output[2] = ((temp[2] & 0x3) << 6) + temp[3];

		result += string((const char*)&output, i - 1L);
	}

	return result;
}

bool string::isFloat()
{
	for (long index = 0L; index < count(); ++index)
	{
		if (!(((*this)[index] >= '0') && ((*this)[index] <= '9')))
			if (((*this)[index] != '.') && ((*this)[index] != '-')) return false;
	}

	return true;
}

bool string::isInteger()
{
	for (long index = 0L; index < count(); ++index)
	{
		if (!(((*this)[index] >= '0') && ((*this)[index] <= '9')))
			if ((*this)[index] != '-') return false;
	}

	return true;
}

bool string::isHex()
{
	for (long index = 0L; index < count(); ++index)
	{
		char temp = (*this)[index];
		if (!((temp >= '0') && (temp <= '9')))
			if (!((temp >= 'A') && (temp <= 'F')))
				if (!((temp >= 'a') && (temp <= 'f')))
					return false;
	}

	return true;
}

string string::fromFloat(float source)
{
	char temp[15]; memset(temp, 0, 15);

	snprintf((char*)temp, 15, "%2.8f", source);

	return string(temp);
}

string string::fromLong(long source)
{
	char temp[15]; memset(temp, 0, 15);

	snprintf((char*)temp, 15, "%ld", source);

	return string(temp);
}

string string::fromInt(int source)
{
	char temp[15]; memset(temp, 0, 15);

	snprintf((char*)temp, 15, "%d", source);

	return string(temp);
}

string string::fromBool(bool source)
{
	char temp[15]; memset(temp, 0, 15);

	snprintf((char*)temp, 15, "%d", (int)source);

	return string(temp);
}

string string::fromTime(time_t source)
{
	string result;
	tm now;

#ifdef WIN32
	localtime_s(&now, &source);
#elif defined __linux__
	localtime_r(&source, &now);
#endif

	if (now.tm_mday<10) result.concat(string("0"));
	result.concat(string::fromInt(now.tm_mday));
	result.concat(string("/"));
	if ((now.tm_mon + 1) < 10) result.concat(string("0"));
	result.concat(string::fromInt(now.tm_mon + 1));
	result.concat(string("/"));
	result.concat(string::fromInt(now.tm_year + 1900));
	result.concat(string(" "));
	if (now.tm_hour<10) result.concat(string("0"));
	result.concat(string::fromInt(now.tm_hour));
	result.concat(string(":"));
	if (now.tm_min<10) result.concat(string("0"));
	result.concat(string::fromInt(now.tm_min));
	result.concat(string(":"));
	if (now.tm_sec<10) result.concat(string("0"));
	result.concat(string::fromInt(now.tm_sec));

	return result;
}

bool string::operator==(const string &source)
{
	return compare(source) == 0;
}

string string::operator<<(string &source)
{
	this->concat(source);
	return *this;
}

string string::operator<<(char *source)
{
	this->concat(source);
	return *this;
}

string string::operator<<(float source)
{
	this->concat(string::fromFloat(source));
	return *this;
}

string string::operator<<(long source)
{
	this->concat(string::fromLong(source));
	return *this;
}

string string::operator<<(int source)
{
	this->concat(string::fromInt(source));
	return *this;
}

string string::operator<<(bool source)
{
	this->concat(string::fromBool(source));
	return *this;
}