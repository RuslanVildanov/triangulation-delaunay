#ifndef TEST_TEST_H
#define TEST_TEST_H

#include <iostream>


#define DELAUNAY_TEST(e)				if(! (e) ) {std::cout << "Test failed! [" << __FUNCTION__ << "]" << std::endl << "\tChecked expression: " << #e << " = " << (e) << std::endl << "\tFile: " << __FILE__ << " Line: " << __LINE__ << std::endl; throw std::exception(); }

#define DELAUNAY_TEST_BEGIN()			bool delaunayTestOk = true; try {
#define DELAUNAY_TEST_END()			} catch (const std::exception&) { delaunayTestOk = false; } if (delaunayTestOk) { std::cout << "Test finished OK! [" << __FUNCTION__ << "]" << std::endl; }

#endif //TEST_TEST_H
