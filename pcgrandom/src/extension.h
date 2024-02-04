#ifndef extension_h
#define extension_h

// The name of the extension affects Lua module name and Java package name.
#define EXTENSION_NAME pcgrandom

// Convert extension name to C const string.
#define STRINGIFY(s) #s
#define STRINGIFY_EXPANDED(s) STRINGIFY(s)
#define EXTENSION_NAME_STRING STRINGIFY_EXPANDED(EXTENSION_NAME)

// Each extension must have unique exported symbols. Construct function names based on the extension name.
#define FUNCTION_NAME(extension_name, function_name) Extension_ ## extension_name ## _ ## function_name
#define FUNCTION_NAME_EXPANDED(extension_name, function_name) FUNCTION_NAME(extension_name, function_name)

#define APP_INITIALIZE FUNCTION_NAME_EXPANDED(EXTENSION_NAME, AppInitialize)
#define APP_FINALIZE FUNCTION_NAME_EXPANDED(EXTENSION_NAME, AppFinalize)
#define INITIALIZE FUNCTION_NAME_EXPANDED(EXTENSION_NAME, Initialize)
#define UPDATE FUNCTION_NAME_EXPANDED(EXTENSION_NAME, Update)
#define FINALIZE FUNCTION_NAME_EXPANDED(EXTENSION_NAME, Finalize)

#include <dmsdk/sdk.h>

#endif
