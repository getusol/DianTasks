#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<cstdio>
const int LINE_MAX = 64;//test2所能读取的最大行数
const int ARGUMENTS_MAX = 8;//test3中所能接受的最大参数数量
void test1();
void test2();
void test3();
void com_input();
bool is_num(std::string);
int main(int arg,char *argv[])
{
    using namespace std;
    if  (arg == 1){
        cout << "Error,Please enter arguments!\n";
        return -1;
    }
    else{
        if (*argv[1] == '1'){
            test1();
        }
        else if (*argv[1] == '2'){
            test2();
        }
        else if (*argv[1] == '3'){
            test3();
        }
        else{
            cout << "Please enter valid arguments!\n";
            return -1;
        }
    }
    return 0;
}

void test1()
{
    using namespace std;
    cout << "Welcome to test1!\n";
    string input;
    while (1)
    {
        cout << "Please enter:";
        getline(cin,input);
        if (input == "Dian"){
            cout << "2002\n";
        }
        else if (input != "Quit"){
            cout << "Error!\n";
        }
        else{
            break;
        }
    }
}

void test2()
{
    using namespace std;
    cout << "Welcome to test2!\n";
    ifstream fin("data.txt");
    if (!fin.is_open()){
        cout << "Error:File opening falied!\n";
        string yesOrNo;
        cout << "Would u like to create it?\n" << "\"yes\" or \"no\":";
        while (1){
            getline(cin,yesOrNo);
            if (yesOrNo != "yes" && yesOrNo != "no"){
                cout << "Thats not valid.\n" << "Please enter \"yes\" or \"no\":";
                continue;
            }
            else if (yesOrNo == "no"){
                cout << "Failed opening file,please press \"enter\" to exit.\n";
                cin.get();
                break;
            }
            else if (yesOrNo == "yes"){
                ofstream fout("data.txt");
                cout << "File has been created.To write in,select mode 3" << endl;
                fout.close();
                break;
            }
        }
        return;
    }
    cout << "File was successfully opened.\n" << "Initializing...\n";
    string temp;//文件内容缓存
    string finalFile[LINE_MAX];
    int i{}; //计数器
    stringstream ssFF;//流化字符串，给下面的getline使用
    while(getline(fin,temp)){
        if (count(temp.begin(),temp.end(),':') == 1){
            ssFF << temp;
            while(getline(ssFF,finalFile[i],':')){
                ++i;
            }
            ssFF.clear(); 
            ssFF.str("");//清空，为下一次流化做准备
        }
        if (i>=LINE_MAX-1)
        {
            cout << "Warning:Failed containing all lines,the rest will be ignored.\n";
            break;
        }
    }
    if (i == 0)
    {
        cout << "Error!Falied finding valid line!Please select mode 3 to write in to fix that." << endl;
        fin.close();
        return;
    }
    cout << "Done!\n" << "Now you can enter:";
    while(1)
    {   
        int outMem{};//输出缓存
        string input;//记录输入
        int suc{}; //记录查询成功次数
        getline(cin,input);
        if (input == "Quit")
        {
            break;
        }
        for(int j{};j<(i+1)/2;++j){
            if (input == finalFile[2*j]){
                if (suc == 0)
                {
                    outMem = 2*j+1;
                    ++suc;
                }
                else if (suc == 1)
                {
                    cout << "Error,duplicate keywords detected!\n";
                    outMem = -1;
                    break;
                }
            }
        }
        if (outMem > 0)
        {
            cout << "Value:" << finalFile[outMem] << endl;
        }
        if (suc == 0)
        {
            cout << "Error:keyword doesnt exsit!\n";
        }
        cout << "Please enter keyword:";
    }
    fin.close();
    return;
}

