#ifndef oub_routines_h
#define oub_routines_h

namespace Oub
{
	namespace Routines
	{
		class If : public Routine
		{
			public:
				const static unsigned char
					defined,
					undefined,
					equal,
					notequal,
					greater,
					less,
					greaterequal,
					lessequal;
				
				class Comparison
				{
					public:
						unsigned char comparator;
						Expression
							left,
							right;
						
						Comparison();
						Comparison(const string &);
						Comparison(unsigned char, const Expression &, const Expression &);
						Comparison(const Comparison &);
						
						const bool evaluate(State &);
				};
				
				list<Comparison> comparisons;
				list<bool> joinings;
				Action
					ifact,
					elseact;
				
				If();
				If(const string &, const Action &);
				If(const string &, const Action &, const Action &);
				If(const list<Comparison> &, const list<bool> &, const Action &, const Action &);
				virtual ~If();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Print : public Routine
		{
			public:
				Expression exp;
				
				Print();
				Print(const string &);
				Print(const Expression &);
				virtual ~Print();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Quit : public Routine
		{
			public:
				Quit();
				virtual ~Quit();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Move : public Routine
		{
			public:
				Expression dest;
				
				Move();
				Move(const string &);
				Move(const Expression &);
				virtual ~Move();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Define : public Routine
		{
			public:
				string varname;
				
				Define();
				Define(const string &);
				virtual ~Define();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Undefine : public Routine
		{
			public:
				string varname;
				
				Undefine();
				Undefine(const string &);
				virtual ~Undefine();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Set : public Routine
		{
			public:
				string varname;
				Expression value;
				
				Set();
				Set(const string &, const string &);
				Set(const string &, const Expression &);
				virtual ~Set();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Save : public Routine
		{
			public:
				Save();
				virtual ~Save();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Load : public Routine
		{
			public:
				Load();
				virtual ~Load();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
		
		class Event : public Routine
		{
			public:
				Expression eventname;
				Event();
				Event(const string &);
				Event(const Expression &);
				virtual ~Event();
				
				virtual void execute(State &);
				virtual Routine *copy() const;
		};
	}
}

#endif /* oub_routines_h */
