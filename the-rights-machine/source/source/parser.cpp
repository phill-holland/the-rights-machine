#include "parser.h"
#include <iostream>
#include <fstream>
#include <cstring>

bool parser::parser::handler::on_document_begin(boost::json::error_code&) 
{ 
    return true; 
}

bool parser::parser::handler::on_document_end(boost::json::error_code&) 
{ 
    //std::cout << "on_document_end\n";
    return true; 
}

bool parser::parser::handler::on_object_begin(boost::json::error_code&)
{
    bool result = params->parents.push(params->key);
    
    params->key.clear();

    return result;
}

bool parser::parser::handler::on_object_end(std::size_t, boost::json::error_code&)
{ 
    params->key.clear();
    queue::base *b = params->task.message.findQ(params->parents.FQDN());
    if (b != NULL) 
    {
        b->flush();
    }

    if(params->parents.FQDN().icompare(params->task.message.items.FQDN()))
    {
        params->task.message.output();
        guid::guid g;
        
        params->task.message.guid = g.get();
        params->task.message.created = global::datetime::now();
        params->task.response = params->responses;
        params->task.notify = params->notify;

        if (params->notify != NULL) params->notify->notifyIn(g);
        if (!params->manager->set(params->task)) return false;

        params->task.message.clear();
    }
                                    
    return params->parents.pop();
}

bool parser::parser::handler::on_array_begin(boost::json::error_code&)
{
    bool result = params->parents.push(params->key);
    
    params->key.clear();

    return result;
}

bool parser::parser::handler::on_array_end(std::size_t, boost::json::error_code&)
{
    params->key.clear();
    return params->parents.pop();
}

bool parser::parser::handler::on_key(boost::json::string_view sv, std::size_t sz, boost::json::error_code&)
{
    params->key = string(sv.data(), sz).trim('"');
    return true;
}

bool parser::parser::handler::on_string(boost::json::string_view sv, std::size_t sz, boost::json::error_code&) 
{ 
    data::json::request::json *current = params->task.message.find(params->parents.FQDN());
    if(current != NULL)
    {
        custom::pair pair(params->key, string(sv.data(), sz));
        current->add(pair);
    }

    return true; 
}

bool parser::parser::handler::on_int64(std::int64_t value, boost::json::string_view, boost::json::error_code&) 
{
    data::json::request::json *current = params->task.message.find(params->parents.FQDN());
    if(current != NULL)
    {
        custom::pair pair(params->key, string::fromInt(value));
        current->add(pair);
    }

    return true;
}

std::size_t parser::parser::write(char const* data, std::size_t size, boost::json::error_code& ec)
{
    try
    {
        auto const n = p.write_some(true, data, size, ec);
        if(!ec && n < size) ec = boost::json::error::extra_data;
        
        return n;
    }
    catch(boost::exception const& ex)//b_ex, std::exception const& ex)
    {   
        //std::cout << "are you sure???\n";
        return -1;
    }
}