#ifndef oub_action_c
#define oub_action_c

namespace Oub
{
	Action::Action() :
		routines()
	{ }
	Action::Action(const Action &a) :
		routines()
	{
		for (list<Routine *>::const_iterator i = a.routines.begin(); i != a.routines.end(); ++i)
			routines.push_back((*i)->copy());
	}
	Action::~Action()
	{
		for (list<Routine *>::iterator i = routines.begin(); i != routines.end(); ++i)
			delete *i;
	}
	void Action::execute(State &state)
	{
		for (list<Routine *>::iterator i = routines.begin(); i != routines.end(); ++i)
			(*i)->execute(state);
	}
}

#endif /* oub_action_c */
