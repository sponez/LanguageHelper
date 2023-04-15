#pragma once
#include "ProgramDirectories.h"
#include "AdditionalFunctions.h"

wstring getPropertyOptionName(ProgramDirectories::Property* prop);
void changeAnswerTimeProperty(wstring& propertyName);
void changeCorrectAnswerProperty(wstring& propertyName);
void setWindowSizeOption(wstring&);
void setFont(wstring&);
void propertiesMenu(wstring&);