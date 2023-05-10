# Консольное приложение для построения области триангуляции по заданным координатам
---

## Requirements
- cmake >= 3.19

### Windows
- Visual Studio 2019. Можно и ниже, главное поддержка стандарта c++17.

### Build
В каталоге проекта запустить:
```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
В каталоге build появится папка Release с DelaunayTriangulation.exe

### Using
```
DelaunayTriangulation.exe -h
```
Отображает список поддерживаемых атрибутов программы.

Если рядом с программой разместить файл sphere-nodes.xyz с координатами 
вершин в специальном формате, то генерацию области триангуляции 
можно запустить командой:
```
DelaunayTriangulation.exe -i sphere-nodes.xyz -o sphere-nodes.output
```
