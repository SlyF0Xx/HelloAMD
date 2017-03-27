#include <fstream>

using namespace std;

int main()
{
	double mass(0);
	int k(0);
	int f(0);
	for (int i(0); i < 8192; i++)
	{
		mass += (double)f*k;
		f++;
		k += 8192;
	}
	ifstream fin("out-1.txt", ios::binary);
	if (!fin)
	{
		float answer;
		fin >> answer;

	}
	return 0;
}