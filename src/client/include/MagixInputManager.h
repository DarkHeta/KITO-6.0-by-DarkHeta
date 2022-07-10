#pragma once

#define INPUT_CONTROL 1
#define INPUT_TEXT 2

#define MAX_TEMPSTORAGE 5

//#include "MagixGUI.h"
#include <OIS.h>
#include "Ogre.h"

using namespace Ogre;

class MagixInputManager
{
protected:
	OverlayElement *mInputBoxText;
	unsigned short inputMode;
	UTFString inputText;
	OIS::KeyCode heldKey;
	unsigned int heldKeyText;
	Real heldDelay;
	unsigned short inputCursorPos;
	UTFString defaultCaption;
	bool clearAtReturn;
	bool allowTempStorage;
	bool allowNewLine;
	bool hideText;
	unsigned short charLimit;
	bool isShiftDown;
	UTFString tempStorage[MAX_TEMPSTORAGE];
	short tempStoragePos;
	int heldHotkey;
public:
	inline std::wstring atow(const std::string& str)
	{
		std::wstringstream wstrm;
		wstrm << str.c_str();
		return wstrm.str();
	}
	MagixInputManager();
	~MagixInputManager();
	void update(const FrameEvent evt);
	bool toggleInputMode();
	unsigned short getInputMode();
	void setString(const UTFString &caption);
	void pushChar(const unsigned int &a);
	void popChar(bool del = false);
	void setInputCursorPos(const unsigned short &pos);
	void setInputText(const UTFString &text, bool enable);
	void normalizeText(UTFString &caption, OverlayElement *box);
	const UTFString getInputText();
	void keyHeld(OIS::Keyboard *keyboard, const OIS::KeyCode &key, const unsigned int &text, bool held);
	void inputHeldKey();
	void setInputBox(OverlayElement *inputBox, const UTFString &caption, unsigned short limit, bool clear, const UTFString &startText, bool bTempStorage, bool bNewLine, bool bHideText);
	OverlayElement* getInputBox();
	void setClearAtReturn(bool clear);
	void pushTempStorage(const UTFString &input);
	const UTFString getTempStorage(bool increment = false, bool decrement = false);
	void hotkeyHeld(const int &key);
	const int getHeldHotkey();

};
