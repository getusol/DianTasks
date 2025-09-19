#include<iostream>
#include<string>
void circle(std::string);
void cIrcle(void);
std::string enter;
int main()
{
	cIrcle();
	return 0;
}
void circle(std::string enter)
{
	using namespace std;
	if (enter == "Dian") {
		cout << "2002\n";
		cIrcle();
	}
	else if (enter != "Quit") {
		cout << "Error!\n";
		cIrcle();
	}
}
void cIrcle()
{
	using namespace std;
	cout << "Please enter: ";
	getline(cin, enter);
	circle(enter);
}
