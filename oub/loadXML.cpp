#ifndef oub_loadxml_c
#define oub_loadxml_c

namespace Oub
{
	const bool interpretAction(const string &text, Action &action)
	{
		size_t
			begin = 0,
			end = 0;
		unsigned int linecount = 0;
		string line;
		
		while (end < text.size())
		{
			begin = end;
			
			++linecount;
			
			end = text.find('\n', begin + 1);
			
			if (end == string::npos)
				end = text.size();
			
			line = text.substr(begin, end - begin);
			
			unsigned int fnw = 0;
			
			while (line[fnw] == ' ' || line[fnw] == '\t' || line[fnw] == '\n')
			{
				++fnw;
				if (fnw == line.size())
					break;
			}
			
			if (fnw == line.size())
				continue;
			
			line = line.substr(fnw, line.size() - fnw);
			
			{
				unsigned int fw = 0;
				
				while (line[fw] != ' ' && line[fw] != '\t' && line[fw] != '\n')
				{
					++fw;
					if (fw == line.size())
						break;
				}
				
				string
					rname = line.substr(0, fw),
					rarg = "";
				
				if (line.size() - fw != 0)
					rarg = line.substr(fw + 1, line.size() - fw - 1);
				
				if (rname == "if")
				{
					unsigned int
						ifstart = begin + fnw + fw,
						ifend = 0,
						nest;
					
					while (ifstart < text.size())
					{
						++ifstart;
						if (text[ifstart] == '(')
							break;
						if (text[ifstart] != ' ' && text[ifstart] != '\t' && text[ifstart] != '\n')
						{
							cout << "Unknown symbol '" << string(text.c_str() + ifstart) << "' between if statement on line " << linecount << " and its conditional." << endl;
							return false;
						}
					}
					
					ifend = ifstart + 1;
					
					nest = 1;
					while (nest != 0)
					{
						++ifend;
						if (ifend >= text.size())
						{
							cout << "Missing end parenthesis in conditional for if statemend on line " << linecount << "." << endl;
							return false;
						}
						if (text[ifend] == ')')
							--nest;
						if (text[ifend] == '(')
							++nest;
					}
					
					string condition = text.substr(ifstart + 1, ifend - ifstart - 1);
					
					unsigned int
						actionstart = ifend + 1,
						actionend = 0;
					
					while (actionstart < text.size())
					{
						++actionstart;
						if (text[actionstart] == '{')
							break;
						if (text[actionstart] != ' ' && text[actionstart] != '\t' && text[actionstart] != '\n')
						{
							cout << "Unknown symbol between conditional for if statement on line " << linecount << " and its body." << endl;
							return false;
						}
					}
					
					actionend = actionstart + 1;
					
					nest = 1;
					while (nest != 0)
					{
						++actionend;
						if (actionend >= text.size())
						{
							cout << "Missing end bracket in body of if statement on line " << linecount << "." << endl;
							return false;
						}
						if (text[actionend] == '}')
							--nest;
						if (text[actionend] == '{')
							++nest;
					}
					
					string body = text.substr(actionstart + 1, actionend - actionstart - 1);
					
					Action
						ifact,
						elseact;
					
					if (!interpretAction(body, ifact))
						return false;
					
					unsigned int
						elsestart = actionend + 1,
						elseend = 0;
					
					while (elsestart < text.size())
					{
						++elsestart;
						if (text[elsestart] == ' ' || text[elsestart] == '\t' || text[elsestart] == '\n')
							continue;
						break;
					}
					
					if (elsestart < text.size())
					{
						if (text.substr(elsestart, 4) == "else")
						{
							elsestart += 3;
						
							while (elsestart < text.size())
							{
								++elsestart;
								if (text[elsestart] == '{')
									break;
								if (text[elsestart] != ' ' && text[elsestart] != '\t' && text[elsestart] != '\n')
								{
									cout << "Unknown symbol between conditional for else of if statement on line " << linecount << " and its body." << endl;
									return false;
								}
							}
							
							elseend = elsestart + 1;
						
							nest = 1;
							while (nest != 0)
							{
								++elseend;
								if (elseend >= text.size())
								{
									cout << "Missing end bracket in else of if statement on line " << linecount << "." << endl;
									return false;
								}
								if (text[elseend] == '}')
									--nest;
								if (text[elseend] == '{')
									++nest;
							}
						
							string elsebody = text.substr(elsestart + 1, elseend - elsestart - 1);
						
							if (!interpretAction(elsebody, elseact))
								return false;
							
							actionend = elseend;
						}
					}
					
					action.routines.push_back(new Routines::If(condition, ifact, elseact));
					
					for (unsigned int x = ifstart; x < actionend + 1; ++x)
					{
						if (x == '\n')
							++linecount;
					}
					
					end = actionend + 1;
					
					continue;
				}
				if (rname == "print")
				{
					action.routines.push_back(new Routines::Print(rarg));
					continue;
				}
				if (rname == "quit")
				{
					action.routines.push_back(new Routines::Quit());
					continue;
				}
				if (rname == "move")
				{
					action.routines.push_back(new Routines::Move(rarg));
					continue;
				}
				if (rname == "define")
				{
					action.routines.push_back(new Routines::Define(rarg));
					continue;
				}
				if (rname == "undefine")
				{
					action.routines.push_back(new Routines::Undefine(rarg));
					continue;
				}
				if (rname == "set")
				{
					unsigned int ws = 0;
					
					while (rarg[ws] != ' ' && rarg[ws] != '\t' && rarg[ws] != '\n')
					{
						++ws;
					}
					
					string varname = rarg.substr(0, ws);
					
					while (rarg[ws] == ' ' || rarg[ws] == '\t' || rarg[ws] == '\n')
					{
						++ws;
					}
					
					string varval = rarg.substr(ws, rarg.size() - ws);
					
					action.routines.push_back(new Routines::Set(varname, varval));
					continue;
				}
				if (rname == "save")
				{
					action.routines.push_back(new Routines::Save());
					continue;
				}
				if (rname == "load")
				{
					action.routines.push_back(new Routines::Load());
					continue;
				}
				if (rname == "event")
				{
					action.routines.push_back(new Routines::Event(rarg));
					continue;
				}
				cout << "Error: Encountered unknown symbol '" << rname << "' on script line " << linecount << "." << endl;
				return false;
			}
		}
		
		return true;
	}
	const bool loadXML(const string &path, State &state)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str(), pugi::parse_full);
		
		if (result.status != pugi::status_ok)
		{
			cout << "Incorrectly formatted or invalid XML on at byte " << result.offset << "." << endl;
			return false;
		}
		
		for (pugi::xml_node root = doc.first_child(), node = root.first_child(); node != 0; node = node.next_sibling())
		{
			if (string(node.name()) == "import")
			{
				if (!loadXML(node.attribute("path").value(), state))
				{
					cout << "Could not read from imported file '" << node.attribute("path").value() << "'." << endl;
					return false;
				}
				continue;
			}
			if (string(node.name()) == "shell")
			{
				Shell &shell = state.shells[node.attribute("id").value()];
				if (!loadShell(node, shell))
					return false;
				continue;
			}
			if (string(node.name()) == "state")
			{
				if (!loadState(node, state))
					return false;
				continue;
			}
			
			cout << "Unrecognized or misplaced node '" << node.name() << "' in oubliette context." << endl;
			return false;
		}
		
		state.changeShell(state.currentShell);
		return true;
	}
	const bool loadShell(pugi::xml_node root, Shell &shell)
	{
		unsigned int i = 0;
		
		for (pugi::xml_node node = root.first_child(); node != 0; node = node.next_sibling(), ++i)
		{
			if (string(node.name()) == "action")
			{
				Action &act = shell.root.actions[node.attribute("event").value()];
				if (!loadAction(node, act))
					return false;
				continue;
			}
			if (string(node.name()) == "command")
			{
				if (!loadCommand(node, shell.root.subcommands[node.attribute("phrase").value()]))
					return false;
				continue;
			}
			
			cout << "Unrecognized or misplaced node '" << node.name() << "' (" << i << " child) in shell context." << endl;
			return false;
		}
		
		return true;
	}
	const bool loadCommand(pugi::xml_node root, Command &cmd)
	{
		for (pugi::xml_node node = root.first_child(); node != 0; node = node.next_sibling())
		{
			if (string(node.name()) == "action")
			{
				Action &act = cmd.actions[node.attribute("event").value()];
				if (!loadAction(node, act))
					return false;
				continue;
			}
			if (string(node.name()) == "command")
			{
				if (!loadCommand(node, cmd.subcommands[node.attribute("phrase").value()]))
					return false;
				continue;
			}
			
			cout << "Unrecognized or misplaced node '" << node.name() << "' in command context." << endl;
			return false;
		}
		
		return true;
	}
	const bool loadAction(pugi::xml_node root, Action &act)
	{
		return interpretAction(root.text().get(), act);
	}
	const bool loadState(pugi::xml_node root, State &state)
	{
		state.currentShell = root.attribute("start").value();
		
		for (pugi::xml_node node = root.first_child(); node != 0; node = node.next_sibling())
		{
			if (string(node.name()) == "variable")
			{
				if (node.attribute("val"))
				{
					state.set(node.attribute("name").value(), node.attribute("val").value());
				}
				else
				{
					state.set(node.attribute("name").value(), "");
				}
				
				continue;
			}
			
			cout << "Unrecognized or misplaced node '" << node.name() << "' in state context." << endl;
			return false;
		}
		
		return true;
	}
}

#endif /* oub_loadxml_c */
