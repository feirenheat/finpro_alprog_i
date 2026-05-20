#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Library_Manager.hpp"

// Pragma ini memberitahu compiler (terutama MSVC) untuk menyertakan library Winsock
#pragma comment(lib, "ws2_32.lib")

std::string extractValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\":\"";
    size_t startPos = json.find(searchKey);
    if (startPos != std::string::npos) {
        startPos += searchKey.length();
        size_t endPos = json.find("\"", startPos);
        if (endPos != std::string::npos) {
            return json.substr(startPos, endPos - startPos);
        }
    }
    return "";
}

int main() {
    LibraryManager adminLib;
    adminLib.addItem(new Book("BK-004", "Fundamental of Electric Circuits", "Matthew Sadiku"));
    adminLib.addItem(new Journal("JR-001", "IEEE Embedded Systems", "TechPub"));
    adminLib.addItem(new Book("BK-001", "Digital Logic Design", "Morris Mano"));
    
    std::cout << "[SYSTEM] Memuat Inventaris...\n";
    adminLib.displayInventaris();

    // 1. Inisialisasi Winsock (Wajib di Windows)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup gagal.\n";
        return 1;
    }

    // 2. Tipe data di Windows menggunakan SOCKET, bukan int
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    std::cout << "\n[SERVER] Menunggu koneksi Perpustakaan di Port 8080...\n";
    
    int addrlen = sizeof(address);
    SOCKET new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    std::cout << "[SERVER] Perpustakaan Terhubung!\n";

    while (true) {
        char buffer[2048] = {0};
        
        // 3. Menggunakan recv() alih-alih read() di Windows
        int valread = recv(new_socket, buffer, 2048, 0);
        if (valread <= 0) {
            std::cout << "\n[SERVER] Perpustakaan terputus.\n";
            break;
        }

        std::string receivedJSON(buffer);
        std::string action = extractValue(receivedJSON, "Action");
        std::string responseJSON;

        if (action == "Pinjam") {
            std::string id = extractValue(receivedJSON, "ID");
            responseJSON = adminLib.pinjamBuku(id);
        } else if (action == "Balik") {
            std::string id = extractValue(receivedJSON, "ID");
            responseJSON = adminLib.pengembalianBuku(id);
        } else if (action == "Cari") {
            std::string id = extractValue(receivedJSON, "ID");
            responseJSON = adminLib.cariBuku(id);
        } else if (action == "AddBuku") {
            std::string id = extractValue(receivedJSON, "ID");
            std::string title = extractValue(receivedJSON, "Title");
            std::string author = extractValue(receivedJSON, "Author");
            
            LibraryItem* newItem = new Book(id, title, author);
            responseJSON = adminLib.addItem(newItem);
        } else if (action == "AddJournal") {
            std::string id = extractValue(receivedJSON, "ID");
            std::string title = extractValue(receivedJSON, "Title");
            std::string fos = extractValue(receivedJSON, "FOS");
            
            LibraryItem* newItem = new Journal(id, title, fos);
            responseJSON = adminLib.addItem(newItem);
        } else {
            responseJSON = "{\"status\":\"error\", \"message\":\"Aksi tidak valid\"}";
        }

        send(new_socket, responseJSON.c_str(), responseJSON.length(), 0);
        
        std::cout << "\n[UPDATE SYSTEM] Setelah instruksi '" << action << "':\n";
        adminLib.displayInventaris();
    }

    // 4. Menggunakan closesocket() dan WSACleanup() di Windows
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}