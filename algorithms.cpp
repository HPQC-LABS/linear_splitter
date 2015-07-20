#include "algorithms.h"

map_iterator max_map_value(map_iterator begin, map_iterator end)
{
    map_iterator max_value_key_position = begin;
    for(map_iterator it = begin; it!=end; it++)
       if(max_value_key_position->second < it->second)
          max_value_key_position = it; 
    return max_value_key_position;
}

