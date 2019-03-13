//
// Copyright(c) 1999-2013 hyrj. All rights reserved.
// hyc libaray
// author:       lipengxiang
// description:  
//

#ifndef __BP_RTASYNCSTRUCT_H_20130516143621__
#define __BP_RTASYNCSTRUCT_H_20130516143621__

#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>

namespace hyc
{
    /**
     * 异步结构,用于多线程操作.
     */
    class AsyncContainer {
    public:
        AsyncContainer()   {}
        virtual ~AsyncContainer()   {}

        /**
         * 多线程操作的前后调用.
         */
        inline bool    tryLock()      { return _sync.tryLock(); }
        inline bool    tryLock() const{ return _sync.tryLock(); }
        inline void    lock()         { _sync.lock(); }
        inline void    lock() const   { _sync.lock(); }
        inline void    unlock()           { _sync.unlock(); }
        inline void    unlock() const     { _sync.unlock(); }

        /**
         * 多线程token.
         */
        inline SyncTokenPtr getToken()    { return _sync.getToken(); }

    private:
        mutable SyncObject  _sync;
    };

    /**
     * 异步map
     */
    template< class Key, class Val, class Compare = std::less<Key> >
    class AsyncMap : public AsyncContainer {
    public:
        typedef typename std::map<Key, Val, Compare>::iterator        iterator;
        typedef typename std::map<Key, Val, Compare>::const_iterator  const_iterator;
        typedef typename std::map<Key, Compare>::reverse_iterator         reverse_iterator;
        typedef typename std::map<Key, Compare>::const_reverse_iterator   const_reverse_iterator;

        AsyncMap()          {}
        virtual ~AsyncMap() { lock(); clear(); unlock(); }   // 析构前清空, 保证多线程安全.

        inline const Val&  operator[](const Key& _Keyval) const { return _map[_Keyval]; }
        inline Val&        operator[](const Key& _Keyval)       { return _map[_Keyval]; }

        inline void     clear()                                 { _map.clear(); }
        inline bool     empty() const                           { return _map.empty(); }
        inline u32      size() const                            { return _map.size(); }
        inline void     insert( const Key& k, const Val& v )    { _map.insert(std::make_pair(k, v)); }
        inline iterator insert( iterator itr, const Key& k, const Val& v )    { return _map.insert(itr, std::make_pair(k, v)); }
        inline void     erase( iterator itr )             		{ _map.erase(itr); }
        inline void     erase( const Key& k )                   { _map.erase(k); }

        inline iterator        find( const Key& k )             { return _map.find(k); }
        inline const_iterator  find( const Key& k ) const       { return _map.find(k); }

        // 返回第一个大于等于k的位置.
        inline iterator        lower_bound( const Key& k )      { return _map.lower_bound(k); }
        inline const_iterator  lower_bound( const Key& k ) const{ return _map.lower_bound(k); }

        // 返回第一个大于k的位置.
        inline iterator        upper_bound( const Key& k )      { return _map.upper_bound(k); }
        inline const_iterator  upper_bound( const Key& k ) const{ return _map.upper_bound(k); }

        inline iterator        begin()                          { return _map.begin(); }
        inline const_iterator  begin() const                    { return _map.begin(); }
        inline iterator        end()                            { return _map.end(); }
        inline const_iterator  end() const                      { return _map.end(); }

    private:
        std::map<Key, Val, Compare>  _map;
    };

    
    /**
     * 异步set
     */
    template< class Key, class Compare = std::less<Key> >
    class AsyncSet : public AsyncContainer {
    public:
        typedef typename std::set<Key, Compare>::iterator        iterator;
        typedef typename std::set<Key, Compare>::const_iterator  const_iterator;
        typedef typename std::set<Key, Compare>::reverse_iterator         reverse_iterator;
        typedef typename std::set<Key, Compare>::const_reverse_iterator   const_reverse_iterator;

        AsyncSet()          {}
        virtual ~AsyncSet() { lock(); clear(); unlock(); }   // 析构前清空, 保证多线程安全.

        inline void     clear()                                 { _map.clear(); }
        inline bool     empty() const                           { return _map.empty(); }
        inline u32      size() const                            { return _map.size(); }
        inline void     insert( const Key& k )                  { _map.insert(k); }
        inline iterator insert( const_iterator itr, const Key& k )  { return _map.insert(itr, k); }
        inline iterator erase( const_iterator itr )             { return _map.erase(itr); }
        inline void     erase( const Key& k )                   { _map.erase(k); }

