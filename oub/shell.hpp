#ifndef oub_shell_h
#define oub_shell_h

namespace Oub
{
	class Shell
	{
		public:
			Command root;
			map<string, string> variables;
			
			Shell();
			Shell(const Shell &);
			~Shell();
			
			void event(const string &, State &);
			const bool defined(const string &);
			void define(const string &);
			void undefine(const string &);
			const string get(const string &);
			void set(const string &, const string &);
	};
}

#endif /* oub_shell_h */
