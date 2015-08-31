#ifndef oub_h
#define oub_h

#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

namespace Oub
{
	using namespace std;
}

#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "pugixml.cpp"

#include "action.hpp"
#include "command.hpp"
#include "expression.hpp"
#include "helpers.hpp"
#include "routine.hpp"
#include "shell.hpp"
#include "state.hpp"

#include "action.cpp"
#include "command.cpp"
#include "expression.cpp"
#include "helpers.cpp"
#include "routine.cpp"
#include "shell.cpp"
#include "state.cpp"

#include "routines.hpp"
#include "routines.cpp"

#include "loadXML.hpp"
#include "loadXML.cpp"

#endif /* oub_h */
