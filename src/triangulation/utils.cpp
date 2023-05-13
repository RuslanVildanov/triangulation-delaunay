#include "utils.h"

#include <sstream>


namespace triangulation
{
	std::string Utils::intToString(int i, size_t minLength)
	{
		std::string result = std::to_string(i);
		if (result.size() < minLength) {
			for(size_t j = result.size(); j <= minLength; j++) {
				result = ' ' + result;
			}
		}
		return result;
	}

	std::string Utils::floatToString(float f, size_t minLength)
	{
		std::ostringstream ss;
		ss << f;
		std::string result(ss.str());
		if (result.size() < minLength) {
			for(size_t j = result.size(); j <= minLength; j++) {
				result = ' ' + result;
			}
		}
		return result;
	}
}
