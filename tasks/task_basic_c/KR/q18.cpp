#include <iostream>
#include <string>
using namespace std;
/* Написать класс FakeDbConnection, который нельзя копировать */
class FakeDbConnection {
    public :
        std::string connectionString ;
        bool connected;
        // конструктор
        FakeDbConnection() = default;    
  
        // деструктор
        ~FakeDbConnection() 
        {

        }
        void SetParameters (std::string setStr, bool setConnect) {   // функция установки параметров
           connectionString =  setStr;
           connected = setConnect;
        };
        bool isConnected () {                               // возвращает значение connected
            return connected;
        }; 
        std::string getConnectionString () {                // возвращает значение connectionString
            return connectionString;
        };
        FakeDbConnection(const FakeDbConnection&) = delete;             /* запрещено копирование */
        FakeDbConnection& operator=(const FakeDbConnection&) = delete;  /* запрещено присваивание */
};

int main ()
{
    std::string connectionStr = "Default String"; 
    bool isconnect = true; 
    FakeDbConnection db;
    db.SetParameters(connectionStr, isconnect); 
    std::cout << db.getConnectionString() << " " << db.isConnected() << endl ;  
   /* FakeDbConnection db2; 
    db2 = db;     // не компилируется, т.к. присваивание запрещено */
    return 0;
}