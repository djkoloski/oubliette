#ifndef oub_state_h
#define oub_state_h

namespace Oub
{
	class State
	{
		public:
			string currentShell;
			map<string, Shell> shells;
			map<string, Action> actions;
			map<string, string> variables;
			
			State();
			State(const State &);
			~State();
			
			void clearVariables();
			
			void event(const string &);
			void changeShell(const string &);
			const bool defined(const string &);
			void define(const string &);
			void undefine(const string &);
			const string get(const string &);
			void set(const string &, const string &);
	};
}

#endif /* oub_state_h */
