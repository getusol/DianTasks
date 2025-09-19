#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<sstream>
const int MAX_PUREFILE_LINE = 32;
int main()
{
	using namespace std;
	ifstream fin("data.txt");//打开文件以及未打开错误输出
	if (!fin.is_open()) {
		cout << "File opening error!" << endl;
		cout << "To exit,press enter!" << endl;
		cin.get();
		return 1;
	}
	cout << "Initializing...\n" << "Please Wait...\n";
	string pureFile[MAX_PUREFILE_LINE];
	string temp;
	int cOunt{};
	int pi{};
	while (getline(fin, temp)) {//将文件中所有只有一个":"的行写入pureFile中
		cOunt = count(temp.begin(), temp.end(), ':');
		if (cOunt == 1) {
			pureFile[pi] = temp;
			pi++;
		}
	}
	stringstream sspF;
	string finalFile[2*MAX_PUREFILE_LINE];
	int fi{};
	for (int j{}; j <= pi - 1; j++) {//将pureFile中的内容一一根据":"拆分成两部分
		sspF << pureFile[j];//流化
		while (getline(sspF, finalFile[fi], ':')) {
			fi++;
		}
		sspF.clear();	//原本问题的来源，没有清空sspF中的内容
		sspF.str("");	//洗涤
	}
	cout << "Done!\n";
	string enter;
	int suc{};
	int output_temp{};
	while (enter != "Quit") {
		cout << "Now you can enter: ";
		getline(cin, enter);
		if (enter != "Quit") {
			for (int j{}; j <= pi - 1; j++) {//遍历所有键
				if (enter == finalFile[2 * j]) {//匹配
					if (suc == 0) {
						output_temp = 2 * j + 1;
						suc = 1;//匹配，准备输出
					}
					else if (suc == 1) {
						cout << "It appears that there are duplicate keywords; please check the file." << endl;
						suc = 2;
						output_temp = 0;//重复键报错
					}
				}
			}
			if (output_temp != 0) {
				cout << finalFile[output_temp] << endl;
				output_temp = 0;//输出
			}
			if (suc == 0) {
				cout << "Error:keyword doesnt exist!" << endl;//未能匹配错误
			}
			else {
				suc = 0;//重置suc值，准备下一次输入
			}
		}
		else {
			return 0;//Quit结束
		}
	}
	return 0;
}