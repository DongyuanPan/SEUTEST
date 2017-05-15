#ifndef DFA_H
#define FDA_H
#include <set>
#include <map>
#include <stack>
#include <queue>
using namespace std;
class DFA
{
public:
	DFA();
	~DFA();

	set<int> epsilonClosure(int i);
	set<int> epsilonClosure(set<int>setTemp);
	set<int> Dmove(set<int> setTemp, int i);
	set<int> Nmove(int setTemp, int i);
	void constructDFA();
	void minDFA();
	int statePatten(int finstate);
	int needDivide(set<int> setTemp,int a);
private:
	map<set<int>, int> Dstates;

};


#endif
