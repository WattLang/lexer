cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME lexer)
project(${PROJECT_NAME})



include_directories(src/ modules/)



# SOURCE FILES
set(MAIN main.cpp)


file(GLOB_RECURSE ALIASES
    "src/aliases/*.cpp"
)

file(GLOB_RECURSE CODE
    "src/code/*.cpp"
)

file(GLOB_RECURSE LIBS
    "src/libs/*.cpp"
)

file(GLOB_RECURSE POSITION
    "src/position/*.cpp"
)

file(GLOB_RECURSE RUN
    "src/run/*.cpp"
)

file(GLOB_RECURSE TOKEN
    "src/token/*.cpp"
)

file(GLOB_RECURSE TYPE
    "src/type/*.cpp"
)

file(GLOB_RECURSE UTILS
    "src/utils/*.cpp"
)






# EXECUTABLE
add_executable(${PROJECT_NAME} ${ALIASES} ${CODE} ${LIBS} ${POSITION} ${RUN} ${TOKEN} ${TYPE} ${UTILS} ${MAIN})







# FILTERS
source_group("aliases" FILES ${ALIASES})
source_group("code" FILES ${CODE})
source_group("libs" FILES ${LIBS})
source_group("position" FILES ${POSITION})
source_group("run" FILES ${RUN})
source_group("token" FILES ${TOKEN})
source_group("type" FILES ${TYPE})
source_group("utils" FILES ${UTILS})






# LINK
#target_link_libraries(${PROJECT_NAME})







set(WARNINGS_AND_ERRORS "-Wall -Wextra -Wfatal-errors -Wshadow -Wunreachable-code -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wold-style-cast -Wdouble-promotion -Wformat=2")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS_AND_ERRORS} -Ofast -flto -m64")


file(COPY data DESTINATION .)







# PROPERTIES
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
