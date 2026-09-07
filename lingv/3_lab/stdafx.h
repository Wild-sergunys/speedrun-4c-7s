// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//


/* 
* Нужно для прекомпиляции файлов котоыре включаются почти везде
* Компиилируеются один раз и сохраняются в .pch
* Если не написать все инклюды буду ткоспилироваться заново
* В целом оставлено для совместимости с старыми VS, полезно, но можно сказать, что легаси
* В C=++ 20+ есть модули, работают более эффективно
*/

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include "lexical_analizer.h"

// TODO: reference additional headers your program requires here
