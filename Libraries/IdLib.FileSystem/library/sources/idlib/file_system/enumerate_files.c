/*
  IdLib File System
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "idlib/file_system/enumerate_files.h"

#include "idlib/file_system/status.h"

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM
	
	#define IDLIB_DIRECTORY_SEPARATOR "\\"

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

	#define IDLIB_DIRECTORY_SEPARATOR "/"

#else

	#error("operating system not (yet) supported")

#endif

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <strsafe.h>
	#include <tchar.h>

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM
	
	#include <sys/types.h>
	#include <dirent.h>
	// strcmp
	#include <string.h>

#else

	#error("operating system not (yet) supported")

#endif

int
idlib_enumerate_files
  (
    char const* path,
		idlib_enumerate_files_callback_context* callback_context,
    idlib_enumerate_files_callback_function* callback_function,
		bool regular_files,
		bool directory_files
  )
{
  if (!path || !callback_function) {
    return IDLIB_ARGUMENT_INVALID;
  }

#if IDLIB_OPERATING_SYSTEM_WINDOWS == IDLIB_OPERATING_SYSTEM

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	StringCchCopy(szDir, MAX_PATH, path);
	StringCchCat(szDir, MAX_PATH, TEXT(IDLIB_DIRECTORY_SEPARATOR "*"));
	HANDLE hFind = INVALID_HANDLE_VALUE;
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return IDLIB_ENVIRONMENT_FAILED;
	}
	do {
		if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, "..")) {
			continue;
		}
		bool is_directory_file = ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		bool is_regular_file = !is_directory_file;
		if ((is_directory_file && directory_files) || (is_regular_file && regular_files)) {
			if (!callback_function(callback_context, ffd.cFileName, strlen(ffd.cFileName))) {
				FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
				return IDLIB_ABORTED;
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;

#elif IDLIB_OPERATING_SYSTEM_LINUX == IDLIB_OPERATING_SYSTEM || \
      IDLIB_OPERATING_SYSTEM_CYGWIN == IDLIB_OPERATING_SYSTEM

	DIR* dir;
	struct dirent* ent;
	dir = opendir(path);
	if (!dir) {
		return IDLIB_ENVIRONMENT_FAILED;
	}
	while ((ent = readdir(dir)) != NULL) {
	#if 0
		fprintf(stdout, "%s (", ent->d_name);
		switch (ent->d_type) {
			case DT_BLK:
				fprintf(stdout, "DB_BLK");
				break;
			case DT_CHR:
				fprintf(stdout, "DT_CHR");
				break;
			case DT_DIR:
				fprintf(stdout, "DT_DIR");
				break;
			case DT_FIFO:
				fprintf(stdout, "DT_FIFO");
				break;
			case DT_LNK:
				fprintf(stdout, "DT_LNK");
				break;
			case DT_REG:
				fprintf(stdout, "DT_REG");
				break;
			case DT_SOCK:
				fprintf(stdout, "DT_SOCK");
				break;
			case DT_UNKNOWN:
				fprintf(stdout, "DT_UNKNOWN");
				break;
			default:
				fprintf(stdout, "<unknown>");
				break;
		};
		fprintf(stdout, ")\n");
	#endif
		if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
			continue;
		}
		if ((ent->d_type == DT_DIR && directory_files) || (ent->d_type == DT_REG && regular_files)) {
			if (!callback_function(callback_context, ent->d_name, strlen(ent->d_name))) {
				closedir(dir);
				dir = NULL;
				return IDLIB_ABORTED;
			}
		}
	}
	closedir(dir);
	dir = NULL;

#else

	#error("operating system not (yet) supported")

#endif

  return IDLIB_SUCCESS;
}

