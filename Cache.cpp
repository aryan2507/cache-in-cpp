#include <bits/stdc++.h>
#include <shared_mutex>
using namespace std;
typedef shared_mutex Lock;
typedef unique_lock< Lock >  WriteLock;
typedef shared_lock< Lock >  ReadLock;

template<class T>
class Node{
public:
    int key;
    T val;
    Node* nxt;
    Node* prev;
    Node(int k, T v):key(k), val(v),nxt(0),prev(0){}
};

template<class T>
class Cache{
private:
    virtual void Used(Node<T>*)=0;
    Lock myLock;
protected:
    int sz;
    unordered_map<int,Node<T>*> m;
    Node<T>* head;
    Node<T>* tail;
    Node<T>* FindNode(int key){
        if(m.find(key)==m.end()){
            return 0;
        }
        return m[key];
    }
    void IsolateNode(Node<T>* n){
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
    void InsertBegin(Node<T>* n){
        if(!head&&!tail){
            head=n;
            tail=n;
            return;
        }
      head->prev=n;
      n->nxt=head;
      head=n;
    }
    void InsertLast(Node<T>* n){
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
        Node<T>* temp=tail;
        IsolateNode(temp);
        DeleteNode(temp);
    }
    void DeleteBegin(){
        if(!head)return;
        Node<T>* temp=head;
        IsolateNode(temp);
        DeleteNode(temp);
    }
    void DeleteNode(Node<T>* n){
        int k=n->key;
        m.erase(k);
        delete n;
    }
public:
    Cache(int sz): sz(sz),head(0), tail(0){}
    virtual void InsertPair(int, T)=0;
    void Insert(int key, T val){
        WriteLock w_lock(myLock);
        InsertPair(key, val);
    }
    T Query(int key){
        ReadLock r_lock(myLock);
        Node<T>* curr=FindNode(key);
        if(!curr)return 0;
        Used(curr);
        return curr->val;
    }
    void PrintIt(){
        Node<T>* temp=head;
        while(temp){
            cout<<temp->val<<" ";
            temp=temp->nxt;
        }
        cout<<endl;
    }

    bool IsPresent(int key){
        ReadLock r_lock(myLock);
        Node<T>* curr=FindNode(key);
        if(!curr)return false;
        return true;
    }
    ~Cache(){
        Node<T>* temp=head;
        while(temp){
            Node<T>* temp1=temp->nxt;
            delete temp;
            temp=temp1;
        }
    }
};
template<class T>
class LRU: public Cache<T>{
private:
    void Used(Node<T>* u){
        this->IsolateNode(u);
        this->InsertBegin(u);
    }
public:
    LRU(int sz):Cache<T>(sz){}
    void InsertPair(int key, T val){
        Node<T>* curr=this->FindNode(key);
        if(curr!=0){
            return;
        };
        curr=new Node<T>(key,val);
        if(this->m.size()==this->sz)
            this->DeleteLast();
        this->m[key]=curr;
        this->InsertBegin(curr);
    }
};

template<class T>
class FIFO: public Cache<T>{
private:
    void Used(Node<T>* u){
    }
public:
    FIFO(int sz):Cache<T>(sz){}
    void InsertPair(int key, T val){
        Node<T>* curr=this->FindNode(key);
        if(curr!=0){
            return;
        };
        curr=new Node<T>(key,val);
        if(this->m.size()==this->sz)
            this->DeleteBegin();
        this->m[key]=curr;
        this->InsertLast(curr);
    }
};

template<class T>
class LIFO: public Cache<T>{
private:
    void Used(Node<T>* u){
    }
public:
    LIFO(int sz):Cache<T>(sz){}
    void InsertPair(int key, T val){
        Node<T>* curr=this->FindNode(key);
        if(curr!=0){
            return;
        };
        curr=new Node<T>(key,val);
        if(this->m.size()==this->sz)
            this->DeleteLast();
        this->m[key]=curr;
        this->InsertLast(curr);
    }
};
