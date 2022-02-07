#include "address.h"

Address::Address(std::string v) 
	: val(v)
{}

std::string Address::to_string() const {
	return val;
}

