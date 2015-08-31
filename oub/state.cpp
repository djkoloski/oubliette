#ifndef oub_state_c
#define oub_state_c

namespace Oub
{
	State::State() :
		currentShell(),
		shells(),
		actions(),
		variables()
	{ }
	State::State(const State &s) :
		currentShell(s.currentShell),
		shells(s.shells),
		actions(s.actions),
		variables(s.variables)
	{ }
	State::~State()
	{ }
	void State::clearVariables()
	{
		variables.clear();
	}
	void State::event(const string &e)
	{
		size_t
			p1 = e.find("::"),
			p2 = e.find("::", p1 + 2);
		
		if (p2 == string::npos)
		{
			shells[currentShell].event(e, *this);
		}
		else
		{
			if (p1 != 0)
			{
				string t = e.substr(0, p1);
				if (shells.count(t))
					shells[t].event(e.substr(p1 + 2, e.size() - p1 - 2), *this);
			}
			else
			{
				string ne = e.substr(p2 + 2, e.size() - p2 - 2);
				if (actions.count(ne) != 0)
					actions[ne].execute(*this);
			}
		}
	}
	void State::changeShell(const string &sh)
	{
		if (sh == "")
			currentShell = "";
		
		if (!shells.count(sh))
			return;
		
		currentShell = sh;
		
		event("::enter");
	}
	const bool State::defined(const string &var)
	{
		size_t p = var.find("::");
		
		if (p == string::npos)
		{
			return shells[currentShell].defined(var);
		}
		else
		{
			if (p != 0)
			{
				string t = var.substr(0, p);
				if (shells.count(t))
					return shells[t].defined(var.substr(p + 2, var.size() - p - 2));
			}
			else
			{
				string vn = var.substr(2, var.size() - 2);
				if (variables.count(vn))
					return true;
			}
		}
		
		return false;
	}
	void State::define(const string &var)
	{
		size_t p = var.find("::");
		
		if (p == string::npos)
		{
			shells[currentShell].define(var);
		}
		else
		{
			if (p != 0)
			{
				string t = var.substr(0, p);
				if (shells.count(t))
					shells[t].define(var.substr(p + 2, var.size() - p - 2));
			}
			else
			{
				string vn = var.substr(2, var.size() - 2);
				if (!variables.count(vn))
					variables[vn] = "";
			}
		}
	}
	void State::undefine(const string &var)
	{
		size_t p = var.find("::");
		
		if (p == string::npos)
		{
			shells[currentShell].undefine(var);
		}
		else
		{
			if (p != 0)
			{
				string t = var.substr(0, p);
				if (shells.count(t))
					shells[t].undefine(var.substr(p + 2, var.size() - p - 2));
			}
			else
			{
				variables.erase(var.substr(2, var.size() - 2));
			}
		}
	}
	const string State::get(const string &var)
	{
		size_t p = var.find("::");
		
		if (p == string::npos)
		{
			return shells[currentShell].get(var);
		}
		else
		{
			if (p != 0)
			{
				string t = var.substr(0, p);
				if (shells.count(t))
					return shells[t].get(var.substr(p + 2, var.size() - p - 2));
			}
			else
			{
				string vn = var.substr(2, var.size() - 2);
				if (variables.count(vn))
					return variables[vn];
			}
		}
		
		return "";
	}
	void State::set(const string &var, const string &val)
	{
		size_t p = var.find("::");
		
		if (p == string::npos)
		{
			shells[currentShell].set(var, val);
		}
		else
		{
			if (p != 0)
			{
				string t = var.substr(0, p);
				if (shells.count(t))
					shells[t].set(var.substr(p + 2, var.size() - p - 2), val);
			}
			else
			{
				string vn = var.substr(2, var.size() - 2);
				variables[vn] = val;
			}
		}
	}
}

#endif /* oub_state_h */
