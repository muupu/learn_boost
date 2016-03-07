
#include <iostream>  
#include <boost/asio.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>  

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
  
int main()  
{  
	//timer_test1();
	timer_test2();
	return 0;  
}  