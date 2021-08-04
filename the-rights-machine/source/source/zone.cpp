#include "zone.h"
#include <iostream>

bool zone::zone::overlapped(zone &source)
{
    return ((start <= source.end) && (end >= source.start));
}

std::vector<zone::zone> zone::zone::split(zone &source)
{
    auto swap = [](global::datetime &a, global::datetime &b)
    {
        global::datetime temp = a;
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

    std::tuple<global::datetime, global::datetime, global::datetime, global::datetime> dates = sort(start, end, source.start, source.end);

    global::datetime start = std::get<0>(dates);
    global::datetime end = std::get<1>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    start = std::get<1>(dates);
    end = std::get<2>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    start = std::get<2>(dates); 
    end = std::get<3>(dates) + (-1);
    if (start != end) result.push_back(zone(start, end));

    return result;
}

void zone::zone::copy(zone const &source)
{
    start = source.start;
    end = source.end;
}

std::tuple<global::datetime, global::datetime, global::datetime, global::datetime> zone::zone::sort(global::datetime a, global::datetime b, global::datetime c, global::datetime d)
{
    auto swap = [](global::datetime &a, global::datetime &b)
    {
        global::datetime temp = a;
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

    return std::tuple<global::datetime, global::datetime, global::datetime, global::datetime>(a, b, c, d);
}
