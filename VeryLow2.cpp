#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include "hashPD.h"
#define i_input _color(10);cout<<'>';_color(7);
#define ptf printf
#define ent ptf("\n");

using namespace std;

string Chara = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

struct CONFIG
{
	string username="用户";
	string rdme="Generated_by_VeryLow,_a_Key_Generator,_by_IQ_Online_Studio,_github.com/iqonli.";
	int rdmee=0;
	int formar=1;
};
struct STRnHASH
{
	string str;
	string hash;
};

CONFIG Config;

void _color(int __c=7)//着色
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),__c);
}

int rando(int a,int b)
{
	static random_device rd;
	static mt19937 gen(rd());
	uniform_int_distribution<> dis(a, b);
	return dis(gen);
}

bool isStoPressed()
{
	return (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
}

string getCurrentTimeString() 
{
	time_t now = time(nullptr);
	tm* localTime = localtime(&now);
	
	ostringstream oss;
	oss << put_time(localTime, "%Y_%m_%d_%H_%M_%S");
	return oss.str();
}

string getRdme(CONFIG a)
{
	if(a.rdmee==1)
	{
		return a.rdme;
	}
	else return "";
}

string generateFileName(const char* strx) // 修改参数类型为const char*，解决字符串常量警告
{
	if(Config.formar==0)return (string)strx;
	string timestamp = getCurrentTimeString();
	string fileName = "key_" + string(strx) + "_" + timestamp +".txt";
	return fileName;
}

std::string modify_string( string& strx, int rands) 
{
	string chara = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	string result = strx;
	
	for (char& c : result) 
	{
		size_t pos = chara.find(c);
		if (pos != string::npos) 
		{
			int random = rando(0,rands);
			int new_index = (static_cast<int>(pos) + random) % chara.length();
			c = chara[new_index];
		}
	}
	
	return result;
}

int main() 
{
	//读取配置
//	streambuf* originalCinBuffer=cin.rdbuf();
	ifstream abc("config.txt");
	if(abc.is_open())
	{
		abc>>Config.username>>Config.rdme>>Config.rdmee>>Config.formar;
		abc.close();
	}
	
	system("title VeryLow 密钥制作 v2.0");
	
//	string infoo = "";
	_color(12);
	ptf("出现\"请按任意键继续...\"时请勿按回车, ");ent system("pause");system("cls");
	while(1)
	{
		_color(14);
		cout<<Config.username;
		_color();
		ptf(",\n");
		_color(10);
		ptf("欢迎来到 VeryLow v2.0");ent
		ptf("by IQ Online Studio, github.com/iqonli;\nVeryLow means 这玩意不能跑GPU加速, 甚至跑不满CPU, 实在非常Low.");
		_color();
		ent
		
		i_input ptf("1=生成密钥;");ent
		i_input ptf("2=ASCII内凯撒;");ent
		i_input ptf("3=暴力枚举破解;");ent
		i_input ptf("4=快速字符串校验;");ent
		i_input ptf("5=快速文件校验;");ent
		i_input ptf("6=设置;");ent ent
		i_input ptf("");
		int x;
		cin>>x;
		if(x==1)
		{
			long long n,f;
			string chara,result,fn;
			
			system("pause");
			ent
			_color(10);
			ptf("指定字符集, 限一行ASCII");
			_color();
			ent
			ptf("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");ent
			do{i_input getline(cin,chara);}while(chara=="");
			
			ent
			_color(10);
			ptf("指定长度");
			_color();
			ent
			do{i_input cin>>n;}while(n<=0);
			
			for(f=0;f<n;f++)
			{
				result+=chara.at(rando(0,chara.size()-1));
			}
			result+=getRdme(Config);
			
			ent
			_color(10);
			ptf("密钥生成成功, 输入!显示密钥, 也可以输入文件名以保存");
			_color();
			ent
			system("pause");
			do{i_input cin>>fn;}while(regex_match(fn,regex("^\\s+$")));
			if(fn=="!")
			{
				ptf("\n================");ent
				cout<<"len="<<result.size()<<endl;
				_color(176);
				cout<<result;
				_color();
				ptf("\n================\n");ent
				system("pause");
				ent
				_color(10);
				ptf("输入文件名以保存");
				_color();
				ent
				do{i_input getline(cin,fn);}while(regex_match(fn,regex("^\\s+$")));
			}
			
			string fn1=generateFileName(fn.c_str());
			ofstream outFile1(fn1);
			
			if (!outFile1) 
			{
				cerr<<"无法保存文件"<<endl;
				return 1;
			}
			
			outFile1<<result;
			outFile1.close();
			
			_color(48);
			cout<<fn1;
			_color(176);
			ptf("已成功保存");
			_color();
			ent
		}
		if(x==2)
		{
			char fn[50];
			string fn1,strxx,result;
			ent
			_color(10);
			ptf("将要操作的文件放到程序同目录, 输入文件名");
			_color();
			ent
			i_input cin>>fn;
			if(fn!="")
			{
				ifstream inFile(fn);
				if(!inFile)
				{
					cerr<<"无法读取文件";
				}
				strxx=string(istreambuf_iterator<char>(inFile), istreambuf_iterator<char>());
				result=modify_string(strxx,128);
				
				ent
				_color(10);
				ptf("密钥生成成功, 输入!显示密钥, 也可以输入文件名以保存");
				_color();
				ent
				system("pause");
				do{i_input cin>>fn1;}while(regex_match(fn1,regex("^\\s+$")));
				if(fn1=="!")
				{
					ptf("\n================");ent
					cout<<"len="<<result.size()<<endl;
					_color(176);
					cout<<result;
					_color();
					ptf("\n================\n");ent
					system("pause");
					ent
					_color(10);
					ptf("输入文件名以保存");
					_color();
					ent
					do{i_input getline(cin,fn1);}while(regex_match(fn1,regex("^\\s+$")));
				}
				
				string fn2=generateFileName(fn1.c_str());
				ofstream outFile0(fn2);
				
				if(!outFile0) 
				{
					cerr<<"无法保存文件"<<endl;
					return 1;
				}
				
				outFile0<<result;
				outFile0.close();
				
				_color(48);
				cout<<fn2;
				_color(176);
				ptf("已成功保存");
				_color();
				ent
			}
		}
		if(x==3)
		{
			int n,m,by,noout;
			string chara,hashs,regi;
			DWORD alg;
			
			ent
			_color(10);
			ptf("选择算法(逗号前后内容都可输入)");
			_color();
			ent
			i_input ptf("1,sha1;");ent
			i_input ptf("2,sha256;");ent
			i_input ptf("5,sha512;");ent
			i_input ptf("m,md5;");ent
			do{i_input cin>>hashs;}while(hashs!="1" and hashs!="2" and hashs!="5" and hashs!="m" and hashs!="sha1" and hashs!="sha256" and hashs!="sha512" and hashs!="md5");
			if(hashs=="1" or hashs=="sha1")alg=CALG_SHA1;
			if(hashs=="2" or hashs=="sha256")alg=CALG_SHA_256;
			if(hashs=="5" or hashs=="sha512")alg=CALG_SHA_512;
			if(hashs=="m" or hashs=="md5")alg=CALG_MD5;
			
			ent
			_color(10);
			ptf("指定字符集, 限一行ASCII");
			_color();
			ent
			ptf("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~");ent
			do{i_input getline(cin,chara);}while(chara=="");
			
			ent
			_color(10);
			ptf("输入枚举字符串长度范围, 两个数回车隔开");
			_color();
			ent
			do{i_input cin>>n;}while(n<=0);
			do{i_input cin>>m;}while(m<=0);
			
//			ent
//			_color(10);
//			ptf("是否顺序枚举?");
//			_color();
//			ent
//			i_input ptf("0 no");ent
//			i_input ptf("1 yes");ent
//			i_input cin>>by;
			by=0;
			
			ent
			_color(10);
			ptf("是否沉默输出(约快5倍)?");
			_color();
			ent
			i_input ptf("0 no");ent
			i_input ptf("1 yes");ent
			i_input cin>>noout;
			
			system("pause");ptf("按回车继续");i_input getchar();getchar();
			ent
			_color(10);
			ptf("输入欲匹配的hash值的正则表达式, 以123开头为^123.*");
			_color();
			ent
			do{i_input getline(cin,regi);}while(regex_match(regi,regex("^\\s+$")));
			regex regii;
			bool regexValid = true;
			try
			{
				regii=regex(regi);
			}catch(const regex_error& e)
			{
				cerr<<"正则表达式语法错误 - "<<e.what()<<endl;
				regexValid=false;
			}
			if(!regexValid)
			{
				system("pause");
				continue;
			}
			
			ent
			_color(176);
			ptf("随时可按esc终止");
			_color();
			ent ent
			
			if(by==1)
			{
				//
			}
			else
			{
//				_color(176);
//				ptf("随时可按esc终止");
//				_color();
				cout<<"寻找中...";
				if(noout)ptf("\n我晒干了沉默\n");
				vector<STRnHASH> finded;
				int f,d=0;
				while(!isStoPressed())
				{
					int maxx=rando(n,m);
					string result="";
					for(f=0;f<maxx;f++)
					{
						result+=chara.at(rando(0,chara.size()-1));
					}
					string hashx=PD_hash(result, false, alg);
					d++;
					if(noout==0)cout<<"\rsum="<<d<<" regex="<<regi<<" "<<hashx;
					if(regex_match(hashx,regii))
					{
						STRnHASH strnhash;
						strnhash.str=result;
						strnhash.hash=hashx;
						finded.push_back(strnhash);
						cout<<"\r"<<d<<"	| 找到"<<finded.size()<<"个符合条件的密钥;\n";
					}
				}
				cout<<"\n"<<d<<"' 下面是所有已找到的密钥\n";
				for(const auto& itit : finded)
				{
					ptf("\n================");ent
					cout<<"len="<<itit.str.size()<<endl;
					cout<<"hash="<<itit.hash<<endl;
					_color(176);
					cout<<itit.str;
					_color();
					ptf("\n================\n");ent
				}
				
				string fn3;
				ent
				_color(10);
				ptf("输入文件夹名以保存");
				_color();
				ent
				do{i_input getline(cin,fn3);}while(regex_match(fn3,regex("^\\s+$")));
				
				system(("mkdir "+fn3).c_str());
				for(const auto& itit : finded)
				{
					string fn4="./"+fn3+"/"+to_string(itit.str.size())+"_"+to_string(rando(10000,99999))+".txt";
					ofstream outFile3(fn4);
					if(!outFile3) 
					{
						cerr<<"无法保存文件"<<endl;
						return 1;
					}
					
					outFile3<<itit.str;
					outFile3.close();
					
					_color(48);
					cout<<fn4;
					_color(176);
					ptf("已成功保存");
					_color();
					ent
				}
			}
		}
		if(x==4 or x==5)
		{
			string xx,hashs;
			DWORD alg;
			
			ent
			_color(10);
			ptf("选择算法(逗号前后内容都可输入)");
			_color();
			ent
			i_input ptf("1,sha1;");ent
			i_input ptf("2,sha256;");ent
			i_input ptf("5,sha512;");ent
			i_input ptf("m,md5;");ent
			do{i_input cin>>hashs;}while(hashs!="1" and hashs!="2" and hashs!="5" and hashs!="m" and hashs!="sha1" and hashs!="sha256" and hashs!="sha512" and hashs!="md5");
			if(hashs=="1" or hashs=="sha1")alg=CALG_SHA1;
			if(hashs=="2" or hashs=="sha256")alg=CALG_SHA_256;
			if(hashs=="5" or hashs=="sha512")alg=CALG_SHA_512;
			if(hashs=="m" or hashs=="md5")alg=CALG_MD5;
			
			if(x==4)
			{
				ptf("按回车继续");i_input getchar();getchar();
				_color(10);
				ptf("输入校验字符串, 仅一行");
				_color();
				ent
				do{i_input getline(cin,xx);}while(xx=="" or xx=="\n");
				string hashx=PD_hash(xx, false, alg);
				ptf("\n================");ent
				cout<<"len="<<hashx.size()<<endl;
				_color(176);
				cout<<hashx;
				_color();
				ptf("\n================\n");ent
			}
			if(x==5)
			{
				ptf("按回车继续");i_input getchar();getchar();
				_color(10);
				ptf("输入文件路径, 仅一行; 或把文件拖进控制台内");
				_color();
				ent
				do{i_input getline(cin,xx);}while(regex_match(xx,regex("^\\s+$")));
				string hashx=PD_hash(xx, true, alg);
				ptf("\n================");ent
				cout<<"len="<<hashx.size()<<endl;
				_color(176);
				cout<<hashx;
				_color();
				ptf("\n================\n");ent
			}
		}
		if(x==6)
		{
			int xx;
			do
			{
				ent
				_color(10);
				ptf("输入选项序号");
				_color();
				ent
				i_input ptf("0=保存&退出;");ent
				i_input cout<<"1=修改用户名 now=["<<Config.username<<"];\n";
				i_input cout<<"2=修改密钥后附加信息 now=["<<Config.rdme<<"];\n";
				i_input cout<<"3=开关附加信息 now=["<<Config.rdmee<<"];\n";
				i_input cout<<"4=开关自动格式化文件名 now=["<<Config.formar<<"];\n";
				i_input cin>>xx;
				if(xx==0)
				{
					ofstream outFile("config.txt");
					if(!outFile) 
					{
						cerr<<"无法保存文件"<<endl;
						return 1;
					}
					outFile<<Config.username<<endl<<Config.rdme<<endl<<Config.rdmee<<endl<<Config.formar<<"\n\nThis is the config file for VeryLow, DO NOT delete.";
					outFile.close();
					break;
				}
				if(xx==1)
				{
					string xxx;
					int f;
					
					system("pause");
					ent
					_color(10);
					ptf("输入用户名, 限一行Unicode, 空格自动转下划线");
					_color();
					ent
					do{i_input getline(cin,xxx);}while(xxx=="");
					
					for(f=0;f<xxx.size();f++)
					{
						if(xxx.at(f)==' ')
						{
							xxx[f]='_';
						}
					}
					Config.username=xxx;
				}
				if(xx==2)
				{
					string xxx;
					int f;
					
					system("pause");
					ent
					_color(10);
					ptf("输入附加信息, 限一行Unicode, 空格自动转下划线");
					_color();
					ent
					do{i_input getline(cin,xxx);}while(xxx=="");
					
					for(f=0;f<xxx.size();f++)
					{
						if(xxx.at(f)==' ')
						{
							xxx[f]='_';
						}
					}
					Config.rdme=xxx;
				}
				if(xx==3)
				{
					Config.rdmee = Config.rdmee==0?1:0;
				}
				if(xx==4)
				{
					Config.formar = Config.formar==0?1:0;
				}
			}while(xx!=0);
		}
		cout<<endl;
	}
	
	system("pause");
	return 0;
}
