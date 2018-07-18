#include "line.h"
#include <tuple>

void data::line::line::clear()
{
	lineID = 0;
	itemID = 0;
	start.clear();
	end.clear();
	exclusivityID = 0;
	typeID = 0;
}

/*
bool data::line::line::overlapped(line &source)
{
	return (!((start > source.end) || (source.start > end)));
}

//std::tuple<data::line::line, data::line::line, data::line::line> data::line::line::split(line &source)
std::vector<data::line::line> data::line::line::split(line &source)
{
	std::vector<data::line::line> result;

	if ((start == source.start) && (end == source.end))
	{
		result.push_back(*this);
		return result;
	}

	std::tuple<datetime, datetime, datetime, datetime> dates = sort(start, end, source.start, source.end);

	datetime start = std::get<0>(dates), end = std::get<1>(dates);
	if (start != end) result.push_back(spawn(start, end));

	datetime start = std::get<1>(dates), end = std::get<2>(dates);
	if (start != end) result.push_back(spawn(start, end));

	datetime start = std::get<2>(dates), end = std::get<3>(dates);
	if (start != end) result.push_back(spawn(start, end));

	return result;
	//return std::tuple<data::line::line, data::line::line, data::line::line>(spawn(std::get<0>(result), std::get<1>(result)), spawn(std::get<1>(result), std::get<2>(result)), spawn(std::get<2>(result), std::get<3>(result)));
}
*/

void data::line::line::copy(line const &source)
{
	lineID = source.lineID;
	itemID = source.itemID;
	start = source.start;
	end = source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

data::line::line data::line::line::spawn(datetime &start, datetime &end)
{
	data::line::line result(*this);

	result.start = start;
	result.end = end;

	return result;
}

bool data::line::line::add(custom::pair &source)
{
	if (string("start").icompare(source.name))
	{
		start.from(source.value);
		return true;
	}

	if (string("end").icompare(source.name))
	{
		end.from(source.value);
		return true;
	}

	if (string("exclusivity").icompare(source.name))
	{
		exclusivityID = source.value.toInteger();
		return true;
	}

	if (string("type").icompare(source.name))
	{
		typeID = source.value.toInteger();
		return true;
	}

	return false;
}

/*
std::tuple<datetime, datetime, datetime, datetime> data::line::line::sort(datetime a, datetime b, datetime c, datetime d)
{
	auto swap = [](datetime &a, datetime &b)
	{
		datetime temp = a;
		a = b;
		b = temp;
	};

	int swaps = 0;

	do
	{
		swaps = 0;
		if (b < a) { swap(a, b); ++swaps; }
		if (c < b) { swap(b, c); ++swaps; }
		if (d < c) { swap(c, d); ++swaps; }
	} while (swaps > 0);

	return std::tuple<datetime, datetime, datetime, datetime>(a, b, c, d);
}
*/