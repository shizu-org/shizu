# Copyright (c) 2024 Michael Heilmann. All rights reserved.

# The following code generates project files to build (and clean) the "Abstract Machine Specification".
# - For the MAKE generator this manifests as a make file.
# - For the VISUAL STUDIO generator this manifests as a project.
# - ...

#project("Specification")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/index.pdf
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/index.tex
  COMMAND latexmk -pdf ${CMAKE_CURRENT_SOURCE_DIR}/index.tex
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/index.aux ${CMAKE_CURRENT_BINARY_DIR}/index.log ${CMAKE_CURRENT_BINARY_DIR}/index.fls ${CMAKE_CURRENT_BINARY_DIR}/index.fdb_latexmk
  COMMENT "building ${CMAKE_CURRENT_SOURCE_DIR}/index.tex => ${CMAKE_CURRENT_BINARY_DIR}/index.pdf"
)

add_custom_target("Specification" ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/index.pdf
)
