#include "Properties.h"

wstring getCorrectAnswerProperty()
{
	if (ProgramDirectories::programProperties.correctAnswersToDelete == SHRT_MAX)
	{
		return L"correctAnswerProperty OFF";
	}

	return L"correctAnswerProperty " + to_wstring(ProgramDirectories::programProperties.correctAnswersToDelete);
}

void changeCorrectAnswerProperty(wstring& propertyName)
{
	if (ProgramDirectories::programProperties.correctAnswersToDelete == SHRT_MAX)
	{
		ProgramDirectories::programProperties.correctAnswersToDelete = 10;
	}
	else if (ProgramDirectories::programProperties.correctAnswersToDelete == 1)
	{
		ProgramDirectories::programProperties.correctAnswersToDelete = SHRT_MAX;
	}
	else
	{
		ProgramDirectories::programProperties.correctAnswersToDelete--;
	}

	propertyName = getCorrectAnswerProperty();
}

void propertiesMenu(wstring&)
{
	wstring correctAnswerProperty = getCorrectAnswerProperty();

	vector<wstring> propertiesNames = { correctAnswerProperty };
	vector<void (*)(wstring&)> functions = { changeCorrectAnswerProperty };
	wstring selectText = L"Current properties";
	wstring exitText = L"Back";
	wconsoleMenu propertiesMenu(propertiesNames, functions, selectText, exitText);

	propertiesMenu.cyclicSelect();
	ProgramDirectories::saveProperties();
}