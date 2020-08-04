cmake_minimum_required(VERSION 3.14)
include(FetchContent)

project(FdiProjekt1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#--------------Zaciągamy biblioteki z Githuba-----------------

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

# Biblioteka do ładowania OpenGLa + nagłówki do OpenGLa
FetchContent_Declare(
  glad
  GIT_REPOSITORY "https://github.com/Dav1dde/glad"
  GIT_TAG "de6c39e3040c987323b8ed078c36442f4fb681b3"
)

FetchContent_MakeAvailable(glm glfw glad)

#-----------------Kod źródłowy---------------------

# Folder z plikami nagłówkowymi
include_directories(source)
include_directories(source/imgui)

set(
  IMGUI_SOURCES
  source/imgui/imgui.cpp
  source/imgui/imgui_demo.cpp
  source/imgui/imgui_draw.cpp
  source/imgui/imgui_impl_glfw.cpp
  source/imgui/imgui_impl_opengl3.cpp
  source/imgui/imgui_widgets.cpp
)
set(
  PROJECT_SOURCES
  source/main.cpp
  source/ui.cpp
  source/simulation.cpp
)
add_executable(${CMAKE_PROJECT_NAME} ${IMGUI_SOURCES} ${PROJECT_SOURCES})

#---------------Dołączamy biblioteki----------------
target_link_libraries(${CMAKE_PROJECT_NAME} glm)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} glad)