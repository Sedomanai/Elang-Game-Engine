/*****************************************************************//**
 * @file   string.h
 *
 * @brief EL is based on char strings only, provided that all files are in UTF-8, including source,
 * bin, config, and other external files. Minimal wstring interface is provided as nothing more
 * than a bridge to WinAPI. If your Windows system is based on a different locale then remember
 * to change the locale to utf-8 with setlocale(LC_ALL, ".UTF-8") and use u8 string literals
 * when you need to encode non-ascii characters.
 *
 * @brief I've tested everything out under Winows 10 1909 18363.1198 and it works fine.
 * I can't say the same for lower versions as I'm not sure, but if your app is not dependent
 * on the console to print your characters then it's probably fine.
 *
 * @brief In any case this header includes string algorithms for char based strings ONLY,
 * specifically Ascii/UTF-8 encoded strings.
 *
 * ///
 *
 * @brief EL 엔진은 소스 바이너리를 포함한 외부파일을 전부 UTF-8이라고 가정하고 코드를 짜기 때문에
 * 파일 입출력 및 스트링을 전부 char을 기반으로 하고 wbcs는 윈도우 API를 건드릴 때만 사용하도록
 * 최소한의 호환 빼고는 거의 지원을 하지 않습니다.
 * 때문에 utf8를 cp949로 인코딩 된 wstring으로 불러들이거나 _T 매크로를 쓰는 등의 지금도 MS
 * 환경에서 널리 사용되는 불편한 API를 최대한 지양하기로 결정?습니다.
 * 윈도우도 10을 시작으로 해서 UTF-8지원을 시작하는 모양에다 char string의 경우 UTF-8를 변환 없이
 * 그대로 저장할 수 있고 ASCII/ANSI 호환성 등의 이점이 있어서 내린 결정입니다.
 *
 * @brief 대신 콘솔의 경우 setlocale(LC_ALL, ".UTF8")로 한글 깨짐 없이 입출력이 되게끔 해야 합니다.
 * 또 파일 주소 등 입출력을 위한 문자열 리터럴의 경우는 반드시 u8리터럴을 써서 한글 및 다국어 문자가
 * 깨지지 않게 해야 합니다.
 * 그래도 wbcs를 위한 기존의 L리터럴과는 달리 똑같은 char기반 스트링이라 파일 입출력이나 스트링
 * 저장 및 변환은 다른 스트링과 똑같다는 점이 편리한 부분입니다. 게다가 ASCII랑은 완벽 호환이라
 * 한국 문자를 쓰지 않는 리터럴은 기본 리터럴을 써도 될 것입니다.
 *
 * @brief UTF-8 UTF-16변환이 필요한 만약의 경우를 위해 북마크 해둔 라이브러리가 있습니다. 나중에
 * WInAPI를 건드리게 되면 사용할 생각입니다. WinAPI 내장 함수도 분명 존재하지만 리눅스 및 다른 OS 환경에서
 * 사용하기 힘들다는 허점이 있으며, 내장 cstring함수의 경우 윈도우 외의 환경에선 wchar_t가
 * UTF-16이 아닌 UCS-2나 다른 알 수 없는 형태로 변환되는 등의 변수가 있고, 마지막으로 codecvt의 utf8-16
 * 변환 함수 같은 경우는 현재 사용 권장이 되지 않는 관계로 후에 보다 OS환경에 영향을 덜 받는
 * 외부 라이브러리로 통일 할 생각입니다. (현재 쓸 일이 없어서 보류)
 * https://codingtidbit.com/2020/02/09/c17-codecvt_utf8-is-deprecated/
 *
 * @author Sedomanai
 * @date   August 2022
 *********************************************************************/
#pragma once
#include <string>
#include <string_view>

namespace el
{
	typedef std::string	string;
	typedef std::string_view strview;
}