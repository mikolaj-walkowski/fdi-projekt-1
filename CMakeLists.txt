cmake_minimum_required(VERSION 3.14)
include(FetchContent)

project(FdiProjekt1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#--------------Zaci¹gamy biblioteki z Githuba-----------------

# Biblioteka do wektorów
FetchContent_Declare(
  glm
  GIT_REPOSITORY "https://github.com/g-truc/glm"
  GIT_TAG "bf71a834948186f4097caa076cd2663c69a10e1e"
)

# Biblioteka do okienek
FetchContent_Declare(
  glfw
  GIT_REPOSITORY "https://github.com/glfw/glfw"
  GIT_TAG "0a49ef0a00baa3ab520ddc452f0e3b1e099c5589"
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Biblioteka do ³adowania OpenGLa + nag³ówki do OpenGLa
FetchContent_Declare(
  glad
  GIT_REPOSITORY "https://github.com/Dav1dde/glad"
  GIT_TAG "de6c39e3040c987323b8ed078c36442f4fb681b3"
)

FetchContent_MakeAvailable(glm glfw glad)

#-----------------Kod Ÿród³owy---------------------

# Folder z plikami nag³ówkowymi
include_directories(source)
include_directories(source/imgui)

# Z³e, niedobre!
# Rekurencyjnie znajdujemy wszystkie pliki o rozszerzeniach .cpp
# w folderze source/ i dodajemy je do kompilacji.
file(GLOB_RECURSE SOURCES "./source/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

#---------------Do³¹czamy biblioteki----------------
target_link_libraries(${CMAKE_PROJECT_NAME} glm)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} glad)