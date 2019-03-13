using System;
using System.Collections.Generic;
using System.Text;
using Hy;
using Hy.OS;

class aThreadPoolWorkItem : IThreadPoolWorkItem
{
    public bool     onInvoke()
    {
        int i = 0;
        while (i<100)
        {
            Console.WriteLine(".net thread 1");
            System.Threading.Thread.Sleep(10);
        }
        return false;
    }
}

class bThreadPoolWorkItem : IThreadPoolWorkItem
{
    public bool onInvoke()
    {
        int i = 0;
        while (i<100)
        {
            Console.WriteLine(".net thread 2");
            System.Threading.Thread.Sleep(10);
        }
        return false;
    }
}

namespace test
{
    class Program
    {
        static void Main(string[] args)
        {
            ThreadPool tp = new ThreadPool();


            ThreadPool.getSingletonPtr().QueueWorkItem( new aThreadPoolWorkItem(), ThreadPoolMode.ThreadPoolMode_Default );
            ThreadPool.getSingletonPtr().QueueWorkItem(new bThreadPoolWorkItem(), ThreadPoolMode.ThreadPoolMode_Default);
            System.Threading.Thread.Sleep(1000000000);
        }
    }
}
