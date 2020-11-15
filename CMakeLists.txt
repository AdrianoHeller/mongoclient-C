# Specify the minimum version you require.
find_package (mongoc-1.0 1.7 REQUIRED)

# The "hello_mongoc.c" sample program is shared among four tests.
add_executable (hello_mongoc ../../hello_mongoc.c)
target_link_libraries (hello_mongoc PRIVATE mongo::mongoc_shared)