#ifndef oub_expression_c
#define oub_expression_c

namespace Oub
{
	Expression::Expression() :
		stack()
	{ }
	Expression::Expression(const string &e) :
		stack()
	{
		string exp = e;
		
		unsigned int
			pos = 0,
			first = 0;
	
		while (pos < exp.size())
		{
			while (pos < exp.size() && exp[pos] == ' ' || exp[pos] == '\t' || exp[pos] == '\r' || exp[pos] == '\n')
			{ ++pos; }
		
			if (pos >= exp.size())
				break;
		
			first = pos;
		
			switch (exp[first])
			{
				case '\"':
				{
					bool escape = false;
				
					while (pos < exp.size())
					{
						++pos;
						if (escape)
						{
							escape = false;
							continue;
						}
						if (exp[pos] == '\\')
						{
							escape = true;
							continue;
						}
						if (exp[pos] == '\"')
							break;
					}
				
					++pos;
				
					string
						s = exp.substr(first + 1, pos - first - 2),
						r;
				
					for (unsigned int x = 0; x < s.size(); ++x)
					{
						if (s[x] == '\\')
						{
							escape = true;
							continue;
						}
						if (escape)
						{
							switch (s[x])
							{
								case 'n':
									r += '\n';
									break;
								case 't':
									r += '\t';
									break;
								case 'r':
									r += '\r';
									break;
								default:
									r += s[x];
									break;
							}
							escape = false;
						}
						else
						{
							r += s[x];
						}
					}
				
					stack.push_back(pair<unsigned char, void *>(2, new string(r)));
				
					break;
				}
				case '+':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(1)));
				
					++pos;
				
					break;
				}
				case '-':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(2)));
				
					++pos;
				
					break;
				}
				case '*':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(3)));
				
					++pos;
				
					break;
				}
				case '/':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(4)));
				
					++pos;
				
					break;
				}
				case '%':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(5)));
				
					++pos;
				
					break;
				}
				case '(':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(6)));
				
					++pos;
				
					break;
				}
				case ')':
				{
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(7)));
				
					++pos;
				
					break;
				}
				default:
				{
					while (pos < exp.size())
					{
						if (
							exp[pos] == ' ' ||
							exp[pos] == '\t' ||
							exp[pos] == '\r' ||
							exp[pos] == '\n' ||
							exp[pos] == '+' ||
							exp[pos] == '-' ||
							exp[pos] == '*' ||
							exp[pos] == '/' ||
							exp[pos] == '%' ||
							exp[pos] == '(' ||
							exp[pos] == ')'
						)
						{
							break;
						}
						++pos;
					}
				
					string piece = exp.substr(first, pos - first);
					double d = 0.0;
			
					if (sscanf(piece.c_str(), "%lf", &d) == 1)
						stack.push_back(pair<unsigned char, void *>(1, new double(d)));
					else
						stack.push_back(pair<unsigned char, void *>(3, new string(piece)));
				
					break;
				}
			}
		}
	}
	Expression::Expression(const Expression &e) :
		stack()
	{
		for (list<pair<unsigned char, void *> >::const_iterator i = e.stack.begin(); i != e.stack.end(); ++i)
		{
			switch (i->first)
			{
				case 1:
					stack.push_back(pair<unsigned char, void *>(1, new double(*(double *)i->second)));
					break;
				case 2:
					stack.push_back(pair<unsigned char, void *>(2, new string(*(string *)i->second)));
					break;
				case 3:
					stack.push_back(pair<unsigned char, void *>(3, new string(*(string *)i->second)));
					break;
				case 4:
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(*(unsigned char *)i->second)));
					break;
				default:
					break;
			}
		}
	}
	Expression::~Expression()
	{
		clear();
	}
	void Expression::clear()
	{
		for (list<pair<unsigned char, void *> >::iterator i = stack.begin(); i != stack.end(); ++i)
		{
			switch (i->first)
			{
				case 1:
				{
					delete (double *)i->second;
					break;
				}
				case 2:
				case 3:
				{
					delete (string *)i->second;
					break;
				}
				case 4:
				{
					delete (unsigned char *)i->second;
					break;
				}
				i->second = 0;
			}
		}
		
		stack.clear();
	}
	const bool Expression::defined(State &state) const
	{
		for (list<pair<unsigned char, void *> >::const_iterator i = stack.begin(); i != stack.end(); ++i)
		{
			if (i->first == 3)
				if (!state.defined(*(string *)i->second))
					return false;
		}
		
		return true;
	}
	const bool Expression::numeric(State &state) const
	{
		for (list<pair<unsigned char, void *> >::const_iterator i = stack.begin(); i != stack.end(); ++i)
		{
			if (i->first == 2)
			{
				return false;
			}
			if (i->first == 3)
			{
				string name = *(string *)i->second;
				
				if (state.defined(name))
				{
					string val = state.get(name);
					double d;
					
					if (sscanf(val.c_str(), "%lf", &d) == 1)
						continue;
					else
						return false;
				}
				else
				{
					return false;
				}
			}
		}
		
		return true;
	}
	const string Expression::evaluate(State &state) const
	{
		list<pair<unsigned char, void *> >
			valstack;
		list<unsigned char>
			opstack;
	
		list<pair<unsigned char, void *> >::const_iterator i = stack.begin();
	
		for (unsigned int x = 0; x < stack.size() && i != stack.end(); ++x, ++i)
		{
			switch (i->first)
			{
				case 1:
				case 2:
				case 3:
					switch (i->first)
					{
						case 1:
							valstack.push_back(pair<unsigned char, void *>(1, new double(*(double *)i->second)));
							break;
						case 2:
						case 3:
							valstack.push_back(pair<unsigned char, void *>(i->first, new string(*(string *)i->second)));
							break;
					}
					
					break;
				case 4:
				{
					unsigned char op = *(unsigned char *)i->second;
				
					bool fold = false;
				
					switch (op)
					{
						case 1:
						case 2:
							if (opstack.back() != 6)
								fold = true;
							break;
						case 3:
						case 4:
						case 5:
							if (opstack.back() >= 3 && opstack.back() != 6)
								fold = true;
							break;
						case 6:
							break;
						case 7:
							fold = true;
							break;
						default:
							break;
					}
				
					if (opstack.size() != 0 && fold)
					{
						list<pair<unsigned char, void *> >::iterator
							j = valstack.end(),
							k = --j;
						--j;
						list<unsigned char>::iterator
							l = opstack.end();
						--l;
					
						bool done = false;
					
						while (opstack.size() > 0 && !done)
						{
							switch (op)
							{
								case 1:
								case 2:
									if (*l == 6)
									{
										done = true;
										continue;
									}
									break;
								case 3:
								case 4:
								case 5:
									if (*l == 6 || *l < 3)
									{
										done = true;
										continue;
									}
									break;
								case 6:
									break;
								case 7:
									if (*l == 6)
									{
										opstack.pop_back();
										done = true;
										continue;
									}
									break;
								default:
									break;
							}
		
							if (j->first == 3)
							{
								string name = *(string *)j->second;
							
								if (state.defined(name))
								{
									string v = state.get(name);
									double d;
				
									if (sscanf(v.c_str(), "%lf", &d) == 1)
									{
										j->first = 1;
										delete (string *)j->second;
										j->second = new double(d);
									}
									else
									{
										j->first = 2;
										*(string *)j->second = v;
									}
								}
								else
								{
									j->first = 2;
									*(string *)j->second = name;
								}
							}
						
							if (k->first == 3)
							{
								string name = *(string *)k->second;
							
								if (state.defined(name))
								{
									string v = state.get(name);
									double d;
				
									if (sscanf(v.c_str(), "%lf", &d) == 1)
									{
										k->first = 1;
										delete (string *)k->second;
										k->second = new double(d);
									}
									else
									{
										k->first = 2;
										*(string *)k->second = v;
									}
								}
								else
								{
									k->first = 2;
									*(string *)k->second = name;
								}
							}
		
							if (j->first == 2 && k->first == 1)
							{
								double
									d = *(double *)k->second,
									t = d;
								unsigned int precision = 0;
			
								while (fmod(t, 1.0) != 0.0)
								{
									t *= 10.0;
									++precision;
								}
			
								char buf[256];
								sprintf(buf, "%.*f", precision, d);
			
								k->first = 2;
								delete (double *)k->second;
								k->second = (void *)new string(buf);
							}
		
							if (k->first == 2 && j->first == 1)
							{
								double
									d = *(double *)j->second,
									t = d;
								unsigned int precision = 0;
			
								while (fmod(t, 1.0) != 0.0)
								{
									t *= 10.0;
									++precision;
								}
			
								char buf[256];
								sprintf(buf, "%.*f", precision, d);
			
								j->first = 2;
								delete (double *)j->second;
								j->second = (void *)new string(buf);
							}
		
							if (j->first == 1)
							{
								double
									v1 = *(double *)j->second,
									v2 = *(double *)k->second;
			
								switch (*l)
								{
									case 1:
										*(double *)j->second = v1 + v2;
										break;
									case 2:
										*(double *)j->second = v1 - v2;
										break;
									case 3:
										*(double *)j->second = v1 * v2;
										break;
									case 4:
										*(double *)j->second = v1 / v2;
										break;
									case 5:
										*(double *)j->second = fmod(v1, v2);
										break;
									default:
										*(double *)j->second = 0.0;
										break;
								}
							}
							else
							{
								string
									v1 = *(string *)j->second,
									v2 = *(string *)k->second;
			
								switch (*l)
								{
									case 1:
										*(string *)j->second = v1 + v2;
										break;
									default:
										*(string *)j->second = "";
										break;
								}
							}
							
							switch (k->first)
							{
								case 1:
									delete (double *)k->second;
									break;
								case 2:
								case 3:
									delete (string *)k->second;
									break;
								case 4:
									delete (unsigned char *)k->second;
									break;
								default:
									break;
							}
		
							--j;
							--k;
							--l;
		
							valstack.pop_back();
							opstack.pop_back();
						}
					}
				
					if (op != 7)
						opstack.push_back(op);
				
					break;
				}
				default:
					break;
			}
		}
	
		if (valstack.size() > 1 && opstack.size() > 0)
		{
			list<pair<unsigned char, void *> >::iterator
				j = valstack.end(),
				k = --j;
			--j;
			list<unsigned char>::iterator
				l = opstack.end();
			--l;
		
			while (valstack.size() > 1 && opstack.size() > 0)
			{
				if (j->first == 3)
				{
					string name = *(string *)j->second;
				
					if (state.defined(name))
					{
						string v = state.get(name);
						double d;
	
						if (sscanf(v.c_str(), "%lf", &d) == 1)
						{
							j->first = 1;
							delete (string *)j->second;
							j->second = new double(d);
						}
						else
						{
							j->first = 2;
							*(string *)j->second = v;
						}
					}
					else
					{
						j->first = 2;
						*(string *)j->second = name;
					}
				}
			
				if (k->first == 3)
				{
					string name = *(string *)k->second;
				
					if (state.defined(name))
					{
						string v = state.get(name);
						double d;
	
						if (sscanf(v.c_str(), "%lf", &d) == 1)
						{
							k->first = 1;
							delete (string *)k->second;
							k->second = new double(d);
						}
						else
						{
							k->first = 2;
							*(string *)k->second = v;
						}
					}
					else
					{
						k->first = 2;
						*(string *)k->second = name;
					}
				}

				if (j->first == 2 && k->first == 1)
				{
					double
						d = *(double *)k->second,
						t = d;
					unsigned int precision = 0;

					while (fmod(t, 1.0) != 0.0)
					{
						t *= 10.0;
						++precision;
					}

					char buf[256];
					sprintf(buf, "%.*f", precision, d);

					k->first = 2;
					delete (double *)k->second;
					k->second = (void *)new string(buf);
				}

				if (k->first == 2 && j->first == 1)
				{
					double
						d = *(double *)j->second,
						t = d;
					unsigned int precision = 0;

					while (fmod(t, 1.0) != 0.0)
					{
						t *= 10.0;
						++precision;
					}

					char buf[256];
					sprintf(buf, "%.*f", precision, d);

					j->first = 2;
					delete (double *)j->second;
					j->second = (void *)new string(buf);
				}

				if (j->first == 1)
				{
					double
						v1 = *(double *)j->second,
						v2 = *(double *)k->second;

					switch (*l)
					{
						case 1:
							*(double *)j->second = v1 + v2;
							break;
						case 2:
							*(double *)j->second = v1 - v2;
							break;
						case 3:
							*(double *)j->second = v1 * v2;
							break;
						case 4:
							*(double *)j->second = v1 / v2;
							break;
						case 5:
							*(double *)j->second = fmod(v1, v2);
							break;
						default:
							*(double *)j->second = 0.0;
							break;
					}
				}
				else
				{
					string
						v1 = *(string *)j->second,
						v2 = *(string *)k->second;

					switch (*l)
					{
						case 1:
							*(string *)j->second = v1 + v2;
							break;
						default:
							*(string *)j->second = "";
							break;
					}
				}
				
				switch (k->first)
				{
					case 1:
						delete (double *)k->second;
						break;
					case 2:
					case 3:
						delete (string *)k->second;
						break;
					case 4:
						delete (unsigned char *)k->second;
						break;
					default:
						break;
				}

				--j;
				--k;
				--l;

				valstack.pop_back();
				opstack.pop_back();
			}
		}
	
		string result;
	
		switch (valstack.back().first)
		{
			case 1:
			{
				double
					d = *(double *)valstack.back().second,
					t = d;
				unsigned int precision = 0;
			
				while (fmod(t, 1.0) != 0.0)
				{
					t *= 10.0;
					++precision;
				}
			
				char buf[256];
				sprintf(buf, "%.*f", precision, d);
			
				result = buf;
				break;
			}
			case 2:
			{
				result = *(string *)valstack.back().second;
				break;
			}
			case 3:
			{
				string name = *(string *)valstack.back().second;
				if (state.defined(name))
					result = state.get(name);
				else
					result = name;
				break;
			}
			default:
				break;
		}
	
		for (list<pair<unsigned char, void *> >::iterator i = valstack.begin(); i != valstack.end(); ++i)
		{
			switch (i->first)
			{
				case 1:
					delete (double *)i->second;
					break;
				case 2:
				case 3:
					delete (string *)i->second;
					break;
				default:
					break;
			}
		}
	
		return result;
	}
	Expression &Expression::operator=(const Expression &e)
	{
		clear();
		
		for (list<pair<unsigned char, void *> >::const_iterator i = e.stack.begin(); i != e.stack.end(); ++i)
		{
			switch (i->first)
			{
				case 1:
					stack.push_back(pair<unsigned char, void *>(1, new double(*(double *)i->second)));
					break;
				case 2:
					stack.push_back(pair<unsigned char, void *>(2, new string(*(string *)i->second)));
					break;
				case 3:
					stack.push_back(pair<unsigned char, void *>(3, new string(*(string *)i->second)));
					break;
				case 4:
					stack.push_back(pair<unsigned char, void *>(4, new unsigned char(*(unsigned char *)i->second)));
					break;
				default:
					break;
			}
		}
	}
}

#endif /* oub_expression_c */
