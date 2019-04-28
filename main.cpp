#include<iostream>
#include<string.h>


using namespace std;

class items
{
protected :
    string name;
    float price;
    int units;
public:
    items(string nname,float pprice,int uunits):name(nname),price(pprice),units(uunits)
    {}

    void addunits(int u)
    {
        units=u;
    }

    void subtractunits(int u)
    {
        units=units-u;
    }

    string getName()
    {
        return name;
    }

    float getPrice()
    {
        return price;
    }
    virtual void getdetail()=0;
    virtual string getType()=0;

};

class stationary:public items
{
    string brand;

public:
    stationary(string nname,float pprice,int uunits,string bbrand):brand(bbrand),items(nname,pprice,uunits)
    {}
    void getdetails()
    {
        cout<<"Name :"<<name<<endl<<"Price : "<<price<<endl<<"Units :  "<<units<<endl<<"Brand :"<<brand<<endl;
    }
    string getType()
    {
        return "Stationary";
    }
};

class newspaper:public items
{
    string publisher;

public:
    newspaper(string nname,float pprice,int uunits,string ppublisher):publisher(ppublisher),items(nname,pprice,uunits)
    {}
    void getdetails()
    {
                cout<<"Name :"<<name<<endl<<"Price : "<<price<<endl<<"Units :  "<<units<<endl<<"Publisher :"<<publisher<<endl;
    }
    string getType()
    {
        return "Newspaper";
    }
};

class book:public items
{
    string author;
    string condition;

public:
    book(string nname,float pprice,int uunits,string aauthor,string ccondition):author(aauthor),condition(ccondition),items(nname,pprice,uunits)
    {}
    void getdetails()
    {
                cout<<"Name :"<<name<<endl<<"Price : "<<price<<endl<<"Units :  "<<units<<endl<<"Author :"<<author<<endl<<"Condition :"<<condition<<endl;
    }
    string getType()
    {
        return "Book";
    }
};

class cart
{
    items **ii;
    int totalprice;
    int itemcount;

public:
    cart()
    {
        itemcount=0;
        totalprice=0;
    }
    void addItem(items *itm,int units)
    {
        ii[itemcount]=itm;
        ii[itemcount]->addunits(units);
        itemcount+=1;
    }
    items *  getItems()
    {

    }
    int getTotal()
    {
        for(int i=0;i<itemcount;i++)
        {
            totalprice+=ii[i]->getPrice();
        }
        return totalprice;
    }
};

class customer
{public:
    int custid;
    string custname;
    cart c;

public:
    customer(int ccustid,string ccustname,cart cc):custid(custid),custname(ccustname)
    {
        c=cc;
    }

    items *searchitem(items **it,int itemsize,string itemname,int units)
    {
        for(int i=0;i<itemsize;i++)
        {
            if(it[i]->getName()==itemname)
            {
                it[i]->subtractunits(units);
                additemtocart(it[i],units);
            }
        }
    }

    bool additemtocart(items *item,int units)
    {
        c.addItem(item,units);
    }
};

class order
{
    int orderid;
    customer cust;
public:
    order(int oorderid,customer ccust):orderid(oorderid),cust(ccust)
    {}

    void print()
    {
        cout<<"Total bill to be paid is  "<<cust.c.getTotal()<<endl;
    }
};

class shop
{
    string shopname;
    int shopno;
    string shopaddress;
    string ownername;
    items **i;
    int noofitems;
    customer *c;
    static shop *instance;
    shop(string sshopname,int sshopno,string ssad,string oownername,items **ii,customer *cc):shopname(sshopname),shopno(sshopno),shopaddress(ssad),ownername(oownername),i(ii),c(cc)
    {
        noofitems=0;
    }

public:

    static shop *getinstance(string sshopname,int sshopno,string ssad,string oownername,items **ii,customer *cc)
    {
        if(!instance)
            instance=new shop(sshopname,sshopno,ssad,oownername,ii,cc);
        return instance;
    }

    items **getItems()
    {
        return this->i;
    }
    void printshopdetails()
    {
        cout<<"Shop Name"<<shopname<<endl<<"Shop Address"<<shopaddress<<endl<<"Shop No"<<shopno<<endl<<"Owner Name"<<ownername<<endl;
    }

    void additems(items *ii)
    {
        i[noofitems]=ii;
        noofitems++;
    }

    void removeitems(items *ii)
    {

    }

    void authentication()
    {}
};

int main(int argc, char** argv)
{
//    newspaper n("Navbharat",10,4,"Times");
    cout<<"Hello World123";
   /* sqlite3* DB;
        int exit = 0;
        exit = sqlite3_open("example.db", &DB);

        if (exit) {
            std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
            return (-1);
        }
        else
            std::cout << "Opened Database Successfully!" << std::endl;
        sqlite3_close(DB);
     */
    return 0;
}
