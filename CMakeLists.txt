cmake_minimum_required(VERSION 3.0)

# This is the name of the resulting library, without
# the .dll (Windows) or .so (GNU+Linux)
set(plugin_name shader_tool)

# This is the project name, often shown in IDEs
# On Windows, this is the filename of the Visual Studio project
project(ShaderTool)

# Use CXX 17
set(CMAKE_CXX_STANDARD 17)

# Find the source file
file(GLOB_RECURSE sources src/*.cpp include/*.h)

set(mxml_sources mxml-attr.c mxml-entity.c mxml-file.c mxml-get.c mxml-index.c
	mxml-node.c mxml-search.c mxml-set.c mxml-private.c mxml-string.c)
list(TRANSFORM mxml_sources PREPEND lib/mxml/)
add_library(mxml STATIC ${mxml_sources})
target_include_directories(mxml PRIVATE lib/configs)
if(UNIX)
	target_compile_options(mxml PRIVATE -std=gnu99 -Wall -fPIC)
elseif(WIN32)
	target_compile_options(mxml PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()
target_include_directories(mxml PUBLIC lib/mxml)

# Set up loading of the statically-linked base library
link_directories(lib)

# Add the library
add_library(${plugin_name} SHARED ${sources})
target_include_directories(${plugin_name} PUBLIC include)

# Include the base library
# TODO add GitLab URL
target_include_directories(${plugin_name} PUBLIC lib)
target_link_libraries(${plugin_name} sblt_plugin)
target_link_libraries(${plugin_name} d3dcompiler)
target_link_libraries(${plugin_name} mxml)

# For development purposes, you may find this helpful
# The mod the DLL will be copied into a mod folder whenever
# you compile the project

set(target_mod_name ShaderPatcher)

add_custom_command(
	TARGET ${plugin_name}
	POST_BUILD
	COMMAND powershell -command copy \\\"$<TARGET_FILE:${plugin_name}>\\\" \\\"C:/Program Files \(x86\)/Steam/steamapps/common/PAYDAY 2/mods/${target_mod_name}/native/${plugin_name}.dll\\\"
)

