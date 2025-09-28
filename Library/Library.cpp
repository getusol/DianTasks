//图书馆座位预约系统
#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>

const int COMKW_MAX = 18;
const std::string USERS[27]={"Admin","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
const std::string comkw[COMKW_MAX] = {"Monday","Tuesday","Wednesday","Thursday","Firday","Saturday","Sunday","Reservation","Reserve","Cancel","Initialize","Clear","Add","Dis","Del","Rev","Chk","Search"};
const int LINE_MAX = 8;
const int COLUM_MAX = 5;
const int FLOOR_MAX = 5;
const int SEAT_MAX = FLOOR_MAX*COLUM_MAX*LINE_MAX;

std::string input;

int login(std::string);
void user(int,std::string);
void admin(std::string);
int commandhead(std::string);
bool is_num(std::string);
int seat_opration(int,int,int,int,int,std::string,int);

using std::cout;

int main()
{
    int log_status{};
    int usc{-1};//用户编号

    using namespace std;
    using std::cout;

    cout << "输入Login以登录，输入Help来获取帮助\n";
    do
    {
        getline(cin,input);
        if (input == "Quit")//退出逻辑
        {
            break;
        }
        if (input == "Help")
        {
            if (log_status == 0)
            {
                cout << "Quit :退出程序\nExit :退出登录\nLogin :登录\nHelp :帮助\n";
            }
            else
            {
                if (usc > 0)
                {
                    cout << "<Day:Monday to Sunday> Floor <Number between 1 to 5> :查询某一天某楼层的座位情况\n"
                        << "Reserve <Day:Monday to Sunday> Floor <Floor> Seat <Line> <Column> :预约某一天某一楼层x行y列的座位\n"
                        << "Reservation :查询自己的预约\n"
                        << "Cancel <Day> <Floor> <Line> <Column> :取消预约\n"
                        << "Cancel ALL :取消所有预约\n"
                        << "Search Seat <Floor> <Line> <Column> :查询座位状态\n"
                        << "Exit :退出登录\n"
                        << "Quit :退出程序\n";
                }
                else if (usc == 0)
                {
                    //管理员可以视为用户-2
                    //管理员的指令如下：
                    cout << "<-1|-2|A~Z>:<Command> :代替其他用户执行命令\n"
                        << "Initialize :重置数据库\n"
                        << "Clear ALL :清除所有座位状态\n"
                        << "Clear RESERVATION :清除所有预约状态\n"
                        << "Clear -1 :清除所有禁用\n"
                        << "Clear -2 :清除所有管理员占用状态\n"
                        << "Clear 1~26 :清除对应用户的预约\n"
                        << "Add <Floor> <Line> <Column> :新增一个座位\n"
                        << "Del <Floor> <Line> <Column> :删除一个座位\n"
                        << "Dis <Floor> <Line> <Column> [<Day>] :禁用一个座位，Day选填，不填即所有\n"
                        << "Rev  <Force|-2~26> <Who> <Day> <Floor> [<Line> <Column>] :预定座位，参数解释:\n"
                        << "Force模式：全覆盖，-1：只覆盖禁用座位，0：只覆盖空余座位，1~26：只覆盖对应用户的座位，[]中的内容不填即全部，<Who> 指定这个座位被覆盖为谁的，-2即Admin\n"
                        << "Chk <Day> Floor <Floor> :查看楼层的座位状况\n"
                        << "Search Seat <Floor> <Line> <Column> :查询座位状态\n"
                        << "::Help :显示普通用户的Help界面\n"
                        << "Help :帮助\n"
                        << "Exit :退出登录\n"
                        << "Quit :退出程序\n";
                }
            }
            input = "";
            continue;
        }
        if (log_status == 0)//未登录
        {
            if (input == "Exit")
            {
                cout << "用户未登录！\n";
                input = "";
                continue;
            }
            else if (input == "Login")
            {
                if (usc == -2)
                {
                    cout << "请输入有效的用户名:" << endl;
                    input = "";
                    continue;
                }
                else
                {
                    usc = -2;
                    cout << "请输入用户名:\n";
                }
                continue;
            }
            else
            {
                if (usc == -2)
                {
                    usc = login(input);
                    input = "";
                    if (usc >= 0)
                    {
                        log_status = 1;
                    }
                    continue;
                }
                else
                {
                    cout << "无法解析的命令，请重试:";
                    input = "";
                    continue;
                }   
        }
        }
        else if (log_status == 1)//已登录
        {
            if (input == "Exit")
            {
                log_status = 0;
                usc = -1;
                input = "";
                cout << "退出登录成功\n";
                continue;
            }
            else if (input == "Login")
            {
                cout << "你已经登陆过了！目前账号为: " << USERS[usc] << endl;
                input = "";
                continue; 
            }
            else
            {
                if (usc == 0)
                {
                    admin(input);
                    input = "";
                    continue;
                }
                else if (usc <= 26 && usc > 0)
                {
                    user(usc,input);
                    input = "";
                    continue;
                }
            }
        }
    } while (1);
    return 0;
}

int login(std::string str)
{
    using namespace std;
    using std::cout;

        for (int i{};i <= 26;++i)
        {
            if (str == USERS[i])
            {
                cout << "登录成功！\n";
                cout << "输入Help以查看帮助\n";
                return i;
            }
        }
        cout << "不存在的用户名\n";
        cout << "请输入用户名:\n";
        return -2;
}

void user(int n,std::string str)
{
    using namespace std;

    string command[8];
    stringstream sstr;
    string line;
    string seat[11];
    short seatId[LINE_MAX*COLUM_MAX][11];
    short n_b[COLUM_MAX];
    int j{}; //j是符合要求的座位数目
    int jnd{}; //计数器2号
    
    sstr << str;
    for (int i{};i<=7;++i)
    {
        getline(sstr,command[i],' ');
    }
    sstr.clear();
    sstr.str("");
    if (commandhead(command[0]) <= 7 && commandhead(command[0]) >= 1)//显示楼层
    {
        if (command[1] == "Floor" && is_num(command[2]) && stoi(command[2]) <= FLOOR_MAX && stoi(command[2]) > 0)
        {
            ifstream fin ("seats.txt");
            if (!fin.is_open())
            {
                std::cout << "读取数据库失败！请检查相关的文件是否缺失！\n";
                return;
            }
            while (getline(fin,line))
            {
                sstr << line;
                for (int i{};i<=10;++i)
                {
                    getline(sstr,seat[i],'|');
                }
                if (stoi(seat[1]) == stoi(command[2]))  //导入所有符合楼层的座位矢量
                {
                    for(int k{};k <= 10;++k)
                    {
                        seatId[j][k] = stoi(seat[k]);
                    }
                    ++j;
                }
                sstr.clear();
                sstr.str("");
            }
            fin.close();
            //显示
            std::cout << "Floor " << stoi(command[2]) << ":=========================================\n\n";
            int gl{1};
            for (int a{};a<LINE_MAX;++a)
            {
                jnd = 0;
                for (int b{};b<j;++b)
                {
                    if (seatId[b][2] == a+1)
                    {
                        n_b[jnd] = b;
                        ++jnd;
                    }
                }
                if (jnd == 0)
                {
                    continue;
                }
                else
                {
                    ++gl;
                    while (gl != a+2)
                    {
                        cout << endl;
                        ++gl;
                    }
                    cout << "                ";
                }
                for (int c{};c<COLUM_MAX;++c)
                {
                    int suc = 0;
                    for (int d{};d<jnd;++d)
                    {
                        if (seatId[n_b[d]][3] == c+1)
                        {
                            suc = 1;
                            int day_info = commandhead(command[0]) + 3;
                            if (seatId[n_b[d]][day_info] == 0)
                            {
                                std::cout << "0    ";
                            }
                            else if (seatId[n_b[d]][day_info] == n)
                            {
                                std::cout << "2    ";
                            }
                            else if (seatId[n_b[d]][day_info] == -1)
                            {
                                std::cout << "#    ";
                            }
                            else
                            {
                                std::cout << "1    ";
                            }
                            break;
                        }
                    }
                    if (suc == 0)
                    {
                        std::cout << "     ";
                    }
                }
                std::cout << "\n\n";
            }
            std::cout << command[0] << "==========================================\n";
        }
        else
        {
            std::cout << "请输入正确的参数！\n";
            return;
        }
    }
    else if (commandhead(command[0]) == 8)//预约查看
    {
        cout << "你当前的预约如下:\n";
        for (int z{};z < 7;++z)
        {
            ifstream fin("seats.txt");
            if (!fin.is_open())
            {
                std::cout << "读取数据库失败！请检查相关的文件是否缺失！\n";
                return;
            }
            int suc{};
            while (getline(fin,line))
            {
                sstr << line;
                for (int y{};y < 11;++y)
                {
                    getline(sstr,seat[y],'|');
                }
                sstr.clear();
                sstr.str("");
                if (stoi(seat[z+4]) == n)
                {
                    ++suc;
                    if (suc == 1)
                    {
                        cout << comkw[z] << ":\n";
                    }
                    cout << "SeatID:" << seat[0] << " Floor:" << seat[1] << " Line:" << seat[2] << " Column:" << seat[3] << endl;
                }
            }
            fin.close();
        }
        cout << "显示完毕！\n";
        return;
    }
    else if (commandhead(command[0]) == 9)//预约
    {
        if (commandhead(command[1]) > 0 && commandhead(command[1]) < 8 && command[2] == "Floor" && is_num(command[3]) && command[4] == "Seat" && is_num(command[5]) && is_num(command[6]))
        {
            int result = seat_opration(n,stoi(command[3]),stoi(command[5]),stoi(command[6]),commandhead(command[1]),"Replace",0);
            if (result == -1)
            {
                cout << "Error:执行失败！\n";
                return;
            }
            else if (result == 0)
            {
                cout << "预约失败，该座位不存在！\n";
                return;
            }
            else if (result == 1)
            {
                cout << "预约成功！\n"
                    << command[1] << endl
                    << "座位信息:\n"
                    << "Floor: " << command[3] << " Line: " << command[5] << " Column: " << command[6] << endl;
                return; 
            }
            else if (result == 2)
            {
                cout << "预约失败！座位被占用或被禁用！\n";
                return;
            }
            else if(result == 3)
            {
                cout << "预约失败！你已经预约过这个座位了！\n";
                return;
            }
            else
            {
                cout << "Error:函数seat_opration()返回了非法值，请联系管理员！\n";
                return;
            }
        }
        else
        {
            cout << "参数错误！\n";
            return;
        }
    }
    else if (commandhead(command[0]) == 10)//Cancel
    {
        if (command[1] == "ALL")
        {
            ifstream fin("seats.txt");
            ofstream fout("temp.txt");
            if (!fin.is_open())
            {
                cout << "数据库发生错误！请联系管理员！\n";
                return;
            }
            int suc{};
            while (getline(fin,line))
            {
                sstr << line;
                for (int z{};z < 11;++z)
                {
                    getline(sstr,seat[z],'|');
                }
                for (int y{};y < 7;++y)
                {
                    if (stoi(seat[y+4]) == n)
                    {
                        suc = 1;
                        seat[y+4] = "0";
                    } 
                }
                for (int x{};x < 10;++x)
                {
                    fout << seat[x] << '|';
                }
                fout << seat[10] << endl;
                sstr.clear();
                sstr.str("");
            }
            if (suc == 0)
            {
                cout << "取消预约失败！找不到这样的座位！\n";
                fin.close();
                fout.close();
                remove("temp.txt");
                return;
            }
            cout << "取消预约成功！\n";
            fin.close();
            fout.close();
            remove("seats.txt");
            rename("temp.txt","seats.txt");
        }
        else if (commandhead(command[1]) < 8 && commandhead(command[1]) > 0 && is_num(command[2]) && is_num(command[3]) && is_num(command[4]))
        {
            switch(seat_opration(0,stoi(command[2]),stoi(command[3]),stoi(command[4]),commandhead(command[1]),"Replace",n))
            {
                case -1 :
                    cout << "Error:执行失败\n";
                    break;
                case 0 :
                    cout << "座位不存在！\n";
                    break;
                case 1 :
                    cout << "取消预约成功！\n";
                    break;
                case 2 :
                    cout << "取消预约失败！你没有预约该座位！\n";
                    break;
                case 3 :
                    cout << "取消预约失败！你没有预约该座位！\n";
                    break;
                default :
                    cout << "Error:seat_opreation()函数返回了不应该的值，请联系管理员\n";
                    break;
            }
        }
        else
        {
            cout << "请输入正确的参数！\n";
            return;
        }
    }
    else if (commandhead(command[0]) == 18)//Search
    {
        if (command[1] == "Seat" && is_num(command[2]) && is_num(command[3]) && is_num(command[4]))
        {
            ifstream fin("seats.txt");
            if (!fin.is_open())
            {
                cout << "数据库损坏，请联系管理员！\n";
                return;
            }
            while (getline(fin,line))
            {
                sstr << line;
                for (int i{};i < 11;++i)
                {
                    getline(sstr,seat[i],'|');
                }
                sstr.clear();
                sstr.str("");
                if (stoi(seat[1]) == stoi(command[2]) && stoi(seat[2]) == stoi(command[3]) && stoi(seat[3]) == stoi(command[4]))
                {
                    cout << "座位 ID:" << seat[0] << " Floor:" << seat[1] << " Line:" << seat[2] << " Column:" << seat[3] << endl;
                    cout << "预约情况如下(按星期一到星期天顺序排序):\n";
                    for (int i{};i < 7;++i)
                    {
                        if (stoi(seat[i+4]) == n)
                        {
                            cout << "2\t";
                        }
                        else if (stoi(seat[i+4]) == 0)
                        {
                            cout << "0\t";
                        }
                        else if (stoi(seat[i+4]) == -1)
                        {
                            cout << "#\t";
                        }
                        else
                        {
                            cout << "1\t";
                        }
                    }
                    cout << endl;
                    fin.close();
                    return;
                }
            }
            cout << "未能找到该座位\n";
            fin.close();
            return;
        }
        else
        {
            cout << "参数错误！\n";
            return;
        }
    }
    else
    {
        std::cout << "未知的命令,请重试。\n";
        return;
    }
}

int commandhead(std::string str)
{
    using namespace std;

    for (int i{0};i < COMKW_MAX;++i)
    {
        if (str == comkw[i])
        {
            return i+1;
        }
    }
    return -1;
}

bool is_num(std::string str)
{
    using namespace std;

    stringstream sstr(str);
    int a{};
    return bool (sstr >> a);
}

//Admin指令处理区域
void admin(std::string str)
{
    using namespace std;

    stringstream sstr;
    string line;
    string command[8];
    
    sstr << str;
    for (int i{};i < 8;++i)
    {
        getline(sstr,command[i],' ');
    }
    sstr.clear();
    sstr.str("");
    if (count(str.begin(),str.end(),':') == 2 && str == "::Help")
    {
        cout << "<Day:Monday to Sunday> Floor <Number between 1 to 5> :查询某一天某楼层的座位情况\n"
                << "Reserve <Day:Monday to Sunday> Floor <Floor> Seat <Line> <Column> :预约某一天某一楼层x行y列的座位\n"
                << "Reservation :查询自己的预约\n"
                << "Cancel <Day> <Floor> <Line> <Column> :取消预约\n"
                << "Search Seat <Floor> <Line> <Column> :查询座位状态\n"
                << "Cancel ALL :取消所有预约\n"
                << "Exit :退出登录\n"
                << "Quit :退出程序\n";
            return;
    }
    else if (count(str.begin(),str.end(),':') == 1)
    {
        sstr << str;
        string userAB[2];
        for (int i{};i < 2;++i)
        {
            getline(sstr,userAB[i],':');
        }
        sstr.clear();
        sstr.str("");
        for (int i{1};i < 27;++i)
        {
            if (userAB[0] == USERS[i])
            {
                cout << "替代用户 " << USERS[i] << " 执行命令: " << userAB[1] << " 成功！\n";
                user(i,userAB[1]);
                return;
            }
        }
        if (userAB[0] == "-1")
        {
            cout << "警告：此模式很可能出现问题，如显示错误，但功能可以正常执行！\n";
            user(-1,userAB[1]);
            return;
        }
        else if (userAB[0] == "-2")
        {
            cout << "成功\n";
            user(-2,userAB[1]);
            return;
        }
        else
        {
            cout << "错误的参数！\n";
            return;
        }
    }
    else if(commandhead(command[0]) > 10 && commandhead(command[0]) < COMKW_MAX + 1)
    {
        if (commandhead(command[0]) == 11)//初始化
        {
            cout << "正在初始化中，请稍等...\n";
            ofstream fout("temp.txt");
            if (!fout.is_open())
            {
                cout << "初始化失败！\n";
                return;
            }
            for (int i{};i < 80;++i)
            {
                fout << i+1 << '|' << i / 16 + 1 << '|' << i % 4 + 1 << '|' << (i % 16) / 4 + 1;
                for (int j{};j < 7;++j)
                {
                    fout << '|' << 0;
                }
                fout << endl;
            }
            fout.close();
            remove("seats.txt");
            rename("temp.txt","seats.txt");
            cout << "初始化完毕！\n";
            return;
        }
        else if (commandhead(command[0]) == 12)//清除
        {
            if (command[1] == "ALL")
            {
                string seat[11]{};
                ifstream fin("seats.txt");
                ofstream fout("temp.txt");
                if (!fin.is_open())
                {
                    cout << "数据库发生错误！请联系管理员！\n";
                    return;
                }
                while (getline(fin,line))
                {
                    sstr << line;
                    for (int z{};z < 11;++z)
                    {
                        getline(sstr,seat[z],'|');
                    }
                    for (int y{};y < 7;++y)
                    {
                        seat[y+4] = "0"; 
                    }
                    for (int x{};x < 10;++x)
                    {
                        fout << seat[x] << '|';
                    }
                    fout << seat[10] << endl;
                    sstr.clear();
                    sstr.str("");
                }
                fout.close();
                fin.close();
                remove("seats.txt");
                rename("temp.txt","seats.txt");
                cout << "清除所有座位状态完成！\n";
                return;
            }
            else if (command[1] == "RESERVATION")
            {
                string seat[11]{};
                ifstream fin("seats.txt");
                ofstream fout("temp.txt");
                if (!fin.is_open())
                {
                    cout << "数据库发生错误！请联系管理员！\n";
                    return;
                }
                while (getline(fin,line))
                {
                    sstr << line;
                    for (int z{};z < 11;++z)
                    {
                        getline(sstr,seat[z],'|');
                    }
                    for (int y{};y < 7;++y)
                    {
                        if (stoi(seat[y+4]) > 0 && stoi(seat[y+4]) < 27)
                        {
                            seat[y+4] = "0"; 
                        }
                    }
                    for (int x{};x < 10;++x)
                    {
                        fout << seat[x] << '|';
                    }
                    fout << seat[10] << endl;
                    sstr.clear();
                    sstr.str("");
                }
                fout.close();
                fin.close();
                remove("seats.txt");
                rename("temp.txt","seats.txt");
                cout << "清除所有座位的用户预约状态完成！\n";
                return;
            }
            else if (command[1] == "-1" || command[1] == "-2")
            {
                user(stoi(command[1]),"Cancel ALL");
                cout << "已执行\n";
                return;
            }
            else if (is_num(command[1]) && stoi(command[1]) > 0 && stoi(command[1]) < 27)
            {
                user(stoi(command[1]),"Cancel ALL");
                cout << "已对用户 " <<  USERS[stoi(command[1])] << " 执行\n";
                return;
            }
            else
            {
                cout << "错误的参数！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 13)//Add
        {
            if (is_num(command[1]) && is_num(command[2]) && is_num(command[3]) && stoi(command[1]) <= FLOOR_MAX && stoi(command[1]) > 0 && stoi(command[2]) > 0 && stoi(command[2]) <= LINE_MAX && stoi(command[3]) > 0 && stoi(command[3]) <= COLUM_MAX)
            {
                string seat[11];
                ifstream fin("seats.txt");
                int cur_line{1};
                if (!fin.is_open())
                {
                    cout << "数据库发生损坏！请尝试初始化！\n";
                    return;
                }
                while(getline(fin,line))
                {
                    ++cur_line;
                    sstr << line;
                    for (int i{};i < 11;++i)
                    {
                        getline(sstr,seat[i],'|');
                    }
                    if (seat[1] == command[1] && seat[2] == command[2] && seat[3] == command[3])
                    {
                        cout << "错误，座位已存在！\n";
                        fin.close();
                        sstr.clear();
                        sstr.str("");
                        return;
                    }
                    sstr.clear();
                    sstr.str("");
                }
                fin.close();
                ofstream fout("seats.txt",ios::app);
                if (!fout.is_open())
                {
                    cout << "数据库损坏!\n";
                    return;
                }
                fout << cur_line << '|' << command[1] << '|' << command[2] << '|' << command[3];
                for(int i{};i < 7;++i)
                {
                    fout << '|' << 0;
                }
                fout << endl;
                fout.close();
                cout << "添加座位 ID: " << cur_line << " Floor: " << command[1] << " Line: " << command[2] << " Column: " << command[3] << " 成功！\n";
                return;
            }
            else
            {
                cout << "错误的参数！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 14)//Dis
        {
            if (is_num(command[1]) && is_num(command[2]) && is_num(command[3]) && stoi(command[1]) <= FLOOR_MAX && stoi(command[1]) > 0 && stoi(command[2]) > 0 && stoi(command[2]) <= LINE_MAX && stoi(command[3]) > 0 && stoi(command[3]) <= COLUM_MAX)
            {
                if (commandhead(command[4]) > 0 && commandhead(command[4]) < 8)
                {
                    switch (seat_opration(-1,stoi(command[1]),stoi(command[2]),stoi(command[3]),commandhead(command[4]),"Force",0))
                    {
                        case -1 :
                            cout << "Error:执行失败！\n";
                            break;
                        case 0 :
                            cout << "座位不存在！\n";
                            break;
                        default :
                            cout << "禁用座位:\n" << "Floor: " << command[1] << " Line: " << command[2] << " Column: " << command[3] << "\n在 " << command[4] << " 成功！\n";
                            break;
                    }
                    return;
                }
                else
                {
                    int suc{3};
                    for(int i{};i < 7;++i)
                    {
                        switch (seat_opration(-1,stoi(command[1]),stoi(command[2]),stoi(command[3]),i+1,"Force",0))
                        {
                            case -1 :
                                suc = -1;//cout << "取消座位失败，请检查数据库！\n";
                                break;
                            case 0 :
                                suc = 0;//cout << "座位不存在！\n";
                                break;
                            default :
                                suc = 1;
                                break;
                        }
                        if (suc == -1)
                        {
                            cout << "禁用座位失败，请检查数据库！\n";
                            break;
                        }
                        else if (suc == 0)
                        {
                            cout << "座位不存在！\n";
                            break;
                        }
                    }
                    if (suc == 1)
                    {
                        cout << "禁用座位成功！座位信息:\n" << "Floor: " << command[1] << " Line: " << command[2] << " Column: " << command[3] << endl;
                    }
                    return;
                }
            }
            else
            {
                cout << "参数错误！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 15)//Del
        {
            if (is_num(command[1]) && is_num(command[2]) && is_num(command[3]))
            {

                string seat[11];
                int cur_line{};
                int suc{};

                ifstream fin("seats.txt");
                ofstream fout("temp.txt");
                if (!fin.is_open() || !fout.is_open())
                {
                    fin.close();
                    fout.close();
                    cout << "数据库出现错误，请检查\n";
                    return;
                }
                while(getline(fin,line))
                {
                    ++cur_line;
                    sstr << line;
                    for (int i{};i < 11;++i)
                    {
                        getline(sstr,seat[i],'|');
                    }
                    sstr.clear();
                    sstr.str("");
                    seat[0] = to_string(cur_line);
                    if (stoi(command[1]) == stoi(seat[1]) && stoi(command[2]) == stoi(seat[2]) && stoi(command[3]) == stoi(seat[3]))
                    {
                        suc = 1;
                        cout << "座位 ID:" << seat[0] << " Floor:" << seat[1] << " Line:" << seat[2] << " Column:" << seat[3] << " 已被成功删除！\n";
                        cur_line = cur_line - 1;
                    }
                    else
                    {
                        for(int i{};i < 10;++i)
                        {
                            fout << seat[i] << '|';
                        }
                        fout << seat[10] << endl;
                    }
                }
                if (suc == 0)
                {
                    cout << "座位不存在！\n";
                }
                fin.close();
                fout.close();
                remove("seats.txt");
                rename("temp.txt","seats.txt");
            }
            else
            {
                cout << "参数错误！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 16)//Rev
        {
            if (commandhead(command[3]) > 0 && commandhead(command[3]) < 8 && is_num(command[4]) && is_num(command[2]) && stoi(command[2]) >= -2 && stoi(command[2]) < 27)
            {
                if (is_num(command[1]) && stoi(command[1]) >= -2 && stoi(command[1]) < 27)
                {
                    if (is_num(command[5]) && is_num(command[6]))
                    {
                        switch (seat_opration(stoi(command[2]),stoi(command[4]),stoi(command[5]),stoi(command[6]),commandhead(command[3]),"Replace",stoi(command[1])))
                        {
                            case -1 :
                                cout << "执行失败\n";
                                break;
                            case 0 :
                                cout << "座位不存在！\n";
                                break;
                            default:
                                cout << "执行完毕\n";
                                break;
                        }
                        return;
                    }
                    else
                    {
                        string seat[11];
                        ifstream fin("seats.txt");
                        ofstream fout("temp.txt");
                        if (!fin.is_open() || !fout.is_open())
                        {
                            cout << "数据库发生错误！\n";
                            fin.close();
                            fout.close();
                            return;
                        }
                        while (getline(fin,line))
                        {
                            sstr << line;
                            for (int i{};i < 11;++i)
                            {
                                getline(sstr,seat[i],'|');
                            }
                            sstr.clear();
                            sstr.str("");
                            if (stoi(seat[1]) == stoi(command[4]) && stoi(seat[commandhead(command[3]) + 3]) == stoi(command[1]))
                            {
                                seat[commandhead(command[3]) + 3] = to_string(stoi(command[2]));
                            }
                            for(int i{};i < 10;++i)
                            {
                                fout << seat[i] << '|';
                            }
                            fout << seat[10] << endl;
                        }
                        fin.close();
                        fout.close();
                        remove("seats.txt");
                        rename("temp.txt","seats.txt");
                        cout << "操作执行完毕！\n";
                        return;
                    }
                }
                else if (command[1] == "Force")
                {
                    if (is_num(command[5]) && is_num(command[6]))
                    {
                        switch (seat_opration(stoi(command[2]),stoi(command[4]),stoi(command[5]),stoi(command[6]),commandhead(command[3]),"Force",0))
                        {
                            case -1 :
                                cout << "执行失败\n";
                                break;
                            case 0 :
                                cout << "座位不存在！\n";
                                break;
                            default:
                                cout << "执行完毕\n";
                                break;
                        }
                        return;
                    }
                    else
                    {
                        string seat[11];
                        ifstream fin("seats.txt");
                        ofstream fout("temp.txt");
                        if (!fin.is_open() || !fout.is_open())
                        {
                            cout << "数据库发生错误！\n";
                            fin.close();
                            fout.close();
                            return;
                        }
                        while (getline(fin,line))
                        {
                            sstr << line;
                            for (int i{};i < 11;++i)
                            {
                                getline(sstr,seat[i],'|');
                            }
                            sstr.clear();
                            sstr.str("");
                            if (stoi(seat[1]) == stoi(command[4]))
                            {
                                seat[commandhead(command[3]) + 3] = to_string(stoi(command[2]));
                            }
                            for(int i{};i < 10;++i)
                            {
                                fout << seat[i] << '|';
                            }
                            fout << seat[10] << endl;
                        }
                        fin.close();
                        fout.close();
                        remove("seats.txt");
                        rename("temp.txt","seats.txt");
                        cout << "操作执行完毕！\n";
                        return;
                    }
                }
                else
                {
                    cout << "参数错误！\n";
                    return;
                }
            }
            else
            {
                cout << "参数错误！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 17)//Chk
        {
            if (commandhead(command[1]) > 0 && commandhead(command[1]) < 8 && command[2] == "Floor" && is_num(command[3]) && stoi(command[3]) <= FLOOR_MAX && stoi(command[3]) > 0)
            {

                string seat[11];
                short seatId[LINE_MAX*COLUM_MAX][11];
                int j{};

                ifstream fin("seats.txt");
                if (!fin.is_open())
                {
                    std::cout << "读取数据库失败！请检查相关的文件是否缺失！\n";
                    return;
                }
                while (getline(fin,line))
                {
                    sstr << line;
                    for (int i{};i<=10;++i)
                    {
                        getline(sstr,seat[i],'|');
                    }
                    sstr.clear();
                    sstr.str("");
                    if (stoi(seat[1]) == stoi(command[3]))  //导入所有符合楼层的座位矢量
                    {
                        for(int i{};i < 11;++i)
                        {
                            seatId[j][i] = stoi(seat[i]);
                        }
                        ++j;
                    }
                }
                fin.close();
                //显示
                int jnd{};
                short n_b[COLUM_MAX];
                std::cout << "Floor " << stoi(command[3]) << ":=========================================\n\n";
                int gl{1};
                for (int a{};a<LINE_MAX;++a)
                {
                    jnd = 0;
                    for (int b{};b<j;++b)   //一个个查找对应a+1行的座位
                    {
                        if (seatId[b][2] == a+1)
                        {
                            n_b[jnd] = b;   //存起来 jnd即为col
                            ++jnd;
                        }
                    }
                    if (jnd == 0)
                    {
                        continue;
                    }//未找到就下一行
                    else//找到了就补充空行（gl 一定 <= a+2)
                    {
                        ++gl;
                        while (gl != a+2)
                        {
                            cout << endl;//用空行来替代原本的输出
                            ++gl;
                        }
                        cout << "                ";//格式对齐，准备输出
                    }
                    for (int c{};c<COLUM_MAX;++c) //一列列输出
                    {
                        int suc = 0;
                        for (int d{};d<jnd;++d) //jnd列数有多少
                        {
                            if (seatId[n_b[d]][3] == c+1)
                            {
                                suc = 1;
                                int day_info = commandhead(command[1]) + 3;
                                if (seatId[n_b[d]][day_info] == 0)
                                {
                                    std::cout << "0    ";
                                }
                                else if (seatId[n_b[d]][day_info] > 0 && seatId[n_b[d]][day_info] < 27)
                                {
                                    std::cout << USERS[seatId[n_b[d]][day_info]] << "    ";
                                }
                                else if (seatId[n_b[d]][day_info] == -1)
                                {
                                    std::cout << "#    ";
                                }
                                else if (seatId[n_b[d]][day_info] == -2)
                                {
                                    std::cout << "@    ";
                                }
                                else
                                {
                                    cout << "?    ";
                                }
                                break;
                            }
                        }
                        if (suc == 0)//没找到
                        {
                            std::cout << "     ";
                        }
                    }
                    std::cout << "\n\n";
                }
                std::cout << command[1] << "==========================================\n";
                cout << "如若出现\"?\",请查看数据库\n";
            }
            else
            {
                std::cout << "请输入正确的参数！\n";
                return;
            }
        }
        else if (commandhead(command[0]) == 18)//Search
        {

            string seat[11];

            if (command[1] == "Seat" && is_num(command[2]) && is_num(command[3]) && is_num(command[4]))
            {
                ifstream fin("seats.txt");
                if (!fin.is_open())
                {   
                    cout << "数据库损坏！\n";
                    return;
                }
                while (getline(fin,line))
                {
                    sstr << line;
                    for (int i{};i < 11;++i)
                    {
                        getline(sstr,seat[i],'|');
                    }
                    sstr.clear();
                    sstr.str("");
                    if (stoi(seat[1]) == stoi(command[2]) && stoi(seat[2]) == stoi(command[3]) && stoi(seat[3]) == stoi(command[4]))
                    {
                        cout << "座位 ID:" << seat[0] << " Floor:" << seat[1] << " Line:" << seat[2] << " Column:" << seat[3] << endl;
                        cout << "预约情况如下(按星期一到星期天顺序排序):\n";
                        for (int i{};i < 7;++i)
                        {
                            if (stoi(seat[i+4]) == -2)
                            {
                                cout << "@\t";
                            }
                            else if (stoi(seat[i+4]) == 0)
                            {
                                cout << "0\t";
                            }
                            else if (stoi(seat[i+4]) == -1)
                            {
                                cout << "#\t";
                            }
                            else if (stoi(seat[i+4]) > 0 && stoi(seat[i+4]) < 27)
                            {
                                cout <<  USERS[stoi(seat[i+4])] << "\t";
                            }
                            else
                            {
                                cout << "?\t";
                            }
                        }
                        cout << endl;
                        cout << "如若出现状态为\"?\"的，请及时检查数据库\n";
                        fin.close();
                        return;
                    }
                }
                cout << "未能找到该座位\n";
                fin.close();
                return;
            }
            else
            {
                cout << "参数错误！\n";
                return;
            }
        }
    }
    else
    {
        cout << "错误的命令！请查看Help\n";
        return;
    }
}

int seat_opration(int who,int flor,int lin,int col,int dy,std::string mode,int target)
{
    using namespace std;

    stringstream sstr;
    string line;
    string seat[11];
    int suc{};
    int cur_line{};

    ifstream fin("seats.txt");
    if (!fin.is_open())
    {
        cout << "数据库损坏！请联系管理员！";
        return -1;
    }
    while(getline(fin,line))
    {
        sstr << line;
        for(int i{};i < 11;++i)
        {    
            getline(sstr,seat[i],'|');
        }
        sstr.clear();
        sstr.str("");
        if (stoi(seat[1]) == flor && stoi(seat[2]) == lin && stoi(seat[3]) == col)
        {
            suc = 1;//座位已找到
            if (mode == "Force")
            {
                seat[dy+3] = to_string(who);
                break;
            }
            else if (mode == "Replace")
            {
                if (stoi(seat[dy+3]) == target)
                {
                    seat[dy+3] = to_string(who);
                    break;
                }
                else if (stoi(seat[dy+3]) == who)
                {
                    fin.close();
                    return 3;//这个座位已经是你的了
                }
                else
                {
                    fin.close();
                    return 2;//被占用或禁用
                }
            }
            else
            {
                fin.close();
                cout << "参数错误!\n";
                return -1;//执行失败
            }
        }
    }
    fin.close();
    if (suc == 0)
    {
        return suc;//未找到座位，座位不存在
    }
    //存入
    ifstream fins("seats.txt");
    ofstream fout("temp.txt");
    if (!fins.is_open() || !fout.is_open())
    {
        fout.close();
        fins.close();
        cout << "文件操作失败\n";
        return -1;
    }
    while(getline(fins,line))
    {
        ++cur_line;
        if (cur_line == stoi(seat[0]))
        {
            for (int i{};i < 10;++i)
            {
                fout << seat[i] << '|';
            }
            fout << seat[10] << endl;
        }
        else
        {
            fout << line << endl;
        }
    }
    fins.close();
    fout.close();
    remove("seats.txt");
    rename("temp.txt","seats.txt");
    return suc;//1,代表成功.
}