// asio网络库：同步socket的Client端

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp> 
#include <boost/bind.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

//IP地址和端点      
//IP地址独立于TCP，UDP等通信协议，asio库使用类ip::address来表示IP地址，可以同时支持ipv4和ipv6两种地址。
void test1()
{
	boost::asio::ip::address addr = addr.from_string("127.0.0.1");
	assert(addr.is_v4());
	std::cout << addr.to_string() << std::endl;
	addr = addr.from_string("ab::12:34:56");
	assert(addr.is_v6());
	//有了ip地址，再加上通信用的端口就构成了一个socket端点，在asio库中用ip::tcp::endpoint类来表示。
	boost::asio::ip::address addr1 = addr1.from_string("127.0.0.1");
	boost::asio::ip::tcp::endpoint ep(addr1, 6688);//创建一个端点对象，端口为6688
	assert(ep.address() == addr1);
	assert(ep.port() == 6688);
}

//可以定时执行任意函数的定时器 a_timer(asyc timer)，
//它持有一个asio定时器对象和一个计数器，还有一个function对象用来保存回调函数
class a_timer
{
private:
	int count, cout_max; //计数器成员变量  
	boost::function<void()> f; //function对象，持有无参数无返回的可调用物  
	boost::asio::deadline_timer t;//asio定时器  
public:
	//构造函数初始化成员变量，将计数器清理，设置计数器的上限，拷贝存储回调函数，并立即启动定时器  
	//之所以要"立即"启动，是因为我们必须包装在io_service.run()之前至少有一个异步操作在执行，
	//否则io_service.run()会因为没有事件处理而立即不等待返回。  
	template<typename F>a_timer(boost::asio::io_service& ios, int x, F func) :f(func), cout_max(x), count(0), t(ios, boost::posix_time::microsec(500))//模板类型，可接受任意可调用物  
	{
		//命名空间下asio::placeholders的一个占位符error，他的作用类似于bind库的占位符_1,_2,用于传递errror_code值。
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}

	//call_func()内部累加计数器，如果计数器未达到上限则调用function对象f
	//然后重新设置定时器的终止时间，再次异步等待被调用，从而达到反复执行的目的。  
	void call_func(const boost::system::error_code &)
	{
		if (count >= cout_max)
		{
			return;
		}
		++count;
		f();
		t.expires_at(t.expires_at() + boost::posix_time::millisec(500));//设置定时器的终止时间为0.5秒之后  
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}
};

void client(boost::asio::io_service& ios)
{
	try
	{
		std::cout << "client start." << std::endl;
		boost::asio::ip::tcp::socket sock(ios);
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 6688);//创建连接端点
		sock.connect(ep);//socket连接到端点
		std::vector<char> str(100, 0);//定义一个vector缓冲区
		sock.read_some(boost::asio::buffer(str));//使用buffer()包装缓冲区接收数据
		std::cout << "recive from: ip:" << sock.remote_endpoint().address() << " port:" << sock.remote_endpoint().port() << std::endl;
		std::cout << "recv:" << &str[0] << std::endl;//输出接收到的数据
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
//同步客户端
void test2()
{
	boost::asio::io_service ios;
	a_timer at(ios, 5, boost::bind(client, boost::ref(ios)));
	ios.run();
}
void test3()
{
}
void test(char t)
{
	std::cout << "press key=====" << t << std::endl;
	switch (t)
	{
	case '1':test1(); break;
	case '2':test2(); break;
	case '3':test3(); break;
	case 27:
	case 'q':exit(0); break;
	default: std::cout << "default " << t << std::endl; break;
	}
}
void main()
{
	while (1)
		test(getch());
}