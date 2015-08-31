#ifndef oub_command_h
#define oub_command_h

namespace Oub
{
	class Command
	{
		public:
			map<string, Action> actions;
			map<string, Command> subcommands;
			
			Command();
			Command(const Command &);
			~Command();
			
			void event(const string &, State &);
	};
}

#endif /* oub_command_h */
