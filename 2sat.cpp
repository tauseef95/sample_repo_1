#include<iostream>
#include<vector>
#include<set>
#include<stack>
#include<algorithm>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

typedef vector<long> vi;
typedef vector<vi> vvi;

#define sz(a) int((a).size())
#define all(a) (a).begin(), (a).end()
#define present(container,a) (container.find((a)) != container.end())
#define revtr(c,i) for(auto i = (c).rbegin(); i != (c).rend(); i++)
#define tr(c,i) for(auto i = (c).begin(); i != (c).end(); i++)

vi v, ft;
vi childsVisited;
set<long> elmnts;
int no_sol = 0;


void DFS(vvi &g, long s, int flag)
{
	stack<long> stk;
	stk.push(s);
	v[s] = 1;

	int jumpOut;

	set<long> scc;
	if(flag == 2)
		scc.insert(s);

	while(!stk.empty())
	{
		jumpOut = 0;
		int cur = stk.top();
		for(; childsVisited[cur] < sz(g[cur]); ++childsVisited[cur]) //Resume checking from the last child that you visited
		{
			int i = g[cur][childsVisited[cur]];

			if(v[i] != 1)
			{
				stk.push(i);
				v[i] = 1;

				if(flag == 2)
				{
					scc.insert(i); //Create the scc

					if(present(scc, i^1)) //Check if i & ~i coexist
					{
						no_sol = 1;
						break;
					}
				}

				jumpOut = 1; //Jump to the while loop's beginning
				break;
			}
		}

		if(no_sol == 1)
			break;

		if(flag == 1 && jumpOut == 0) //Record the finishing time order of vertices
			ft.push_back(stk.top());

		if(jumpOut == 0)
			stk.pop();
	}
}


void sat2(vvi &g, vvi &gr)
{
	cout<<"\nInside pass 1\n";

	revtr(elmnts, it)
		if(v[*it] != 1)
			DFS(gr, *it, 1);

	cout<<"\nPass 1 completed\n";

	fill(all(v), 0);
	fill(all(childsVisited), 0);

	cout<<"\nInside pass 2\n";

	revtr(ft, it)
		if(v[*it] != 1)
		{
			DFS(g, *it, 2);
			if(no_sol == 1)
				break;
		}

	cout<<"\nPass 2 completed\n";
}


int main()
{
	vvi g, gr;
	ifstream file("/home/tauseef/Desktop/DAA2/2sat1.txt");
	long first, second;
	string line;
	int n, N;

	getline(file,line);
	stringstream ss(line);
	ss >> n;
	N = 2*n + 2;

	g.resize(N);
	gr.resize(N);
	v.resize(N);
	childsVisited.resize(N);

	while(getline(file,line,'\n')) //Reading from file
	{
		stringstream ss(line);
		ss >> first;

		if(first < 0)
		{
			first = -first;
			first = 2*first + 1;
		}
		else
			first = 2*first;

		ss >> second;

		if(second < 0)
		{
			second = -second;
			second = 2*second + 1;
		}
		else
			second = 2*second;


		elmnts.insert(first);	//Set of 2-sat variables present
		elmnts.insert(second);

		g[first^1].push_back(second); //Creating inference graphs G & Grev
		g[second^1].push_back(first);
		gr[second].push_back(first^1);
		gr[first].push_back(second^1);
	}

	cout<<"\nfile read successfully\n";

	sat2(g, gr);

	if(no_sol == 0)
		cout<<"\nThere exists a 2-sat solution\n";
	else
		cout<<"\nSolution doesn't exist\n";
	
	file.close();
}
