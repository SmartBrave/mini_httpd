#include<iostream>
using namespace std;

int main()
{
    cout<<"<html>"<<endl;
    cout<<"<head><meta charset=\"utf-8\">"<<endl;
    cout<<"<title>登陆</title></head>"<<endl;
    cout<<"<body>";
    cout<<"<form action=\"...\" method=\"GET\"><br/>"<<endl;
    cout<<"用户名:<input type=\"text\" name=\"user\"/><br/>"<<endl;
    cout<<"密码:<input type=\"password\" name=\"passwd\"/><br/>"<<endl;
    cout<<"<input type=\"submit\"/>"<<endl;
    cout<<"</body>"<<endl;
    cout<<"</html>"<<endl;
    return 0;
}
