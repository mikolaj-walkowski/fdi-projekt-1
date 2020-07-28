# Projekt nr 1 na FDI

### Dependencies
- CMake w wersji co najmniej 3.14
- Kompilator obsługujący standard C++14

### Budowanie projektu
Skrypty do budowania oraz uruchamiania programu znajdują się w folderze `./scripts`
Do budowania programu można także użyć poleceń:
```
cd out
cmake ..
cmake --build .
```
Pierwsza kompilacja będzie trwała dość długo, bo CMake będzie zaciągał kilka bibliotek z Githuba.
Gotowy program znajdziemy w folderze `./out/Debug`.

### Użyte biblioteki
- [GLM](https://github.com/g-truc/glm) (wektory)
- [Imgui](https://github.com/ocornut/imgui) (GUI)
- [GLAD](https://github.com/Dav1dde/glad) (ładowanie + nagłówki OpenGLa)
- [GLFW](https://github.com/glfw/glfw) (okienko)