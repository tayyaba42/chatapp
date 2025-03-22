cmake_minimum_required(VERSION 3.10)

project(chatapp)

# Enable testing
enable_testing()

# Add C++ source files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add test files
file(GLOB_RECURSE TESTS test/*.cpp)

# Add database file path
set(DATABASE_FILE_PATH "/home/tayyaba/Desktop/projects/chatapp/src/chatapp.db")  

# Copy the database file to the build directory
configure_file(${DATABASE_FILE_PATH} ${CMAKE_CURRENT_BINARY_DIR}/chatapp.db COPYONLY)

# Find SQLite3 package
find_package(SQLite3 REQUIRED)

# Add an executable target for your main program
add_executable(ChatApp ${SOURCES})
target_link_libraries(ChatApp SQLite::SQLite3)

# Download and link GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Add test executable target
add_executable(MyTests ${TESTS})

# Link GoogleTest and SQLite3 to test executable
target_link_libraries(MyTests gtest gtest_main SQLite::SQLite3)

# Register tests
add_test(NAME MyTests COMMAND MyTests ${CMAKE_CURRENT_BINARY_DIR}/chatapp.db)

# Install executables
install(TARGETS ChatApp MyTests DESTINATION bin)

# CPack settings for packaging
set(CPACK_PACKAGE_NAME "chatapp")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "DEB")
include(CPack)
