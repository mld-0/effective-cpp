#include "date.h"

Date::Date(std::string v) 
	: val(v)
{}

std::string Date::to_string() const {
	return val;
}

