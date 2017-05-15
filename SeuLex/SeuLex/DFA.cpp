#include <iostream>
#include "DFA.h"
#include <set>
#include <map>
#include <stack>
#include <queue>
using namespace std;


map<int, set<int>> espClsTbl; //epsilon 闭包  I0----In
map<int, set<int>> Dtran;
map<set<int>, int> Dstates;
queue<set<int>> unmarkepsCl;//存储尚未运行的epsilon闭包
int DFAtable[100][12];//DFA状态表
set<int> minDFAtable[100][12];
set<int> t[1000][128];
int finalStatePatten[1000];
int symbol = 3;
int statanum = 0;
int group[20];

DFA::DFA() {

}

DFA::~DFA() {

}
set<int> DFA::epsilonClosure(int i)
{
	if (espClsTbl.count(i) == 1) {//查找epsilon闭包表中是否包含Ii
		return espClsTbl[i];//包含则返回对应闭包
	}

	//不包含则生成新的闭包
	set<int> newepsCl;//新epsilon闭包
	newepsCl.insert(i);//首先将当前状态加入该 闭包，而后寻找epsilon边加入新节点，直至闭包不再扩大
	stack<int> nodes;//使用栈来依次将节点有序的寻找闭包，首先将第一个节点加入栈
	nodes.push(i);

	while (!nodes.empty()) {
		int temp = nodes.top();
		nodes.pop();

		set<int> epstemp = DFA::Nmove(temp,0);//测试   temp通过0（epsilon）到达的集合
		set<int>::iterator oneepstemp = epstemp.begin();
		int count = 0;
		while (count < epstemp.size()) {
			int u = *oneepstemp;
			if (newepsCl.count(u) == 0) {
				nodes.push(u);
			}
			newepsCl.insert(u);
			count++;
		}

	}
	espClsTbl[i] = newepsCl;
	return newepsCl;
}

set<int> DFA::epsilonClosure(set<int> Temp) {
	set<int> Result_temp = Temp;
	set<int>::iterator i = Temp.begin();
	int count = 0;
	while (count < Temp.size()) {
		//set_union(Result_temp.begin(), Result_temp.end(), epsilonClosure(*i).begin(), epsilonClosure(*i).end(), Result_temp.begin());
		set<int> T = epsilonClosure(*i);
		set<int>::iterator j =T .begin();
		for (int k = 0; k < T.size(); k++)
		{
			Result_temp.insert(*j);
			j++;
		}
		count++;
		i++;
	}

	return Result_temp;

}

set<int> DFA::Dmove(set<int> setTemp, int a) {

	set<int> Result_temp;
	set<int>::iterator i = setTemp.begin();
	int count = 0;
	while (count < setTemp.size()) {
		//set_union(Result_temp.begin(),Result_temp.end(),ConDFA.Nmove(*i,a).begin() , ConDFA.Nmove(*i, a).end(), Result_temp.begin());
		set<int> T = DFA::Nmove(*i, a);
		set<int>::iterator j = T.begin();
		for (int k = 0; k <T.size(); k++)
		{
			Result_temp.insert(*j);
			j++;
		}
		count++;
		i++;
	}

	return Result_temp;
}

set<int> DFA::Nmove (int state, int symble) {
	set<int> dess = t[state][symble];
	return dess;
}

int DFA::statePatten(int finstate) {
	if (finstate == 12)
		return 1;
	return 0;
}

void DFA::constructDFA() {

	set<int> eps0 = DFA::epsilonClosure(0);
	Dstates[eps0] = 0;
	Dtran[0] = eps0;
	unmarkepsCl.push(eps0);
	int k = 0;
	while (!unmarkepsCl.empty()) {
		set<int> setTemp = unmarkepsCl.front();
		unmarkepsCl.pop();

		for (int i = 1; i < 3; i++) {
			set<int> U = DFA::epsilonClosure(DFA::Dmove(setTemp, i));//setTemp 通过 i 能够到达的 点的集合
			if (U.empty()) {
				continue;
			}
			if (Dstates.count(U) == 0) {
				Dstates[U] = Dstates.size();
				Dtran[Dtran.size()] = U;
				unmarkepsCl.push(U);

			}
			DFAtable[Dstates[setTemp]][i] =Dstates[U];
		}
	}
	//确定终态
	for (int i = 0; i < Dtran.size(); i++) {
		set<int> setT = Dtran[i];
		set<int>::iterator j = setT.begin();
		int count = 0;
		while (count < setT.size()) {
			if (DFA::statePatten(*j)) {//是终态返回1.
				finalStatePatten[i] = 1;
				break;
			}
			count++;
			j++;
		}
	}
	
}

