#include "test/responses.h"

void tests::data::responses::reset(web::page &source)
{
    init = false;

    boost::json::stream_parser p;
    boost::json::error_code ec;

    string data = source.data();
    p.write(data.c_str(), data.length(), ec);
    if(ec) return;

    boost::json::value value = p.release();

    parse(value);

    init = true;
}

void tests::data::responses::parse(const boost::json::value &value)
{
    switch(value.kind())
    {
        case boost::json::kind::object:
        {
            //os << "{\n";
            //indent->append(4, ' ');
            auto const& obj = value.get_object();
            if(! obj.empty())
            {
                auto it = obj.begin();
                for(;;)
                {
                    key = boost::json::serialize(it->key());// << " : ";   
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
                    //os << ",\n";
                }
            }
            //os << "\n";
            //indent->resize(indent->size() - 4);
            //os << *indent << "}";
            break;
        }

        case boost::json::kind::array:
        {
            //os << "[\n";
            //indent->append(4, ' ');
            auto const& arr = value.get_array();
            if(! arr.empty())
            {
                auto it = arr.begin();
                for(;;)
                {
                    //os << *indent;
                    parse(*it);
                    if(++it == arr.end())
                        break;
                   // os << ",\n";
                }
            }
            //os << "\n";
            //indent->resize(indent->size() - 4);
            //os << *indent << "]";
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

            //os << 
            break;
        }
/*
        case boost::json::kind::uint64:
            os << jv.get_uint64();
            break;

        case boost::json::kind::int64:
            os << jv.get_int64();
            break;

        case boost::json::kind::double_:
            os << jv.get_double();
            break;

        case boost::json::kind::bool_:
            if(jv.get_bool())
                os << "true";
            else
                os << "false";
            break;
        */
    }
}