#pragma once
#include "ProgramDirectories.h"
#include "AdditionalFunctions.h"

wstring findWordPath(wstring word, wstring language);
void getRandomWordsFromLearned(wstring&);
void addTranslationsManual(wstring& word, wstring& wordPath, vector<wstring>& translations);
void deleteTranslationManual(wstring& word, wstring& wordPath, vector<wstring>& translations, wstring& translationToDelete);
void addWordFunction(wstring&);
void deleteWordFunction(wstring&);
void moveToUnlearned(wstring&);
void addTranslationFunction(wstring&);
void translationEditorFunction(wstring& optionName);
void translatorEditor(wstring&);
void displayTranslations(wstring&);
void ranameWordFunction(wstring&);
void wordEditor(wstring& word);
void displayAllSavedWordsToEdit(wstring&);
void addNewOrEdit(wstring& stage);
void wordsStageToManage(wstring& language);
void wordsLanguageToManage(wstring&);