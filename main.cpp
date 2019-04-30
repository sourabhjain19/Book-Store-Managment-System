#include<iostream>
#include<string.h>
#include<exception>
#include<sqlite3.h>
#include<vector>

using namespace std;

/* Database code functions */
sqlite3 *DB;
vector<string> shp;

//single item query
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      shp.push_back(argv[i]);
   }
   cout<<endl;
   return 0;
}

//multyple items query
struct Items
{
    string name;
    float price;
    string type;
    string brand;
    string publisher;
    string author;
    string condition;
    int units;
};

void query_step(const char* query, vector<Items> &res)
{
    sqlite3_stmt * stmt;

    int rc = sqlite3_prepare_v2(DB,query,-1,&stmt,NULL);

    if(rc != SQLITE_OK)
    {
        cout<<"Error connecting to the database "<<sqlite3_errmsg(DB);
        return;
    }

    while(1)
    {
        rc = sqlite3_step(stmt);
        if(rc==SQLITE_DONE)
            break;
        if (rc != SQLITE_ROW) {
            cout<<"Internal database error "<< sqlite3_errmsg(DB);
            break;
        }

        Items it;
        const char* text;
        text = (const char*)sqlite3_column_text(stmt,0);
        it.name = text?string(text):"NULL";
        it.price = sqlite3_column_int(stmt,1);
        text = (const char*)sqlite3_column_text(stmt,2);
        it.type = text?string(text):"NULL";
        text = (const char*)sqlite3_column_text(stmt,3);
        it.brand = text?string(text):"NULL";
        text = (const char*)sqlite3_column_text(stmt,4);
        it.publisher = text?string(text):"NULL";
        text = (const char*)sqlite3_column_text(stmt,5);
        it.author = text?string(text):"NULL";
        text = (const char*)sqlite3_column_text(stmt,6);
        it.condition = text?string(text):"NULL";
        it.units = sqlite3_column_int(stmt,7);
        res.push_back(it);
    }
    sqlite3_finalize(stmt);
}

//converts string class to char string
void stoa(string s, char c[])
{
    int len=s.length();
    int i;
    for(i=0;i<len;i++)
    {
        c[i]=s[i];
    }
    c[i]='\0';
}

/*end database code*/


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
            cout<<endl;
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
    int choice,choice1,choice2,n,flag; //for switch statements
    string name,type;
    int units;
    float price;
    string brand,condition;
    customer cust[100];
    items *itms[100];
    shop *sp;
    string quer;
    char c[150];

    //Connecting database
    int exit = 0;
    char *zErrMsg = 0;
    exit = sqlite3_open("data.db", &DB);
    if (exit) {
        std::cerr << "Error open DB "<< std::endl;
        return (-1);
    }
    else
        std::cout << "Database connected successfully" << std::endl;


    //Initialisation code
    
    sqlite3_exec(DB, "select * from shop",callback,0,&zErrMsg);
    sp = sp->getinstance(*(shp.begin()),stoi(*(shp.begin()+1)),*(shp.begin()+2),*(shp.begin()+3), itms, cust);
    
    vector<Items> res;

    query_step("select * from items",res);

    for(auto i=res.begin();i!=res.end();i++)
    {
        if((*i).type=="stationary")
        {
            items *tempi = new stationary((*i).name,(*i).price,(*i).units,(*i).brand);
            sp->additems(tempi);
        }
        else if((*i).type=="book")
        {
            items *tempi = new book((*i).name,(*i).price,(*i).units,(*i).author,(*i).condition);
            sp->additems(tempi);
        }
        else
        {
            items *tempi = new newspaper((*i).name,(*i).price,(*i).units,(*i).publisher);
            sp->additems(tempi);
        }
    }

    cout<<endl<<"Loaded successfully"<<endl;

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
                    while(1)
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
                                              sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                              quer="insert into items values('" + name + "'," + to_string(price) +"," + "'stationary'" + ",'" + brand +"',?,?,?," + to_string(units) +")";
                                              stoa(quer,c);
                                              sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                              sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
                                              break;
                                       case 2:cout<<"Enter the Name,Price,Units,Publisher:";
                                              cin>>name>>price>>units>>brand;
                                              tempi=new newspaper(name,price,units,brand);
                                              sp->additems(tempi);
                                              sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                              quer="insert into items values('" + name + "'," + to_string(price) +"," + "'newspaper'" + ",?,'" + brand +"',?,?," + to_string(units) +")";
                                              stoa(quer,c);
                                              sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                              sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
                                              break;
                                       case 3:cout<<"Enter the Name,Price,Units,Author,Condition :";
                                              cin>>name>>price>>units>>brand>>condition;
                                              tempi=new book(name,price,units,brand,condition);
                                              sp->additems(tempi);
                                              sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                              quer="insert into items values('" + name + "'," + to_string(price) +"," + "'book'" + ",?,?,'" + brand + "','" + condition +"'," + to_string(units) +")";
                                              stoa(quer,c);
                                              sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                              sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
                                              break;
                                       default : cout<<"Invalid Entry";
                                   }//END OF ITEM CREATION CASE
                                   cout<<"Items Successfully Added"<<endl;
                               }
                               break;
                            case 2:
                               cout<<"1> Stationary"<<endl;
                               cout<<"2> Newspaper"<<endl;
                               cout<<"3> Book"<<endl;
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
                                        sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                        quer="delete from items where name='"+name+"' and type='stationary'";
                                        stoa(quer,c);
                                        sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                        sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
                                        break;
                                    case 2:
                                        cout<<"Enter the name of Item to be deleted :";
                                        cin>>name;
                                        sp->removeitems(name,"Newspaper");
                                        sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                        quer="delete from items where name='"+name+"' and type='newspaper'";
                                        stoa(quer,c);
                                        sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                        sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
                                        break;
                                    case 3 :
                                        cout<<"Enter the name of Item to be deleted :";
                                        cin>>name;
                                        sp->removeitems(name,"Book");
                                        sqlite3_exec(DB,"begin transaction",callback,0,&zErrMsg);
                                        quer="delete from items where name='"+name+"' and type='book'";
                                        stoa(quer,c);
                                        sqlite3_exec(DB,c,callback,0,&zErrMsg);
                                        sqlite3_exec(DB,"commit",callback,0,&zErrMsg);
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
                        }//END OF ADMIN CHOICE
                    }
                }
            break;
            case 2:
                {
                    cout<<"Welcome!\n";
                    cout<<"Enter your Name :";
                    cin>>name;
                    customer c(name);
                    cout<<"Hello"+ c.custname<<endl;
                    while(1)
                    {
                        try
                        {
                            cout<<"__________Customer__________"<<endl;
                            cout<<"1> Add Items"<<endl;
                            cout<<"2> View Items in cart"<<endl;
                            cout<<"3> View items"<<endl;
                            cout<<"4> Generate Bill"<<endl;
                            cout<<"5> Exit"<<endl;
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
                            case 2:
                                c.c.print();
                                break;
                            case 3:
                                cout<<"The items are:"<<endl;
                                sp->printitems();
                                break;
                            case 4:
                                cout<<"Total Amount to be Paid : "<<c.c.getTotal()<<endl;
                                break;
                            case 5:
                                cout<<"Thank You Please Visit Again .....\nAmount Payable : "<<c.c.getTotal()<<endl;
                                return 0;
                            default:
                                cout<<"Invalid choice"<<endl;
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
        }//END OF USER TYPE CASE;
    }
    return 0;
}
