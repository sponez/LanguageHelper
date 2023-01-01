#include "AdditionalFunctions.h"
#include "wconsoleMenu.h"
#include "TestProcess.h"
#include "ManageWordsProcess.h"
#include "ViewProcess.h"
using namespace std;

int main(int argc, wchar_t* argv[])
{
	ProgramDirectories::getProgramDirectories();

	vector<wstring> options = { L"View saved words" , L"Testing" , L"Manage words" };
	vector<void (*)(wstring&)> functions = { wordsLanguageToView, wordsLanguageToTest , wordsLanguageToManage };
	wconsoleMenu languageHelperMenu(options, functions, L"What do you want to do", L"Close application");
	languageHelperMenu.cyclicSelect();
}