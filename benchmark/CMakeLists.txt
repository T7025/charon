#include(Celero)

#set(CELERO_SRC )

add_executable(runBenchMarks main.cpp calcNexPos.cpp defaultSettings.h calculateNextStep.cpp)
target_link_libraries(runBenchMarks libcharon celero)
