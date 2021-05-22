#include<iostream>
using namespace std;

int main()
{
    cout<<"Content-type: text/html\n\n"<<endl;
    cout<<"<html>"<<endl;
    cout<<"<head><meta charset=\"utf-8\">"<<endl;
    cout<<"<title>登陆</title></head>"<<endl;
    cout<<"<body>";
    cout<<"<div align=\"center\" style=\"height:300px;\">"<<endl;

    cout<<"<form action=\"register\" method=\"POST\"><br/>"<<endl;
    cout<<"用户名:<input type=\"text\" name=\"user\"/><br/>"<<endl;
    cout<<"密&nbsp;码:<input type=\"password\" name=\"passwd\"/><br/>"<<endl;
    cout<<"<input type=\"submit\" value=\"注册\"/>"<<endl;
    cout<<"</form>"<<endl;
    cout<<"</div>"<<endl;
    cout<<"</body>"<<endl;
    cout<<"</html>"<<endl;
    return 0;
}

