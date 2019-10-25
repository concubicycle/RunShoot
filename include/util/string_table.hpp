//
// Created by sava on 10/24/19.
//

#ifndef __STRING_TABLE_HPP_
#define __STRING_TABLE_HPP_

#include <unordered_map>

class string_table
{
public:

    unsigned long hash_and_store(std::string str)
    {
        auto hash = std::hash<std::string>{}(str);
        _hash_to_string[hash] = str;
        return hash;
    }

    std::string operator[](unsigned long key)
    {
        return _hash_to_string[key];
    }

    static unsigned long hash_for(std::string str)
    {
        return std::hash<std::string>{}(str);
    }

private:
    std::unordered_map<unsigned long, std::string> _hash_to_string;
};

#endif //__STRING_TABLE_HPP_
