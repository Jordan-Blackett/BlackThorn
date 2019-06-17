#pragma once

#ifdef BT_PLATFORM_WINDOWS
	#ifdef BT_BUILD_DLL
		#define BLACKTHORN_API __declspec(dllexport)
	#else
		#define BLACKTHORN_API __declspec(dllimport)
	#endif
#else
	#error BlackThorn only supports windows!
#endif
