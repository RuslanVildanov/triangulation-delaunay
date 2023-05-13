#ifndef TRIANGULATION_UTILS_H
#define TRIANGULATION_UTILS_H

#include <string>


namespace triangulation
{
	class Utils
	{
	public:
		Utils() = delete;
		~Utils() = delete;
		Utils(const Utils&) = delete;
		Utils(Utils&&) = delete;
		Utils & operator=(const Utils&) = delete;
		Utils & operator=(Utils&&) = delete;

		static std::string intToString(int i, size_t minLength);

		static std::string floatToString(float f, size_t minLength);

	};
}

#endif //TRIANGULATION_UTILS_H
