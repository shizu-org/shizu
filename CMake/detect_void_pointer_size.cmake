#
# Shizu
# Copyright (C) 2018-2024 Michael Heilmann. All rights reserved.
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

# SUMMARY
# Define ${target}.sizeof_void_pointer to the size, in Bytes, of a void pointer.
#
# DETAIL
# Define ${target}.sizeof_void_pointer to the size, in Bytes, of a void pointer.
# This is usually 4 (on 32 Bit systems) or 8 (on 64 Bit systems).
# PARAM target The target.
macro(Shizu_defineVoidPointerSize target)
  set(${target}.sizeof_void_pointer ${CMAKE_SIZEOF_VOID_P})
endmacro()
