
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

pbuilder_library(
        TARGET GHOST-Utilities
        SOURCES ${SRC}
        PROJECT_LIBRARIES ToolFrameworkCore::ToolFrameworkCore
        PUBLIC_EXTERNAL_LIBRARIES ${PUBLIC_EXT_LIBS}
        PRIVATE_EXTERNAL_LIBRARIES ${PRIVATE_EXT_LIBS}
)

pbuilder_install_headers(${HEADERS})



pbuilder_component_install_and_export(
        COMPONENT Library
        LIBTARGETS GHOST-Utilities
)
