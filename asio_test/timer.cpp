
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
  
int main()  
{  
	timer_test1();
  
	return 0;  
}  