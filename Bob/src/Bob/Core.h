#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#ifndef BOB_PLATFORM_WINDOWS
			#define BOB_PLATFORM_WINDOWS
		#endif // !BOB_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define BOB_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define BOB_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
	 /* We also have to check __ANDROID__ before __linux__
		* since android is based on the linux kernel
		* it has __linux__ defined */
#elif defined(__ANDROID__)
	#define BOB_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#ifndef BOB_PLATFORM_LINUX
		#define BOB_PLATFORM_LINUX
	#endif 
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef BOB_PLATFORM_WINDOWS
	#if BOB_DYNAMIC_LINK
		#ifdef BOB_BUILD_DLL
			#define BOB_API __declspec(dllexport)
		#else
			#define BOB_API __declspec(dllimport)
		#endif
	#else
			#define BOB_API
	#endif
#elif defined(BOB_PLATFORM_LINUX)
	#if BOB_DYNAMIC_LINK
		#ifdef BOB_BUILD_DLL
			#define BOB_API __attribute__((visibility("default")))
		#else
			#define BOB_API 
		#endif
	#else
			#define BOB_API
	#endif
#else
	#error Bob only supports Windows and Linux!
#endif // End of DLL support


#ifdef BOB_DEBUG
	#define BOB_ENABLE_ASSERTS
#else
#endif

#ifdef BOB_ENABLE_ASSERTS
	#ifdef BOB_PLATFORM_WINDOWS
		#define BOB_ASSERT(name, x, ...) { if(!(x)) { BOB_ERROR(name, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#elif defined(BOB_PLATFORM_LINUX)
		#include <signal.h>
		#define BOB_ASSERT(name, x, ...) { if(!(x)) { BOB_ERROR(name, "Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
	#else
		#error Platform not regognize
	#endif
#else
	#define BOB_ASSERT(name, x, ...)
#endif