#ifndef TEST_DELAUNAY_SERVICE_TEST_H
#define TEST_DELAUNAY_SERVICE_TEST_H


namespace test::delaunay
{
	class DelaunayServiceTest
	{
	public:
		DelaunayServiceTest() = default;
		~DelaunayServiceTest() = default;
		DelaunayServiceTest(const DelaunayServiceTest&) = delete;
		DelaunayServiceTest(DelaunayServiceTest&&) = delete;
		DelaunayServiceTest & operator=(const DelaunayServiceTest&) = delete;
		DelaunayServiceTest & operator=(DelaunayServiceTest&&) = delete;

		void testIterativeDynamicProcess1();

	};
}

#endif //TEST_DELAUNAY_SERVICE_TEST_H
