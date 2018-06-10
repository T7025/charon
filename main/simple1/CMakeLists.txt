set(LIB_SRC
        Universe.cpp Universe.h)

add_library(libcharon01 ${LIB_SRC})
#target_link_libraries(libcharon ${LIBS})
target_link_libraries(libcharon01 spdlog)

add_executable(charon01 main.cpp)
#target_include_directories(charon )
target_link_libraries(charon01 ${LIBS} libcharon01)