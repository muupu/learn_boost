// asio网络库：异步socket的server端

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

void async_test1() {}

//异步server
//异步程序的处理流程与同步程序基本相同，
//只需要把原有的同步调用函数都换成前缀是async_的异步调用函数，
//并增加回调函数，在回调函数中再启动一个异步调用。
class server
{
private:
	boost::asio::io_service& ios;
	boost::asio::ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
public:

	// acceptor在指定的端口号接受连接
	server(boost::asio::io_service& io) :
		ios(io), 
		acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6688))
	{
		start();
	}

	//start()函数用于启动异步接受连接，需要调用acceptor的async_accept()函数。
	//为了能够让socket能够被异步调用后还能使用，我们必须使用shared_ptr来创建socket对象的智能指针，
	//它可以再程序的整个生命周期中存在，直到没有人使用它为止。
	void start()
	{
		sock_pt sock(new boost::asio::ip::tcp::socket(ios));//智能指针
		//启动异步监听服务
		acceptor.async_accept(*sock, bind(&server::accept_handler, this, boost::asio::placeholders::error, sock));
	}

	//当有TCP连接发生时，server::accept_handler()函数将被调用，它使用socket对象发送数据。
	void accept_handler(const boost::system::error_code& ec, sock_pt sock)
	{
		if (ec)//检测错误码
		{
			return;
		}
		//输出连接的客户端信息
		std::cout << "client:";
		std::cout << sock->remote_endpoint().address() << " port:" << sock->remote_endpoint().port() << std::endl;
		
		sock->async_write_some(boost::asio::buffer("hello asio"), bind(&server::write_handler, this, boost::asio::placeholders::error));
		
		//再次启动异步接受连接
		//首先它必须检测asio传递的error_code,保证没有错误发生。
		//然后调用socket对象的async_write_some()异步发送数据。
		//同样，我们必须再为这个异步调用编写回调函数write_handler()。
		//当发送完数据后不要忘了调用start()再次启动服务器接收连接，否则当完成数据发送后io_service将因为没有时间处理而结束运行。
		start();
				
	}

	void write_handler(const boost::system::error_code&)
	{
		std::cout << "send msg complete." << std::endl;
	}
};
void async_test2()
{
	try
	{
		std::cout << "server start." << std::endl;
		boost::asio::io_service ios;
		server serv(ios);
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void async_test(char t)
{
	std::cout << "press key=====" << t << std::endl;
	switch (t)
	{
	case '1':async_test1(); break;
	case '2':async_test2(); break;
	case 27:
	case 'q':exit(0); break;
	default: std::cout << "default " << t << std::endl; break;
	}
}

void main()
{
	async_test2();
}