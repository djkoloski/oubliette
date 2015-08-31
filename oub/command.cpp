#ifndef oub_command_c
#define oub_command_c

namespace Oub
{
	Command::Command() :
		actions(),
		subcommands()
	{ }
	Command::Command(const Command &c) :
		actions(c.actions),
		subcommands(c.subcommands)
	{ }
	Command::~Command()
	{ }
	void Command::event(const string &ev, State &state)
	{
		size_t p = ev.find("::");
		
		if (p == string::npos)
		{
			for (map<string, Action>::iterator i = actions.begin(); i != actions.end(); ++i)
			{
				if (i->first == ev)
				{
					i->second.execute(state);
					return;
				}
			}
		}
		else
		{
			if (p == 0)
			{
				string e = ev.substr(2, ev.size() - 2);
				for (map<string, Action>::iterator i = actions.begin(); i != actions.end(); ++i)
				{
					if (i->first == e)
					{
						i->second.execute(state);
						return;
					}
				}
			}
			else
			{
				string cmd = ev.substr(0, p);
			
				for (map<string, Command>::iterator i = subcommands.begin(); i != subcommands.end(); ++i)
				{
					if (i->first.size() > cmd.size())
						continue;
				
					if (cmd.substr(0, i->first.size()) == i->first)
					{
						string nev = trimWS(ev.substr(i->first.size(), ev.size() - i->first.size()));
					
						if (nev[0] == ':' && nev[1] == ':')
							i->second.event(nev.substr(2, nev.size() - 2), state);
						else
							i->second.event(nev, state);
				
						return;
					}
				}
				
				for (map<string, Action>::iterator i = actions.begin(); i != actions.end(); ++i)
				{
					if (i->first == "default")
					{
						state.set("::_input", cmd);
						i->second.execute(state);
						state.undefine("::_input");
					}
				}
			}
		}
	}
}

#endif /* oub_command_c */