void DFA::minDFA() {
	map<set<int>, int> fenzu;
	map<int, set<int>> symfenzu;
	queue<set<int>> dengdaifenzu;
	map<set<int>, int> numofsymbol;
	set<int> unfin;
	set<int> fin;
	int extingishstate[100];
	memset(extingishstate,-1,sizeof(extingishstate));
	for (int i = 0; i < Dtran.size(); i++) {
		if (finalStatePatten[i] == 1)
		{
			fin.insert(i);
			group[i] = 1;
		}
		else
		{
			unfin.insert(i);
			group[i] = 0;
		}
	}

	dengdaifenzu.push(unfin);
	numofsymbol[unfin] = numofsymbol.size();
	dengdaifenzu.push(fin);
	numofsymbol[fin] = numofsymbol.size();

	while (!dengdaifenzu.empty()) {
		set<int> divided = dengdaifenzu.front();
		dengdaifenzu.pop();
		int i = needDivide(divided, 1);
		if (i < symbol) {//若符号序号 
			set<int> inthis;
			set<int> nointhis;
			set<int> zero;
			
			set<int>::iterator j = divided.begin();
			int g = group[DFAtable[*j][i]];
			int count = 0;
			while (count < divided.size()) {
				int num = DFAtable[*j][i];

				if (DFAtable[*j][i] == -1) {					
					zero.insert(*j);
				}
				else if (g != group[DFAtable[*j][i]]) {
					nointhis.insert(*j);
				}
				else {
					inthis.insert(*j);
				}
				count++;
				j++;
			}

			if (inthis.size() == 1) {
				fenzu[inthis] = fenzu.size();
				symfenzu[symfenzu.size()] = inthis;
			}
			if (nointhis.size() == 1) {
				fenzu[nointhis] = fenzu.size();
				symfenzu[symfenzu.size()] = nointhis;
			}
			if (zero.size() == 1) {
				fenzu[zero] = fenzu.size();
				symfenzu[symfenzu.size()] = zero;
			}

			if (inthis.size() > 1) {
				dengdaifenzu.push(inthis);
				numofsymbol[inthis] = numofsymbol.size();
				set<int>::iterator p = inthis.begin();
				for (; p != inthis.end(); p++) {
					group[*p] = numofsymbol.size();
				}
			}
			if (nointhis.size() > 1) {
				dengdaifenzu.push(nointhis);
				numofsymbol[nointhis] = numofsymbol.size();
				set<int>::iterator p = nointhis.begin();
				for (; p != nointhis.end(); p++) {
					group[*p] = numofsymbol.size();
				}
			}
			if (zero.size() > 1) {
				dengdaifenzu.push(zero);
				numofsymbol[zero] = numofsymbol.size();
				set<int>::iterator p = zero.begin();
				for (; p != zero.end(); p++) {
					group[*p] = numofsymbol.size();
				}
			}

 
		}
		else {
			fenzu[divided] = fenzu.size();
			symfenzu[symfenzu.size()] = divided;
		}

	}

	statanum = Dtran.size();
	//构造最小化DFA状态转换表
	for (int i = 0; i < fenzu.size(); i++) {
		set<int> T = symfenzu[i];
		if (T.size() > 1) {
			set<int>::iterator j = T.begin();
			
			int count = 0;
			while (count < T.size()-1) {
				j++;
				int b = *j;
				extingishstate[b] = *T.begin();
				for (int k = 1; k < symbol; k++) {
					minDFAtable[*T.begin()][k].insert(DFAtable[*T.begin()][k]);
					int r = DFAtable[b][k];
					minDFAtable[*T.begin()][k].insert(r);
					}
				count++;
			}
			}

			if (T.size() == 1){
				
				set<int>::iterator j = T.begin();
				//extingishstate[*j] = *j;
				for (int k = 1; k < symbol; k++) {
						minDFAtable[*T.begin()][k].insert(DFAtable[*j][k]);
					}
				
			}
		}

	for (int i = 0; i < statanum; i++) {
		for (int j = 1; j < symbol; j++) {
			
			set<int> T = minDFAtable[i][j];
			int count = 0;
			set<int>::iterator k = T.begin();
			while (count<T.size()) {
				if (extingishstate[*k] > -1) {
					minDFAtable[i][j].erase(*k);
					minDFAtable[i][j].insert(extingishstate[*k]);
				}
				count++;
				k++;
			}
		}
	}

}
//返回使得该集合分组的符号序号
int DFA::needDivide(set<int> setTemp,int a) {
	for (int i = a; i < symbol; i++) {
		set<int> temp;
		set<int>::iterator j = setTemp.begin();
		int g = group[DFAtable[*j][i]];
		int count = 0;
		while (count < setTemp.size()) {
			if (g != group[DFAtable[*j][i]]) {//找到了当前集合内元素通过某个符号能够到达不再当前集合内的元素，则应该拆分
				return i;
			}
			count++;
			j++;
		}

	}
	return symbol;
}

int main()
{
	t[0][1] = {1};
	t[1][0] = {2};
	t[2][0] = {3};
	t[2][1] = {2};
	t[3][0] = {4};
	t[4][0] = {9};
	t[4][2] = {5};
	t[5][1] = {6};
	t[6][0] = {7};
	t[7][0] = {8};
	t[7][2] = {7};
	t[8][1] = {4};
	t[9][1] = {10};
	t[9][2] = {10};
	t[10][0] = {11};
	t[11][0] = {12};
	t[11][2] = {11};
	/*t[0][0] = { 1 };
	t[1][0] = { 2 };
	t[1][1] = { 1 };
	t[1][2] = { 1 };
	t[2][1] = { 3 };
	t[3][1] = { 4 };
	t[3][2] = { 4 };
	t[4][1] = { 5 };
	t[4][2] = { 5 };*/

	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 12; j++) {
			DFAtable[i][j] = -1;//表示空集
		}
	}
	//
	DFA ConDFA;
	ConDFA.constructDFA();

	//最小化
	ConDFA.minDFA();
	for (int i = 0; i < statanum; i++) {
		for (int j = 1; j < symbol; j++) {
			cout << "[" << i << "," << j << "]  {";
			set<int> T = minDFAtable[i][j];
			int count = 0;
			set<int>::iterator k = T.begin();
			while (count<T.size()) {
				cout << *k << " ";
				count++;
				k++;
			}

			cout << " }" << endl;
		}
	}
	finalStatePatten[1000];
	for (int i = 0; i < statanum; i++) {
		cout << finalStatePatten[i] << endl;
	}
	system("pause");
	return 0;
	
}