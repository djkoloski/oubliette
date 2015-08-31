#ifndef oub_routine_h
#define oub_routine_h

namespace Oub
{
	class Routine
	{
		public:
			virtual ~Routine();
			virtual void execute(State &) = 0;
			virtual Routine *copy() const = 0;
	};
}

#endif /* oub_routine_h */