        inline iterator        find( const Key& k )             { return _map.find(k); }
        inline const_iterator  find( const Key& k ) const       { return _map.find(k); }

        // 返回第一个大于等于k的位置.
        inline iterator        lower_bound( const Key& k )      { return _map.lower_bound(k); }
        inline const_iterator  lower_bound( const Key& k ) const{ return _map.lower_bound(k); }

        // 返回第一个大于k的位置.
        inline iterator        upper_bound( const Key& k )      { return _map.upper_bound(k); }
        inline const_iterator  upper_bound( const Key& k ) const{ return _map.upper_bound(k); }

        inline iterator        begin()                          { return _map.begin(); }
        inline const_iterator  begin() const                    { return _map.begin(); }
        inline iterator        end()                            { return _map.end(); }
        inline const_iterator  end() const                      { return _map.end(); }

    private:
        std::set<Key, Compare>  _map;
    };

    /**
     * 异步list.
     */
    template< class T >
    class AsyncList : public AsyncContainer {
    public:
        typedef typename std::list<T>::iterator                 iterator;
        typedef typename std::list<T>::const_iterator           const_iterator;
        typedef typename std::list<T>::reverse_iterator         reverse_iterator;
        typedef typename std::list<T>::const_reverse_iterator   const_reverse_iterator;

        AsyncList()          {}
        virtual ~AsyncList() { lock(); clear(); unlock(); }   // 析构前清空, 保证多线程安全.

        inline void     clear()                                 { _list.clear(); }
        inline bool     empty() const                           { return _list.empty(); }
        inline u32      size() const                            { return _list.size(); }
        inline void     insert( const_iterator itr, const T& v ){ _list.insert(itr, v); }
        inline void     push_front( const T& v )                { _list.push_front(v); }
        inline void     push_back( const T& v )                 { _list.push_back(v); }
        inline void     pop_front()                             { _list.pop_front(); }
        inline void     pop_back()                              { _list.pop_back(); }
        inline T        front()                                 { return _list.front(); }
        inline const T  front() const                           { return _list.front(); }
        inline T        back()                                  { return _list.back(); }
        inline const T  back() const                            { return _list.back(); }

        template<class _Pr1>
        inline void     remove_if(_Pr1 pred)                    { _list.remove_if(pred); }
        inline iterator erase( iterator itr )             	    { return _list.erase(itr); }
        inline iterator        find( const T& v )               { return std::find(begin(), end(), v); }
        inline const_iterator  find( const T& v ) const         { return std::find(begin(), end(), v); }

        inline iterator        begin()                          { return _list.begin(); }
        inline const_iterator  begin() const                    { return _list.begin(); }
        inline iterator        end()                            { return _list.end(); }
        inline const_iterator  end() const                      { return _list.end(); }

        inline reverse_iterator         rbegin()                { return _list.rbegin(); }
        inline const_reverse_iterator   rbegin() const          { return _list.rbegin(); }
        inline reverse_iterator         rend()                  { return _list.rend(); }
        inline const_reverse_iterator   rend() const            { return _list.rend(); }

    private:
        std::list<T>    _list;
    };

    /**
     * 异步queue
     */
    template< class T >
    class AsyncQueue : public AsyncContainer {
    public:

        AsyncQueue()          {}
        virtual ~AsyncQueue() { lock(); clear(); unlock(); }   // 析构前清空, 保证多线程安全.

        inline void     clear()             { while (!empty()) pop(); }
        inline bool     empty() const       { return _que.empty(); }
        inline u32      size() const        { return _que.size(); }
        inline void     push( const T& v )  { _que.push(v); }
        inline void     pop()               { _que.pop(); }
        inline T        front()             { return _que.front(); }
        inline const T  front() const       { return _que.front(); }

    private:
        std::queue<T>   _que;
    };

    
    /**
     * 异步stack
     */
    template< class T >
    class AsyncStack : public AsyncContainer {
    public:

        AsyncStack()          {}
        virtual ~AsyncStack() { lock(); clear(); unlock(); }   // 析构前清空, 保证多线程安全.

        inline void     clear()             { while (!empty()) pop(); }
        inline bool     empty() const       { return _stak.empty(); }
        inline u32      size() const        { return _stak.size(); }
        inline void     push( const T& v )  { _stak.push(v); }
        inline void     pop()               { _stak.pop(); }
        inline T        top()               { return _stak.top(); }
        inline const T  top() const         { return _stak.top(); }

    private:
        std::stack<T>   _stak;
    };


} // end namespace hyc

#endif // end of define __BP_RTASYNCSTRUCT_H_20130516143621__