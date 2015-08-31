#ifndef oub_shell_c
#define oub_shell_c

namespace Oub
{
	Shell::Shell() :
		root(),
		variables()
	{ }
	Shell::Shell(const Shell &s) :
		root(s.root),
		variables(s.variables)
	{ }
	Shell::~Shell()
	{ }
	void Shell::event(const string &ev, State &state)
	{
		root.event(ev, state);
	}
	const bool Shell::defined(const string &v)
	{
		if (variables.count(v))
			return true;
		return false;
	}
	void Shell::define(const string &v)
	{
		if (!variables.count(v))
			variables[v] = "";
	}
	void Shell::undefine(const string &v)
	{
		if (variables.count(v))
			variables.erase(v);
	}
	const string Shell::get(const string &v)
	{
		if (variables.count(v))
			return variables[v];
		return "";
	}
	void Shell::set(const string &vn, const string &va)
	{
		variables[vn] = va;
	}
}

#endif /* oub_shell_c */
