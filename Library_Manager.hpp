#include <vector>
#include <iostream>
#include "Library_Item.hpp"

class LibraryManager{
    private:
        std::vector <LibraryItem*> inventaris;

    public:
        ~LibraryManager() {
        for (auto item : inventaris) {
            delete item;
        }
    }

    void InsertionSort(){
        int size = inventaris.size();
        for(int i =0; i < size; i++){
            LibraryItem* n = inventaris[i];
            int j = i-1;

            while(j>=0 && inventaris[j]->getID() > n->getID()){
                inventaris[j+1]=inventaris[j];
                j=j-1;
            }
            inventaris[j+1]=n;
        }
    }

    LibraryItem* BinarySearch(const std::string& targetID){

        int left = 0;
        int right = inventaris.size() - 1;

        while(left <= right){
            int mid = left + (right - left) / 2;

            if(inventaris[mid]->getID() == targetID){
                return inventaris[mid];
            }
            if(inventaris[mid]->getID() > targetID){
                right = mid -1;
            }else{
                left = mid + 1;
            }
            
        }
        return nullptr;
    }

    std::string addItem(LibraryItem* book) {
        inventaris.push_back(book);
        InsertionSort();
        return "{\"Status\":\"Sukses!\", \"Pesan\":\"Buku berhasil ditambahkan dan sistem telah diurutkan ulang\"}";
    }

    std::string cariBuku(const std::string& targetID){
        LibraryItem* book = BinarySearch(targetID);
        if(!book) return "{\"Status\":\"Gagal!\", \"Pesan\":\"Buku tidak ditemukan\"}";
        return "{\"Status\":\"Sukses!\", \"Pesan\":\"Buku berhasil ditemukan\"}";
    }

    std::string pinjamBuku(const std::string& targetID){
        LibraryItem* book = BinarySearch(targetID);
        if(!book) return "{\"Status\":\"Gagal!\", \"Pesan\":\"Buku tidak ditemukan\"}";
        if(!book->checkAvail()) return "{\"Status\":\"Gagal!\", \"Pesan\":\"Buku sedang dipinjam\"}";

        book->setStatus(false);
        return "{\"Status\":\"Sukses!\", \"Pesan\":\"Buku berhasil dipinjam\", \"Buku\":" + book->generateJSON() + "}";
    }

    std::string pengembalianBuku(const std::string& targetID){
        LibraryItem* book = BinarySearch(targetID);
        if(!book) return "{\"Status\":\"Gagal!\", \"Pesan\":\"Buku tidak ditemukan\"}";
        if(book->checkAvail()) return "{\"Status\":\"Gagal!\", \"Pesan\":\"Buku masih tersedia\"}";

        book->setStatus(true);
        return "{\"Status\":\"Sukses!\", \"Pesan\":\"Buku berhasil dikembalikkan\", \"Buku\":" + book->generateJSON() + "}";
    }

    void displayInventaris(){
        std::cout << "=== Inventaris (Terurut) ===\n";
        for (auto item : inventaris) {
            std::cout << "[" << item->bookType() << "] " 
                      << item->getID() << " - " << item->getTitle() 
                      << " | " << (item->checkAvail() ? "Tersedia" : "Dipinjam") 
                      << std::endl;
        }
    }
};