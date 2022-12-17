#include "AdditionalFunctions.h"
#include "wconsoleMenu.h"
#include "TestProcess.h"
#include "ManageWordsProcess.h"
#include "ViewProcess.h"
using namespace std;

int main(int argc, wchar_t* argv[])
{
	wchar_t c_username[257];
	DWORD username_len = 257;
	GetUserNameW(c_username, &username_len);
	globalPath = L"C:\\Users\\" + wstring(c_username) + L"\\AppData\\Local\\Language Helper";
	createDirrectory(globalPath);

	wifstream savedLanguagesFile(globalPath + L"\\languages.cfg");
	savedLanguagesFile.imbue(std::locale(savedLanguagesFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	if (savedLanguagesFile.is_open())
	{
		savedLanguagesFile >> firstLanguage >> secondLanguage;
		savedLanguagesFile.close();
	}
	else
	{
		wcout << L"Global initializing." << endl;
		wcout << L"Please, enter the first using language: ";
		wcin >> firstLanguage;

		wcout << L"Please, enter the second using language: ";
		wcin >> secondLanguage;
		wcin.ignore(LLONG_MAX, L'\n');

		wstringStandartForm(firstLanguage);
		wstringStandartForm(secondLanguage);

		wofstream languages(globalPath + L"\\languages.cfg");
		languages << firstLanguage << endl << secondLanguage;
		languages.close();
	}

	vector<wstring> options = { L"View saved words" , L"Testing" , L"Manage words"};
	vector<void (*)(wstring&)> functions = { viewOption, testingOption , manageWordsOption };
	wconsoleMenu languageHelperMenu(options, functions, L"What do you want to do", L"Close application");
	languageHelperMenu.cyclicSelect();
}