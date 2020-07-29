# Projekt nr 1 na FDI

### Dependencies
- CMake w wersji co najmniej 3.14
- Kompilator obsługujący standard C++14 (na pewno działają Visual C++ oraz MinGW na Windowsie)

### Budowanie projektu

Najpierw musimy zgenerować pliki do projektu. Polecam zrobić to w innym folderze niż folder w którym mamy repo, bo CMake zaśmieci ten folder różnymi plikami potrzebnymi do kompilacji.

- Kompilator Visual C++: `cmake ścieżka-do-repo`
- MinGW: `cmake -G "MinGW Makefiles" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ścieżka-do-repo`

Kompilujemy używając komendy `cmake --build ścieżka-do-projektu`. Pierwsza kompilacja może potrwać dość długo, bo CMake będzie zaciągać potrzebne biblioteki z GitHuba.

### Użyte biblioteki
- [GLM](https://github.com/g-truc/glm) (wektory)
- [Imgui](https://github.com/ocornut/imgui) (GUI)
- [GLAD](https://github.com/Dav1dde/glad) (ładowanie + nagłówki OpenGLa)
- [GLFW](https://github.com/glfw/glfw) (okienko)