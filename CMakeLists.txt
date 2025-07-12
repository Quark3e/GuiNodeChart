cmake_minimum_required(VERSION 3.18)
project(GuiNodeChart)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_FIND_DEBUG_MODE 1)

if(WIN32)
    # find_package(Allegro CONFIG REQUIRED PATHS "C:/Users/berkh/Libraries/vcpkg/installed/x64-windows/share/allegro5")
else()

endif(WIN32)


# add the relevant packages
#imgui

set(GNC_DIR__INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(GNC_DIR__SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(GNC_DIR__LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib")


set(GNC_FILES__INCLUDE
    "${GNC_DIR__INCLUDE}/globals_includes.hpp"
    "${GNC_DIR__INCLUDE}/global_variables.hpp"
    "${GNC_DIR__INCLUDE}/useful.hpp"
    "${GNC_DIR__INCLUDE}/extra_imgui.hpp"
    "${GNC_DIR__INCLUDE}/bezier_curve.hpp"
    "${GNC_DIR__INCLUDE}/gui_nodeChart.hpp"
)
set(GNC_FILES__SOURCES
    "${GNC_DIR__SOURCES}/main.cpp"
    "${GNC_DIR__SOURCES}/globals_variables.cpp"
    "${GNC_DIR__SOURCES}/gui_nodeChart.cpp"
    "${GNC_DIR__SOURCES}/menuScreens.cpp"
)


set(GNC_DIR__LIB_INCLUDES
    "${GNC_DIR__LIBRARY}/imgui"
    "${GNC_DIR__LIBRARY}/imgui/backends"
    "${GNC_DIR__LIBRARY}/imgui/misc/cpp"
    "${GNC_DIR__LIBRARY}/JsonParser/include/"
    "${GNC_DIR__LIBRARY}/KeyBindHandler/include/"
    "${GNC_DIR__LIBRARY}/LazyDiyDict/include/"
    "${GNC_DIR__LIBRARY}/Pos2d/include/"
)

file(GLOB GNC_FILES__LIB_SOURCES__IMGUI CONFIGURE_DEPENDS "${GNC_DIR__LIBRARY}/imgui/imgui*.cpp")
list(APPEND GNC_FILES__LIB_SOURCES__IMGUI "${GNC_DIR__LIBRARY}/imgui/backends/imgui_impl_allegro5.cpp")
list(APPEND GNC_FILES__LIB_SOURCES__IMGUI "${GNC_DIR__LIBRARY}/imgui/addons/imguidatechooser/imguidatechooser.cpp")
list(APPEND GNC_FILES__LIB_SOURCES__IMGUI "${GNC_DIR__LIBRARY}/imgui/misc/cpp/imgui_stdlib.cpp")

message(STATUS "GNC_FILES__LIB_SOURCES__IMGUI : ${GNC_FILES__LIB_SOURCES__IMGUI}")

# Use CMakeLists files from the libraries themselves
# set(GNC_DIR__LIB_SOURCES
#     "${GNC_DIR__LIBRARY}/imgui/"
# )
add_subdirectory("${GNC_DIR__LIBRARY}/JsonParser/")
add_subdirectory("${GNC_DIR__LIBRARY}/KeyBindHandler/")
add_subdirectory("${GNC_DIR__LIBRARY}/LazyDiyDict/")


add_executable(${PROJECT_NAME}
    ${GNC_FILES__SOURCES}
    ${GNC_FILES__LIB_SOURCES__IMGUI}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${GNC_DIR__INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC ${GNC_DIR__LIB_INCLUDES})

target_link_libraries(${PROJECT_NAME} PUBLIC LazyDiyDict)
target_link_libraries(${PROJECT_NAME} PUBLIC KeyBindHandler)
target_link_libraries(${PROJECT_NAME} PUBLIC JsonParser)

target_link_libraries(${PROJECT_NAME} PUBLIC
    allegro
    allegro_main
    allegro_primitives
    allegro_image
)
