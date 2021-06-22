//#include <boost/json/src.hpp>
#include <boost/json.hpp>
#include <boost/json/basic_parser_impl.hpp>
//#include <boost/json/error.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "custom/string.h"
#include "message.h"
#include "pair.h"
#include "crumbs.h"

#if !defined(__BSTTST)
#define __BSTTST

//#define BOOST_JSON_STANDALONE ON

namespace bsttst
{
    //using namespace boost::json;
/*
    class moo
    {
    public:
        data::json::request::json *current;
        string key;
    };
*/
// todo
// pass in data::json::rquest:json instead of msg class
// validate user/apiKeys -- move into header of HTTP request
// upon end of query object, create event function, for pushing
// query into engine, for output

/*

guid::guid g;
											task.message.guid = g.get();
											task.message.created = global::datetime::now();

											if (!c->configuration.manager->set(task))
											{
												error(string("MESSAGE_PUSH"));
											}

// CLEAR TASK>MESSAGE AFTERWARD
// NEW THREAD, FOR PUSHING OUT RESULTS WHEN DONE, INTO SAME STREAM..

// PASS HTTP OUTPUT STREAM INTO CONFIGURATION MANAGER..??

                                            */

    class bsttst
    {
        struct handler
        {
            data::message::message *msg;
            //data::json::request::json *current;
            string key;

            //std::vector<moo> current;
            crumbs::crumbs parents;

            handler(data::message::message *msg) //data::json::request::json *current;
            {
                this->msg = msg;
                //current = nullptr;
            }
            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            bool on_document_begin( boost::json::error_code& ) { return true; }
            bool on_document_end( boost::json::error_code& ) 
            { 
                return true; 
            }
            bool on_object_begin( boost::json::error_code& )
            {
                bool result = true;
                //if(key.size() > 0)
                //{
                    std::cout << "obj begin " << key << "\n";
                    result = parents.push(key);
                    key.clear();
                //}

                return result;
            }
            bool on_object_end( std::size_t, boost::json::error_code& )
            { 
                // CHECK, IS OBJECT QUERY
                // NEED TO PUSH INTO ENGINE
                // HAVE EVENT FUNCTION PUSH INTO STRUCT CONSTRUCTOR

                std::cout << "object end " << parents.FQDN() << "\n";
                key.clear();

                queue::base *b = msg->findQ(parents.FQDN());
                if (b != NULL)
                {
                    std::cout << "Flushed " << parents.FQDN() << "\n";
                    b->flush();
                }

                return parents.pop();
                //return true; 
            }
            bool on_array_begin( boost::json::error_code& )
            {
                bool result = true;
                //if(key.size() > 0)
                //{
                    std::cout << "array begin " << key << "\n";

                    result = parents.push(key);
                    key.clear();
                //}

                return result;
            }
            bool on_array_end( std::size_t, boost::json::error_code& )
            {
                std::cout << "array end\n";
                key.clear();

                queue::base *b = msg->findQ(parents.FQDN());
                if (b != NULL)
                {
                    std::cout << "Flushed " << parents.FQDN() << "\n";
                    b->flush();
                }

                return parents.pop();
                //return true;
            }
            bool on_key_part( boost::json::string_view sv, std::size_t, boost::json::error_code& ) 
            {
                   //std::string temp(sv.data());
                    //std::cout << "on_key_part \"" << temp << "\"\n";

                return true;
            }
            bool on_key( boost::json::string_view sv, std::size_t sz, boost::json::error_code& )
            {
                // use crumbs for //parent1//parent2 etc..
                string temp(sv.data(),sz);
                temp = temp.trim('"');
                std::cout << "on_key {" << temp << "}\n";
                std::cout << parents.FQDN() << "\n";
                key = temp;
// KEY NEEDS TO BE THE CRUMBS
// CURRENT NEEDS TO BE A STACK
/*
data::json::request::json *tt = msg->find(temp);
if(tt != NULL)
{
    moo tmoo;
    tmoo.key = temp;
    tmoo.current = tt;
    current.push_back(tmoo);
    //current = tt;
}
*/
//else if(current != NULL)
//{
    //current->add()
//}
                //if(msg->find(temp)!=NULL)
                /*if(current != NULL)
                {
                    std::cout << "found\n";
                }*/
                return true;
            }
            bool on_string_part( boost::json::string_view, std::size_t, boost::json::error_code& ) { return true; }
            bool on_string( boost::json::string_view sv, std::size_t sz, boost::json::error_code& ) 
            { 
                //std::cout << "on_string\n";
                std::cout << "on_string " << parents.FQDN() << " (" << key << ")\n";
                data::json::request::json *current = msg->find(parents.FQDN());
                if(current != NULL)
                {
                    custom::pair pair;
                    pair.name = key;
                    pair.value = string(sv.data(), sz);
                    std::cout << "FOUND string " << key << ", " << pair.value << "\n";
                    current->add(pair);
                }

                return true; 
            }
            bool on_number_part( boost::json::string_view, boost::json::error_code& ) { return true; }
            bool on_int64( std::int64_t value, boost::json::string_view, boost::json::error_code& ) 
            {
                std::cout << "on_int64 " << parents.FQDN() << "," << key << "\n";
                data::json::request::json *current = msg->find(parents.FQDN());
                if(current != NULL)
                {
                    custom::pair pair;
                    pair.name = key;
                    pair.value = string::fromInt(value);
                    std::cout << "FOUND int " << key << ", " << value << "\n";
                    current->add(pair);
                }
                /*
                if(current.size() > 0)
                {
                    moo tmoo = *(--current.end());
                    custom::pair pair;
                    pair.name = key;
                    pair.value = string::fromInt(value);
                    tmoo.current->add(pair);
                    //std::cout << key << ":int\n";
                    //if(current != NULL)
                    //{

                    //}
                }*/
                return true;
            }
            bool on_uint64( std::uint64_t, boost::json::string_view, boost::json::error_code& ) 
            { 
                std::cout << "on_uint64\n";
                return true; 
            }
            bool on_double( double, boost::json::string_view, boost::json::error_code& ) { return true; }
            bool on_bool( bool, boost::json::error_code& ) { return true; }
            bool on_null( boost::json::error_code& ) { return true; }
            bool on_comment_part(boost::json::string_view, boost::json::error_code&) { return true; }
            bool on_comment(boost::json::string_view, boost::json::error_code&) { return true; }
        };

        boost::json::basic_parser<handler> p;

        data::message::message msg;

    public:
        std::size_t write(char const* data, std::size_t size, boost::json::error_code& ec)
        {
            auto const n = p.write_some( false, data, size, ec );
            if(! ec && n < size)
            {
                //ec = error::extra_data;
            }
            return n;
        }

    public:
        bsttst() : p(boost::json::parse_options(),&msg) { }
        ~bsttst() { }

        void go(std::string filename);
        void output() { msg.output(); }
    };

};

#endif