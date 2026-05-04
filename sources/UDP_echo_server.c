#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SIZE 1000

int main() {
    //TCP와 달리 연결된 상태로 통신 X, 연결설정 X(listen(), accept() 불필요)
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);//소켓 라이브러리 초기화

    SOCKET server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);//UDP 선택하여 소켓 생성, 접속유무 판단 소켓
    
    SOCKADDR_IN server_addr;
    ZeroMemory(&server_addr, sizeof(server_addr));//0으로 초기화
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(ADDR_ANY);//허용할 클라이언트의 주소
    server_addr.sin_port = htons(5000);//포트넘버

    bind(server_socket, (SOCKADDR *) &server_addr, sizeof(server_addr));//소켓에 열어놓을 주소 포트 초기화

    char recv_message[SIZE];
    
    while(TRUE){    
        printf("Waiting for client.\n");

        SOCKADDR_IN client_addr;
        int client_addr_size = sizeof(client_addr);
        int recv_len = recvfrom(server_socket, recv_message, sizeof(recv_message) - 1, 0,  (SOCKADDR *)&client_addr, &client_addr_size);//클라이언트의 요청을 버퍼에서 들고옴
            
        if(recv_len == SOCKET_ERROR) {
            printf("Socket error. \n");
            
            continue;//다른 클라이언트의 요청 처리
        }
            
        int connect_close = 0;
        recv_message[recv_len] = '\0';
        if((strcmp(recv_message, "quit")&&strcmp(recv_message, "QUIT")) == connect_close) continue;//탈출후 소켓 닫고 다음 연결시도
        
        printf("Received from client : %s \n", recv_message);//에코 서버라서 클라이언트가 보낸데이터 그대로
        
        sendto(server_socket, recv_message, (int)strlen(recv_message), 0, (SOCKADDR *)&client_addr, client_addr_size);
    }

    //서버는 항상 켜져있음
    
    return 0;
}