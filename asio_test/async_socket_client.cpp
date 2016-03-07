// asio网络库：异步socket的client端

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

//using namespace std;
//using namespace boost;
//using namespace boost::asio;

//异步client
class AsyncClient
{
private:
	boost::asio::io_service& ios;
	boost::asio::ip::tcp::endpoint ep;
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
public:
	AsyncClient(boost::asio::io_service& io) :
		ios(io), 
		ep(boost::asio::ip::address::from_string("127.0.0.1"), 6688)
	{
		start();
	}
	void start()
	{
		sock_pt sock(new boost::asio::ip::tcp::socket(ios));//智能指针
		sock->async_connect(ep, bind(&AsyncClient::conn_handler, this, boost::asio::placeholders::error, sock));
	}
	void conn_handler(const boost::system::error_code& ec, sock_pt sock)
	{
		if (ec)//检测错误码
		{
			return;
		}

		//输出连接的服务器端信息
		std::cout << "recive from:";
		std::cout << sock->remote_endpoint().address() << " port:" << sock->remote_endpoint().port() << std::endl;
		
		//建立接收数据的缓冲区
		boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100, 0));

		//异步读取数据
		sock->async_read_some(boost::asio::buffer(*str), bind(&AsyncClient::read_handler, this, boost::asio::placeholders::error, str));
		
		// 再次启动异步连接
		start();
	}
	void read_handler(const boost::system::error_code& ec, boost::shared_ptr<std::vector<char>>str)
	{
		if (ec)
		{
			return;
		}
		std::cout << &(*str)[0] << std::endl;//输出接收的数据
	}
};
void async_client()
{
	try
	{
		std::cout << "client start." << std::endl;
		boost::asio::io_service ios;
		AsyncClient cl(ios);
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void main()
{
	async_client();
}