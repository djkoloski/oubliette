#ifndef oub_loadxml_h
#define oub_loadxml_h

namespace Oub
{
	const bool interpretAction(const string &, Action &);
	const bool loadXML(const string &, State &);
	const bool loadShell(pugi::xml_node, Shell &);
	const bool loadCommand(pugi::xml_node, Command &);
	const bool loadAction(pugi::xml_node, Action &);
	const bool loadState(pugi::xml_node, State &);
}

#endif /* oub_loadxml_h */
