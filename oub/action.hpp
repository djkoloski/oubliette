#ifndef oub_action_h
#define oub_action_h

namespace Oub
{
	class Routine;
	class State;
	
	class Action
	{
		public:
			list<Routine *> routines;
			
			Action();
			Action(const Action &);
			~Action();
			
			void execute(State &);
	};
}

#endif /* oub_action_h */
