/*
* =============================================================================
*
*       文件名:  		KDeque.hpp
*
*    	 描述:    		阻塞队列类
*
*       版本:    		V1.0
*       创建时间:  	2019-04-21 17:22
*       编译器:  		gcc
*       作者:  		huangpengfei, 798677116@qq.com
*       修改日志:  	1. Create file.
*
* =============================================================================
*/


#ifndef __KDeque_H
#define __KDeque_H
#include <condition_variable>  
#include <mutex>  
#include <deque>
#include <iostream>
using namespace std;

template<class T>
class KDeque
{
public:
	typedef std::unique_lock<std::mutex> KDeqlock;

	/*
	*函数功能：队列的构造函数
	*
	*输入参数：maxCapacity默认为-1表示构造一个没有上线的队列，maxCapacity其他表示队列最大的元素个数
	*
	*返回值：void
	*/
	KDeque(const int maxCapacity = -1, const bool running = true) :m_maxCapacity(maxCapacity), m_running(running) {}



	/*
	*函数功能：队列的析构函数
	*
	*输入参数：void
	*
	*返回值：void
	*/
	~KDeque() { m_deque.clear(); }



	/*
	*函数功能：获取队列的元素个数
	*
	*输入参数：void
	*
	*返回值：队列中元素的个数
	*/
	size_t size();



	/*
	*函数功能：将元素添加到队列
	*
	*输入参数：与队列中元素类型一样的对象的引用
	*
	*返回值：void
	*/
	void push_back(const T &item);



	/*
	*函数功能：删除队列的第一个元素
	*
	*输入参数：void
	*
	*返回值：队列第一个元素
	*/
	T pop_front();



	/*
	*函数功能：删除队列的队尾的元素
	*
	*输入参数：void
	*
	*返回值：队列的队尾的元素
	*/
	T pop_back();



	/*
	*函数功能：判断队列是否为空
	*
	*输入参数：void
	*
	*返回值：为空为false,不为空位true
	*/
	bool empty();



	/*
	*函数功能：判断对队列是否为满
	*
	*输入参数：void
	*
	*返回值：不为满false,满为true
	*/
	bool full();



	/*
	*函数功能：获取队列的第一个元素的位置
	*
	*输入参数：void
	*
	*返回值：返回队列的第一个元素的迭代器
	*/
	typename deque<T>::iterator begin();



	/*
	*函数功能：获取队列的最后一个元素的位置
	*
	*输入参数：void
	*
	*返回值：返回队列的最后一个元素的迭代器
	*/
	typename deque<T>::iterator end();



	/*
	*函数功能：获取STL的原始的deque
	*
	*输入参数：void
	*
	*返回值：原始的STL的deque的引用
	*/
	typename std::deque<T>& get();



	/*
	*函数功能：重载“[]”操作符
	*
	*输入参数：第N个元素
	*
	*返回值：队列的第N个元素的引用
	*/
	T& operator[](const int index) const;


	void set_stop(const bool stop = false);

	void set_start(const bool stop = true);
private:
	bool hasCapacity() const;

	deque<T> m_deque;

	const int m_maxCapacity;

	bool m_running;
	std::mutex m_mutex;

	std::condition_variable m_notEmpty;

	std::condition_variable m_notFull;
};

template<class T>  size_t KDeque<T>::size()
{
	KDeqlock lock(m_mutex);
	return m_deque.size();
}

template<class T> void KDeque<T>::push_back(const T &item)
{
	KDeqlock lock(m_mutex);
	if (true == hasCapacity())
	{
		while (m_deque.size() >= m_maxCapacity)
		{
			m_notFull.wait(lock);
		}
	}
	m_deque.push_back(item);
	m_notEmpty.notify_all();
}

template<class T> T KDeque<T>::pop_front()
{
	KDeqlock lock(m_mutex);
	//暂时先注释掉 发现配合线程池使用时会有cpu高的问题 待分析
	/*while (m_deque.empty() && m_running)
	{
		cout << "wait" << endl;
		m_notEmpty.wait(lock);
	}*/
	if (!m_running || m_deque.empty())
	{
		return NULL;
	}
	T tmp = *m_deque.begin();
	m_deque.pop_front();

	m_notFull.notify_all();

	lock.unlock();
	return tmp;
}

template<class T> T KDeque<T>::pop_back()
{
	KDeqlock lock(m_mutex);
	//暂时先注释掉 发现配合线程池使用时会有cpu高的问题 待分析
	/*while ((m_deque.empty()) && m_running)
	{
		m_notEmpty.wait(lock);
	}*/
	if (!m_running || m_deque.empty())
	{
		return NULL;
	}
	T tmp = *m_deque.end();
	m_deque.pop_back();

	m_notFull.notify_all();

	lock.unlock();
	return tmp;
}

template<class T> inline bool KDeque<T>::empty()
{
	KDeqlock lock(m_mutex);
	return m_deque.empty();
}

template<class T> bool KDeque<T>::full()
{
	if (false == hasCapacity)
	{
		return false;
	}

	KDeqlock lock(m_mutex);
	return m_deque.size() >= m_maxCapacity;
}

template<class T> inline T& KDeque<T>::operator[](const int index) const
{
	KDeqlock lock(m_mutex);
	return m_deque[index];
}

template<class T> inline typename deque<T>::iterator KDeque<T>::begin()
{
	KDeqlock lock(m_mutex);
	return m_deque.begin();
}

template<class T> inline typename deque<T>::iterator KDeque<T>::end()
{
	KDeqlock lock(m_mutex);
	return m_deque.end();
}

template<class T> typename std::deque<T>& KDeque<T>::get()
{
	KDeqlock lock(m_mutex);
	return m_deque;
}

template<class T> inline bool KDeque<T>::hasCapacity() const
{
	return m_maxCapacity > 0;
}
template<class T> inline void KDeque<T>::set_stop(const bool stop)
{
	m_running = stop;
	m_notEmpty.notify_all();
}

template<class T> inline void KDeque<T>::set_start(const bool stop)
{
	m_running = stop;
	m_notEmpty.notify_all();
}

#endif