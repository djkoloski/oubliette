#ifndef oub_helpers_c
#define oub_helpers_c

namespace Oub
{
	const string trimWS(const string &s)
	{
		if (!s.size())
			return "";
		
		unsigned int
			fw = 0,
			lw = s.size();
		
		while (s[fw] == ' ' || s[fw] == '\t' || s[fw] == '\n')
		{
			++fw;
			if (fw == s.size())
				break;
		}
		
		while (s[lw] == ' ' || s[lw] == '\t' || s[lw] == '\n' || s[lw] == 0)
		{
			if (lw == 0)
				break;
			--lw;
		}
		
		return s.substr(fw, lw - fw + 1);
	}
}

#endif /* oub_helpers_c */
