#ifndef DELAUNAY_UTILS_H
#define DELAUNAY_UTILS_H

#include <list>
#include <vector>
#include <memory>
#include <unordered_set>


namespace delaunay
{
	struct Triangle;
	struct Vertex2D;
	class DelaunayFactory;

	enum class Direction
	{
		Right = 0,	// по часовой стрелки
		Left,		// против часовой стрелки
		Collinear	// параллельны
	};

	class Utils
	{
	public:
		Utils() = delete;
		~Utils() = delete;
		Utils(const Utils&) = delete;
		Utils(Utils&&) = delete;
		Utils & operator=(const Utils&) = delete;
		Utils & operator=(Utils&&) = delete;

		/**
		 * Синус угла треугольника в вершине 1
		 * @param vertex1 - вершина вверху треугольника
		 * @param vertex2 - вершина справа треугольника
		 * @param vertex3 - вершина слева треугольника
		 * @param ok
		 * @return
		 */
		static float sinAlpha(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				const std::shared_ptr<Vertex2D>& vertex3,
				bool *ok) noexcept;

		/**
		 * Косинус угла треугольника в вершине 1
		 * @param vertex1 - вершина вверху треугольника
		 * @param vertex2 - вершина справа треугольника
		 * @param vertex3 - вершина слева треугольника
		 * @param ok
		 * @return
		 */
		static float cosAlpha(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				const std::shared_ptr<Vertex2D>& vertex3,
				bool *ok) noexcept;

		/**
		 * Выбирает точки для первого треугольника, чтобы все точки не лежали на одной прямой
		 * @param vertices
		 * @return
		 */
		static std::shared_ptr<Triangle> createFirstTriangle(
				const std::list<std::shared_ptr<Vertex2D> > & vertices,
				DelaunayFactory* delaunayFactory);

		/**
		 * Проверяет условие Делоне методом модифицированной проверки суммы противолежащих
		 * углов. Углы проверяются в triangle и треугольнике напротив вершины vertexNumber
		 * @param triangle
		 * @param vertexNumber
		 * @return
		 */
		static bool checkSumOppositeAngles(
				const std::shared_ptr<Triangle>& triangle,
				int vertexNumber) noexcept;

		// Проверяет условие Делоне через уравнение описанной окружности
		// углов. Углы между: triangle1.vertex0 и triangle2.vertex0
		// TODO: Возвращает не верный результат - потом разобраться
		static bool checkCircleEquation(
				const std::shared_ptr<Triangle>& triangle1,
				const std::shared_ptr<Triangle>& triangle2) noexcept;

		/**
		 * Центр масс треугольника (просто центр треугольника) - точка пересечения его медиан
		 * @param triangle
		 * @return
		 */
		static std::shared_ptr<Vertex2D> centerOfMass(
				const std::shared_ptr<Triangle>& triangle);

		static float distance(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				bool *ok) noexcept;

		/**
		 * Связывает внутренний треугольник innerTriangle с внешними при этом
		 * происходит проверка, что внутренний и внешний треугольник имеют общее ребро.
		 * Внешний треугольник может инициализироваться пустым указателем - в этом случае
		 * происходит разрыв связи.
		 * @param innerTriangle - внутренний треугольник (не пустой указатель)
		 * @param outerTriangle1 - может быть пустым указателем
		 * @param outerTriangle2 - может быть пустым указателем
		 * @param outerTriangle3 - может быть пустым указателем
		 * @return Функция вернёт true в случае, если переданные внешние треугольники
		 * действительно имеют общие рёбра с соответствующими рёбрами @innerTriangle .
		 */
		static bool bindTriangles(
				std::shared_ptr<Triangle>& innerTriangle,
				std::shared_ptr<Triangle>& outerTriangle1,
				std::shared_ptr<Triangle>& outerTriangle2,
				std::shared_ptr<Triangle>& outerTriangle3) noexcept;

		static bool triangleRect(
				const std::shared_ptr<Triangle>& triangle,
				float& minX,
				float& minY,
				float& maxX,
				float& maxY) noexcept;

		/**
		 * Возвращает направление с какой стороны находится отрезок (vertix1; vertix3)
		 * относительно отрезка (vertix1; vertix2)
		 * @param vertex1 - общая координата для отрезков (vertix1; vertix2) и (vertix1; vertix3)
		 * @param vertex2
		 * @param vertex3
		 * @param ok
		 * @return
		 */
		static Direction direction(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				const std::shared_ptr<Vertex2D>& vertex3,
				bool* ok) noexcept;

		/**
		 * Проверяет пересекаются ли отрезки (vertix1; vertix2) и (vertix3; vertix4)
		 * @param vertex1
		 * @param vertex2
		 * @param vertex3
		 * @param vertex4
		 * @param trueWhenVertexOnSegment - признак нужно ли учитывать ситуацию когда
		 * только вершина одного отрезка соприкасается с другим.
		 * Если задать true, то функция будет воспринимать соприкосновение вершины одного
		 * отрезка с другим отрезком как пересечение.
		 * @param ok
		 * @return
		 */
		static bool isSegmentsIntersect(
				const std::shared_ptr<Vertex2D>& vertex1,
				const std::shared_ptr<Vertex2D>& vertex2,
				const std::shared_ptr<Vertex2D>& vertex3,
				const std::shared_ptr<Vertex2D>& vertex4,
				bool trueWhenVertexOnSegment,
				bool* ok) noexcept;

		/**
		 * Создаёт четыре вершины в виде прямоугольника, которые полностью покроют все
		 * вершины vertices
		 * @param vertices
		 * @return Четыре новых вершины, которые образуют прямоугольник, если vertices
		 * содержит хотя бы одну вершину.
		 *
		 * Порядок созданных вершин:
		 * 1 - minX ; minY
		 * 2 - minX ; maxY
		 * 3 - maxX ; minY
		 * 4 - maxX ; maxY
		 */
		static std::vector<std::shared_ptr<Vertex2D> > createVerticesAsRectangle(
				const std::list<std::shared_ptr<Vertex2D> >& vertices);

	};
}

#endif //DELAUNAY_UTILS_H
