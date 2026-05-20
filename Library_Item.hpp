#include <sstream>
#include <string>

class LibraryItem{
    protected:
        std::string bookID;
        std::string bookTitle;
        bool isAvail;
    
    public:
        LibraryItem(std::string id, std::string t):
            bookID(id), bookTitle(t), isAvail(true){}
        virtual ~LibraryItem(){}
        
        virtual std::string bookType() =0;

        virtual std::string generateJSON(){
            std::stringstream ss;
            ss  << "{\"ID\":\"" << bookID
                << "\",\"Title\":\"" << bookTitle
                << "\",\"Status:\":\"" << (isAvail ? "true" : "false") << "}";
            return ss.str();
        }

        std::string getID(){return bookID;}
        std::string getTitle(){return bookTitle;}
        bool checkAvail(){return isAvail;}
        void setStatus(bool status){isAvail = status;}

};


class Book : public LibraryItem{
    private:
        std::string author;
    
    public:
        Book(std::string id, std::string t, std::string a) 
            : LibraryItem(id, t), author(a){}
        
        std::string bookType() override { return "Book"; }

        std::string generateJSON() override {
            std::stringstream ss;
            ss << "{\"Type\":\"Book\",\"ID\":\"" << bookID 
            << "\",\"Title\":\"" << bookTitle 
            << "\",\"Author\":\"" << author 
            << "\",\"Status\":" << (isAvail ? "true" : "false") << "}";
            return ss.str();
        }
};

class Journal : public LibraryItem{
    private:
        std::string fieldStudy;
    
    public:
        Journal(std::string id, std::string t, std::string f) 
            : LibraryItem(id, t), fieldStudy(f){}
    
        std::string bookType() override { return "Journal"; }

        std::string generateJSON() override {
            std::stringstream ss;
            ss << "{\"Type\":\"Journal\",\"ID\":\"" << bookID 
            << "\",\"Title\":\"" << bookTitle 
            << "\",\"FOS\":\"" << fieldStudy 
            << "\",\"Status\":" << (isAvail ? "true" : "false") << "}";
            return ss.str();
        }
        
};