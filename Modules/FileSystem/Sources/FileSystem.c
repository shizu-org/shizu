#include "FileSystem.h"

static Shizu_TypeDescriptor const Shizu_FileSystem_Type = {
	.postCreateType = NULL,
	.preDestroyType = NULL,
	.visitType = NULL,
	.size = sizeof(Shizu_FileSystem),
	.visit = NULL,
	.finalize = NULL,
	.dispatchSize = sizeof(Shizu_FileSystem_Dispatch),
	.dispatchInitialize = NULL,
	.dispatchUninitialize = NULL,
};

Shizu_defineType(Shizu_FileSystem, Shizu_Object);
