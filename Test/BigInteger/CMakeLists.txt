#
# Shizu
# Copyright (C) 2024 Michael Heilmann. All rights reserved.
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#

set(name ${Shizu.project-name}.Test.BigInteger)

Shizu_beginExecutable()

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger/Main.c)

# base 10 big integers
list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testFromInteger.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testFromInteger.h)

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testType.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testType.h)

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testDefault.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testDefault.h)

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testToInteger.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testToInteger.h)

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testArithmetic.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testArithmetic.h)

list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger10/testFromString.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger10/testFromString.h)

# base 2 big integers
list(APPEND ${name}.source_files Sources/Shizu.Test.BigInteger2/testType.c)
list(APPEND ${name}.header_files Sources/Shizu.Test.BigInteger2/testType.h)

Shizu_endExecutable()

target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/Sources)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sources)

target_link_libraries(${name} PRIVATE ${Shizu.project-name})

add_test(NAME ${name} COMMAND ${name} WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>)

on_executable(${name})
