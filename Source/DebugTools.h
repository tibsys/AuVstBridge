#pragma once

#include <iostream>
#ifdef _MSC_VER
#include <Windows.h>
#endif

class DebugTools {
private:
	DebugTools() {}

public:
	static void log(std::stringstream msg) {
		msg << std::endl; //Append a LF
		DebugTools::log(msg.str());
	};

	static void log(const char *msg) {
		DebugTools::log(std::string (msg));
	}

	static void log(std::string msg) {
#ifndef _DEBUG
		return;
#else
#ifdef _MSC_VER
		OutputDebugStringA(msg.append("\n").c_str());
#else
		std::cout << msg.c_str() << std::endl;
#endif
#endif
	};
};