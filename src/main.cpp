#include <iostream>
#include "string.hpp"
//using namespace std;

int main()
{
	// set up locale
	std::clog << "libc locale: " << std::setlocale(LC_ALL, "") << std::endl; // setup C rtl locale
	std::locale::global(std::locale("")); // setup C++ rtl global default locale
	std::clog << "std C++ locale: " << std::locale().name() << std::endl;

	//во вторых - установите локаль для потоков ввода-вывода

	std::cout.imbue(std::locale());
	std::cerr.imbue(std::locale());
	std::clog.imbue(std::locale());

	std::wcout.imbue(std::locale());
	std::wcerr.imbue(std::locale());
	std::wclog.imbue(std::locale());

	// и в третьих - отсоедините потоки ввода-вывода C++ от потоков ввода-вывода C библиотеки

	// for correct io with wcin/wcout
	std::ios::sync_with_stdio(false); // not needed if used only w* streams
	utf::string str = utf::string("Hello World! Привет!");

	//std::wstring s = str;

	std::wcout << str << std::endl;

	std::wcin >> str;

	std::wcout << str << std::endl;

	return 0;
}

