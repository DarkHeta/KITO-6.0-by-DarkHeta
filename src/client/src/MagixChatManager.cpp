#include "MagixChatManager.h"

void MagixChatManager::reset(bool clearHistory)
{
	if (clearHistory)
	{
		for (int i = 0; i<MAX_CHANNELS; i++)
		{
			chatString[i].clear();
			chatSayer[i].clear();
			chatType[i].clear();
			hasNewLine[i] = false;
		}
	}
}
void MagixChatManager::push(const UTFString &caption, const String &sayer, const unsigned char &type)
{
	const unsigned short tChannel = (type == CHAT_PRIVATE ? channel : ((type == CHAT_LOCAL || type == CHAT_LOCALACTION || type == CHAT_LOCALEVENT) ? 0 : ((type == CHAT_PARTY || type == CHAT_PARTYACTION || type == CHAT_PARTYEVENT) ? 2 : 1)));
	chatString[tChannel].push_back(caption);
	chatSayer[tChannel].push_back(sayer);
	chatType[tChannel].push_back(type);
	if (int(chatString[tChannel].size())>MAX_LINES)
	{
		chatString[tChannel].erase(chatString[tChannel].begin(), ++chatString[tChannel].begin());
		chatSayer[tChannel].erase(chatSayer[tChannel].begin(), ++chatSayer[tChannel].begin());
		chatType[tChannel].erase(chatType[tChannel].begin(), ++chatType[tChannel].begin());
	}

	hasNewLine[tChannel] = true;
}
const String MagixChatManager::getChatBlock(const unsigned short &lines, const Real &boxWidth, const Real &charHeight, const Real &lastOffset)
{
	vector<String>::type tChatBlock;
	tChatBlock.clear();
	String tFinalChat = "";
	unsigned short tFinalLines = 0;
	int start = int(chatString[channel].size()) - lines - (int)lastOffset*(int(chatString[channel].size()) - lines);
	if (start<0)start = 0;
	//Process entire chatblock within estimated range
	for (int i = start; i<int(chatString[channel].size()); i++)
	{
		const String tLineBlock = prefixChatLine(processChatString(chatString[channel][i], chatSayer[channel][i], chatType[channel][i], boxWidth, charHeight), chatSayer[channel][i], chatType[channel][i]);
		vector<String>::type tCaption = StringUtil::split(tLineBlock, "\n");

		for (int j = 0; j<int(tCaption.size()); j++)
		{
			tChatBlock.push_back(tCaption[j]);
		}
	}
	//Process actual block to return
	start = int(tChatBlock.size()) - lines - (int)lastOffset*(int(tChatBlock.size()) - lines);
	if (start<0)start = 0;
	for (int i = start; i<int(tChatBlock.size()); i++)
	{
		tFinalChat += (i == start ? "" : "\n");
		tFinalChat += tChatBlock[i];
		tFinalLines += 1;
		if (tFinalLines >= lines)return tFinalChat;
	}
	return tFinalChat;
}
void MagixChatManager::processInput(UTFString &caption, unsigned char &type, UTFString &param)
{
	if (channel == 0)type = CHAT_LOCAL;
	if (channel == 2)type = CHAT_PARTY;
	param = "";

	//Set home point
	if (StringUtil::startsWith(caption, COMMAND_SETHOME))
	{
		type = CHAT_COMMAND;
		param = COMMAND_SETHOME;
		return;
	}
	//Go home point
	if (StringUtil::startsWith(caption, COMMAND_GOHOME))
	{
		type = CHAT_COMMAND;
		param = COMMAND_GOHOME;
		return;
	}
	//Reset home point
	if (StringUtil::startsWith(caption, COMMAND_RESETHOME))
	{
		type = CHAT_COMMAND;
		param = COMMAND_RESETHOME;
		return;
	}
	//get position
	if (StringUtil::startsWith(caption, COMMAND_POSITION))
	{
		type = CHAT_COMMAND;
		param = COMMAND_POSITION;
		return;
	}
	//friend
	if (StringUtil::startsWith(caption, COMMAND_FRIEND))
	{
		caption.erase(0, 8);
		type = CHAT_COMMAND;
		param = COMMAND_FRIEND;
		return;
	}
	//unfriend
	if (StringUtil::startsWith(caption, COMMAND_UNFRIEND))
	{
		caption.erase(0, 10);
		type = CHAT_COMMAND;
		param = COMMAND_UNFRIEND;
		return;
	}
	//block
	if (StringUtil::startsWith(caption, COMMAND_BLOCK))
	{
		caption.erase(0, 7);
		type = CHAT_COMMAND;
		param = COMMAND_BLOCK;
		return;
	}
	//unblock
	if (StringUtil::startsWith(caption, COMMAND_UNBLOCK))
	{
		caption.erase(0, 9);
		type = CHAT_COMMAND;
		param = COMMAND_UNBLOCK;
		return;
	}
	//kick
	if (StringUtil::startsWith(caption, COMMAND_KICK))
	{
		caption.erase(0, 6);
		type = CHAT_COMMAND;
		param = COMMAND_KICK;
		return;
	}
	//roll
	if (StringUtil::startsWith(caption, COMMAND_ROLL))
	{
		caption.erase(0, 6);
		type = CHAT_COMMAND;
		param = COMMAND_ROLL;
		return;
	}
	//createitem
	if (StringUtil::startsWith(caption, COMMAND_CREATEITEM))
	{
		caption.erase(0, 12);
		type = CHAT_COMMAND;
		param = COMMAND_CREATEITEM;
		return;
	}
	//godspeak
	if (StringUtil::startsWith(caption, COMMAND_GODSPEAK))
	{
		caption.erase(0, 10);
		type = CHAT_COMMAND;
		param = COMMAND_GODSPEAK;
		return;
	}
	//modon/off
	if (StringUtil::startsWith(caption, COMMAND_MODON))
	{
		type = CHAT_COMMAND;
		param = COMMAND_MODON;
		return;
	}
	if (StringUtil::startsWith(caption, COMMAND_MODOFF))
	{
		type = CHAT_COMMAND;
		param = COMMAND_MODOFF;
		return;
	}
	//Earthquakeeeeeee
	if (StringUtil::startsWith(caption, COMMAND_EARTHQUAKE))
	{
		type = CHAT_COMMAND;
		param = COMMAND_EARTHQUAKE;
		return;
	}
	//Clear All history
	if (StringUtil::startsWith(caption, COMMAND_CLEARALL))
	{
		type = CHAT_COMMAND;
		param = COMMAND_CLEARALL;
		reset(true);
		return;
	}
	//Clear history
	if (StringUtil::startsWith(caption, COMMAND_CLEAR))
	{
		type = CHAT_COMMAND;
		param = COMMAND_CLEAR;
		chatString[channel].clear();
		chatSayer[channel].clear();
		chatType[channel].clear();
		hasNewLine[channel] = false;
		return;
	}
	//Lipsync :U
	if (StringUtil::startsWith(caption, COMMAND_LIPSYNC))
	{
		caption.erase(0, 9);
		type = CHAT_LIPSYNC;
		return;
	}
	//Goto
	if (StringUtil::startsWith(caption, COMMAND_GOTO))
	{
		caption.erase(0, 6);
		type = CHAT_COMMAND;
		param = COMMAND_GOTO;
		return;
	}
	//Spawn
	if (StringUtil::startsWith(caption, COMMAND_SPAWN))
	{
		caption.erase(0, 7);
		type = CHAT_COMMAND;
		param = COMMAND_SPAWN;
		return;
	}
	//Where
	if (StringUtil::startsWith(caption, COMMAND_WHERE))
	{
		caption.erase(0, 7);
		type = CHAT_COMMAND;
		param = COMMAND_WHERE;
		return;
	}
	//Whois
	if (StringUtil::startsWith(caption, COMMAND_WHOIS))
	{
		caption.erase(0, 7);
		type = CHAT_COMMAND;
		param = COMMAND_WHOIS;
		return;
	}
	//Release pet
	if (StringUtil::startsWith(caption, COMMAND_RELEASEPET))
	{
		type = CHAT_COMMAND;
		param = COMMAND_RELEASEPET;
		return;
	}
	//Pet follow
	if (StringUtil::startsWith(caption, COMMAND_PETFOLLOW))
	{
		type = CHAT_COMMAND;
		param = COMMAND_PETFOLLOW;
		return;
	}
	//Commands
	if (StringUtil::startsWith(caption, COMMAND_COMMANDS))
	{
		type = CHAT_COMMAND;
		param = COMMAND_COMMANDS;
		return;
	}
	//Massblock
	if (StringUtil::startsWith(caption, COMMAND_MASSBLOCK))
	{
		caption.erase(0, 11);
		type = CHAT_COMMAND;
		param = COMMAND_MASSBLOCK;
		return;
	}
	//Ban
	if (StringUtil::startsWith(caption, COMMAND_BAN))
	{
		caption.erase(0, 5);
		type = CHAT_COMMAND;
		param = COMMAND_BAN;
		return;
	}
	//Party
	if (StringUtil::startsWith(caption, COMMAND_PARTY))
	{
		caption.erase(0, 7);
		type = CHAT_COMMAND;
		param = COMMAND_PARTY;
		return;
	}
	//Savechat
	if (StringUtil::startsWith(caption, COMMAND_SAVECHAT))
	{
		if (caption.length()>10)caption.erase(0, 10);
		else caption = "";
		type = CHAT_COMMAND;
		param = COMMAND_SAVECHAT;
		return;
	}
	//Action
	if (StringUtil::startsWith(caption, "/me "))
	{
		caption.erase(0, 4);
		switch (type)
		{
		case CHAT_PARTY: type = CHAT_PARTYACTION; break;
		case CHAT_LOCAL: type = CHAT_LOCALACTION; break;
		default: type = CHAT_ACTION; break;
		}

		return;
	}
	//spawnitem
	if (StringUtil::startsWith(caption, COMMAND_SPAWNITEM))
	{
		caption.erase(0, 11);
		type = CHAT_COMMAND;
		param = COMMAND_SPAWNITEM;
		return;
	}
	//Private message
	if (StringUtil::startsWith(caption, "/"))
	{
		const vector<String>::type tSplit = StringUtil::split(caption, ":", 1);
		if (tSplit.size()>1 && tSplit[0].length()>1)
		{
			param = tSplit[0];
			param.erase(0, 1);

			caption = tSplit[1];

			type = CHAT_PRIVATE;
		}
		else type = CHAT_INVALID;
		return;
	}
}
void MagixChatManager::say(MagixUnitManager *unitMgr, MagixUnit *target, const UTFString &caption, const unsigned char &type)
{
	const UTFString tName = (((!doLocalUsername && (type == CHAT_LOCAL || type == CHAT_LOCALACTION || type == CHAT_LOCALEVENT || type == CHAT_PARTY || type == CHAT_PARTYACTION || type == CHAT_PARTYEVENT)) ||
		(doGeneralCharname && (type == CHAT_GENERAL || type == CHAT_ACTION || type == CHAT_EVENT)) ||
		target->getUser() == "") ? target->getName() : target->getUser());
	if ((type != CHAT_LOCAL && type != CHAT_LOCALACTION) || target->getBodyEnt()->isVisible())push(caption, tName, type);
	if ((type == CHAT_LOCAL || type == CHAT_PARTY) && target->getBodyEnt()->isVisible())unitMgr->createChatBubble(target, caption);
}
void MagixChatManager::message(const UTFString &caption, const unsigned char &type)
{
	if (type == 0)push(caption, "", (channel == 0 ? CHAT_LOCAL : (channel == 1 ? CHAT_GENERAL : CHAT_PARTY)));
	else push(caption, "", type);
}
bool MagixChatManager::popHasNewLine(const unsigned char &chan)
{
	const bool tNewLine = hasNewLine[chan];
	hasNewLine[chan] = false;
	return tNewLine;
}
bool MagixChatManager::getHasNewLine(const unsigned char &chan)
{
	return hasNewLine[chan];
}
const UTFString MagixChatManager::prefixChatLine(const UTFString &caption, const String &sayer, const unsigned char &type, bool isPostfixLength)
{
	//Prefix
	UTFString tLine = "";
	if (sayer != "" && !isPostfixLength)
	{
		if (type == CHAT_ADMIN)tLine = L"" + tLine + "(" USER_ADMIN_TEXT ")";
		else if (type == CHAT_MOD)tLine = L"" + tLine + "(" USER_MOD_TEXT ")";
		if (type == CHAT_ACTION || type == CHAT_PARTYACTION || type == CHAT_LOCALACTION)tLine = L"" + tLine + sayer + " ";
		else if (type == CHAT_PRIVATE)tLine = L"" + tLine + "((" + sayer + ")) ";
		else if (type == CHAT_EVENT || type == CHAT_LOCALEVENT || type == CHAT_PARTYEVENT)tLine = L"" + tLine + "[" + sayer + " ";
		else tLine = L"" + tLine + "<" + sayer + "> ";
	}
	//Caption
	tLine = L"" + tLine + caption;
	//Postfix
	if (sayer != "" && (caption != "" || isPostfixLength))
	{
		if (type == CHAT_EVENT || type == CHAT_LOCALEVENT || type == CHAT_PARTYEVENT)tLine = L"" + tLine + "]";
	}

	return tLine;
}
const unsigned short MagixChatManager::getPrefixLength(const String &sayer, const unsigned char &type)
{
	return (int)prefixChatLine("", sayer, type, false).length();
}
const unsigned short MagixChatManager::getPostfixLength(const String &sayer, const unsigned char &type)
{
	return (int)prefixChatLine("", sayer, type, true).length();
}
const UTFString MagixChatManager::processChatString(const UTFString &caption, const String &sayer, const unsigned char &type, const Real &boxWidth, const Real &charHeight)
{
	UTFString tCaption = prefixChatLine(caption, sayer, type);
	//String tPrefix = prefixChatLine("",sayer,type);
	const unsigned short tPrefix = getPrefixLength(sayer, type);
	const unsigned short tPostfix = getPostfixLength(sayer, type);
	if (tCaption.length()>0)
	{
		//size caption
		const Font *pFont = dynamic_cast<Ogre::Font*>(Ogre::FontManager::getSingleton().getByName("Tahoma").getPointer());
		const Real tHeight = charHeight;

		int tSpacePos = -1;
		Real tTextWidth = 0;
		Real tWidthFromSpace = 0;
		for (int i = 0; i<int(tCaption.length()); i++)
		{
			if (tCaption[i] == 0x0020)
			{
				tTextWidth += pFont->getGlyphAspectRatio(0x0030);
				tWidthFromSpace += pFont->getGlyphAspectRatio(0x0030);
			}
			else
			{
				tTextWidth += pFont->getGlyphAspectRatio(tCaption[i]);
				tWidthFromSpace += pFont->getGlyphAspectRatio(tCaption[i]);
			}

			if (tCaption[i] == ' ' && i>tPrefix)
			{
				tSpacePos = i;
				tWidthFromSpace = pFont->getGlyphAspectRatio(0x0030);
			}
			if (tTextWidth*tHeight >= boxWidth)
			{
				if (tSpacePos != -1)
				{
					tCaption[tSpacePos] = '\n';
					tTextWidth = tWidthFromSpace;
					tSpacePos = -1;
				}
				else
				{
					tCaption.insert(i + 1, String(1, '\n'));
					tTextWidth = 0;
				}
			}
		}
	}

	tCaption.erase(0, tPrefix);
	tCaption.erase(tCaption.length() - tPostfix, tPostfix);
	return tCaption;
}
void MagixChatManager::toggleChannel()
{
	channel++;
	if (channel >= MAX_CHANNELS)channel = 0;
}
void MagixChatManager::setChannel(const unsigned char &value)
{
	channel = value;
	if (channel >= MAX_CHANNELS)channel = MAX_CHANNELS - 1;
}
const unsigned char MagixChatManager::getChannel()
{
	return channel;
}
const vector<const unsigned char>::type MagixChatManager::getOtherChannels()
{
	vector<const unsigned char>::type tChannels;
	for (int i = 0; i<MAX_CHANNELS; i++)
		if (i != channel)tChannels.push_back(i);
	return tChannels;
}
void MagixChatManager::saveChatLog(const String &directory, const String &filename)
{
	std::ofstream outFile;
	const String tFilename = directory + filename;
	outFile.open(tFilename.c_str());
	if (outFile.good())
	{
		for (int i = 0; i<(int)chatString[channel].size(); i++)
		{
			const String tBuffer = prefixChatLine(chatString[channel][i], chatSayer[channel][i], chatType[channel][i]) + "\n";
			outFile.write(tBuffer.c_str(), (int)tBuffer.length());
		}
	}
	outFile.close();
	message("Saved chat log as " + filename + ".");
}