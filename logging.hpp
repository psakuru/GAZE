#ifndef LOGGING_HPP
#define LOGGING_HPP
#include<iostream>

#if DEBUG
static std::ostream &dout = cout;
#else
static std::stringstream dout;
#endif

#endif
