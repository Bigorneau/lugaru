#ifndef _LUGARU_GL_H_
#define _LUGARU_GL_H_


#include <cstring>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>

#ifndef WIN32

#include <gl.h>
#include <glu.h>
#include <glext.h>

#else

#define WIN32_LEAN_AND_MEAN
#define Polygon WinPolygon
#include <windows.h>
#undef Polygon
#define GL_GLEXT_PROTOTYPES
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <gl/glext.h>
#include "WinDefs.h"
#include "il/ilut.h"

#define glDeleteTextures( a, b) glDeleteTextures( (a), (const unsigned int *)(b) );

struct RGBColor
{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};
typedef struct RGBColor RGBColor;
typedef RGBColor * RGBColorPtr;

#endif

using namespace std;

#include "logger/logger.h"

#endif