void test3()
{
    using namespace std;
    cout << "Welcome to mode 3!\n";
    cout << "Enter \"help\" for help.\n";
    do
    {
        string com[ARGUMENTS_MAX];//指令
        string args[ARGUMENTS_MAX];
        string input;
        stringstream sinput;
        int i{};//计数
        getline(cin,input);
        if (input == "Quit")
        {
            break;
        }
        sinput << input;
        while (getline(sinput,args[i],' '))
        {
            ++i;
            if (i>=ARGUMENTS_MAX-1)
            {
                break;
            }
        }
        sinput.clear();
        sinput.str("");
        string temp;//缓冲区
        int nline{1}; //标记行数
        if (args[0] == "help")
        {
            cout << "help :to get help.\nchk :to show all contents in data.txt,also create it.\ndel :to delete a specefic line or entire file.\nadd :to replace a specefic line or add something to the end of the txt.\n"
                << "Quit :to exit.\ninsert :to create an empty line below a specefic line.\nINSERT :to create an empty line over a specefic line.\n"; 
            continue;
        }
        else if (args[0] == "chk")
        {
            ifstream fin("data.txt");
            if (!fin.is_open())
            {
                cout << "Error:Cant open file \"data.txt\".\n";
                fin.close();
                ofstream fout("data.txt");
                fout.close();
                continue;
            }
            while(getline(fin,temp))
            {
                cout << nline << ":\t" << temp << endl;
                ++nline;
            }
            fin.close();
            continue;
        }
        else if (args[0] == "del")
        {
            if (is_num(args[1]))
            {
                ofstream fout("temp.txt");
                ifstream fin("data.txt");
                if (!fin.is_open() || !fout.is_open())
                {
                    cout << "Error!Cant open files!\n";
                    fout.close();
                    fin.close();
                    continue;
                }
                int cur_line{1};
                while (getline(fin,temp))
                {
                    if (cur_line != stoi(args[1]))
                    {
                        fout << temp << endl;
                    }
                    ++cur_line;
                }
                fin.close();
                fout.close();
                remove("data.txt");
                rename("temp.txt","data.txt");
                continue;
            }
            else
            {
                ofstream fout("temp.txt");
                if (!fout.is_open())
                {
                    cout << "Error!Cant create files!\n";
                    fout.close();
                    continue;
                }
                fout.close();
                remove("data.txt");
                rename("temp.txt","data.txt");
            }
        }
        else if (args[0] == "add")
        {
            if (is_num(args[1]))
            {
                ofstream fout("temp.txt");
                ifstream fin("data.txt");
                if (!fin.is_open() || !fout.is_open())
                {
                    cout << "Error!Cant open files!\n";
                    fout.close();
                    fin.close();
                    remove("temp.txt");
                    continue;
                }
                int cur_line{1};
                while (getline(fin,temp))
                {
                    if (cur_line == stoi(args[1]))
                    {
                        cout << "Line " << cur_line << ":" << temp <<endl;
                        cout << "Please enter new content:";
                        getline(cin,temp);
                    }
                    fout << temp << endl;
                    ++cur_line;
                }
                fin.close();
                fout.close();
                remove("data.txt");
                rename("temp.txt","data.txt");
                continue;
            }
            else
            {
                ofstream fout("data.txt",ios::app);
                if (!fout.is_open())
                {
                    cout << "Failed opening file!\n";
                    fout.close();
                    continue;
                }
                cout << "data.txt | enter \"Quit\" to save and quit.\n";
                ifstream fin("data.txt");
                if (!fin.is_open())
                {
                    cout << "Error:Cant open file \"data.txt\".\n";
                    fin.close();
                    continue;
                }
                while(getline(fin,temp))
                {
                    cout << temp << endl;
                }
                cout << "\b";
                fin.close();
                temp = "";
                while (getline(cin,temp))
                {
                    if (temp == "Quit")
                    {
                        cout << endl;
                        break;
                    }
                    fout << temp << endl;
                }
                fout.close();
                continue;
            }
        }
        else if (args[0] == "insert")
        {
            if (is_num(args[1]))
            {
                ofstream fout("temp.txt");
                ifstream fin("data.txt");
                if (!fin.is_open() || !fout.is_open())
                {
                    cout << "Error!Cant open files!\n";
                    fout.close();
                    fin.close();
                    remove("temp.txt");
                    continue;
                }
                int cur_line{1};
                while (getline(fin,temp))
                {
                    fout << temp << endl;
                    if (cur_line == stoi(args[1]))
                    {
                        fout << endl;
                    }
                    ++cur_line;
                }
                fin.close();
                fout.close();
                remove("data.txt");
                rename("temp.txt","data.txt");
                continue;
            }
            else
            {
                cout << "Error.Invalid Arguments.\n";
                continue;
            }
        }
        else if (args[0] == "INSERT")
        {
            if (is_num(args[1]))
            {
                ofstream fout("temp.txt");
                ifstream fin("data.txt");
                if (!fin.is_open() || !fout.is_open())
                {
                    cout << "Error!Cant open files!\n";
                    fout.close();
                    fin.close();
                    remove("temp.txt");
                    continue;
                }
                int cur_line{1};
                while (getline(fin,temp))
                {
                    if (cur_line == stoi(args[1]))
                    {
                        fout << endl;
                    }
                    fout << temp << endl;
                    ++cur_line;
                }
                fin.close();
                fout.close();
                remove("data.txt");
                rename("temp.txt","data.txt");
                continue;
            }
            else
            {
                cout << "Error.Invalid Arguments.\n";
                continue;
            }
        }
        else
        {
            cout << "Unknown command,please enter \"help\" for help.\n";
        }
    } while (1);
}

bool is_num(std::string str)
{
    using namespace std;
    stringstream sin(str);
    int n{};
    if (!(sin >> n))
    {
        return false;
    }
    return true;
}