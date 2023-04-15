#include "Properties.h"

wstring getPropertyOptionName(ProgramDirectories::Property* prop)
{
	if (prop->value >= INT_MAX)
	{
		return prop->name + L" OFF";
	}

	return prop->name + L" " + to_wstring(prop->value);
}

void changeAnswerTimeProperty(wstring& propertyName)
{
	ProgramDirectories::Property* answerTimeProperty = &ProgramDirectories::programProperties.millisecondsToAnswerForCharacter;

	if (answerTimeProperty->value == INT_MAX)
	{
		answerTimeProperty->value = 2000;
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
		correctAnswerProperty->value = 5;
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

void setWindowSizeOption(wstring&)
{
	_wsystem(L"cls");

	wcout << L"Window width in pixels = ";
	wcin >> ProgramDirectories::programProperties.windowWidth.value;
	if (ProgramDirectories::programProperties.windowWidth.value <= 400)
	{
		ProgramDirectories::programProperties.windowWidth.value = 400;
	}

	wcout << L"Window height in pixels = ";
	wcin >> ProgramDirectories::programProperties.windowHeight.value;
	if (ProgramDirectories::programProperties.windowHeight.value <= 400)
	{
		ProgramDirectories::programProperties.windowHeight.value = 400;
	}

	wconsoleMenu::setWindowSize(
		ProgramDirectories::programProperties.windowWidth.value,
		ProgramDirectories::programProperties.windowHeight.value
	);
}

void setFont(wstring&)
{
	_wsystem(L"cls");

	wcout << L"Font weight (> 10) = ";
	wcin >> ProgramDirectories::programProperties.fontWeight.value;
	if (ProgramDirectories::programProperties.fontWeight.value < 11)
	{
		ProgramDirectories::programProperties.fontWeight.value = 11;
	}

	wcout << L"Font width (> 10) = ";
	wcin >> ProgramDirectories::programProperties.fontWidth.value;
	if (ProgramDirectories::programProperties.fontWidth.value < 11)
	{
		ProgramDirectories::programProperties.fontWidth.value = 11;
	}

	wcout << L"Font height (> 10) = ";
	wcin >> ProgramDirectories::programProperties.fontHeight.value;
	if (ProgramDirectories::programProperties.fontHeight.value < 11)
	{
		ProgramDirectories::programProperties.fontHeight.value = 11;
	}

	wconsoleMenu::setFontInfo(
		ProgramDirectories::programProperties.fontWeight.value, 
		ProgramDirectories::programProperties.fontWidth.value,
		ProgramDirectories::programProperties.fontHeight.value
	);
}

void propertiesMenu(wstring&)
{
	vector<wstring> propertiesNames = {
		getPropertyOptionName(&ProgramDirectories::programProperties.correctAnswersToDelete) ,
		getPropertyOptionName(&ProgramDirectories::programProperties.millisecondsToAnswerForCharacter) ,
		L"Set window size" ,
		L"Set font"
	};
	vector<void (*)(wstring&)> functions = { changeCorrectAnswerProperty , changeAnswerTimeProperty , setWindowSizeOption , setFont };
	wstring selectText = L"Current properties";
	wstring exitText = L"Back";
	wconsoleMenu propertiesMenu(propertiesNames, functions, selectText, exitText);

	propertiesMenu.cyclicSelect();
	ProgramDirectories::saveProperties();
}