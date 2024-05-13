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

Shizu_FileSystem*
Shizu_FileSystem_create
	(
		Shizu_State* state
	)
{
	Shizu_Type* type = Shizu_FileSystem_getType(state);
	Shizu_FileSystem* self = (Shizu_FileSystem*)Shizu_Gc_allocateObject(state, sizeof(Shizu_FileSystem));
	((Shizu_Object*)self)->type = type;
	return self;
}
