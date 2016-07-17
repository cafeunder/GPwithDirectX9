#ifndef SYSTEM_UTILITIES_DEF
#define SYSTEM_UTILITIES_DEF

#include <cassert>

//条件に一致した場合アサートを返すマクロ関数
#define HALT(condition) (assert(!(condition)))

///Visual Studioの出力ウィンドウ-デバッグに書式付き文字列を表示する関数（改行なし）
void DebugPrint(const char* format, ...);
///Visual Studioの出力ウィンドウ-デバッグに書式付き文字列を表示する関数（改行あり）
void DebugPrintLine(const char* format, ...);

#endif