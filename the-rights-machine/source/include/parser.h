#include <boost/json.hpp>
#include <boost/json/basic_parser_impl.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "custom/string.h"
#include "message.h"
#include "pair.h"
#include "crumbs.h"
#include "task.h"
#include "notification.h"
#include "manager.h"

#if !defined(__PARSER)
#define __PARSER

namespace parser
{ 
    class parser
    {
        class parameters
        {
        public:
            manager::manager *manager;
            custom::chain<data::response::response> *responses;
            notification::notification *notify;
            compute::task task;            
            crumbs::crumbs parents;
            
            string key;

        public:
            parameters(manager::manager *manager, 
                       custom::chain<data::response::response> *responses,
                       notification::notification *notify)
            {
                this->manager = manager;
                this->responses = responses;
                this->notify = notify;

                clear();
            }

            void clear()
            {
                key.clear();
                parents.clear();
                task.message.clear();
            }
        };

        struct handler
        {
            parameters *params;

            handler(parameters *params)
            {
                this->params = params;
            }

            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            bool on_document_begin(boost::json::error_code&);
            bool on_document_end(boost::json::error_code&);
            bool on_object_begin(boost::json::error_code&);
            bool on_object_end(std::size_t, boost::json::error_code&);            
            bool on_array_begin(boost::json::error_code&);
            bool on_array_end(std::size_t, boost::json::error_code&);
            bool on_key_part(boost::json::string_view sv, std::size_t, boost::json::error_code&) { return true; } 
            bool on_key(boost::json::string_view sv, std::size_t sz, boost::json::error_code&);
            bool on_string_part(boost::json::string_view, std::size_t, boost::json::error_code&) { return true; }
            bool on_string(boost::json::string_view sv, std::size_t sz, boost::json::error_code&);
            bool on_number_part(boost::json::string_view, boost::json::error_code&) { return true; }
            bool on_int64(std::int64_t value, boost::json::string_view, boost::json::error_code&);
            bool on_uint64( std::uint64_t, boost::json::string_view, boost::json::error_code& ) { return true; }
            bool on_double( double, boost::json::string_view, boost::json::error_code& ) { return true; }
            bool on_bool( bool, boost::json::error_code& ) { return true; }
            bool on_null( boost::json::error_code& ) { return true; }
            bool on_comment_part(boost::json::string_view, boost::json::error_code&) { return true; }
            bool on_comment(boost::json::string_view, boost::json::error_code&) { return true; }
        };

        boost::json::basic_parser<handler> p;
        parameters params;

    public:
        parser(manager::manager *manager, 
               custom::chain<data::response::response> *responses,
               notification::notification *notify) : params(manager, responses, notify), p(boost::json::parse_options(), &params) { }
        ~parser() { }

        void clear() { params.clear(); p.reset(); }
        void done() { p.done(); }

        std::size_t write(char const* data, std::size_t size, boost::json::error_code& ec);
    };
};

#endif