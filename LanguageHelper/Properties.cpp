#include "Properties.h"

wstring getPropertyOptionName(ProgramDirectories::Property* prop)
{
	if (
		prop->name == ProgramDirectories::programProperties.correctAnswersToDelete.name ||
		prop->name == ProgramDirectories::programProperties.millisecondsToAnswerForCharacter.name
	) {
		if (prop->value >= INT_MAX)
		{
			return prop->name + L" OFF";
		}

		return prop->name + L" " + to_wstring(prop->value);
	}
	else if (
		prop->name == ProgramDirectories::programProperties.endlessTesting.name ||
		prop->name == ProgramDirectories::programProperties.randomTesting.name
	) {
		if (prop->value == 0)
		{
			return prop->name + L" OFF";
		}
		else
		{
			return prop->name + L" ON";
		}
	}
}

void changeAnswerTimeProperty(wstring& propertyName)
{
	ProgramDirectories::Property* answerTimeProperty = &ProgramDirectories::programProperties.millisecondsToAnswerForCharacter;

	wcout << L"Set time to answer in milliseconds for character: ";
	if (!(wcin >> answerTimeProperty->value))
	{
		answerTimeProperty->value = INT_MAX;
		wcin.clear();
	}
}

void changeCorrectAnswerProperty(wstring& propertyName)
{
	ProgramDirectories::Property* correctAnswerProperty = &ProgramDirectories::programProperties.correctAnswersToDelete;
	ProgramDirectories::Property* randomTestingProperty = &ProgramDirectories::programProperties.randomTesting;

	if (randomTestingProperty->value)
	{
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
	}
}

void changeEndlessTestingProperty(wstring& propertyName)
{
	ProgramDirectories::Property* endlessTestingProperty = &ProgramDirectories::programProperties.endlessTesting;

	if (endlessTestingProperty->value == 0)
	{
		endlessTestingProperty->value = 1;
	}
	else
	{
		endlessTestingProperty->value = 0;
	}
}

void changeRandomTestingProperty(wstring& propertyName)
{
	ProgramDirectories::Property* correctAnswerProperty = &ProgramDirectories::programProperties.correctAnswersToDelete;
	ProgramDirectories::Property* randomTestingProperty = &ProgramDirectories::programProperties.randomTesting;

	if (randomTestingProperty->value == 0)
	{
		randomTestingProperty->value = 1;
	}
	else
	{
		randomTestingProperty->value = 0;
		correctAnswerProperty->value = INT_MAX;
	}
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

	if (askQuestion(L"Would you like to use asian characters?")) {
		ProgramDirectories::programProperties.fontIndex.value = 1;
	}
	else {
		ProgramDirectories::programProperties.fontIndex.value = 0;
	}

	wconsoleMenu::setFontInfo(
		ProgramDirectories::programProperties.fontWeight.value, 
		ProgramDirectories::programProperties.fontWidth.value,
		ProgramDirectories::programProperties.fontHeight.value,
		ProgramDirectories::programProperties.fontIndex.value
	);
}

void deleteProfile(wstring&) {
	wstring profilePath = ProgramDirectories::globalPath + L'\0';

	SHFILEOPSTRUCT shfo = {
		NULL,
		FO_DELETE,
		profilePath.c_str(),
		NULL,
		FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
		FALSE,
		NULL,
		NULL
	};

	SHFileOperationW(&shfo);
	exit(0);
}

void propertiesMenu(wstring&)
{
	vector<void (*)(wstring&)> functions = {
		changeCorrectAnswerProperty ,
		changeAnswerTimeProperty ,
		changeEndlessTestingProperty ,
		changeRandomTestingProperty ,
		setWindowSizeOption ,
		setFont ,
		deleteProfile
	};

	wstring selectText = L"Current properties";
	wstring exitText = L"Back";
	wconsoleMenu propertiesMenu;

	short optionNum = 0;
	do
	{
		vector<wstring> propertiesNames = {
			getPropertyOptionName(&ProgramDirectories::programProperties.correctAnswersToDelete) ,
			getPropertyOptionName(&ProgramDirectories::programProperties.millisecondsToAnswerForCharacter) ,
			getPropertyOptionName(&ProgramDirectories::programProperties.endlessTesting) ,
			getPropertyOptionName(&ProgramDirectories::programProperties.randomTesting) ,
			L"Set window size" ,
			L"Set font" ,
			L"Delete profile"
		};

		propertiesMenu = wconsoleMenu(propertiesNames, functions, selectText, exitText);
		propertiesMenu.singleSelect(optionNum);
	} while (optionNum != -1);

	ProgramDirectories::saveProperties();
}