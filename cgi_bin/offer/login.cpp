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

    cout<<"<h2 align=\"center\">终于等到你!</h2>";
    cout<<"<p>本网站为今年找工作的同学而开发，可以为你记录都投了哪些公司的简历，目前处于哪一个环节等,不用再重复找人内推。当然，这些信息需要你手动更新:)</p>"<<endl;
    cout<<"<p>你可以添加一个公司，然后勾选你目前对应的环节，保存即可!</p>"<<endl;
    //cout<<"<p>请保存下方的二维码图片，以便于出门在外随时查看。</p>"<<endl;
    //cout<<"<b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;By Smart</b>"<<endl;
    //cout<<"<p>这是一张图片</p>"<<endl;
    cout<<"<p>请先注册,以便于我为你记录~</p>"<<endl;
    cout<<"<form action=\"show_offer\" method=\"POST\"><br/>"<<endl;
    cout<<"用户名:<input type=\"text\" name=\"user\"/><br/>"<<endl;
    cout<<"密&nbsp;码:<input type=\"password\" name=\"passwd\"/><br/>"<<endl;
    cout<<"<input type=\"submit\" value=\"登陆\"/>"<<endl;
    cout<<"&nbsp;&nbsp;<a href=\"create_user\">注册</a>"<<endl;
    cout<<"</form>"<<endl;
    cout<<"</div>"<<endl;
    cout<<"</body>"<<endl;
    cout<<"</html>"<<endl;
    return 0;
}
