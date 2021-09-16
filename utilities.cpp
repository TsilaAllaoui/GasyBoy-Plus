#include "utilities.h"

string int2hex(int value)
{
	stringstream s;
	s << hex << value;
	string str = s.str();
	for (int i=0; i<str.size();i++)
        str[i] = toupper(str[i]);
	return str;
}
