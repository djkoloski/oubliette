#ifndef oub_expression_h
#define oub_expression_h

namespace Oub
{
	class Expression
	{
		public:
			list<pair<unsigned char, void *> > stack;
			
			Expression();
			Expression(const string &);
			Expression(const Expression &);
			~Expression();
			
			void clear();
			
			const bool defined(State &) const;
			const bool numeric(State &) const;
			const string evaluate(State &) const;
			
			Expression &operator=(const Expression &);
	};
}

#endif /* oub_expression_h */
