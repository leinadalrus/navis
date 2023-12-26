#ifndef PALE_NOEL_HH
#define PALE_NOEL_HH

#if _WIN32 || __linux__ || __MACH__

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <istream>
#include <ostream>

#include <string>
#include <type_traits>

#endif// _WIN32 || __linux__ || __MACH__

#ifdef _WIN32
#include <C:\\raylib\\raylib\\src\\raylib.h>
#endif// _WIN32

#ifdef __linux__
#include <raylib.h>
#endif//  __linux__

#endif// PALE_NOEL_HH
