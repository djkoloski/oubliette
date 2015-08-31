#ifndef oub_routines_c
#define oub_routines_c

namespace Oub
{
	namespace Routines
	{
		const unsigned char
			If::defined = 1,
			If::undefined = 2,
			If::equal = 3,
			If::notequal = 4,
			If::greater = 5,
			If::less = 6,
			If::greaterequal = 7,
			If::lessequal = 8;
		
		If::Comparison::Comparison() :
			comparator(0),
			left(),
			right()
		{ }
		If::Comparison::Comparison(const string &str) :
			comparator(0),
			left(),
			right()
		{
			if (!str.size())
				return;
			
			size_t cmp = str.find("==");
			
			if (cmp != string::npos)
			{
				comparator = If::equal;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 2, str.size() - cmp - 2));
				return;
			}
			
			cmp = str.find("!=");
			
			if (cmp != string::npos)
			{
				comparator = If::notequal;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 2, str.size() - cmp - 2));
				return;
			}
			
			cmp = str.find(">=");
			
			if (cmp != string::npos)
			{
				comparator = If::greaterequal;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 2, str.size() - cmp - 2));
				return;
			}
			
			cmp = str.find("<=");
			
			if (cmp != string::npos)
			{
				comparator = If::lessequal;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 2, str.size() - cmp - 2));
				return;
			}
			
			cmp = str.find('>');
			
			if (cmp != string::npos)
			{
				comparator = If::greater;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 1, str.size() - cmp - 1));
				return;
			}
			
			cmp = str.find('<');
			
			if (cmp != string::npos)
			{
				comparator = If::less;
				left = trimWS(str.substr(0, cmp));
				right = trimWS(str.substr(cmp + 1, str.size() - cmp - 1));
				return;
			}
			
			cmp = str.find('!');
			
			if (cmp != string::npos)
			{
				comparator = If::undefined;
				string t = trimWS(str.substr(cmp + 1, str.size() - cmp - 1));
				left = t;
				right = t;
				return;
			}
			
			comparator = If::defined;
			string t = trimWS(str);
			left = t;
			right = t;
		}
		If::Comparison::Comparison(unsigned char c, const Expression &l, const Expression &r) :
			comparator(c),
			left(l),
			right(r)
		{ }
		If::Comparison::Comparison(const typename If::Comparison &c) :
			comparator(c.comparator),
			left(c.left),
			right(c.right)
		{ }
		const bool If::Comparison::evaluate(State &state)
		{
			switch (comparator)
			{
				case If::defined:
					return left.defined(state);
				case If::undefined:
					return !left.defined(state);
				case If::equal:
					return left.evaluate(state) == right.evaluate(state);
				case If::notequal:
					return left.evaluate(state) != right.evaluate(state);
				case If::greater:
				{
					if (!left.numeric(state) || !right.numeric(state))
						return false;
					
					double
						l,
						r;
					
					sscanf(left.evaluate(state).c_str(), "%lf", &l);
					sscanf(right.evaluate(state).c_str(), "%lf", &r);
					
					return l > r;
				}
				case If::less:
				{
					if (!left.numeric(state) || !right.numeric(state))
						return false;
					
					double
						l,
						r;
					
					sscanf(left.evaluate(state).c_str(), "%lf", &l);
					sscanf(right.evaluate(state).c_str(), "%lf", &r);
					
					return l < r;
				}
				case If::greaterequal:
				{
					if (!left.numeric(state) || !right.numeric(state))
						return false;
					
					double
						l,
						r;
					
					sscanf(left.evaluate(state).c_str(), "%lf", &l);
					sscanf(right.evaluate(state).c_str(), "%lf", &r);
					
					return l >= r;
				}
				case If::lessequal:
				{
					if (!left.numeric(state) || !right.numeric(state))
						return false;
					
					double
						l,
						r;
					
					sscanf(left.evaluate(state).c_str(), "%lf", &l);
					sscanf(right.evaluate(state).c_str(), "%lf", &r);
					
					return l <= r;
				}
				default:
					break;
			}
			
			return false;
		}
		
		If::If() :
			comparisons(),
			joinings(),
			ifact(),
			elseact()
		{ }
		If::If(const string &cond, const Action &ia) :
			comparisons(),
			joinings(),
			ifact(ia),
			elseact()
		{
			string c = cond;
			
			size_t
				andpos,
				orpos;
			
			while (true)
			{
				andpos = c.find("&&");
				orpos = c.find("||");
				
				if (andpos == string::npos && orpos == string::npos)
				{
					comparisons.push_back(c);
					break;
				}
				
				if (andpos < orpos || orpos == string::npos)
				{
					comparisons.push_back(c.substr(0, andpos));
					joinings.push_back(true);
					if (c.size() < andpos + 2)
						break;
					c = c.substr(andpos + 2, c.size() - andpos - 2);
				}
				else
				{
					comparisons.push_back(c.substr(0, orpos));
					joinings.push_back(false);
					if (c.size() < orpos + 2)
						break;
					c = c.substr(orpos + 2, c.size() - orpos - 2);
				}
			}
		}
		If::If(const string &cond, const Action &ia, const Action &ea) :
			comparisons(),
			joinings(),
			ifact(ia),
			elseact(ea)
		{
			string c = cond;
			
			size_t
				andpos,
				orpos;
			
			while (true)
			{
				andpos = c.find("&&");
				orpos = c.find("||");
				
				if (andpos == string::npos && orpos == string::npos)
				{
					comparisons.push_back(c);
					break;
				}
				
				if (andpos < orpos || orpos == string::npos)
				{
					comparisons.push_back(c.substr(0, andpos));
					joinings.push_back(true);
					if (c.size() < andpos + 2)
						break;
					c = c.substr(andpos + 2, c.size() - andpos - 2);
				}
				else
				{
					comparisons.push_back(c.substr(0, orpos));
					joinings.push_back(false);
					if (c.size() < orpos + 2)
						break;
					c = c.substr(orpos + 2, c.size() - orpos - 2);
				}
			}
		}
		If::If(const list<Comparison> &c, const list<bool> &j, const Action &ia, const Action &ea) :
			comparisons(c),
			joinings(j),
			ifact(ia),
			elseact(ea)
		{ }
		If::~If()
		{ }
		void If::execute(State &state)
		{
			if (comparisons.size() < 1)
				return;
			
			list<typename If::Comparison>::iterator i = comparisons.begin();
			
			bool result = i->evaluate(state);
			
			++i;
			
			for (list<bool>::iterator j = joinings.begin(); i != comparisons.end() && j != joinings.end(); ++i, ++j)
				if (*j)
					result = result && i->evaluate(state);
				else
					result = result || i->evaluate(state);
			
			if (result)
				ifact.execute(state);
			else
				elseact.execute(state);
		}
		Routine *If::copy() const
		{
			return new If(comparisons, joinings, ifact, elseact);
		}
		
		Print::Print() :
			exp()
		{ }
		Print::Print(const string &e) :
			exp(e)
		{ }
		Print::Print(const Expression &e) :
			exp(e)
		{ }
		Print::~Print()
		{ }
		void Print::execute(State &state)
		{
			cout << exp.evaluate(state) << endl;
		}
		Routine *Print::copy() const
		{
			return new Print(exp);
		}
		
		Quit::Quit()
		{ }
		Quit::~Quit()
		{ }
		void Quit::execute(State &state)
		{
			state.changeShell("");
		}
		Routine *Quit::copy() const
		{
			return new Quit();
		}
		
		Move::Move() :
			dest()
		{ }
		Move::Move(const string &d) :
			dest(d)
		{ }
		Move::Move(const Expression &d) :
			dest(d)
		{ }
		Move::~Move()
		{ }
		void Move::execute(State &state)
		{
			state.changeShell(dest.evaluate(state));
		}
		Routine *Move::copy() const
		{
			return new Move(dest);
		}
		
		Define::Define() :
			varname("")
		{ }
		Define::Define(const string &v) :
			varname(v)
		{ }
		Define::~Define()
		{ }
		void Define::execute(State &state)
		{
			state.define(varname);
		}
		Routine *Define::copy() const
		{
			return new Define(varname);
		}
		
		Undefine::Undefine() :
			varname("")
		{ }
		Undefine::Undefine(const string &v) :
			varname(v)
		{ }
		Undefine::~Undefine()
		{ }
		void Undefine::execute(State &state)
		{
			state.undefine(varname);
		}
		Routine *Undefine::copy() const
		{
			return new Undefine(varname);
		}
		
		Set::Set() :
			varname("")
		{ }
		Set::Set(const string &vn, const string &va) :
			varname(vn),
			value(va)
		{ }
		Set::Set(const string &vn, const Expression &va) :
			varname(vn),
			value(va)
		{ }
		Set::~Set()
		{ }
		void Set::execute(State &state)
		{
			state.set(varname, value.evaluate(state));
		}
		Routine *Set::copy() const
		{
			return new Set(varname, value);
		}
		
		Save::Save()
		{ }
		Save::~Save()
		{ }
		void Save::execute(State &state)
		{
			ofstream output;
			
			while (true)
			{
				cout << "Where would you like to save to? (q to quit)" << endl << "> ";
			
				string path;
				getline(cin, path);
				
				if (path == "q" || path == "Q")
					return;
				
				output.open(path.c_str(), ofstream::binary);
				
				if (output.good())
					break;
				
				output.clear();
				cout << "Couldn't open file '" << path << "'." << endl;
			}
			
			output << state.currentShell << endl << state.variables.size() << endl;
			
			for (map<string, string>::iterator i = state.variables.begin(); i != state.variables.end(); ++i)
			{
				output << i->first << endl << i->second << endl;
			}
			
			for (map<string, Shell>::iterator i = state.shells.begin(); i != state.shells.end(); ++i)
			{
				output << i->first << endl << i->second.variables.size() << endl;
				
				for (map<string, string>::iterator j = i->second.variables.begin(); j != i->second.variables.end(); ++j)
				{
					output << j->first << endl << j->second << endl;
				}
			}
			
			output.close();
			
			cout << "Success." << endl;
		}
		Routine *Save::copy() const
		{
			return new Save();
		}
		
		Load::Load()
		{ }
		Load::~Load()
		{ }
		void Load::execute(State &state)
		{
			ifstream input;
			
			while (true)
			{
				cout << "Where would you like to load from? (q to quit)" << endl << "> ";
				
				string path;
				getline(cin, path);
				
				if (path == "q" || path == "Q")
					return;
				
				input.open(path.c_str(), ifstream::binary);
				
				if (input.good())
					break;
				
				input.clear();
				cout << "Couldn't open file '" << path << "'." << endl;
			}
			
			unsigned int count = 0;
			string
				startShell,
				targetShell,
				temp;
			
			getline(input, startShell);
			input >> count;
			getline(input, temp);
			
			string
				name,
				val;
			
			state.clearVariables();
			
			for (unsigned int x = 0; x < count; ++x)
			{
				getline(input, name);
				getline(input, val);
				state.set(name, val);
			}
			
			while (input.good())
			{
				getline(input, targetShell);
				input >> count;
				getline(input, temp);
				
				if (!input.good())
					break;
				
				for (unsigned int x = 0; x < count; ++x)
				{
					getline(input, name);
					getline(input, val);
					state.shells[targetShell].set(name, val);
				}
			}
			
			input.close();
			
			cout << "Success." << endl;
			
			state.changeShell(startShell);
		}
		Routine *Load::copy() const
		{
			return new Load();
		}
		
		Event::Event() :
			eventname()
		{ }
		Event::Event(const string &en) :
			eventname(en)
		{ }
		Event::Event(const Expression &en) :
			eventname(en)
		{ }
		Event::~Event()
		{ }
		void Event::execute(State &state)
		{
			string r = eventname.evaluate(state);
			
			if (r.size())
				state.event(r);
		}
		Routine *Event::copy() const
		{
			return new Event(eventname);
		}
	}
}

#endif /* oub_routines_c */
