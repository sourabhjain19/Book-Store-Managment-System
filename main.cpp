#include<iostream>
#include<string.h>
#include<exception>


using namespace std;

class items
{
protected :
    string name;
    float price;
    int units;
public:
    items(string nname="",float pprice=0.0,int uunits=0):name(nname),price(pprice),units(uunits)
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
    virtual void getdetails()=0;
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
    items **getItems()
    {
        return ii;
    }
    void print()
    {
        for(int i=0;i<itemcount;i++)
        {
            ii[i]->getdetails();
        }
    }

    int getTotal()
    {
        for(int i=0;i<itemcount;i++)
        {
            totalprice+=ii[i]->getPrice();
        }
        return totalprice;
    }
    int getitemscount()
    {
        return itemcount;
    }
};

class customer
{
public:
    string custname;
    cart c;

public:
    customer(string ccustname=""):custname(ccustname)
    {
    }

    items *searchitem(items **it,int itemsize,string itemname,int units,string type)
    {
        int flag=0;
        for(int i=0;i<itemsize;i++)
        {
            if(it[i]->getName()==itemname && it[i]->getType()==type)
            {
                flag=1;
                it[i]->subtractunits(units);
                additemtocart(it[i],units);
            }
        }
        if(flag==0)
            throw "Item Not Found";
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

    static shop *getinstance(string sshopname,int sshopno,string ssad,string oownername, items **ii,customer *cc)
    {
        if(!instance)
            instance=new shop(sshopname,sshopno,ssad,oownername,ii,cc);
        return instance;
    }

    void setnoofitems(int no)
    {
        noofitems=no;
    }
    items **getItems()
    {
        return this->i;
    }
    void printitems()
    {
        for(int j=0;j<noofitems;j++)
        {
            i[j]->getdetails();
        }
    }
    void printshopdetails()
    {
        cout<<"Shop Name"<<shopname<<endl<<"Shop Address"<<shopaddress<<endl<<"Shop No"<<shopno<<endl<<"Owner Name"<<ownername<<endl;
    }

    void additems(items *ii)
    {
        i[noofitems]=ii;
        noofitems++;
        cout<<"Items Successfully Added"<<endl;
    }

    void removeitems(string itemname,string type)
    {
        int flag=1;
        for(int j=0;j<noofitems;j++)
        {
            if(i[j]->getName()==itemname && i[j]->getType()==type)
            {
                flag=0;
                i[j]->addunits(0);
                cout<<"Item Removed Successfully\n";
            }
        }
        if(flag==1)
        {
            throw "Name not found";
        }
    }

    int getnoofitems()
    {
        return noofitems;
    }

    int authentication()
    {
        string usr,pswd;
        cout<<"Enter the username: ";
        cin>>usr;
        cout<<"Enter the password: ";
        cin>>pswd;
        try
        {
            if(!(usr==ownername && pswd=="Password"))
            {
                throw "\nAuthentication Failure\n";
            }
            else
                  cout<<"\nAuthentication successful\n";
                  return 0;
        }

        catch(const char *msg)
        {
            cout<<msg;
            return 1;
        }

    }
};

shop* shop::instance=NULL;

int main(int argc, char** argv)
{
    int choice,choice1,choice2,n,flag;
    string name,type;
    int price,units;
    string brand,condition;
    customer cust[100];
    items *itms[100];
    shop *sp;
    sp = sp->getinstance("Oxford",12,"Vidyanagar, Hubli","Admin", itms, cust);
    while(1)
    {
        cout<<"__________Users__________"<<endl;
        cout<<"1> Owner"<<endl;
        cout<<"2> Customer"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        switch (choice)
        {
            case 1:
                flag=sp->authentication();
                if(flag==1)
                {}
                else
                {
                cout<<"__________Owner__________"<<endl;
                cout<<"1>Add Items"<<endl;
                cout<<"2>Delete Items"<<endl;
                cout<<"3>View Items"<<endl;
                cout<<"4>Exit"<<endl;
                cout<<"Enter your choice: ";
                cin>>choice1;
                switch(choice1)
                {
                     case 1:
                                    cout<<"Enter the No of Unique Items to be added : ";
                                    cin>>n;
                                    cout<<"Enter the items to be Added : "<<endl;
                                    for(int i=0;i<n;i++)
                                    {
                                        cout<<"1>Stationary"<<endl;
                                        cout<<"2>Newspaper"<<endl;
                                        cout<<"3>Book"<<endl;
                                         cout<<"Enter the type of Item: ";
                                        cin>>choice2;
                                        switch(choice2)
                                        {
                                            items *tempi;
                                            case 1:cout<<"Enter the Name,Price,Units,Brand :";
                                            cin>>name>>price>>units>>brand;
                                            tempi=new stationary(name,price,units,brand);
                                            sp->additems(tempi);
                                            break;
                                            case 2:cout<<"Enter the Name,Price,Units,Publisher:";
                                            cin>>name>>price>>units>>brand;
                                            tempi=new newspaper(name,price,units,brand);
                                            sp->additems(tempi);
                                            break;
                                            case 3:cout<<"Enter the Name,Price,Units,Author,Condition :";
                                            cin>>name>>price>>units>>brand>>condition;
                                            tempi=new book(name,price,units,brand,condition);
                                            sp->additems(tempi);
                                            break;
                                            default : cout<<"Invalid Entry";
                                        }

                                    }
                                    break;
                        case 2:
                                       cout<<"1>Stationary"<<endl;
                                       cout<<"2>Newspaper"<<endl;
                                       cout<<"3>Book"<<endl;
                                       cout<<"Enter the type of item to be deleted : ";
                                       cin>>choice2;
                                       try
                                       {
                                       switch(choice2)
                                       {
                                       case 1 :
                                                        cout<<"Enter the name of Item to be deleted :";
                                                        cin>>name;
                                                        sp->removeitems(name,"Stationary");
                                                        break;
                                       case 2:
                                                        cout<<"Enter the name of Item to be deleted :";
                                                        cin>>name;
                                                        sp->removeitems(name,"Newspaper");
                                                        break;
                                       case 3 :
                                                        cout<<"Enter the name of Item to be deleted :";
                                                        cin>>name;
                                                        sp->removeitems(name,"Book");
                                                        break;
                                       }
                                       }
                                       catch(const char * msg)
                                       {
                                           cout<<msg<<endl;
                                       }
                                       break;
                        case 3:sp->printitems();
                                    break;
                        case 4:return 0;
                }
                }
                break;
            case 2:
                {
                cout<<"Welcome!\n";
                cout<<"Enter your Name :";
                cin>>name;
                customer c(name);
                while(1)
                {
                try
                {
                cout<<"__________Customer__________"<<endl;
                cout<<"1>Add Items"<<endl;
                cout<<"2> View Items"<<endl;
                cout<<"3>Generate Bill"<<endl;
                cout<<"4>Exit"<<endl;
                cout<<"Enter your choice: ";
                cin>>choice1;
                switch(choice1)
                {
                case 1:
                    {
                    cout<<"Enter the no of Items to be purchased :";
                    cin>>n;
                    for(int j=0;j<n;j++)
                    {
                        cout<<"Enter the name ,type and units of item : ";
                        cin>>name>>type>>units;
                        c.searchitem(sp->getItems(),sp->getnoofitems(),name,units,type);
                    }
                    }
                    break;
                case 2 :
                        c.c.print();
                        break;
                case 3:
                    cout<<"Total Amount to be Paid : "<<c.c.getTotal()<<endl;
                    break;
                case 4:
                    cout<<"Thank You Please Visit Again .....\nAmount Payable : "<<c.c.getTotal()<<endl;
                    return 0;
            }
                }
                catch(const char * msg)
                {
                    cout<<msg<<endl;
                }
                }
                }
                break;

            default:
                cout<<"\nInvalid choice!\n";
                break;
        }
    }
    return 0;
}
