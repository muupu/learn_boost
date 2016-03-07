// asio网络库：同步socket的server端

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

//server端
//它用一个acceptor对象在6688端口接受连接，当有连接时使用socket对象发送一个字符串
void socket_server()
{
	try
	{
		boost::asio::io_service ios;

		//acceptor类对于的socket API的accept()函数功能，他用于服务器端，在指定的端口号接受连接，必须配合socket类才能完成通信。
		boost::asio::ip::tcp::acceptor acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6688));
		std::cout << acceptor.local_endpoint().address() << std::endl;
		while (true)
		{
			boost::asio::ip::tcp::socket sock(ios);
			acceptor.accept(sock);//阻塞等待socket连接
			std::cout << "client:ip:" << sock.remote_endpoint().address() << "   port:" << sock.remote_endpoint().port() << std::endl;
			
			//服务器端程序里要注意的是自由函数buffer(),它可用包装很多种类的容器成为asio组件可用的缓冲区类型。
			//通常我们不能直接把数组，vector等容器用作asio的读写参数，必须使用buffer()函数包装。
			sock.write_some(boost::asio::buffer("hello asio"));
			
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//void main()
//{
//	socket_server();
//}