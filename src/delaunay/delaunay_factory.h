#ifndef DELAUNAY_DELAUNAY_FACTORY_H
#define DELAUNAY_DELAUNAY_FACTORY_H

#include <memory>


namespace delaunay
{
	struct Triangle;
	struct Vertex2D;

	class DelaunayFactory
	{
	private:
		int _triangleId = 1;

	public:
		DelaunayFactory() = default;
		~DelaunayFactory() = default;
		DelaunayFactory(const DelaunayFactory&) = delete;
		DelaunayFactory(DelaunayFactory&&) = delete;
		DelaunayFactory & operator=(const DelaunayFactory&) = delete;
		DelaunayFactory & operator=(DelaunayFactory&&) = delete;

		std::shared_ptr<Triangle> createTriangle(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				const std::shared_ptr<Vertex2D>& vertex3);

	};
}

#endif //DELAUNAY_DELAUNAY_FACTORY_H
