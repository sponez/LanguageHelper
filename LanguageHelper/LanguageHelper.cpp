#include "AdditionalFunctions.h"
#include "ConsoleMenu.h"
#include "TestProcess.h"
#include "ViewProcess.h"
#include "AddProcess.h"
#include "UpdateProcess.h"
#include "DeleteProcess.h"
using namespace std;

int main(int argc, wchar_t* argv[])
{
	int setmode;
	setmode = _setmode(_fileno(stdout), _O_U16TEXT);
	setmode = _setmode(_fileno(stdin), _O_U16TEXT);
	setmode = _setmode(_fileno(stderr), _O_U16TEXT);

	wchar_t c_username[257];
	DWORD username_len = 257;
	GetUserNameW(c_username, &username_len);
	globalPath = L"C:\\Users\\" + wstring(c_username) + L"\\AppData\\Local\\LanguageHelper";
	createDirrectory(globalPath);

	wifstream savedLanguagesFile(globalPath + L"\\languages.cfg");
	if (savedLanguagesFile.is_open())
	{
		savedLanguagesFile >> firstLanguage >> secondLanguage;
		savedLanguagesFile.close();
	}
	else
	{
		wcout << L"Global initializing.\nPlease, enter the first using language: ";
		wcin >> firstLanguage;

		wcout << L"Please, enter the second using language: ";
		wcin >> secondLanguage;

		wstringStandartForm(firstLanguage);
		wstringStandartForm(secondLanguage);

		wofstream languages(globalPath + L"\\languages.cfg");
		languages << firstLanguage << endl << secondLanguage;
		languages.close();
	}

	vector<wstring> options = { L"Testing" , L"View existing words",  L"Add word" , L"Update word", L"Delete word" };
	vector<void (*)(wstring)> functions = { testingOption, viewWordsOption, addWordOption, updateWordOption, deleteWordOption };
	wconsoleMenu languageHelperMenu(L"What do you want to do", options, functions, L"Close application");
	languageHelperMenu.select();
}