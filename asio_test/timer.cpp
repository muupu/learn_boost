
#include <iostream>  
#include <boost/asio.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/function.hpp>   
#include <boost/bind.hpp>  

//同步定时器
//可以把它与thread库的sleep()函数对比研究一下
//两种虽然都是等待，但内部机制完全不同：thread库的sleep()使用了互斥量和条件变量，在线程中等待，
//而asio则是调用了操作系统的异步机制，如select，epool等完成。  
//同步定时器的用法很简单，但它演示了asio程序的基本结构和流程：
//一个asio程序首先要定义一个io_service对象，它是前摄器模式中最重的proactor角色，
//然后我们声明一个IO操作(这里是定时器),并把它挂接在io_service上，再然后就可以执行后续的 同步或异步操作。  
void timer_test1()
{
	boost::asio::io_service ios; //所有的asio程序必须要有一个io_service对象  

	boost::asio::deadline_timer t(ios, boost::posix_time::seconds(5)); //5秒钟后定时器终止  
	std::cout << t.expires_at() << std::endl;//查看终止的决定时间  
	t.wait();//调用wait()同步等待

	std::cout << "Hello, world!\n";
}

void myprint(boost::system::error_code ec)
{
	std::cout << "hello asio" << std::endl;
}

//异步定时器
//代码大致与同步定时器相同，增加了回调函数，并使用io_service_run()和定时器的async_wait()
//首先我们要定义回调函数，asio库要求回调函数只能有一个参数，而且这个参数必须是const asio::error_code&类型
void timer_test2()
{
	boost::asio::io_service ios;
	boost::asio::deadline_timer t(ios, boost::posix_time::seconds(3));

	//异步等待，传入回调函数，立即返回  
	//它通知io_service异步的执行IO操作，并且注册了回调函数，用于在IO操作完成时有事件多路分离器分派返回值(error_code)调用。  
	t.async_wait(myprint);
	std::cout << "it show before t expired." << std::endl;

	//最后我们必须调用io_service的run()成员函数，它启动前摄器的事件处理循环，阻塞等待所有的操作完成并分派事件。
	//如果不调用run()那么虽然被异步执行了，但没有一个等待他完成的机制，回调函数将得不到执行的机会。 
	ios.run();

	//将与hello asio一起输出，说明run()是阻塞函数  
	std::cout << "runned" << std::endl;
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

void print1(int index)
{
	std::cout << "hello asio:" << index << std::endl;
}

void print2()
{
	std::cout << "hello boost" << std::endl;
}

void timer_test3()
{
	boost::asio::io_service ios;
	a_timer a(ios, 10, boost::bind(print1, 2));//启用第一个定时器  
	a_timer at(ios, 5, print2);//启用第二个定时器  
	ios.run();//ios等待异步调用结束  
}

//int main()  
//{  
//	//timer_test1();
//	//timer_test2();
//	timer_test3();
//	return 0;  
//}  