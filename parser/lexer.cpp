#include "Lexer.h"

int lex(std::fstream& file, DELIM_MAP& delimiters, LEXED_LIST& lexListContainer)
{
	unsigned long lineNb = 1;
	bool inComment = false;

	if (!file.is_open())
		return LEX_FILE_NOT_OPENED;

	std::string buf;
	int iResult = file.get();

	while (iResult != EOF)
	{
		buf += iResult;

		if(iResult == '\n')
			lineNb++;

		DELIM_MAP::iterator delimSearch;
		for (delimSearch = delimiters.begin(); delimSearch != delimiters.end(); delimSearch++)
			if (delimSearch->first.size() <= buf.size())
				if (delimSearch->first == buf.substr(buf.size() - delimSearch->first.size()))
					break;

		if (delimSearch != delimiters.end())
		{
			if(delimSearch->second == START_COMMENT)
				inComment = true;
			
			if(delimSearch->second == END_COMMENT)
			{
				inComment = false;
				buf = "";
			}
			else if(!inComment)
			{
				if ((buf.size() - delimSearch->first.size()) > 0)
				lexListContainer.push_back({buf.substr(0, buf.size() - delimSearch->first.size()), lineNb});

				if (delimSearch->second == SAVE)
					lexListContainer.push_back({delimSearch->first, lineNb});

				buf = ""; //reset buffer

				if (delimSearch->second == LINE_COMMENT)
				{
					do
					{
						iResult = file.get();
					} while (iResult != EOF && iResult != '\n');
					
				}
				else if (delimSearch->second == TOGGLE_STRING)
				{
					while (buf.size() < delimSearch->first.size())
					{
						iResult = file.get();

						if (iResult == EOF)
							return MISSING_END_SEPARATOR;

						if(iResult == '\n')
							lineNb++;

						buf += iResult;
					}

					while (buf.substr(buf.size() - delimSearch->first.size()) != delimSearch->first)
					{
						iResult = file.get();

						if (iResult == EOF)
							return MISSING_END_SEPARATOR;

						if(iResult == '\n')
							lineNb++;

						buf += iResult;
					}
					buf = buf.substr(0, buf.size() - 1);
				}
			}
		}

		iResult = file.get();
	}

	if(buf.size() && !inComment)
		lexListContainer.push_back({buf, lineNb});

	return LEX_SUCCESS;
}