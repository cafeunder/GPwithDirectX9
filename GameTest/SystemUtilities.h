#ifndef SYSTEM_UTILITIES_DEF
#define SYSTEM_UTILITIES_DEF

#include <cassert>

//�����Ɉ�v�����ꍇ�A�T�[�g��Ԃ��}�N���֐�
#define HALT(condition) (assert(!(condition)))

///Visual Studio�̏o�̓E�B���h�E-�f�o�b�O�ɏ����t���������\������֐��i���s�Ȃ��j
void DebugPrint(const char* format, ...);
///Visual Studio�̏o�̓E�B���h�E-�f�o�b�O�ɏ����t���������\������֐��i���s����j
void DebugPrintLine(const char* format, ...);

#endif