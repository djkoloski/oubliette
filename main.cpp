#include "oub/oub.h"
using namespace Oub;

int main()
{
	while (true)
	{
		State state;
	
		string path;
	
		while (true)
		{
			cout << "Enter path to file to load. (q to quit)" << endl << "> ";
			getline(cin, path);
			
			if (path == "q" || path == "Q")
				return 0;
		
			if (loadXML(path, state))
				break;
		}
	
		while (state.currentShell != "")
		{
			cout << ">> ";
			string command;
			getline(cin, command);
			state.event(command + "::call");
		}
		
		cout << endl << endl;
	}
	
	return 0;
}
