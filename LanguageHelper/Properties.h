#pragma once
#include "ProgramDirectories.h"
#include "AdditionalFunctions.h"

wstring getPropertyOptionName(ProgramDirectories::Property* prop);
void changeMaxLinesInWindow(wstring& propertyName);
void changeAnswerTimeProperty(wstring& propertyName);
void changeCorrectAnswerProperty(wstring& propertyName);
void propertiesMenu(wstring&);