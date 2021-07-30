#include "test/responses.h"
#include <iostream>

void tests::data::responses::reset(web::page &source)
{
    init = false;

    boost::json::stream_parser p;
    boost::json::error_code ec;

    try
    {
        string data = source.data();
        //std::cout << "response " << data;
        p.write(data.c_str(), data.length(), ec);
        if(ec) return;

        boost::json::value value = p.release();

        parse(value);
    }
    catch(boost::exception const& ex)
	{
        return;
    }

    init = true;
}

void tests::data::responses::parse(const boost::json::value &value)
{
    switch(value.kind())
    {
        case boost::json::kind::object:
        {
            auto const& obj = value.get_object();
            if(! obj.empty())
            {
                auto it = obj.begin();
                for(;;)
                {
                    key = boost::json::serialize(it->key());
                    key = key.trim('"');                 
                    parse(it->value());
                    
                    if(++it == obj.end()) 
                    {
                        if(key.length() > 0)
                        {
                            data.push_back(temp);
                            key.clear();
                        }
                        
                        break;
                    }
                }
            }

            break;
        }

        case boost::json::kind::array:
        {
            auto const& arr = value.get_array();
            if(! arr.empty())
            {
                auto it = arr.begin();
                for(;;)
                {
                    parse(*it);
                    if(++it == arr.end())
                        break;
                }
            }

            break;
        }

        case boost::json::kind::string:
        {
            string v = boost::json::serialize(value.get_string());
            v = v.trim('"');

            if(key.compare(string("GUID")) == 0) temp.GUID = v;
            else if(key.compare(string("status")) == 0) temp.status = v;
            else if(key.compare(string("available")) == 0) temp.available = v;
            else if(key.compare(string("created")) == 0) temp.created.from(v);
            else if(key.compare(string("name")) == 0) temp.name = v;

            break;
        }
    }
}