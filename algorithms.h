//algorithms.h

#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

#include <map>
#include <vector>
#include <iostream>
#include <iterator>
#include <sstream>
#include <random>

//Template for outputting vectors
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& x)
{
        std::copy(x.begin(),x.end(),std::ostream_iterator<T>(os," "));
            return os;
}

//Find index corresponding to max value in map
typedef std::map<unsigned,unsigned>::iterator map_iterator;
map_iterator max_map_value(map_iterator, map_iterator);

#endif
