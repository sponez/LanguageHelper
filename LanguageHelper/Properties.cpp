#include "Properties.h"

wstring getPropertyOptionName(ProgramDirectories::Property* prop)
{
	if (prop->value >= INT_MAX)
	{
		return prop->name + L" OFF";
	}

	return prop->name + L" " + to_wstring(prop->value);
}

void changeMaxLinesInWindow(wstring& propertyName)
{
	ProgramDirectories::Property* maxLinesInWindowProperty = &ProgramDirectories::programProperties.maxLinesInWindow;

	if (maxLinesInWindowProperty->value <= 5)
	{
		maxLinesInWindowProperty->value = 100;
	}
	else
	{
		maxLinesInWindowProperty->value -= 1;
	}

	wconsoleMenu::maxLinesInWindow = maxLinesInWindowProperty->value;
	propertyName = getPropertyOptionName(maxLinesInWindowProperty);
}

void changeAnswerTimeProperty(wstring& propertyName)
{
	ProgramDirectories::Property* answerTimeProperty = &ProgramDirectories::programProperties.millisecondsToAnswerForCharacter;

	if (answerTimeProperty->value == INT_MAX)
	{
		answerTimeProperty->value = 3000;
	}
	else if (answerTimeProperty->value <= 200)
	{
		answerTimeProperty->value = INT_MAX;
	}
	else
	{
		answerTimeProperty->value -= 200;
	}

	propertyName = getPropertyOptionName(answerTimeProperty);
}

void changeCorrectAnswerProperty(wstring& propertyName)
{
	ProgramDirectories::Property* correctAnswerProperty = &ProgramDirectories::programProperties.correctAnswersToDelete;

	if (correctAnswerProperty->value == INT_MAX)
	{
		correctAnswerProperty->value = 10;
	}
	else if (correctAnswerProperty->value <= 1)
	{
		correctAnswerProperty->value = INT_MAX;
	}
	else
	{
		correctAnswerProperty->value--;
	}

	propertyName = getPropertyOptionName(correctAnswerProperty);
}

void propertiesMenu(wstring&)
{
	vector<wstring> propertiesNames = {
		getPropertyOptionName(&ProgramDirectories::programProperties.correctAnswersToDelete) ,
		getPropertyOptionName(&ProgramDirectories::programProperties.millisecondsToAnswerForCharacter) ,
		getPropertyOptionName(&ProgramDirectories::programProperties.maxLinesInWindow)
	};
	vector<void (*)(wstring&)> functions = { changeCorrectAnswerProperty , changeAnswerTimeProperty , changeMaxLinesInWindow };
	wstring selectText = L"Current properties";
	wstring exitText = L"Back";
	wconsoleMenu propertiesMenu(propertiesNames, functions, selectText, exitText);

	propertiesMenu.cyclicSelect();
	ProgramDirectories::saveProperties();
}