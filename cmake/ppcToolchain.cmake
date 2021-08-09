cmake_minimum_required(VERSION 3.2)
# Set the path to the repo root
set(SC_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." CACHE FILEPATH "The root directory to the Scuffedcraft repo")
mark_as_advanced(SC_ROOT)


# Firstly ensure that the user has got the sc_env.sh file created.
