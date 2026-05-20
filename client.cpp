#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // 1. Inisialisasi Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup gagal.\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Pembuatan socket gagal.\n";
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Gagal menghubungi server!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "=== PERPUSTAKAAN TEKNIK ELEKTRO ===\n";

    while (true) {
        int pilihan;
        std::cout << "\n1. Pinjam Buku\n2. Pengembalian Buku\n3. Cari Buku (Cek Status)\n4. Tambah Koleksi Baru (Buku)\n5. Tambah Koleksi Baru (Journal)\n6. Keluar\nMenu: ";
        std::cin >> pilihan;

        if (pilihan == 6) break;

        std::string requestJSON = "";
        
        if (pilihan == 1 || pilihan == 2 || pilihan == 3) {
            std::string id;
            std::cout << "Masukkan ID Item: ";
            std::cin >> id;
            std::string action = (pilihan == 1) ? "Pinjam" : (pilihan == 2) ? "Balik" : "Cari";
            requestJSON = "{\"Action\":\"" + action + "\", \"ID\":\"" + id + "\"}";
        
        } else if (pilihan == 4) {
            std::string id, title, author;
            std::cout << "Masukkan ID Baru: ";
            std::cin >> id;
            std::cout << "Masukkan Judul: ";
            std::cin >> std::ws; 
            std::getline(std::cin, title);
            std::cout << "Masukkan Penulis: ";
            std::getline(std::cin, author);
            
            requestJSON = "{\"Action\":\"AddBuku\", \"ID\":\"" + id + "\", \"Title\":\"" + title + "\", \"Author\":\"" + author + "\"}";
        } else if (pilihan == 5) {
            std::string id, title, fos;
            std::cout << "Masukkan ID Baru: ";
            std::cin >> id;
            std::cout << "Masukkan Judul: ";
            std::cin >> std::ws; 
            std::getline(std::cin, title);
            std::cout << "Masukkan Field of Study: ";
            std::getline(std::cin, fos);
            
            requestJSON = "{\"Action\":\"AddJournal\", \"ID\":\"" + id + "\", \"Title\":\"" + title + "\", \"FOS\":\"" + fos + "\"}";
        } else {
            std::cout << "Pilihan tidak valid.\n";
            continue;
        }

        send(sock, requestJSON.c_str(), requestJSON.length(), 0);
        
        char buffer[2048] = {0};
        
        // 2. Gunakan recv() untuk membaca di Windows
        recv(sock, buffer, 2048, 0);
        std::cout << "\n[RESPONS SERVER]: " << buffer << std::endl;
    }

    // 3. Matikan Socket Windows dengan bersih
    closesocket(sock);
    WSACleanup();
    return 0;
}