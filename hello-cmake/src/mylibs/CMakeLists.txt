ADD_LIBRARY(mylibs mylib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/mylibs)
ADD_EXECUTABLE(exename myapp.cpp)
TARGET_LINK_LIBRARIES(exename mylibs)
