#include <bits/stdc++.h>
#include <shared_mutex>
using namespace std;
typedef shared_mutex Lock;
typedef unique_lock< Lock >  WriteLock;
typedef shared_lock< Lock >  ReadLock;

template<class K, class T>
class Node{
public:
    K key;
    T val;
    Node* nxt;
    Node* prev;
    Node(K k, T v):key(k), val(v),nxt(0),prev(0){}
};

template<class K,class T>
class Cache{
private:
    virtual void Used(Node<K,T>*)=0;
    Lock myLock;
    virtual void Evict()=0;
    virtual void InsertNode(Node<K,T>*)=0;
protected:
    int sz;
    unordered_map<K,Node<K,T>*> m;
    Node<K,T>* head;
    Node<K,T>* tail;
    Node<K,T>* FindNode(K key){
        if(m.find(key)==m.end()){
            return 0;
        }
        return m[key];
    }
    void IsolateNode(Node<K,T>* n){
        if(head==n && tail==n){
            head=0,tail=0;
            return;
        }
        if(n->prev){
            n->prev->nxt=n->nxt;
        }else{
            head=n->nxt;
        }
        if(n->nxt){
            n->nxt->prev=n->prev;
        }else{
            tail=n->prev;
        }
    }
    void InsertBegin(Node<K,T>* n){
        if(!head&&!tail){
            head=n;
            tail=n;
            return;
        }
      head->prev=n;
      n->nxt=head;
      head=n;
    }
    void InsertLast(Node<K,T>* n){
        if(!head&&!tail){
            head=n;
            tail=n;
            return;
        }
        tail->nxt=n;
        n->prev=tail;
        tail=n;
    }
    void DeleteLast(){
        if(!tail) return;
        Node<K,T>* temp=tail;
        IsolateNode(temp);
        DeleteNode(temp);
    }
    void DeleteBegin(){
        if(!head)return;
        Node<K,T>* temp=head;
        IsolateNode(temp);
        DeleteNode(temp);
    }
    void DeleteNode(Node<K,T>* n){
        int k=n->key;
        m.erase(k);
        delete n;
    }

public:
    Cache(int sz): sz(sz),head(0), tail(0){}
    void Insert(K key, T val){
        WriteLock w_lock(myLock);
        Node<K,T>* curr=this->FindNode(key);
        if(curr!=0){
            return;
        };
        Evict();
        curr=new Node<K,T>(key,val);
        InsertNode(curr);
    }
    T* Query(K key){
        ReadLock r_lock(myLock);
        Node<K,T>* curr=FindNode(key);
        if(!curr)return nullptr;
        Used(curr);
        return &(curr->val);
    }
    bool IsPresent(K key){
        ReadLock r_lock(myLock);
        Node<K,T>* curr=FindNode(key);
        if(!curr)return false;
        return true;
    }
    ~Cache(){
        Node<K,T>* temp=head;
        while(temp){
            Node<K,T>* temp1=temp->nxt;
            delete temp;
            temp=temp1;
        }
    }
};
template<class K, class T>
class LRU: public Cache<K,T>{
private:
    void Used(Node<K,T>* u){
        this->IsolateNode(u);
        this->InsertBegin(u);
    }
    void Evict(){
        if(this->m.size()==this->sz)
            this->DeleteLast();
    }
    void InsertNode(Node<K,T>* curr){
        this->m[curr->key]=curr;
        this->InsertBegin(curr);
    }
public:
    LRU(int sz):Cache<K,T>(sz){}
};

template<class K, class T>
class FIFO: public Cache<K,T>{
private:
    void Used(Node<K,T>* u){
    }
    void Evict(){
        if(this->m.size()==this->sz)
            this->DeleteBegin();
    }
    void InsertNode(Node<K,T>* curr){
        this->m[curr->key]=curr;
        this->InsertLast(curr);
    }
public:
    FIFO(int sz):Cache<K,T>(sz){}
};

template<class K,class T>
class LIFO: public Cache<K,T>{
private:
    void Used(Node<K,T>* u){
    }
    void Evict(){
        if(this->m.size()==this->sz)
            this->DeleteLast();
    }
    void InsertNode(Node<K,T>* curr){
        this->m[curr->key]=curr;
        this->InsertLast(curr);
    }
public:
    LIFO(int sz):Cache<K,T>(sz){}
};
