#pragma once
#ifdef ELANG_LIBRARY
#define ELANG_DLL __declspec(dllexport)
#else
#define ELANG_DLL __declspec(dllimport)
#endif

#pragma warning( disable : 4251 ) // compose non dll data type (from static library)
#pragma warning( disable : 4275 ) // inherit non dll (from static library)
#pragma warning( disable : 4661 ) // inherit non dll template class 
