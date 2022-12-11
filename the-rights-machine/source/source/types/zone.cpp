#include "types/zone.h"
#include <iostream>

bool types::zone::overlapped(zone &source)
{
    return ((start <= source.end) && (end >= source.start));
}

std::vector<types::zone> types::zone::split(zone &source)
{
    auto swap = [](types::datetime &a, types::datetime &b)
    {
        types::datetime temp = a;
        a = b;
        b = temp;
    };

    std::vector<zone> result;

    if (end < start) swap(start, end);

    if ((start == source.start) && (end == source.end))
    {
        result.push_back(zone(start, end));
        return result;
    }

    std::tuple<types::datetime, types::datetime, types::datetime, types::datetime> dates = sort(start, end, source.start, source.end);

    types::datetime start = std::get<0>(dates);
    types::datetime end = std::get<1>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    start = std::get<1>(dates);
    end = std::get<2>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    start = std::get<2>(dates); 
    end = std::get<3>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    return result;
}

void types::zone::copy(zone const &source)
{
    start = source.start;
    end = source.end;
}

std::tuple<types::datetime, types::datetime, types::datetime, types::datetime> types::zone::sort(types::datetime a, types::datetime b, types::datetime c, types::datetime d)
{
    auto swap = [](datetime &a, datetime &b)
    {
        types::datetime temp = a;
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

    return std::tuple<types::datetime, types::datetime, types::datetime, types::datetime>(a, b, c, d);
}
